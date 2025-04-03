#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <math.h>

int main(){
    printf("time_t bits (1 bit +- sign): %ld\n\n", sizeof(time_t) * 8);
    time_t time1 = pow(2, 63) - 1;
    printf("Max time_t is 2^63 - 1: %ld\n\n", time1);
    printf("++time1 result: %ld\nNegative value mrans 2^63 is max time, because it OVERFLOWED when ++time1\n", ++time1);
}
