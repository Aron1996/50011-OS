#include <stdio.h>
int
main(int argc, char **argv) {
  printf("Welcome to the High Low game...\n");
  while(1){
    int input;
    int low=1;
    int high =100;
   
    printf("Think of a number between 1 and 100 and press <enter> \n");
    while(low<=high){
      char condition;
      int mid=(low+high)/2;
     
      printf("Is it higher than %d? (y/n)\n",mid);
      scanf(" %c", &condition);
      if(condition == 'n'){
	high = mid -1;
      }
      else if(condition == 'y'){
	low = mid +1;
      }else{
	printf("Type y or n\n");
      }
    }
    char c;
    printf("\n>>>>>> The number is %d\n\n",low);
    printf("Do you want to continue playing (y/n)?");
    scanf(" %c", &c);
    if(c == 'n'){
      printf("Thanks for playing!!!\n");
      break;
    }
  }
}

