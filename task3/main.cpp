#include "./ukkonen.h"
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <time.h>

constexpr unsigned replications = 5;

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

template<typename T>
double arithmetic_mean(T* data, uint32_t nelem) {
  double sum = 0.0;
  for (uint32_t i = 0; i <nelem; ++i) {
    sum += data[i];
  }
  return sum / static_cast<double>(nelem);
}

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


std::string load_file(std::string filename){
    std::ifstream t(filename);
    return std::string((std::istreambuf_iterator<char>(t)),
        std::istreambuf_iterator<char>());

}

void print(Node* node, int depth){
    std::cout << "|";

    for(unsigned i = 0; i < depth; i++)
        std::cout << "\t|";

     std::cout << node->getLabel() << " (" << node->getStartIndex() << ":" << node->getEndIndex() << ")" << std::endl;

    std::vector<Node*> children = node->getChildren();

    for(unsigned i = 0; i != children.size(); i++)
        print(children[i], depth + 1);

}

void getInteralNodesWithLabels(Node * node, std::vector<std::string> & labels, std::string current){

    std::vector<Node *> children = node->getChildren();

    auto is_internal_node = [&children](){
        for (auto & child : children)
            if (child->getChildren().size() == 0)
                return true;
            return false;
        };

    if (is_internal_node())
        labels.push_back(current);

    for (auto & child : children){
        if (child->getChildren().size() > 0){
            std::stringstream ss;
            ss << current << child->getLabel();
            getInteralNodesWithLabels(child, labels, ss.str());
        }
    }
}

int main(){

    std::string text = "ababbaa";
    text = "banana$";
    text = "GEEKSFORGEEKS$";
    text = "aba$";
    text = "ababbaa$";
    text = "ababba$";
    Ukkonen ukk;
    // Node* root;

    // root = ukk.buildSuffixTree(text);
    // print(root, 0);

    std::cout << "\n\n* _ | \t | _ * \t * _ | \t | _ * \t * _ | \t | _ * \t\n\n";

    std::vector<std::string> labels;
    std::string current = "";


    std::cout << "\n\n* _ | \t | _ * \t * _ | \t | _ * \t * _ | \t | _ * \t\n\n";



    // getLongestRepeatedSS(root);


    // std::stringstream ss;
    // ss << "book1.txt";
    // std::string s = load_file(ss.str());
    // root = ukk.buildSuffixTree(s);
    // print(root, 0);


    // std::stringstream title;
    // std::stringstream s;

    // for (int i = 1; i <= 1; ++i){
    //     title.str("");
    //     title << "book" << i << ".txt";

    //     s << load_file(title.str()) << "€";
    // }

    // text = s.str();

    // root = ukk.buildSuffixTree(text);


    // getInteralNodesWithLabels(root, labels, current);


    // auto comp = [](std::string const &first, std::string const &second){
    //     if(first.length() > second.length())
    //         return true;
    //     else if (first.length() < second.length())
    //         return false;

    //     return first.compare(second) <= 0;
    // };

    // std::sort<std::vector<std::string>::iterator, decltype(comp)>(
    //     labels.begin(), labels.end(), comp);

    // for(int i = 0; ( i < 3 && i < labels.size()); i++ )
    //     std::cout << labels[i] << std::endl;


    double samples[replications];

    std::stringstream s;

    for (int i = 1; i <= 5; ++i){

        std::stringstream title;
        title.str("");

        title << "book" << i << ".txt";

        s << load_file(title.str()) << "€";

        for( unsigned int sampleRun = 0; sampleRun < replications; ++sampleRun ){

            double _time = measure::ms([ &ukk, &s ](){
                Node* root = ukk.buildSuffixTree(s.str());
            });

            samples[sampleRun] = _time;
        }
        std::cout << "\tCHARACTERS\t" << s.str().size();
        std::cout << "\tAVG_TIME_MEAN_BOOK_" << i <<"\t" << arithmetic_mean(samples, replications);
        std::cout << "\tSTD_DEVIATION_" << i <<"\t" << sample_standard_deviation(samples, replications);
    }


}
