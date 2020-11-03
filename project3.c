#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

//test values below:
int arr[] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8}; //10 values sorted: 2,3,4,6,7,8,12,15,18,19

//int arr[] = {5, 3, 2, 1, 4}; //5 values: odd test Sorted: 1,2,3,4,5

//int arr[] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8, 7, 12, 19, 3, 18, 4, 2, 6, 15, 8,7, 12, 19, 3, 18, 4, 2, 6, 15, 8,7, 12, 19, 3, 18, 4, 2, 6, 15, 8,7, 12, 19, 3, 18, 4, 2, 6, 15, 8,7, 12, 19, 3, 18, 4, 2, 6, 15, 8,7, 12, 19, 3, 18, 4, 2, 6, 15, 8,7, 12, 19, 3, 18, 4, 2, 6, 15, 8, 7, 12, 19, 3, 18, 4, 2, 6, 15, 8,7, 12, 19, 3, 18, 4, 2, 6, 15, 8};// 100 values Sorted: same as length 10 but values 10x over

//test values above^

#define SIZE (sizeof(arr) / sizeof(arr[0])) //finding length of a array
int mid = SIZE / 2;                         //middle value used for indexing
int sortedArr[SIZE];                        //final location of sorted values
int firstHalf[SIZE / 2];                    // stores first half of arr
int secondHalf[SIZE / 2];                   // stores second half of arr
int length;
int length2;

void *insertionSort(int array[])
{
    int val;
    for (int i = 1; i < SIZE / 2; i++)
    {
        val = array[i];
        int j = i;
        for (; j > 0; j--)
        {
            if (val < array[j - 1])
            {
                array[j] = array[j - 1];
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

void *insertionSort2(int array[])
{
    int val;
    for (int i = 1; i < SIZE; i++)
    {
        val = array[i];
        int j = i;
        for (; j > 0; j--)
        {
            if (val < array[j - 1])
            {
                array[j] = array[j - 1];
            }
            else
            {
                break;
            }
        }
        array[j] = val;
    }
    printf("Sorted array: ");
    for (int k = 0; k < SIZE; k++)
    {
        printf("%d ", array[k]);
    }
    printf("\n");
}

void *copyArraysEven()
{
    for (int i = 0; i < mid; i++) //copies first half of array
    {
        firstHalf[i] = arr[i];
    }
    //firstHalf = 7, 12, 19, 3, 18
    //sorted = 3,7,12,18,19

    for (int j = 0; j < mid; j++) //copies second half of array
    {
        secondHalf[j] = arr[mid + j];
    }
    //endHalf = 4, 2, 6, 15, 8
    //sorted = 2,4,6,8,15
}

//5, 3, 2, 1, 4
void *copyArraysOdd()
{
    length = mid + 1;
    length2 = mid;
    printf("FIRST HALF: ");
    for (int i = 0; i < length; i++) //copies first half of array
    {
        firstHalf[i] = arr[i];
        printf("%d ", firstHalf[i]);
    }
    printf("SECOND HALF: ");
    for (int j = 0; j < length2; j++) //copies second half of array
    {
        secondHalf[j] = arr[length2 + j + 1];
        printf("%d ", secondHalf[j]);
    }
}

void *mergeArraysSortEven()
{
    for (int i = 0; i < mid; i++)
    {
        sortedArr[i] = firstHalf[i];
    }

    for (int j = 0; j < mid; j++)
    {
        sortedArr[mid + j] = secondHalf[j];
    }
    insertionSort2(sortedArr);
}

//5, 3, 2, 1, 4
void *mergeArraysSortOdd()
{
    int i = 0;
    for (; i < SIZE - length; i++)
    {
        //printf("FIRSTHALF: %d \n", firstHalf[i]);
        sortedArr[i] = firstHalf[i];
        //printf("SORTED: %d \n", sortedArr[i]);
    }
    
    for (int j = 0; j < SIZE - length2; j++)
    {
        //printf("FIRSTHALF: %d \n", firstHalf[i]);
        sortedArr[i] = secondHalf[j];
        //printf("SORTED: %d \n", sortedArr[i]);
        i++;
    }
    insertionSort2(sortedArr);
}

int main(int argc, char *argv[])
{
    if (SIZE % 2 == 0)
    {
        copyArraysEven(); //copies values from arr to 2 arrays
    }
    else
    {
        copyArraysOdd();
    }

    pthread_t thread1, thread2, parent;
    pthread_create(&thread1, NULL, (void *)insertionSort, (void *)(intptr_t)firstHalf);
    pthread_create(&thread2, NULL, (void *)insertionSort, (void *)(intptr_t)secondHalf);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    if (SIZE % 2 == 0)
    {
        pthread_create(&parent, NULL, (void *)mergeArraysSortEven, NULL);
        pthread_join(parent, NULL);
    }
    else
    {
        pthread_create(&parent, NULL, (void *)mergeArraysSortOdd, NULL);
        pthread_join(parent, NULL);
    }

    /*testing output of 2 subarrays
    printf("FIRST: ");
    for (int i = 0; i < mid; i++)
    {
        printf("%d ", firstHalf[i]);
    }
    printf("\nSECOND");
    for (int i = 0; i < mid; i++)
    {
        printf("%d ", secondHalf[i]);
    }
    */
    return 0;
}