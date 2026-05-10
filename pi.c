/*
 * security.c  –  Home Security System, Raspberry Pi Zero W
 *
 * Threads:
 *   thread_lcd      – updates 16×2 I2C display every second
 *   thread_ir       – reads LIRC events; arms / disarms / silences
 *   thread_reed     – polls reed switch; triggers alarm when armed
 *   thread_alarm    – blinks Red LED while state == TRIGGERED
 *   thread_doorbell – takes photo when button pressed
 *
 * Build:  make
 * Run:    sudo ./security
 *
 * WiringPi ↔ GPIO ↔ Physical pin map
 *   WP0 = GPIO17 = Pin 11  ← Reed switch OUT
 *   WP1 = GPIO18 = Pin 12  ← IR receiver OUT  (also dtoverlay gpio_pin)
 *   WP2 = GPIO27 = Pin 13  ← Red  LED (330Ω)
 *   WP3 = GPIO22 = Pin 15  ← Green LED (330Ω)
 *   WP4 = GPIO23 = Pin 16  ← Doorbell button
 *   SDA  = Pin 3  \__ I2C to LCD PCF8574 backpack
 *   SCL  = Pin 5  /
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <lirc/lirc_client.h>

#define PIN_REED    0
#define PIN_IR      1
#define PIN_LED_R   2
#define PIN_LED_G   3
#define PIN_BUTTON  4


#define LCD_ADDR    0x27
#define LCD_BLEN    1

#define WEB_ROOT    "/home/pi/html"
#define IMG_PATH    WEB_ROOT "/recent.jpg"
#define CAM_CMD     "rpicam-still -t 1 -o " IMG_PATH " --nopreview 2>/dev/null"


typedef enum { STATE_DISARMED=0, STATE_ARMED, STATE_TRIGGERED } SystemState;

static volatile SystemState g_state   = STATE_DISARMED;
static volatile int          g_silence = 0;
static pthread_mutex_t       g_mutex   = PTHREAD_MUTEX_INITIALIZER;
static struct lirc_config   *g_lirc_cfg = NULL;
static int lcd_fd = -1;


static void lcd_write_word(int data) {
    int tmp = data;
    if (LCD_BLEN) tmp |= 0x08; else tmp &= 0xF7;
    wiringPiI2CWrite(lcd_fd, tmp);
}
static void lcd_nibble(int nibble, int rs) {
    int buf = nibble | 0x04 | (rs ? 0x01 : 0x00);
    lcd_write_word(buf);  delay(2);
    lcd_write_word(buf & 0xFB);
}
static void lcd_send_command(int cmd) {
    lcd_nibble(cmd & 0xF0, 0);
    lcd_nibble((cmd & 0x0F) << 4, 0);
}
static void lcd_send_data(int dat) {
    lcd_nibble(dat & 0xF0, 1);
    lcd_nibble((dat & 0x0F) << 4, 1);
}
static void lcd_init(void) {
    lcd_send_command(0x33); lcd_send_command(0x32);
    lcd_send_command(0x28); lcd_send_command(0x0C);
    lcd_send_command(0x01); lcd_send_command(0x06);
    delay(5);
}
static void lcd_clear(void) { lcd_send_command(0x01); delay(2); }
static void lcd_write(int col, int row, const char *s) {
    lcd_send_command((row ? 0xC0 : 0x80) + col);
    while (*s) lcd_send_data((int)(*s++));
}


static void led_red   (void) { digitalWrite(PIN_LED_R,HIGH); digitalWrite(PIN_LED_G,LOW);  }
static void led_green (void) { digitalWrite(PIN_LED_R,LOW);  digitalWrite(PIN_LED_G,HIGH); }
static void led_off   (void) { digitalWrite(PIN_LED_R,LOW);  digitalWrite(PIN_LED_G,LOW);  }


static void take_picture(void) {
    printf("[CAM] Capturing -> %s\n", IMG_PATH);
    if (system(CAM_CMD) != 0)
        fprintf(stderr, "[CAM] rpicam-still failed\n");
    else
        printf("[CAM] Saved.\n");
}

static void *thread_lcd(void *arg) {
    (void)arg;
    char row0[17], row1[17];
    time_t now;
    struct tm *tm_info;

    while (1) {
        pthread_mutex_lock(&g_mutex);
        SystemState s = g_state;
        pthread_mutex_unlock(&g_mutex);

        switch (s) {
            case STATE_DISARMED:  snprintf(row0,17,"Status:DISARMED "); break;
            case STATE_ARMED:     snprintf(row0,17,"Status:ARMED    "); break;
            case STATE_TRIGGERED: snprintf(row0,17,"Status:TRIGGERED"); break;
        }
        time(&now); tm_info = localtime(&now);
        strftime(row1, 17, "%m/%d %H:%M:%S", tm_info);

        lcd_clear();
        lcd_write(0,0,row0);
        lcd_write(0,1,row1);
        sleep(1);
    }
    return NULL;
}

static void *thread_ir(void *arg) {
    (void)arg;
    char *code;

    if (lirc_init("security", 1) == -1) {
        fprintf(stderr, "[IR] lirc_init failed – is lircd running?\n");
        return NULL;
    }
    if (lirc_readconfig(NULL, &g_lirc_cfg, NULL) != 0) {
        fprintf(stderr, "[IR] lirc_readconfig failed – check ~/.lircrc\n");
        lirc_deinit(); return NULL;
    }
    printf("[IR] Listening...\n");

    while (lirc_nextcode(&code) == 0) {
        if (!code) continue;
        char buf[128];
        while (lirc_code2char(g_lirc_cfg, code, buf) == 0 && buf[0]) {
            if (strstr(buf,"KEY_PLAYPAUSE") || strstr(buf,"KEY_EQUAL") ||
                strstr(buf,"BTN_0"))
            {
                pthread_mutex_lock(&g_mutex);
                switch (g_state) {
                    case STATE_DISARMED:
                        g_state = STATE_ARMED;   led_green();
                        printf("[IR] ARMED\n"); break;
                    case STATE_ARMED:
                        g_state = STATE_DISARMED; led_off();
                        printf("[IR] DISARMED\n"); break;
                    case STATE_TRIGGERED:
                        g_silence = 1;
                        g_state   = STATE_DISARMED; led_off();
                        printf("[IR] SILENCED\n"); break;
                }
                pthread_mutex_unlock(&g_mutex);
            }
        }
        free(code);
    }
    lirc_freeconfig(g_lirc_cfg);
    lirc_deinit();
    return NULL;
}


static void *thread_reed(void *arg) {
    (void)arg;
    int prev = digitalRead(PIN_REED);

    while (1) {
        int cur = digitalRead(PIN_REED);
        pthread_mutex_lock(&g_mutex);
        SystemState s = g_state;
        pthread_mutex_unlock(&g_mutex);

        if (cur == HIGH && prev == LOW && s == STATE_ARMED) {
            printf("[REED] Sensor tripped! Triggering alarm.\n");
            pthread_mutex_lock(&g_mutex);
            g_state   = STATE_TRIGGERED;
            g_silence = 0;
            pthread_mutex_unlock(&g_mutex);
            take_picture();
        }
        prev = cur;
        delay(50);
    }
    return NULL;
}


static void *thread_alarm(void *arg) {
    (void)arg;
    while (1) {
        pthread_mutex_lock(&g_mutex);
        SystemState s = g_state;
        int         q = g_silence;
        pthread_mutex_unlock(&g_mutex);

        if (s == STATE_TRIGGERED && !q) {
            led_red();  delay(200);
            led_off(); delay(200);
        } else {
            delay(100);
        }
    }
    return NULL;
}


static void *thread_doorbell(void *arg) {
    (void)arg;
    int prev = HIGH;

    while (1) {
        int cur = digitalRead(PIN_BUTTON);
        if (cur == LOW && prev == HIGH) {
            printf("[BELL] Doorbell pressed!\n");
            led_green(); delay(300);
            pthread_mutex_lock(&g_mutex);
            SystemState s = g_state;
            pthread_mutex_unlock(&g_mutex);
            if (s == STATE_ARMED) led_green();
            else if (s != STATE_TRIGGERED) led_off();
            take_picture();
        }
        prev = cur;
        delay(50);
    }
    return NULL;
}


int main(void) {
    printf("=== Home Security System ===\n");

    if (wiringPiSetup() == -1) {
        fprintf(stderr, "wiringPiSetup failed\n"); return 1;
    }

    pinMode(PIN_REED,   INPUT);  pullUpDnControl(PIN_REED,   PUD_DOWN);
    pinMode(PIN_IR,     INPUT);
    pinMode(PIN_LED_R,  OUTPUT);
    pinMode(PIN_LED_G,  OUTPUT);
    pinMode(PIN_BUTTON, INPUT);  pullUpDnControl(PIN_BUTTON, PUD_UP);
    led_off();

    lcd_fd = wiringPiI2CSetup(LCD_ADDR);
    if (lcd_fd == -1) {
        fprintf(stderr, "LCD I2C init failed (addr 0x%02X)\n", LCD_ADDR);
        return 1;
    }
    lcd_init();
    lcd_write(0,0,"Security System ");
    lcd_write(0,1,"Initializing... ");
    sleep(1);

    pthread_t t_lcd, t_ir, t_reed, t_alarm, t_bell;
    pthread_create(&t_lcd,   NULL, thread_lcd,      NULL);
    pthread_create(&t_ir,    NULL, thread_ir,       NULL);
    pthread_create(&t_reed,  NULL, thread_reed,     NULL);
    pthread_create(&t_alarm, NULL, thread_alarm,    NULL);
    pthread_create(&t_bell,  NULL, thread_doorbell, NULL);

    printf("[MAIN] Running. Press remote to ARM/DISARM/SILENCE.\n");

    pthread_join(t_lcd,   NULL);
    pthread_join(t_ir,    NULL);
    pthread_join(t_reed,  NULL);
    pthread_join(t_alarm, NULL);
    pthread_join(t_bell,  NULL);

    lcd_clear(); led_off();
    return 0;
}