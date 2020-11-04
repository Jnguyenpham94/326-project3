#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

//test values below:
int arr[] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8}; //10 values sorted: 2,3,4,6,7,8,12,15,18,19

//int arr[] = {5, 11, 3, 10, 2, 9, 6, 7, 1, 4, 8}; //9 values: odd test Sorted: 1,2,3,4,5,6,7,8,9,10,11

//int arr[] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8, 7, 12, 19, 3, 18, 4, 2, 6, 15, 8,7, 12, 19, 3, 18, 4, 2, 6, 15, 8,7, 12, 19, 3, 18, 4, 2, 6, 15, 8,7, 12, 19, 3, 18, 4, 2, 6, 15, 8,7, 12, 19, 3, 18, 4, 2, 6, 15, 8,7, 12, 19, 3, 18, 4, 2, 6, 15, 8,7, 12, 19, 3, 18, 4, 2, 6, 15, 8, 7, 12, 19, 3, 18, 4, 2, 6, 15, 8,7, 12, 19, 3, 18, 4, 2, 6, 15, 8};// 100 values Sorted: same as length 10 but values 10x over

//test values above^

#define SIZE (sizeof(arr) / sizeof(arr[0])) //finding length of a array
int mid = SIZE / 2;                         //middle value used for indexing
int sortedArr[SIZE];                        //final location of sorted values
int firstHalf[5];                    // stores first half of arr
int secondHalf[5];                   // stores second half of arr
int length;
int length2;

pthread_mutex_t lock;

void *insertionSort(int array[])
{
    pthread_mutex_lock(&lock);
    
    int i, j, val;
    for (i = 1; i < mid; i++)
    {
        val = array[i];
        j = i-1;
        while(j >= 0 && array[j] > val)
        {
            array[j+1] = array[j];
            j = j-1;
        }
        array[j+1] = val;
    }
    pthread_mutex_unlock(&lock);
    //return array;
}

void *insertionSort2(int array[])
{
    int i, j, val;
    for (i = 1; i < SIZE; i++)
    {
        val = array[i];
        j = i-1;
        while(j >= 0 && array[j] > val)
        {
            array[j+1] = array[j];
            j = j-1;
        }
        array[j+1] = val;
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

//5, 11, 3, 10, 2, 9, 6, 7, 1, 4, 8}
void *copyArraysOdd()
{
    //printf("FIRST HALF: ");
    for (int i = 0; i < length; i++) //copies first half of array
    {
        firstHalf[i] = arr[i];
        //printf("%d ", firstHalf[i]);
    }
    //printf("SECOND HALF: ");
    for (int j = 0; j < length2+2; j++) //copies second half of array
    {
        secondHalf[j] = arr[length2 + j + 1];
        //printf("%d ", secondHalf[j]);
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

//5, 11, 3, 10, 2, 9, 6, 7, 1, 4, 8}
void *mergeArraysSortOdd()
{
    int i = 0;
    for (; i < length; i++)
    {
        sortedArr[i] = firstHalf[i];
    }
    /*
    printf("\nODD FirstHalf: %d\n", length);
    for (int x=0; x < length; x++)
    {
        printf("%d ",firstHalf[x]);
    }
    */
    for (int j = 0; j < length2; j++)
    {
        sortedArr[i] = secondHalf[j];
        i++;
    }
    /*
    printf("\nODD SecondHalf: %d\n", length2);
    for (int y=0; y < length2; y++)
    {
        printf("%d ",secondHalf[y]);
    }
    printf("\nODD PRESORT:\n");
    for (int z=0; z < SIZE; z++)
    {
        printf("%d ",sortedArr[z]);
    }
    */
    insertionSort2(sortedArr);
}

int main(int argc, char *argv[])
{
    if (SIZE % 2 == 0)
    {
        copyArraysEven(); //copies values from arr to 2 arrays if even array length
    }
    else
    {
        length = mid+1;
        length2 = mid;
        firstHalf[length];
        secondHalf[length2];
        copyArraysOdd(); //copies values from arr to 2 arrays if odd array length
    }

    pthread_t thread1, thread2, parent;
    pthread_mutex_init(&lock, NULL);

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
    pthread_mutex_destroy(&lock);
    return 0;
}