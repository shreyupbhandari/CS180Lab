#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

const int NUM_THREADS = 8;
long DARTS_PER_THREAD = 1000000;

// Global variables shared by all threads (which will cause issues due to race condition.)
long global_total_darts = 0;
long global_in_circle = 0;
long global_out_circle = 0;

// This is the function that each thread will run.
void* throw_darts(void* arg) {

    long thread_id = (long)arg;
    unsigned int my_seed = thread_id + 1; //Unique seed per thread so break the psuedo random number generator :P.
    
    for (long i = 0; i < DARTS_PER_THREAD; i++) 
    {
        double x = (double)rand_r(&my_seed) / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand_r(&my_seed) / RAND_MAX * 2.0 - 1.0;
        
        // All threads are changing this variable at the same time.
        global_total_darts++; 
        
        if (x * x + y * y <= 1.0) 
        {
            global_in_circle++; 
        } else 
        {
            global_out_circle++;
        }
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    
    for (long i = 0; i < NUM_THREADS; i++) 
    {
        pthread_create(&threads[i], NULL, throw_darts, (void*)i);
    }
    
    // Wait for all 8 threads to finish their work
    for (int i = 0; i < NUM_THREADS; i++) 
    {
        pthread_join(threads[i], NULL);
    }
    
    printf("Task 2: Buggy Multithreaded Version simulation:\n");
    printf("Expected total darts: %ld\n", NUM_THREADS * DARTS_PER_THREAD);
    printf("Actual recorded darts: %ld\n", global_total_darts);
    printf("Inside + Outside = %ld\n", global_in_circle + global_out_circle);
    
    // The numbers above likely won't match up.
    
    return 0;
}