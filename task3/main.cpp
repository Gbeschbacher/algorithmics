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


void doTraversal(Node *n, int labelHeight, int* maxHeight, int* substringStartIndex){
    if(n == NULL)
        return;

    std::cout << "DO TRAVERSAL\n" << std::endl;

    std::cout << "label: " << n->getLabel() << std::endl;
    std::cout << "start: " << n->getStartIndex() << std::endl;
    std::cout << "end: " << n->getEndIndex() << std::endl;

    int i=0;
    std::vector<Node*> children = n->getChildren();

    if(children.size() > 1){ //If it is internal node
        std::cout <<"\nIS INTERNAL\n" <<std::endl;
        for (i = 0; i < children.size(); i++){
            doTraversal(children[i], labelHeight + children[i]->getEndIndex() - children[i]->getStartIndex() , maxHeight, substringStartIndex);
        }
    }
    else if(*maxHeight < labelHeight - children[i]->getEndIndex() - children[i]->getStartIndex()){
        std::cout <<"ELSE IF" <<std::endl;
        *maxHeight = labelHeight - children[i]->getEndIndex() - children[i]->getStartIndex();
        // *substringStartIndex = n->suffixIndex;
    }
}


void getLongestRepeatedSubstring(Node* root, std::string text){
    int maxHeight = 0;
    int substringStartIndex = 0;

    doTraversal(root, 0, &maxHeight, &substringStartIndex);
    // printf("maxHeight %d, substringStartIndex %d\n", maxHeight, substringStartIndex);

    // printf("Longest Repeated Substring ", text);

    int k;
    for (k=0; k<maxHeight; k++)
        printf("%c", text[k + substringStartIndex]);
    if(k == 0)
        printf("No repeated substring");
    printf("\n");
}


int main(){
    std::string text = "ababbaa";
    text = "banana";
    text = "ababbaa";
    text = "GEEKSFORGEEKS";
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
