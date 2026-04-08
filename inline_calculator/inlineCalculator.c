#include <stdio.h>
#include <stdlib.h>

void six_number_summary(int *a, int size);
int get_percentile(int *a, int size, float fraction);

int main(int argc, char **argv) 
{
    int *a = NULL;
    int size = 0;

    if (argc > 1)
     {
        // Handle Command Line Arguments
        size = argc - 1;
        a = (int *)malloc(size * sizeof(int));
        for (int i = 0; i < size; i++) {
            a[i] = atoi(argv[i + 1]);
        }
    } else 
    {
        // Handle Piped Input / Stdin
        int capacity = 10;
        a = (int *)malloc(capacity * sizeof(int));
        int val;
        while (scanf("%d", &val) == 1) {
            if (size >= capacity) {
                capacity *= 2;
                a = (int *)realloc(a, capacity * sizeof(int));
            }
            a[size++] = val;
        }
    }

    if (size > 0) 
    {
        six_number_summary(a, size);
        free(a);
    } else 
    {
        printf("No input provided.\n");
    }

    return 0;
}

void six_number_summary(int *a, int size) 
{

    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (a[j] > a[j + 1]) {
                int temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }

    
    double sum = 0;
    for (int i = 0; i < size; i++) {
        sum += a[i];
    }

   
    int q1 = get_percentile(a, size, 0.25);
    int median = get_percentile(a, size, 0.50);
    int q3 = get_percentile(a, size, 0.75);

   
    printf("   Min:    %d\n", a[0]);
    printf("    Q1:    %d\n", q1);
    printf("Median:    %d\n", median); 
    printf("    Q3:    %d\n", q3);
    printf("   Max:    %d\n", a[size - 1]);
    printf("  Mean:    %.1f\n", sum / size);
}

int get_percentile(int *a, int size, float fraction) {
   
    int index = (int)(fraction * size);
    return a[index];
}