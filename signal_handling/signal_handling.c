#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
int seconds_remaining=300;
int password[3];

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
    for (int i=0 ; i<3 ; i++)
    {
        password[i]=rand()%10; //Stores the password combination in an integer array allocated statically.
    }

    for (int i=0 ; i<3 ; i++)
    {
        printf("%d",password[i]);
    }

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
    int entered_password[3],count=0;
    int time_taken_to_enter_password;
    time_t start= time(NULL);
    for(int i=0; i<3 ;i++)
    {
        printf("\nEnter digit %d to the 3 digit combination to disarm the bomb:\n",i+1);
        scanf("%d",&entered_password[i]);
    }
    time_t end= time(NULL);
    double difference=difftime(end,start); //difftime() is the standard way to find the difference in time between two values in seconds.
    seconds_remaining=seconds_remaining-difference;
    printf("\nYou took %.2f seconds to type the password!, which has been deducted from the timer.\n",difference);
    for (int i=0; i<3 ; i++)
    {
        if (password[i]==entered_password[i])
        {
         count++;   
        }
    }
    if (count==3)
    {
        printf("\nThe bomb was disarmed successfully!\n");
        exit(1);
    }
    else
    {
        if(count>=1)
        {
            printf("\n%d numbers match.\n",count);
        }
        else
        {
            printf("\nNone of the digits matched.\n");
        }
        seconds_remaining=seconds_remaining-50;
        printf("\nPenalized 50 seconds for entering the wrong password!\n");
    }

}