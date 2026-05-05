#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

const int NUM_THREADS = 8;
long DARTS_PER_THREAD = 1000000;

//Arrays so each thread has its own personal counter.
long thread_total_darts[8] = {0};
long thread_in_circle[8] = {0};
long thread_out_circle[8] = {0};

//The function each thread runs.
void* throw_darts_fixed(void* arg) {
    long thread_id = (long)arg;
    unsigned int my_seed = thread_id + 1; // Unique seed per thread so break the psuedo random number generator :P
    
    for (long i = 0; i < DARTS_PER_THREAD; i++) 
    {
        double x = (double)rand_r(&my_seed) / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand_r(&my_seed) / RAND_MAX * 2.0 - 1.0;
        
        // The thread only updates its specific slot in the array based on its ID
        thread_total_darts[thread_id]++;
        
        if (x * x + y * y <= 1.0) 
        {
            thread_in_circle[thread_id]++;
        } else 
        {
            thread_out_circle[thread_id]++;
        }
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    
    for (long i = 0; i < NUM_THREADS; i++) 
    {
        pthread_create(&threads[i], NULL, throw_darts_fixed, (void*)i);
    }
    
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    
    // Now that the threads are done, add up all the personal counters.
    long grand_total_darts = 0;
    long grand_total_in = 0;
    long grand_total_out = 0;
    
    for (int i = 0; i < NUM_THREADS; i++) 
    {
        grand_total_darts += thread_total_darts[i];
        grand_total_in += thread_in_circle[i];
        grand_total_out += thread_out_circle[i];
    }
    
    // Calculate Pi.
    double pi_estimate = 4.0 * ((double)grand_total_in / grand_total_darts);
    
    printf("Task 3: Fixed Multithreaded Version simulation:\n");
    printf("Total darts thrown: %ld\n", grand_total_darts);
    printf("Darts inside/on the circle: %ld\n", grand_total_in);
    printf("Darts outside circle: %ld\n", grand_total_out);
    printf("Inside + Outside = %ld (Should match total)\n", grand_total_in + grand_total_out);
    printf("Estimated value of Pi: %f\n", pi_estimate);
    
    return 0;
}