#include <stdio.h>
#include <stdlib.h>

int main() {
    // Number of darts we want to throw.
    long total_darts = 100000000; 
    long darts_in_circle = 0;
    
    unsigned my_seed = 1;
    
    for (long i = 0; i < total_darts; i++) 
    {
        double x = (double)rand_r(&my_seed) / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand_r(&my_seed) / RAND_MAX * 2.0 - 1.0;
        
        // Check if the dart landed inside the circle using Pythagorean theorem (x^2 + y^2 <= 1)
        if (x * x + y * y <= 1.0) {
            darts_in_circle++;
        }
    }
    double pi_estimate = 4.0 * ((double)darts_in_circle / total_darts);
    
    printf("Task 1: Sequential Version simulation:\n");
    printf("Total darts thrown: %ld\n", total_darts);
    printf("Darts that landed inside/on the circle: %ld\n", darts_in_circle);
    printf("Estimated value os Pi is: %f\n", pi_estimate);
    
    return 0;
}