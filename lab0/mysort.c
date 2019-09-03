#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "mysort.h"


void merge (int arr[], int start, int mid, int end)
{
    int i, j, k = 0;

    //Calculate the length of two sub-array
    int len_l = mid - start + 1;
    int len_r = end - mid;

    //declare two temperorary right and left sun-array
    int temp_l[len_l];
    int temp_r[len_r];

    //populate both of the temperorary array
    for (i = 0; i < len_l; i++)
    {
        temp_l[i] = arr[start + i];
    }

    for (j = 0; j < len_r; j++)
    {
        temp_r[j] = arr[mid + 1 + j];
    }

    i = 0;
    j = 0;
    k = start; //Update k to point to the initial element

    //merge the temp array into main array
    while ((i < len_l) && (j < len_r))
    {
        if (temp_l[i] <= temp_r[j])
        {
            arr[k] = temp_l[i];
            i++;
        } else {
            arr[k] = temp_r[j];
            j++;
        }

        k++;
    }


    //fill the remaining elements of left array
    while (i < len_l)
    {
        arr[k] = temp_l[i];
        i++;
        k++;
    }

    //fill the remaining elements of right array
    while (i < len_r)
    {
        arr[k] = temp_r[j];
        j++;
        k++;
    }
}


void mergesort (int arr[], int start, int end)
{
    if (start < end)
    {
        //calculate the middle element for partision
        int mid = ((start + end) / 2);
    
        mergesort (arr, start, mid);
        mergesort (arr, (mid + 1), end);

        merge (arr, start, mid, end);
    }
}

