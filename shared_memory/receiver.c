#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      
#include <sys/mman.h>   
#include <unistd.h>     
#include <errno.h> 
int main(int argc, char **argv)
{
    char *name = "/memorysharing";
    const int SIZE = 16*sizeof(int);
    int fd = shm_open(name, O_RDONLY, 0644); //Permission represented in octal notation.
    if (fd==-1)
    {
        fprintf(stderr,"shm_open failed!\n");
        exit(1);
    }
    int *shared_array= (int *)mmap(NULL, SIZE, PROT_READ, MAP_SHARED, fd,0);
    if (shared_array == MAP_FAILED)
    {
        fprintf(stderr,"mmap failed!\n");
        exit(1);
    }
    for (int i=0 ; i<16 ; i++)
    {
        printf("shared_array[%d]=%d",i,shared_array[i]);
    }
    if (munmap(shared_array,SIZE)==-1)
    {
        fprintf(stderr,"unmapping failed!\n");
        exit(1);
    }
    if (close(fd)==-1)
    {
        fprintf(stderr,"memory object closing failed!\n");
        exit(1);
    }
    if (shm_unlink(name)==-1)
    {
        fprintf(stderr,"memory object deleting failed!\n");
        exit(1);
    }
    printf("Deleted\n");
    return 0;
}