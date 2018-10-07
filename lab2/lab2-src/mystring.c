
#include <stdlib.h>
#include "mystring.h"

// Type "man string" to see what every function expects.

int mystrlen(char * s) {
    int count=0;
    while (s[count]!='\0') {
        count++;
    }
    return count;

}

char * mystrcpy(char * dest, char * src) {
    int i=0;
    while(src[i]!='\0'){
        dest[i] = src[i];
        i++;
    }
    dest[i]= '\0';
    return dest;
}

char * mystrcat(char * dest, char * src) {
    int i=0;
    int j=0;
    while(dest[j]!='\0'){
        j++;
    }
    while(src[i]!='\0'){
        dest[i+j] = src[i];
        i++;
    }
    dest[i+j]='\0';
    return dest;
}

int mystrcmp(char * s1, char * s2) {
    int i= 0;
    while(s1[i] == s2[i] && s1[i] != '\0'){
        i++;
    }
    if(s1[i]<s2[i]){
        return -1;
    }
    else if(s1[i]>s2[i]){
        return 1;
    }
    else{
        return 0;
    }
}

char * mystrstr(char * hay, char * needle) {
    int i=0;
    int j=0;
    while((hay[i]!='\0') && (needle[j]!='\0')){
        if (hay[i] != needle[j]) {
            i++;
            j = 0;
        }
        else {
            i++;
            j++;
            if (needle[j] =='\0'){
                return hay+(i-j);
            }
        }
    }
    return NULL;
}

char * mystrdup(char * s) {
    char *a = malloc(sizeof(char*)*mystrlen(s));
    if(s ==NULL){
        return NULL;
    }
    a=mystrcpy(a, s);
    return a;
}

char * mymemcpy(char * dest, char * src, int n)
{
    for (int i=0; i<n; i++){
        dest[i] = src[i];
    }
    return dest;
}

