#include <stdio.h>
#include "binaryConverter.h"
#include <stdlib.h>

int main()
{
    int n1, n2, overflow = 0;
    printf("Enter two integers: ");
    scanf("%d %d", &n1, &n2);

    int *bin1 = binaryConverter(n1);
    int *bin2 = binaryConverter(n2);

    printf("%d in 2's complement form: ",n1); 
    display(bin1);
    printf("%d in 2's complement form: ",n2); 
    display(bin2);

    int *sum = addBinary(bin1, bin2, &overflow);
    printf("Sum:      "); 
    display(sum);

    if (overflow) 
    {
        printf("Warning: Overflow/Underflow occurred!\n");
    }

    int *negatedSum = negateBinary(sum);
    printf("Negation: "); 
    display(negatedSum);

    printf("Sum of %d and %d in Base 10: %d\n",n1,n2,binaryToDecimal(sum));

    free(bin1); 
    free(bin2); 
    free(sum); 
    free(negatedSum);
    return 0;
}