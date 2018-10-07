
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void mymemdump(FILE *fd, char * p , int len);

struct X{
  char a;
  int i;
  char b;
  int *p;
};

struct List {
  char * str;
  struct List * next;
};

int
main() {
  char str[20];
  int a = 5;
  int b = -5;
  double y = 12;
  struct X x;
  struct List * head;

  x.a = 'A';
  x.i = 9;
  x.b = '0';
  x.p = &x.i;
  strcpy(str, "Hello world\n");
  printf("&str=0x%lX\n", (unsigned long)str);
  printf("&a=0x%lX\n", (unsigned long)&a);
  printf("&b=0x%lX\n", (unsigned long)&b);
  printf("&x=0x%lX\n", (unsigned long)&x.a);
  printf("&y=0x%lX\n", (unsigned long) &y);
  printf("&i=0x%lX\n", (unsigned long)&x.i);
  printf("&b=0x%lX\n", (unsigned long)&x.b);
  printf("&p=0x%lX\n", (unsigned long)&x.p);
  printf("pvalue%p\n", &x.p);
  mymemdump(stdout, (char *) &x.a, 64);
  head = (struct List *) malloc(sizeof(struct List));
  head->str=strdup("Welcome ");
  head->next = (struct List *) malloc(sizeof(struct List));
  head->next->str = strdup("to ");
  head->next->next = (struct List *) malloc(sizeof(struct List));
  head->next->next->str = strdup("cs250");
  head->next->next->next = NULL;
  printf("head=0x%lx\n", (unsigned long) head);
  printf("head=%s\n",  head);
  printf("headstr=0x%lx\n",(unsigned long) head->str);
  printf("headstr=%s\n",  head->str);
  printf("headnext=0x%lx\n", (unsigned long) head->next);
  printf("headnext=%s\n",  head->next);
  printf("head=0x%lx\n", (unsigned long) head->next->str);
  printf("head=%s\n", (unsigned long) head->next->str);
  printf("headnextnext=0x%lx\n", (unsigned long) head->next->next);
  printf("headnextnext=%s\n",  head->next->next);
  printf("headnextnextstr=0x%lx\n", (unsigned long) head->next->next->str);
  printf("headnextnextstr=%s\n",  head->next->next->str);
  printf("headnextnextnext=0x%lx\n", (unsigned long) head->next->next->next);
  printf("headnextnxextnext=%s\n",  head->next->next->next);
  mymemdump(stdout, (char*) head, 128);
}

