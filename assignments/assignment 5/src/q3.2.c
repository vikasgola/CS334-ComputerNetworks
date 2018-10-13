#include<stdio.h>

// change byte order nbo to hbo and hbo to nbo
void changeByteOrder(char *changeit,char *changed,int size){
    // reverse the array of char
    for(int i=0;i<size;i++){
        changed[i] = changeit[size-1-i];
    }
}

int main(int argc, char **argv){
    char a[4] = {12,34,56,78};
    char b[4];
    // testing
    changeByteOrder(a,b,4);
    
    printf("%d\n",b[0]);
    return 0;
}