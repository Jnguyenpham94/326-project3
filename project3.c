#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

int arr[] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8};// sorted: 2,3,4,6,7,8,12,15,18,19
int SIZE = sizeof(arr) / sizeof(arr[0]);

int *insertionSort(int array[])
{
    int val;
    for (int i = 1; i < SIZE; i++)
    {
        val = array[i];
        int j = i;
        for (; j > 0; j--)
        {
            if(val < array[j-1])
            {
                array[j] = array[j-1];
            }
            else
            {
                break;
            }
            
        }
        array[j] = val;
    }
    return array;
}

int main(int argc, char *argv[])
{
    
    return 0;
}