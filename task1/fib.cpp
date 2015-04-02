#include <cstddef>
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdio.h>
#include <gmp.h>
#include <gmpxx.h>
#include <cmath>
#include <chrono>
#include <time.h>

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

    template<typename _function, typename ..._args>
    static decltype(auto) ms(_function func, _args&&... args){
      auto x = std::chrono::system_clock::now();
      func(std::forward<_args>(args)...);
      auto y = std::chrono::system_clock::now();
      return std::chrono::duration_cast<std::chrono::microseconds>(y - x).count();
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
mpz_class fib1( unsigned int n ) {

    if ( n == 0 )
        return 0;

    if ( n == 1 || n == 2 )
        return 1;

    return fib1( n - 1 ) + fib1( n - 2 );

}


/**
 *
 * ITERATIVE
 *
 * */
mpz_class fib2( unsigned int n ){


    if ( n == 0 )
        return 0;

    if ( n == 1 || n == 2 )
        return 1;

    mpz_class res,
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
mpz_class fib3( int n ){
    mpz_class fib[2][2] = {{1,1},{1,0}},
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
void multiply( mpz_class F[2][2], mpz_class M[2][2] ){

    mpz_class x =  F[0][0] * M[0][0] + F[0][1] * M[1][0];
    mpz_class y =  F[0][0] * M[0][1] + F[0][1] * M[1][1];
    mpz_class z =  F[1][0] * M[0][0] + F[1][1] * M[1][0];
    mpz_class w =  F[1][0] * M[0][1] + F[1][1] * M[1][1];

    F[0][0] = x;
    F[0][1] = y;
    F[1][0] = z;
    F[1][1] = w;
}

void power( mpz_class F[2][2], int n ){

    int i;
    mpz_class M[2][2] = {{1,1},{1,0}};

    // n - 1 times multiply the matrix to {{1,0},{0,1}}
    for ( i = 2; i <= n; i++ )
        multiply(F, M);
}

mpz_class fib4( int n ){
    mpz_class F[2][2] = {{1,1},{1,0}};

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

void powerOptimized( mpz_class F[2][2], int n ){

    if( n == 0 || n == 1)
        return;

    mpz_class M[2][2] = {{1,1},{1,0}};

    powerOptimized(F, n/2);
    multiply(F, F);

    if( n%2 != 0 )
       multiply(F, M);
}

mpz_class fib5( int n ){
    mpz_class F[2][2] = {{1,1},{1,0}};

    if( n == 0 )
        return 0;

    powerOptimized(F, n-1);

    return F[0][0];
}

int main(){

    double samples[replications];

    std::ofstream _fib1 ("fib1.dat");
    std::ofstream _fib2 ("fib2.dat");
    std::ofstream _fib3 ("fib3.dat");
    std::ofstream _fib4 ("fib4.dat");
    std::ofstream _fib5 ("fib5.dat");


/**
 *
 * ITERATIVE
 *
 * */
   for( unsigned int i = 100; i <= 1000000; i *= 10 ){
    _fib2 << "fibonacci_iterative\t" << i << "\t";
        for( unsigned int sampleRun = 0; sampleRun < replications; ++sampleRun ){
            double time = measure::cycles([&i](){
                fib2(i);
            });
            samples[sampleRun] = time;
        }
        _fib2 << "\ttime_mean\t" << arithmetic_mean(samples, replications);
        _fib2 << "\tstandard_deviation\t" << sample_standard_deviation(samples, replications) << std::endl;
    }

/**
 *
 * MATRIX WITH BIT-MASK
 *
 * */
   for( unsigned int i = 100; i <= 1000000; i *= 10 ){
    _fib3 << "fibonacci_matrix_bitmask\t" << i << "\t";
        for( unsigned int sampleRun = 0; sampleRun < replications; ++sampleRun ){
            double time = measure::cycles([&i](){
                fib3(i);
            });
            samples[sampleRun] = time;
        }
        _fib3 << "\ttime_mean\t" << arithmetic_mean(samples, replications);
        _fib3 << "\tstandard_deviation\t" << sample_standard_deviation(samples, replications) << std::endl;
    }

/*
 *
 * MATRIX WITH HELPER FUNCTIONS
 *
 * */
   for( unsigned int i = 100; i <= 1000000; i *= 10 ){
    _fib4 << "fibonacci_matrix_helpermask\t" << i << "\t";
        for( unsigned int sampleRun = 0; sampleRun < replications; ++sampleRun ){
            double time = measure::ms([&i](){
                fib4(i);
            });
            samples[sampleRun] = time;
        }
        _fib4 << "\ttime_mean\t" << arithmetic_mean(samples, replications);
        _fib4 << "\tstandard_deviation\t" << sample_standard_deviation(samples, replications) << std::endl;
    }

/**
 *
 * MATRIX WITH OPTIMIZED HELPER FUNCTIONS
 *
 * */
   for( unsigned int i = 100; i <= 1000000; i *= 10 ){
    _fib5 << "fibonacci_matrix_opzimized_helper\t" << i << "\t";
        for( unsigned int sampleRun = 0; sampleRun < replications; ++sampleRun ){
            auto time = measure::ms([&i](){
                fib5(i);
            });
            samples[sampleRun] = time;
        }
        _fib5 << "\ttime_mean\t" << arithmetic_mean(samples, replications);
        _fib5 << "\tstandard_deviation\t" << sample_standard_deviation(samples, replications) << std::endl;
    }


// /**
//  *
//  * RECURSIVE (STACK TOO SMALL FOR FIB(100))
//  *
//  * */
//    // for( unsigned int i = 100; i <= 1000000; i *= 10 ){
//    //  std::cout << "fibonacci recursive\t" << i << "\t";
//    //      for( unsigned int sampleRun = 0; sampleRun < replications; ++sampleRun ){
//    //          uint64_t time = measure::cycles([&res,&i](){
//    //              fib1(res, i);
//    //          });
//    //          samples[sampleRun] = time;
//    //      }
//    //      std::cout << "\ttime mean\t" << arithmetic_mean(samples, replications);
//    //      std::cout << "\tdeviation\t" << sample_standard_deviation(samples, replications) << std::endl;
//    //  }


    _fib1.close();
    _fib2.close();
    _fib3.close();
    _fib4.close();
    _fib5.close();

   return 0;
}

