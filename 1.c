#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <math.h>

int main(){
    printf("time_t bits (1 bit +- sign): %ld\n", sizeof(time_t) * 8);
    time_t time1 = pow(2, 63) - 1;
    printf("Max time_t is 2^63: %ld\n", time1);
    printf("++time1 result: %ld\nNegative value - OVERFLOWED\n", ++time1);
}