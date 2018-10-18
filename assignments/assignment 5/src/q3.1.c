#include<stdio.h>

int main(int argc, char **argv){
    // storing 1 in data
    int data = 1;
    // getting first byte of data in "firstByte"
    char firstByte = data;
    // checking what firstByte stores
    
    // if it stores 1 that means it is little endian as it stores least significant digit first.
    if( firstByte == 1){
        printf("Little Endian Machine.\n");
    }else{
        printf("Big Endian Machine.\n");
    }
    return 0;
}