
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void mymemdump(FILE * fd, char * p , int len) {
    // Add your code here.
    // You may see p as an array.
    // p[0] will return the element 0 
    // p[1] will return the element 1 and so on
  int i;
  //    int count;

  for (i=0; i < len; i+=16) {
    if(i%16==0){
      fprintf(fd, "0x%016lX: ", (unsigned long) p+i);
    }
    for (int j =i; j < i+16 && j <len; j++) {
      int c = p[j]&0xFF; // Get value at [p]. The &0xFF is to make sure you truncate to 8bits or one byte.
      // Print first byte as hexadecimal
      fprintf(fd, "%02X ", c);
    }
    int s = len;
    while (s < i + 16){
	s++;
	fprintf(fd, "%s ", "  ");}
    fprintf(fd, "%s", " ");
    for (int j =i; j < i+16 && j <len; j++) {
      int c = p[j]&0xFF; // Get value at [p]. The &0xFF is to make sure you truncate to 8bits or one byte.

      // Print first byte as character. Only print characters >= 32 that are the printable characters.
      fprintf(fd, "%c", (c>=32 && c <= 127)?c:'.');
    }

    fprintf(fd,"\n");

  }
  
  
}

