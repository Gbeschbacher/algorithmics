#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <time.h>
constexpr unsigned replications = 10;

/**
 *
 * TIME MEASURE STRUCT
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
      auto x = std::chrono::high_resolution_clock::now();
      func(std::forward<_args>(args)...);
      auto y = std::chrono::high_resolution_clock::now();
      return std::chrono::duration_cast<std::chrono::milliseconds>(y - x).count();
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
 * HELPER CLASS FOR READING LINE OF FILE
 *
 * */

std::string readLine(std::string filepath){
  std::ifstream file;
  file.open(filepath);

  std::string line;
  std::getline(file, line);

  return line;
}


/**
 *
 * KMP COMPUTE BORDER FUNCTION
 *
 * */

void compute_borders(int *border, int m, std::string s){

  border[0]= -1;
  border[1]= 0;

  int i = border[1];

  for(int j = 2; j<=m; j++){
    while((i >= 0) && (s[i] != s[j - 1]))
      i = border[i];
    i++;
    border[j] = i;
  }

}


/**
 *
 * KMP
 *
 * */

int kmp(std::string t, std::string s){

  int n = t.size();
  int m = s.size();

  int * border = new int[m + 1];

  compute_borders(border, m, s);

  int i = 0;
  int j = 0;
  int count = 0;
  while(i <= n - m){

    while(t[i+j] == s[j]){
      j++;
      if(j==m){
        count++;
      }
    }
    i = i + (j - border[j]);
    j = std::max(0, border[j]);
  }

  return count;
}

/**
 *
 * naive
 *
 * */
int naive(std::string t, std::string s){
  int n = t.size();
  int m = s.size();

  int i = 0;
  int j = 0;
  int count = 0;
  while(i <= n - m){
    while(t[i+j] == s[j]){
        j++;
        if(j == m){
            count++;
        }
    }
    i++;
    j=0;
  }
  return count;
}




/**
 *
 * MAIN
 *
 * */

int main(){

  std::string t = readLine("first_quater_of_chromosome1.fa");
  std::string s = readLine("parkinson_gene.fa");

  double samples_kmp[replications];
  double samples_naive[replications];

  int result = 0;


  for( unsigned int i = 10; i <= 100; i += 10 ){
    std::cout << "PREFIX_SIZE\t" << i;
        for( unsigned int sampleRun = 0; sampleRun < replications; ++sampleRun ){

            std::string s_substring = s.substr(0,i);

            double time_kmp = measure::ms([ &t, &s_substring, &result ](){
                result = kmp(t, s_substring);
            });
            samples_kmp[sampleRun] = time_kmp;

            double time_naive = measure::ms([ &t, &s_substring, &result ](){
                result = naive(t, s_substring);
            });
            samples_naive[sampleRun] = time_naive;
        }
        std::cout << "\tOCCURRENCES\t" << result;
        std::cout << "\tAVG_TIME_MEAN_KMP\t" << arithmetic_mean(samples_kmp, replications);
        std::cout << "\tSTD_DEVIATION_KMP\t" << sample_standard_deviation(samples_kmp, replications);
        std::cout << "\tAVG_TIME_MEAN_NAIVE\t" << arithmetic_mean(samples_naive, replications);
        std::cout << "\tSTD_DEVIATION_NAIVE\t" << sample_standard_deviation(samples_naive, replications) << std::endl;
    }

}