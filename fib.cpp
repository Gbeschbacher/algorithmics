#include <iostream>
#include <cstring>

/**
 *
 * RECURSIVE
 *
 * */
long long fib1( unsigned int n ) {
    if ( n == 0 )
        return 0;

    if ( n == 1 || n == 2 )
        return 1;

    return fib1( n - 1 ) + fib1( n - 2 );
}


/**
 *
 * TEMP-VARIABLE
 *
 * */
long long fib2( unsigned int n ){

    if ( n == 0 )
        return 0;

    if ( n == 1 || n == 2 )
        return 1;

    long long res,
        x = 0,
        y = 1;

    for( unsigned int i = 1; i < n; ++i ){
        res = x + y;
        x = y;
        y = res;

    }


    return res;
}


/**
 *
 * MATRIX WITH MASK
 *
 * */
long long fib3( int n ){
    long long fib[2][2] = {{1,1},{1,0}},
        ret[2][2] = {{1,0},{0,1}},
        tmp[2][2] = {{0,0},{0,0}};

    int i,j,k;

    while( n ){

        if( n & 1 ){
           memset( tmp, 0, sizeof tmp );
            for( i = 0; i < 2; ++i )
                for( j = 0; j < 2; ++j )
                    for( k = 0; k < 2; ++k )
                        tmp[i][j] = ( tmp[i][j] + ret[i][k] * fib[k][j] );

            for(i=0; i<2; i++)
                for(j=0; j<2; j++)
                    ret[i][j] = tmp[i][j];
        }

        memset(tmp, 0, sizeof tmp);
        for( i = 0; i < 2; ++i )
            for( j = 0 ; j < 2 ; ++j )
                for( k = 0; k < 2; ++k )
                    tmp[i][j] = ( tmp[i][j] + fib[i][k] * fib[k][j] );

        for( i = 0; i < 2; ++i )
            for( j = 0; j < 2; ++j )
                fib[i][j] = tmp[i][j];
        n /= 2;
    }

    return ( ret[0][1] );
}

/**
 *
 * MATRIX WITH HELPER FUNCTIONS
 *
 * */
void multiply( long long F[2][2], long long M[2][2] ){

    long long x =  F[0][0] * M[0][0] + F[0][1] * M[1][0];
    long long y =  F[0][0] * M[0][1] + F[0][1] * M[1][1];
    long long z =  F[1][0] * M[0][0] + F[1][1] * M[1][0];
    long long w =  F[1][0] * M[0][1] + F[1][1] * M[1][1];

    F[0][0] = x;
    F[0][1] = y;
    F[1][0] = z;
    F[1][1] = w;
}

void power( long long F[2][2], int n ){

    int i;
    long long M[2][2] = {{1,1},{1,0}};

    // n - 1 times multiply the matrix to {{1,0},{0,1}}
    for ( i = 2; i <= n; i++ )
        multiply(F, M);
}

long long fib4( int n ){
    long long F[2][2] = {{1,1},{1,0}};

    if( n == 0 )
        return 0;

    power(F, n-1);

    return F[0][0];
}

/**
 *
 * MATRIX WITH OPTIMIZED HELPER FUNCTIONS (POWER)
 *
 * */

void powerOptimized( long long F[2][2], int n ){

    if( n == 0 || n == 1)
        return;

    long long M[2][2] = {{1,1},{1,0}};

    powerOptimized(F, n/2);
    multiply(F, F);

    if( n%2 != 0 )
       multiply(F, M);
}

long long fib5( int n ){
    long long F[2][2] = {{1,1},{1,0}};

    if( n == 0 )
        return 0;

    powerOptimized(F, n-1);

    return F[0][0];
}



using namespace std;
int main(){

   for( unsigned int i = 100; i < 1000000; i * 10 )
    cout << fib5(i) << endl;

   return 0;
}

