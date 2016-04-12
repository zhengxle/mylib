/*
 * =====================================================================================
 *
 *       Filename:  bitmap.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2014年07月25日 13时28分28秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  xiaolerzheng (zxl), xiaolerzheng@gmail.com
 *        Company:
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#define unlikely(exp)   __builtin_expect(!!(exp), 0)
#define likely(exp)     __builtin_expect(!!(exp), 1)
#define SZ (12 << 17)
#define SIZE 10000
unsigned long item_list[SIZE];
/*
* is_inset check whether input is in array represented set, if not, set bit corresponding
*/
static inline int is_inset(unsigned long long *array, unsigned int size, unsigned long long input)
{
   static unsigned long long test_bit[] = {
       0x01L << 0, 0x01L << 1, 0x01L << 2, 0x01L << 3, 0x01L << 4, 0x01L << 5, 0x01L << 6, 0x01L << 7,
       0x01L << 8, 0x01L << 9, 0x01L << 10, 0x01L << 11, 0x01L << 12, 0x01L << 13, 0x01L << 14, 0x01L << 15,
       0x01L << 16, 0x01L << 17, 0x01L << 18, 0x01L << 19, 0x01L << 20, 0x01L << 21, 0x01L << 22, 0x01L << 23,
       0x01L << 24, 0x01L << 25, 0x01L << 26, 0x01L << 27, 0x01L << 28, 0x01L << 29, 0x01L << 30, 0x01L << 31,
       0x01L << 32, 0x01L << 33, 0x01L << 34, 0x01L << 35, 0x01L << 36, 0x01L << 37, 0x01L << 38, 0x01L << 39,
       0x01L << 40, 0x01L << 41, 0x01L << 42, 0x01L << 43, 0x01L << 44, 0x01L << 45, 0x01L << 46, 0x01L << 47,
       0x01L << 48, 0x01L << 49, 0x01L << 50, 0x01L << 51, 0x01L << 52, 0x01L << 53, 0x01L << 54, 0x01L << 55,
       0x01L << 56, 0x01L << 57, 0x01L << 58, 0x01L << 59, 0x01L << 60, 0x01L << 61, 0x01L << 62, 0x01L << 63
   };
   unsigned long index = input >> 6;
   if (unlikely(index >= size)) {
       return 0;
   }
   unsigned int bit_index = input & 0x3F;
   if ((array[index] & test_bit[bit_index]) == 0) {
       array[index] |= test_bit[bit_index];
       return 0;
   }
   return 1;
}

int main(int argc, char *argv[])
{
    register int i, j;
    struct timeval start, end, sum;
    srand(time(NULL));
    unsigned long long *array = (unsigned long long *) calloc(SZ, sizeof(unsigned long long));
    unsigned long long *array2 = (unsigned long long *) calloc(SIZE, sizeof(unsigned long long));
    printf("array add=[%p]\n", array);
    unsigned long long num;
    j = 0;
    int total;
    int result;
    /* gettimeofday(&start, NULL); */
    memset(&sum, 0, sizeof(sum));
    for (j = 0; j < 20; j++) {
        unsigned long long max = 0, min = 5000000;
        for (i = 0; i < SIZE; i++) {
            num = rand() % (SZ << 6);
            item_list[i] = num;
            max = max > num ? max : num;
        }
        total = 0;
        gettimeofday(&start, NULL);
        for (i = 0; i < SIZE; i++) {
            result = is_inset(array, SZ, item_list[i]);
            if (result == 0)
                array2[total] = item_list[i];
            total += !result;
        }
        gettimeofday(&end, NULL);
        struct timeval delta;
        memset(&delta, 0, sizeof(delta));
        timersub(&end, &start, &delta);
        timeradd(&sum, &delta, &sum);
        printf("total is %d\n", total);
        printf("%d, consume time = %lu\n", j, 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec));
        printf("%d clear bit set, max = %llu, min=[%llu]\n", j, max, min);
        gettimeofday(&start, NULL);
        for (i = total; i >= 0; --i) {
            array[array2[i] >> 6] = 0;
        }
        printf("result maxbit = %llu\n", array[max >> 6]);
        gettimeofday(&end, NULL);
        memset(&delta, 0, sizeof(delta));
        timersub(&end, &start, &delta);
        timeradd(&sum, &delta, &sum);
        for (i = 0; i < (int)((max >> 6) + 1); i++) {
            if (array[i]) {
                printf("%d error!\n", i);
            }
        }
        printf("%d, clear time = %lu\n", j, 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec));
    }
    free(array);
    free(array2);
    printf("average time=[%ld]\n", (1000000 * sum.tv_sec + sum.tv_usec)/j);
    return 0;
}

