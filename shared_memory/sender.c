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
    int fd = shm_open(name,O_CREAT|O_RDWR|S_IRUSR, 0766);
    if (fd==-1)
    {
        fprintf(stderr,"shm_open failed!\n");
        exit(1);
    }
    ftruncate(fd,SIZE);
    int *shared_array= (int *)mmap(NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd,0);
    if (shared_array == MAP_FAILED)
    {
        fprintf(stderr,"mmap failed!\n");
        exit(1);
    }
    for (int i=0 ; i<16 ; i++)
    {
        shared_array[i]=i*i;
        printf("shared_array[%d]=%d",i,shared_array[i]);
    }
    if (munmap(shared_array,SIZE)==-1)
    {
        fprintf(stderr,"unmapping failed!\n");
        exit(1);
    }
    printf("Unmapped...\n");
    if (close(fd)==-1)
    {
        fprintf(stderr,"memory object closing failed!\n");
        exit(1);
    }
    printf("Closed... done.\n");
    
    return 0;
}
