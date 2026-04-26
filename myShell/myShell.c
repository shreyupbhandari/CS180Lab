#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    
    printf("Hi! Welcome to ShreyShell!\n");
    char *command=(char *)malloc(sizeof(char)*100);
    char **argument=(char **) malloc(sizeof(char *)*10);
    do
    {
        printf("ShreyupShell | ");
        scanf(" %[^\n]",command);
        char *commandCopy=strdup(command);
        int argumentSize=0;
        char *t=strtok(commandCopy," ");
        while(t!=NULL)
        {
            argument[argumentSize]=strdup(t);
            t=strtok(NULL," ");
            argumentSize++;
        }
        argument[argumentSize]=NULL;
        if (strcmp(argument[0],"exit")==0)
        {
            free(commandCopy);
            for (int k=0; k<argumentSize; k++)
            {
                free(argument[k]);
            }
            break;
        }
        if (fork()==0)
        {
            printf("Executing %s command.\n",argument[0]);
            execvp(argument[0],argument);
            exit(1);
        }
        else
        {
            wait(NULL);
            printf("Executed %s command successfully!\n",argument[0]);
        }
        free(commandCopy);
        for (int k=0; k<argumentSize; k++)
        {
            free(argument[k]);
        }
    }
    while(strcmp(argument[0],"exit")!=0);
    free(command);
    free(argument);
    printf("Exiting ShreyShell :(\n");

    return 0;

}
