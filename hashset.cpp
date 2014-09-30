/*
 * =====================================================================================
 *
 *       Filename:  hashset.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年09月29日 08时00分04秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  xiaoliang (zxl), xiaoliang.zxl@taobao.com
 *        Company:  Taobao.com
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <set>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
using namespace std;

#define unlikely(exp)   __builtin_expect(!!(exp), 0)
#define likely(exp)     __builtin_expect(!!(exp), 1)
#define SZ (12 << 17)
#define SIZE 6000
#define TABSIZE 2531
unsigned long item_list[SIZE];
int main(int argc, char *argv[])
{
    int i, j;
    struct timeval start, end;
    set<unsigned long long> table[TABSIZE];
    srand(time(NULL));
    j = 0;
    unsigned long long num;
    size_t total;
    struct timeval duration;
    memset(&duration, 0, sizeof(duration));
    for (j = 0; j < 20; j++) {
        for (i = 0; i < SIZE; i++) {
            num = rand() % (SZ << 6);
            item_list[i] = num;
        }
        total = 0;
        gettimeofday(&start, NULL);
        for (i = 0; i < SIZE; i++) {
            set<unsigned long long> &tab = table[item_list[i]%TABSIZE];
            if (tab.find(item_list[i]) == tab.end()) {
                tab.insert(item_list[i]);
            }
        }
        gettimeofday(&end, NULL);
        for (i = 0; i < TABSIZE; i++) {
            // printf("table[%d].size=[%lu]\n", i, table[i].size());
            total += table[i].size();
        }
        printf("total is %zd\n", total);
        struct timeval delta;
        timersub(&end, &start, &delta);
        printf("%d, time = %lu\n", j, 1000000 * (delta.tv_sec) + (delta.tv_usec));
        timeradd(&duration, &delta, &duration);
        gettimeofday(&start, NULL);
        for (i = 0; i < TABSIZE; i++) {
            table[i].clear();
        }
        gettimeofday(&end, NULL);
        timersub(&end, &start, &delta);
        printf("%d, clear time = %lu\n", j, 1000000 * (delta.tv_sec) + (delta.tv_usec));
        timeradd(&duration, &delta, &duration);
    }

    printf("average time = %lu\n", (1000000 * (duration.tv_sec) + (duration.tv_usec))/20);
    return 0;
}

