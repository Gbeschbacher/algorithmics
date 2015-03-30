#include <cstddef>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <gmp.h>
#include <cmath>

constexpr unsigned replications = 5;

/**
 *
 * TIME MEASURE
 *
 * */

struct measure {

  private:
    static uint64_t rdtsc(){
      unsigned int lo, hi;
      __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
      return ((uint64_t)hi << 32) | lo;
    }

  public:
    template<typename _function, typename ..._args>
    static uint64_t cycles(_function func, _args&&... args){
      uint64_t x = measure::rdtsc();
      func(std::forward<_args>(args)...);
      uint64_t y = measure::rdtsc();
      return (y - x);
    }

};

/**
 *
 * ARITHMETIC MEAN
 *
 * */

template<typename T>
double arithmetic_mean(T* data, uint32_t nelem) {
  double sum = 0.0;
  for (uint32_t i = 0; i <nelem; ++i) {
    sum += data[i];
  }
  return sum / static_cast<double>(nelem);
}


/**
 *
 * STANDARD DEVIATION
 *
 * */

template<typename T>
double sample_standard_deviation(T* data, uint32_t nelem) {
  double sum = 0.0;
  double mean = arithmetic_mean(data, nelem);

  for (uint32_t i = 0; i <nelem; ++i) {
    double diff = data[i]-mean;
    sum += diff * diff;
  }

  return sqrt((1.0/static_cast<double>(nelem-1)) * sum);
}


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

int main(){

    uint64_t samples[replications];
    mpz_t res;
    mpz_init(res);

/**
 *
 * ITERATIVE
 *
 * */
   for( unsigned int i = 100; i <= 1000000; i *= 10 ){
    std::cout << "fibonacci iterative\t" << i << "\t";
        for( unsigned int sampleRun = 0; sampleRun < replications; ++sampleRun ){
            uint64_t time = measure::cycles([&res,&i](){
                fib2(res, i);
            });
            samples[sampleRun] = time;
        }
        std::cout << "\ttime mean\t" << arithmetic_mean(samples, replications);
        std::cout << "\tdeviation\t" << sample_standard_deviation(samples, replications) << std::endl;
    }

/**
 *
 * MATRIX WITH BIT-MASK
 *
 * */
   for( unsigned int i = 100; i <= 1000000; i *= 10 ){
    std::cout << "fibonacci matrix bitmask\t" << i << "\t";
        for( unsigned int sampleRun = 0; sampleRun < replications; ++sampleRun ){
            uint64_t time = measure::cycles([&i](){
                fib3(i);
            });
            samples[sampleRun] = time;
        }
        std::cout << "\ttime mean\t" << arithmetic_mean(samples, replications);
        std::cout << "\tdeviation\t" << sample_standard_deviation(samples, replications) << std::endl;
    }

/*
 *
 * MATRIX WITH HELPER FUNCTIONS
 *
 * */
   for( unsigned int i = 100; i <= 1000000; i *= 10 ){
    std::cout << "fibonacci matrix helpermask\t" << i << "\t";
        for( unsigned int sampleRun = 0; sampleRun < replications; ++sampleRun ){
            uint64_t time = measure::cycles([&i](){
                fib4(i);
            });
            samples[sampleRun] = time;
        }
        std::cout << "\ttime mean\t" << arithmetic_mean(samples, replications);
        std::cout << "\tdeviation\t" << sample_standard_deviation(samples, replications) << std::endl;
    }

/**
 *
 * MATRIX WITH OPTIMIZED HELPER FUNCTIONS
 *
 * */
   for( unsigned int i = 100; i <= 1000000; i *= 10 ){
    std::cout << "fibonacci matrix opzimized helper\t" << i << "\t";
        for( unsigned int sampleRun = 0; sampleRun < replications; ++sampleRun ){
            uint64_t time = measure::cycles([&i](){
                fib5(i);
            });
            samples[sampleRun] = time;
        }
        std::cout << "\ttime mean\t" << arithmetic_mean(samples, replications);
        std::cout << "\tdeviation\t" << sample_standard_deviation(samples, replications) << std::endl;
    }


/**
 *
 * RECURSIVE
 *
 * */
   for( unsigned int i = 100; i <= 1000000; i *= 10 ){
    std::cout << "fibonacci recursive\t" << i << "\t";
        for( unsigned int sampleRun = 0; sampleRun < replications; ++sampleRun ){
            uint64_t time = measure::cycles([&res,&i](){
                fib1(res, i);
            });
            samples[sampleRun] = time;
        }
        std::cout << "\ttime mean\t" << arithmetic_mean(samples, replications);
        std::cout << "\tdeviation\t" << sample_standard_deviation(samples, replications) << std::endl;
    }


   return 0;
}

