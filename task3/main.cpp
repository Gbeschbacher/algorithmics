#include "./ukkonen.h"
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <algorithm>

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
    text = "ababbaa";
    text = "banana$";
    text = "GEEKSFORGEEKS$";
    Ukkonen ukk;
    Node* root;

    root = ukk.buildSuffixTree(text);
    print(root, 0);

    std::cout << "\n\n* _ | \t | _ * \t * _ | \t | _ * \t * _ | \t | _ * \t\n\n";

    std::vector<std::string> labels;
    std::string current = "";
    getInteralNodesWithLabels(root, labels, current);

    std::cout << "\n\n* _ | \t | _ * \t * _ | \t | _ * \t * _ | \t | _ * \t\n\n";




    auto comp = [](std::string const &first, std::string const &second){
        if(first.length() > second.length())
            return true;
        else if (first.length() < second.length())
            return false;

        return first.compare(second) <= 0;
    };

    std::sort<std::vector<std::string>::iterator, decltype(comp)>(
        labels.begin(), labels.end(), comp);

    for(int i = 0; i < labels.size(); i++ )
        std::cout << labels[i] << std::endl;

    // getLongestRepeatedSS(root);


    // std::stringstream ss;
    // ss << "book1.txt";
    // std::string s = load_file(ss.str());
    // root = ukk.buildSuffixTree(s);
    // print(root, 0);

        // 5 stages for 5 books

    // std::stringstream title;
    // std::stringstream s;

    // for (int i = 1; i <= 5; ++i){
    //     title.str("");
    //     title << "book" << i << ".txt";

    //     s << load_file(title.str()) << "€";
    // }

    // text = s.str();

    // std::cout << text << std::endl;

    // root = ukk.buildSuffixTree(text);


}
