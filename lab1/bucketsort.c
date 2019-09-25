#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

//function declaration
void display(int arr[], int size);
int getMax(int arr[], int size);
void bucketSort(int arr[], int size);

using namespace std;

void display(int arr[], int size) {
  int i;
  for(i = 0; i < size; i++) {
	printf("%d\n", arr[i]);
  }
}


int get_bucket_range(int arr[], int size, int thread)
{
	int max = 0, bucket = 0, divider = 0;

	//find max
	max = *max_element(arr, arr + size);
	divider = ceil(float(max + 1) / bucket);

	return divider;
}

void bucketSort(int arr[], int size) {
   
 
  
  //insert element into bucket
  for(i = 0; i < size; i++) {
    j = floor( arr[i] / divider );
    B[j].push_back(arr[i]);
  }
  

  //append back the elements from the buckets
  k = 0;
  for(i = 0; i < bucket; i++) {
    for(j = 0; j < B[i].size(); j++) {
      arr[k++] = B[i][j];
    }
  }
}

int main(void) {
  //unsorted elements
  int arr[] = {22,45,12,8,10,6,72,81,33,18,50,14};

  //size of the array
  int n = sizeof(arr)/sizeof(arr[0]);

  //output unsorted elements
  display(arr, n);

  //sort the elements
  bucketSort(arr, n);

	printf("\n\n");
  //display sorted elements
  display(arr, n);

  return 0;
}
