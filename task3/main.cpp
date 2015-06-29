#include "./ukkonen.h"
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>

std::string load_file(std::string filename){
    std::ifstream t(filename);
    return std::string((std::istreambuf_iterator<char>(t)),
        std::istreambuf_iterator<char>());

}

int main()
{
    std::string text = "ababbaa";
    Ukkonen ukk;
    Node* root;

    // root = ukk.buildSuffixTree(text);
    // root->print(root);

    std::stringstream ss;
    ss << "book1.txt";
    std::string s = load_file(ss.str());
    root = ukk.buildSuffixTree(s);
    root->print(root);


    // for (std::size_t i = 1; i <= 5; ++i)
    // {
    //     std::stringstream ss;
    //     ss << "book" << i << ".txt";
    //     std::string s = msc::load_file(ss.str());
    //     t.extend(s);
    // }



}
