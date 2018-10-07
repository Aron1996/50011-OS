#include "mysort.h"
#include <alloca.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
//
// Sort an array of element of any type
// it uses "compFunc" to sort the elements.
// The elements are sorted such as:
//
// if ascending != 0
//   compFunc( array[ i ], array[ i+1 ] ) <= 0
// else
//   compFunc( array[ i ], array[ i+1 ] ) >= 0
//
// See test_sort to see how to use mysort.
//
void mysort( int n,                      // Number of elements
	     int elementSize,            // Size of each element
	     void * array,               // Pointer to an array
	     int ascending,              // 0 -> descending; 1 -> ascending
	     CompareFunction compFunc )  // Comparison function.
{
  // Add your code here. Use any sorting algorithm you want.
  unsigned char *a = (unsigned char*)array;
  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - i -1; j++) {
      void * item = (void *)((char *)array + j * elementSize);
      void * itemNext = (void *)((char *)array + (j+1) * elementSize);
      void * temp = (void *)malloc(elementSize);
      if (ascending == 0) {
	if (compFunc(item, itemNext) < 0) {
	  memcpy ( temp, &a[j * elementSize], elementSize );
	  memcpy ( &a[j * elementSize], &a[(j + 1) * elementSize], elementSize );
	  memcpy ( &a[(j +1) * elementSize], temp, elementSize );
	}
      }else{
	if (compFunc(item, itemNext) > 0) {
	  memcpy ( temp, &a[j * elementSize], elementSize );
	  memcpy ( &a[j * elementSize], &a[(j + 1) * elementSize], elementSize );
	  memcpy ( &a[(j +1) * elementSize], temp, elementSize );
	}
      }
      free(temp);
    }
  }
}

