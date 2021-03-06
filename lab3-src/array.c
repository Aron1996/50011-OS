#include <stdio.h>

#include "array.h"

// Return sum of the array
double sumArray(int n, double * array) {
  double sum = 0;
  
  double * p = array;
  double * pend = p+n;

  while (p < pend) {
    sum += *p;
    p++;
  }

  return sum;
}

// Return maximum element of array
double maxArray(int n, double * array) {
  double max = 0;

  double * p = array;
  double * pend = p+n;

  while(p < pend){
    if(*p>max){
      max = *p;
    }
    p++;
  }
  return max;
 
}

// Return minimum element of array
double minArray(int n, double * array) {

  double min = 0;

  double * p = array;
  double * pend = p+n;

  while(p < pend){
    if(*p<min){
      min = *p;
    }
    p++;
  }
  return min;
}

// Find the position int he array of the first element x 
// such that min<=x<=max or -1 if no element was found
int findArray(int n, double * array, double min, double max) {
  double * p = array;
  double * pend = p+n;
  int count;
  while(p < pend){
    if(min<*p && *p<max){
      return count;;
    }
    p++;
    count++;
  }
  return -1;
    }

// Sort array without using [] operator. Use pointers 
// Hint: Use a pointer to the current and another to the next element
int sortArray(int n, double * array) {
  double * p = array;
  for (double *i= array; i < p+n-1; i++) {
    for (double *j= array; j < p+n-1; j++) {
      if(*j > *(j+1)){
	double temp = *j;
	*j = *(j+1);
	*(j+1) = temp;
      }

    }
  }
  return 0;
  
  
}

// Print array
void printArray(int n, double * array) {
  double * p = array;
  double * pend = p+n;
  int count;
  while(p < pend){
    printf("%d:%f\n", count, *p);
    p++;
    count++;
  }
}
  



