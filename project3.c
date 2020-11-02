#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

int arr[] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8}; // sorted: 2,3,4,6,7,8,12,15,18,19
#define SIZE (sizeof(arr) / sizeof(arr[0]))
int mid = SIZE / 2;
int sortedArr[SIZE]; //final location of sorted values

pthread_mutex_t lock;

void *insertionSort(int array[])
{
    pthread_mutex_lock(&lock);
    int val;
    for (int i = 1; i < mid; i++)
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
    pthread_mutex_unlock(&lock);
    return array;
}

int *mergeArray()
{
}

int main(int argc, char *argv[])
{
    int firstHalf[mid];
    int endHalf[mid];

    for (int i = 0; i < SIZE/2; i++) //copies first half of array
    {
        firstHalf[i] = arr[i];
    }
    //firstHalf = 7, 12, 19, 3, 18
    //sorted = 3,7,12,18,19

    for (int j = 0; j < SIZE/2; j++) //copies second half of array
    {
        endHalf[j] = arr[mid + j];
    }
    //endHalf = 4, 2, 6, 15, 8
    //sorted = 2,4,6,8,15

    pthread_mutex_init(&lock, NULL); //initialize mutex var
    pthread_t thread, thread2, parent;
    pthread_create(&thread, NULL, (void *)insertionSort, (void *)(intptr_t)firstHalf);
    pthread_join(thread, NULL);
    pthread_create(&thread2, NULL, (void *)insertionSort, (void *)(intptr_t)endHalf);
    pthread_join(thread2, NULL);
    //pthread_create(parent, NULL, mergeArray);

    for(int i=0; i<mid;i++)
    {
        printf("%d\n", firstHalf[i]);
    }

    printf("SECOND\n");
    for(int i=0; i<mid;i++)
    {
        printf("%d\n", endHalf[i]);
    }

    pthread_mutex_destroy(&lock);
    return 0;
}