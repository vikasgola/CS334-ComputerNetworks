#include<stdio.h>

// defining byte datatype for easy handling of bytes 
union byte{
    int x;
    char bytes[sizeof (int)];
};

// using the fact that variables of union points to same address
// change byte order nbo to hbo and hbo to nbo
void changeByteOrder(union byte *changeit,union byte *changed){
    // reverse the array of char
    for(int i=0;i<sizeof(int);i++){
        changed->bytes[sizeof(int) - i - 1] = changeit->bytes[i];
    }
}

int main(int argc, char **argv){
    union byte a,b;
    a.x = 0x01234567;

    // testing
    changeByteOrder(&a,&b);

    // printing
    printf("Before:  %x\n",a.x);    
    printf("After: %x\n", b.x);
    return 0;
}