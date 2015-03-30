#include <cstddef>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <gmp.h>

/**
 *
 * RECURSIVE
 *
 * */
void fib1( mpz_t res, unsigned int n ) {

    if ( n == 0 )
        mpz_set_ui( res, 0 );
    else if ( n == 1 || n == 2 )
        mpz_set_ui( res, 1 );
    else{
        mpz_t f1, f2;

        mpz_init( f1 );
        mpz_init( f2 );

        fib1( f1, n - 1 );
        fib1( f2, n - 2 );
        mpz_add( res, f1, f2 );
    }
}


/**
 *
 * ITERATIVE
 *
 * */
void fib2( mpz_t res, unsigned int n ){


    if ( n == 0 )
        mpz_set_ui( res, 0 );
    else if ( n == 1 || n == 2 )
        mpz_set_ui( res, 1 );
    else{

        mpz_t x, y;

        mpz_init( x );
        mpz_init( y );

        mpz_set_ui( x, 0 );
        mpz_set_ui( y, 1 );

        for( unsigned int i = 1; i < n; ++i ){
            mpz_add( res, x, y );
            mpz_set( x, y );
            mpz_set( y, res );
        }
    }
}


/**
 *
 * MATRIX WITH MASK
 *
 * */
void fib3( mpz_t res, unsigned int n ){

    mpz_t fib, ret, tmp;

    mpz_init( fib );
    mpz_init( ret );
    mpz_init( tmp );



    fib[2][2] = {{1,1},{1,0}};
    ret[2][2] = {{1,0},{0,1}};
    tmp[2][2] = {{0,0},{0,0}};

    // int i,j,k;

    // while( n ){

    //     if( n & 1 ){
    //        memset( tmp, 0, sizeof tmp );
    //         for( i = 0; i < 2; ++i )
    //             for( j = 0; j < 2; ++j )
    //                 for( k = 0; k < 2; ++k )
    //                     tmp[i][j] = ( tmp[i][j] + ret[i][k] * fib[k][j] );

    //         for(i=0; i<2; i++)
    //             for(j=0; j<2; j++)
    //                 ret[i][j] = tmp[i][j];
    //     }

    //     memset(tmp, 0, sizeof tmp);
    //     for( i = 0; i < 2; ++i )
    //         for( j = 0 ; j < 2 ; ++j )
    //             for( k = 0; k < 2; ++k )
    //                 tmp[i][j] = ( tmp[i][j] + fib[i][k] * fib[k][j] );

    //     for( i = 0; i < 2; ++i )
    //         for( j = 0; j < 2; ++j )
    //             fib[i][j] = tmp[i][j];
    //     n /= 2;
    // }

    // return ( ret[0][1] );
}

/**
 *
 * MATRIX WITH HELPER FUNCTIONS
 *
 * */
void multiply( unsigned long long F[2][2], unsigned long long M[2][2] ){

    unsigned long long x =  F[0][0] * M[0][0] + F[0][1] * M[1][0];
    unsigned long long y =  F[0][0] * M[0][1] + F[0][1] * M[1][1];
    unsigned long long z =  F[1][0] * M[0][0] + F[1][1] * M[1][0];
    unsigned long long w =  F[1][0] * M[0][1] + F[1][1] * M[1][1];

    F[0][0] = x;
    F[0][1] = y;
    F[1][0] = z;
    F[1][1] = w;
}

void power( unsigned long long F[2][2], int n ){

    int i;
    unsigned long long M[2][2] = {{1,1},{1,0}};

    // n - 1 times multiply the matrix to {{1,0},{0,1}}
    for ( i = 2; i <= n; i++ )
        multiply(F, M);
}

unsigned long long fib4( int n ){
    unsigned long long F[2][2] = {{1,1},{1,0}};

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

void powerOptimized( unsigned long long F[2][2], int n ){

    if( n == 0 || n == 1)
        return;

    unsigned long long M[2][2] = {{1,1},{1,0}};

    powerOptimized(F, n/2);
    multiply(F, F);

    if( n%2 != 0 )
       multiply(F, M);
}

unsigned long long fib5( int n ){
    unsigned long long F[2][2] = {{1,1},{1,0}};

    if( n == 0 )
        return 0;

    powerOptimized(F, n-1);

    return F[0][0];
}

int main(){
/*
   for( unsigned int i = 100; i <= 1000000; i *= 10 ){
        cout << i << "\t" << fib2(i) << endl;
    }*/


    mpz_t res;
    mpz_init(res);

    for( unsigned long int i = 100; i <= 1000000; i*= 10){
        std::cout << i << "\t";
        fib2(res, i);
        gmp_printf("%Zd\n", res);
    }

   return 0;
}

