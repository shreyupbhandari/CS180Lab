#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    
    printf("Hi! Welcome to ShreyShell!\n");
    char *command=NULL;
    char **argument=NULL;
    if (NULL==(command=(char *)malloc(sizeof(char)*100)))
    {
        fprintf(stderr,"Malloc to the commmad failed!\n");
    }
    if (NULL==(argument=(char **) malloc(sizeof(char *)*10)))
    {
        fprintf(stderr,"Malloc to the command vector failed!\n");
    }
    do
    {
        printf("ShreyupShell | ");
        scanf(" %[^\n]",command); //Read everything except a newline. Otherwise, scanf() terminates when a space appears. Also, a space is added before the % to account for buffer.
        char *commandCopy=strdup(command);
        int argumentSize=0;
        char *t=strtok(commandCopy," ");
        while(t!=NULL)
        {
            argument[argumentSize]=strdup(t);
            t=strtok(NULL," "); //NULL is passed from the second time because strdup() has a static internal pointer that keeps track of the delimeter.
            argumentSize++;
        }
        argument[argumentSize]=NULL;//The end of the vector passed to execvp() must terminate with NULL.
        if (strcmp(argument[0],"exit")==0) //strcmp() compares two strings and returns 0 if the two strings are equal.
        {
            free(commandCopy); //strdup() is a malloc wrapper and has to be freed.
            for (int k=0; k<argumentSize; k++)
            {
                free(argument[k]);
            }
            break;
        }
        if (fork()==0)
        {
            printf("Executing %s command.\n",argument[0]);
            execvp(argument[0],argument); //The first argument is the program name, and the second is a vector that ends with a NULL pointer.
            exit(1); //Since exec() replaces the calling program's memory sign with the new program, the code below it only runs if exec() fails.
                    //exiting is crucial, otherwise the child process will be a zombie.
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
