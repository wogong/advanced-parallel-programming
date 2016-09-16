/* 
 * OMP for loop
 * date: 2016-08-08
 * author: Cheng Zhen 
 **/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int m,i;
    int n = 20;
    int a[n];
    a[0] = 0;
    a[10] = 45; 

#pragma omp parallel for  private(m,i)
    for (m=0; m<2; m++) {
        for (i=m*10; i<m*10+10; i++) {
            a[i+1] = i*(i+1)/2;
        }
    }

    //output
    for (int i=0; i<n; i++) {
        printf("a[%d]=%d\n",i,a[i]);
    }

    return 0;
}
