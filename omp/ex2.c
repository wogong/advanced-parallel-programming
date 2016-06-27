/* 
 * OMP for loop
 * date: 2016-06-07
 * author: Cheng Zhen 
 **/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int n = 20;
    int a[n];
    a[0] = 0;

#pragma omp parallel shared(n,a)
    {
#pragma omp for schedule(static) nowait
        for (int i=0; i<n; i++) {
            a[i+1] = i*(i+1)/2;
        }
    }

    /* output
    for (int i=0; i<n; i++) {
        printf("a[%d]=%d\n",i,a[i]);
    }
    */

    return 0;
}
