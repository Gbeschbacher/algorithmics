#include "./ukkonen.h"
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <queue>

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

static std::string getText(long l){
    long current = l;
    std::stringstream ss;

    while(current > 0){
        ss << (current % 2 == 0 ? "a" : "b");
        current /= 2;
    }

    return ss.str();
}

static void test(){
    for(long l = 1; l < std::numeric_limits<long>::max(); ++l){
        Ukkonen ukk;
        Node* root;
        root = ukk.buildSuffixTree(getText(l));
    }
}



int main(){

    std::string text = "ababbaa";
    text = "GEEKSFORGEEKS";
    text = "ababbaa";
    text = "banana$";
    Ukkonen ukk;
    Node* root;

    root = ukk.buildSuffixTree(text);
    print(root, 0);

    std::cout << "\n\n* _ | \t | _ * \t * _ | \t | _ * \t * _ | \t | _ * \t\n\n";


    // getLongestRepeatedSubstring(root, text);

    // std::stringstream ss;
    // ss << "book1.txt";
    // std::string s = load_file(ss.str());
    // root = ukk.buildSuffixTree(s);
    // root->print(root);

}
