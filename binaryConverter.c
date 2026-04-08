#include <stdio.h>
#include <stdlib.h>
#include <math.h> // The math header file is included to use the pow() function.
#include "binaryConverter.h"

int *binaryConverter(int number);
void binaryAllocation(int *binaryArray, int remainder);
int divisionCounter(int number);
void display(int *binaryArray);

int divisionCounter(int number)//Counter function implemented to count the number of divisions on the number.
{
    int count=0;
    while (number!=0)
    {
        number/=2;
        count++;
    }
    return count;
}
int *binaryConverter(int number)
{
    const int MaxValue = pow(2,NumberOfBits-1)-1;
    const int MinValue = -pow(2,NumberOfBits-1);
    if (number>MaxValue || number<MinValue)
    {
        if (number>MaxValue)
        {
            fprintf(stderr,"Converting %d to 16 bit binary causes overflow. The range is (%d to %d).\n",number,MinValue,MaxValue);
        }
        else
        {
            fprintf(stderr,"Converting %d to 16 bit binary causes underflow. The range is (%d to %d).\n",number,MinValue,MaxValue);
        }
        exit(1);
    }
    int *binaryArray;
    if (NULL==(binaryArray=(int *)calloc(NumberOfBits,sizeof(int)))) //Calloc to initialize all indexes to the value 0.
    {
        fprintf(stderr, "Dynamic memory allocation failed for the binary array!\n");
    }

    if (number<0)
    {
        int negativeIndex=0;
        const int signedvalue=pow(2,NumberOfBits-1);
        binaryArray[negativeIndex]=1;
        int remainder = signedvalue +number;
        binaryAllocation(binaryArray, remainder);
    }
    else
    {
        binaryAllocation(binaryArray, number); //Positive numbers that have a 0 for the signed bit.
    }
    return binaryArray;
}

void binaryAllocation(int *binaryArray, int number)
{
    int index=NumberOfBits-1;
    while (number!=0)
    {
        int remainder;
        remainder=number%2;
        binaryArray[index]=remainder;
        index--;
        number/=2;
    }
}

void display(int *binaryArray)
{
    for (int i = 0; i < NumberOfBits ; i++) 
    {
        printf("%d",binaryArray[i]);
    } 
    printf("\n");
}

// Performs bitwise addition and checks for signed overflow
int *addBinary(int *a, int *b, int *overflow) 
{
    int *sum = (int *)calloc(NumberOfBits, sizeof(int));
    int carry = 0;
    for (int i = NumberOfBits - 1; i >= 0; i--) 
    {
        int s = a[i] + b[i] + carry;
        sum[i] = s % 2;
        carry = s / 2;
    }
    // Signed overflow occurs if operands have same sign but result differs
    if (a[0] == b[0] && sum[0] != a[0]) 
    {
        *overflow = 1;
    }
    return sum;
}

// Computes two's complement by flipping bits and adding one
int *negateBinary(int *binaryArray) 
{
    int *negated = (int *)calloc(NumberOfBits, sizeof(int));
    int carry = 1;
    for (int i = NumberOfBits - 1; i >= 0; i--) 
    {
        int flip = !binaryArray[i];
        negated[i] = (flip + carry) % 2;
        carry = (flip + carry) / 2;
    }
    return negated;
}

// Converts binary array back to base 10 integer
int binaryToDecimal(int *binaryArray) 
{
    int decimal = 0;
    if (binaryArray[0] == 1) 
    { // Handle negative sign bit
        decimal = -pow(2, NumberOfBits - 1);
    }
    for (int i = 1; i < NumberOfBits; i++) 
    {
        if (binaryArray[i] == 1) 
        {
            decimal += pow(2, NumberOfBits - 1 - i);
        }
    }
    return decimal;
}


