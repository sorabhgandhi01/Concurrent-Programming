/**
 * @\file   mysort.h
 * @\author Sorabh Gandhi
 * @\brief  This file contains the definition of merge sort algorithm
 * @\date   09/05/2019
 *
 * @\reference  https://www.geeksforgeeks.org/merge-sort/
 *
 */

#ifndef __MYSORT_H__
#define __MYSORT_H__


/**
-----------------------------------------------------------------------------------------------------
mergesort
-----------------------------------------------------------------------------------------------------
*   @\brief This function recursively divides the array into pieces and then call the merge
*           function to re-arrange them in the main input array
*
*   @\param arr[]   Input array of elements
*
*           start   Position of the first element in the input array
*
*           end     Position of the last element in the input array
*
*   @\return    void
*
*/
void mergesort (int arr[], int start, int end);

#endif
