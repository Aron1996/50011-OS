#include "mysort.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "List.h"
#include "Stack.h"

int a[] = {45, 23, 78, 12, 100, 1, 100, 34, 90, 78 };

int
main( int argc, char ** argv )
{
  Stack s = Stack();
  int i;

  int nelements = sizeof( a )/ sizeof( int );
  for ( i = 0; i < nelements; i++ ) {
    s.append(a [i]);
  }

  printf("List after sorting...\n");

  s.print();

}
