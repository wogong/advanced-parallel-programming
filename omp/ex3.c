/* 
 * OMP for Gauss
 * date: 2016-06-28
 * author: Cheng Zhen 
 **/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void gaussr_omp(int n, double a[n][n], double b[n]) {
    int row,col;
    double x[n];
    
    for (row=n-1; row>=0; row--) {
        x[row] = b[row];
        int sum=0;
#pragma omp parallel for schedule(runtime) reduction(+:sum)
        for (col=row+1; col<n; col++){
            //printf("col=%d, thread_id=%d\n", col, omp_get_thread_num());
            sum += a[row][col]*x[col];
        }
        x[row] = (x[row]-sum)/a[row][row];
        //printf("x[%d]=%f\n",row,x[row]);
    }
}

void gaussr(int n, double a[n][n], double b[n]) {
    int row,col;
    double x[n];
    
    for (row=n-1; row>=0; row--) {
        x[row] = b[row];
        for (col=row+1; col<n; col++){
            x[row] = x[row] - a[row][col]*x[col];
        }
        x[row] = x[row]/a[row][row];
        printf("x[%d]=%f\n",row,x[row]);
    }
}

void gaussc(int n, double a[n][n], double b[n]) {
    int row,col;
    double x[n];
    
    for (row=0; row<n; row++)
        x[row]=b[row];

    for (col=n-1; col>=0; col--){
        x[col] /= a[col][col];
        for (row=0; row<col; row++){
            x[row] = x[row] - a[row][col]*x[col];
        }
        printf("x[%d]=%f\n",row,x[row]);
    }
}

void ut(int n, double a[n][n], double b[n]) {
    int i, j, k;
    double m;
    
    for (i=0; i<n-1; i++) {
        for (j=i+1; j<n; j++) {
            m = a[j][i]/a[i][i];
            for (k=i; k<n; k++) {
                a[j][k] = a[j][k] - a[i][k]*m;
            }
            b[j] = b[j] - b[i]*m;
        }
    }

}

int main() {
    int i, j, k;
    int n=3;
    struct timeval start, end;

    double a[3][3] = {
        {2, -3, 0},
        {4, -5, 1},
        {2, -1, -3}
    };
    double a2[3][3] = {
        {2, -3, 0},
        {0, 1, 1},
        {0, 0, -5}
    };
    double b[3] = {3, 7, 5};
    double b2[3] = {3, 1, 0};
    
    int nn=1000;
    int row=nn;
    int col=nn;
    double aa[row][col];
    double bb[row];
    for (i=0; i<row; i++){
        for (j=i; j<col; j++){
            aa[i][j]=1;
            //printf("%d ",aa[i][j]);
        }
        bb[i]=row-i;
        //printf("\n");
    }

    gettimeofday( &start, NULL );
    gaussr_omp(row, aa, bb);
    gettimeofday( &end, NULL );
    int timeuse = 1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec -start.tv_usec;
    printf("time: %d us\n", timeuse);

    return 0;
}
