#include <stdio.h>
int main() { 
    printf("%d\n", sizeof(double)); // some compilers print 8
    printf("%d\n", sizeof(long double)); // some compilers print 16
    return 0;
}
