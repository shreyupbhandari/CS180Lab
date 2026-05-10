#ifndef LCD_H
#define LCD_H

int lcd_setup();
void lcd_write(int x, int y, char *data);
void lcd_clear();

#endif