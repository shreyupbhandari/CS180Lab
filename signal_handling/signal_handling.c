#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
int seconds_remaining=300;
int password;

void terminating_signal_handlers(int signal);
void increase_timer_signal_handler(int signal);
void disarm_chance_signal_handler(int signal);
void disarm();

int main()
{
    signal(SIGINT,terminating_signal_handlers);
    signal(SIGTERM,terminating_signal_handlers);
    signal(SIGUSR1,increase_timer_signal_handler);
    signal(SIGUSR2,disarm_chance_signal_handler);
    srand(time(NULL)); //Seeding the random number according to the current time.
    password=rand()%900+100; //Stores the password combination in an integer array allocated statically.
    printf("%d",password);

    while (seconds_remaining<=300)
    {
        printf("\n%d seconds remain.\n",seconds_remaining);
        fflush(stdout); //Because printf() is buffered, this ensures the messages appears immediately.
        sleep(1);
        seconds_remaining--;
        if(seconds_remaining<=0)
        {
            printf("\nBOOM! You have failed to save the building.\n");
            exit(1);
        }
    }
    return 0;
}

void terminating_signal_handlers(int signal)
{
    write(STDOUT_FILENO, "\nHaha! Tricked you.\n",21); //Signal handlers run asynchronously, so printf() might crash.
    seconds_remaining=seconds_remaining-100;
}

void increase_timer_signal_handler(int signal)
{
    seconds_remaining=300;
    write(STDOUT_FILENO, "\nTimer set to 300 again.\n",26);
}

void disarm_chance_signal_handler(int signal)
{
    disarm();
}

void disarm()
{   
    int entered_password;
    printf("Enter a three digit combination to disarm the bomb:\n");
    scanf("%d",&entered_password);
    if(password==entered_password)
    {
        printf("\nThe bomb has been disarmed successfully!.\n");
        exit(1);
    }
    else
    {
        seconds_remaining=seconds_remaining-50;
        printf("\nPenalized 50 seconds for entering the wrong password!\n");
    }


}