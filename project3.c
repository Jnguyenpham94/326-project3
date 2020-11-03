#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

int arr[] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8}; //10 values sorted: 2,3,4,6,7,8,12,15,18,19
//test values below:

//int arr[] = {100, 7, 12, 30, 34, 19, 3, 50, 22, 18, 4, 10, 2, 40, 6, 15, 8};// 17 values sorted: 2, 3, 4, 6, 7, 8, 10, 12, 15, 18, 19, 22, 30, 34, 40, 50, 100

//int arr[] = {3,2,1};//3 values: odd test Sorted: 1,2,3

//int arr[] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8, 7, 12, 19, 3, 18, 4, 2, 6, 15, 8,7, 12, 19, 3, 18, 4, 2, 6, 15, 8,7, 12, 19, 3, 18, 4, 2, 6, 15, 8,7, 12, 19, 3, 18, 4, 2, 6, 15, 8,7, 12, 19, 3, 18, 4, 2, 6, 15, 8,7, 12, 19, 3, 18, 4, 2, 6, 15, 8,7, 12, 19, 3, 18, 4, 2, 6, 15, 8, 7, 12, 19, 3, 18, 4, 2, 6, 15, 8,7, 12, 19, 3, 18, 4, 2, 6, 15, 8};// 100 values Sorted: same as length 10 but values 10x over

//test values above^

#define SIZE (sizeof(arr) / sizeof(arr[0]))     //finding length of a array
int mid = SIZE / 2;                             //middle value used for indexing
int sortedArr[SIZE];                            //final location of sorted values
int firstHalf[SIZE / 2];                        // stores first half of arr
int secondHalf[SIZE / 2];                       // stores second half of arr

pthread_mutex_t lock;

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

void *mergeArraysSort()
{
    for (int i = 0; i < SIZE/2; i++)
    {
        sortedArr[i] = firstHalf[i];
    }

    for (int j = 0; j < SIZE/2; j++)
    {
        sortedArr[mid + j] = secondHalf[j];
    }
    insertionSort2(sortedArr);
}

void copyArraysEven()
{
    for (int i = 0; i < SIZE / 2; i++) //copies first half of array
    {
        firstHalf[i] = arr[i];
    }
    //firstHalf = 7, 12, 19, 3, 18
    //sorted = 3,7,12,18,19

    for (int j = 0; j < SIZE / 2; j++) //copies second half of array
    {
        secondHalf[j] = arr[mid + j];
    }
    //endHalf = 4, 2, 6, 15, 8
    //sorted = 2,4,6,8,15
}

//3 2 1
void copyArraysOdd()
{
    int length = mid +1;
    int length2 = mid;

    for (int i = 0; i < length; i++) //copies first half of array
    {
        firstHalf[i] = arr[i];
        printf("%d ", firstHalf[i]);
    }

    for (int j = 0; j < length2; j++) //copies second half of array
    {
        secondHalf[j] = arr[length2 + j + 1];
        printf("%d ", secondHalf[j]);
    }
}

int main(int argc, char *argv[])
{
    if(SIZE % 2 == 0)
    {
        copyArraysEven();//copies values from arr to 2 arrays
    }
    else
    {
        copyArraysOdd();
    }

    pthread_t thread, thread2, parent;
    pthread_create(&thread, NULL, (void *)insertionSort, (void *)(intptr_t)firstHalf);
    pthread_join(thread, NULL);
    pthread_create(&thread2, NULL, (void *)insertionSort, (void *)(intptr_t)secondHalf);
    pthread_join(thread2, NULL);
    pthread_create(&parent, NULL, (void *)mergeArraysSort, NULL);
    pthread_join(parent, NULL);

    /*testing output of 2 subarrays
    for (int i = 0; i < mid; i++)
    {
        printf("%d ", firstHalf[i]);
    }
    printf("SECOND\n");
    for (int i = 0; i < mid; i++)
    {
        printf("%d ", secondHalf[i]);
    }
    */

    return 0;
}
