#ifndef UKKONEN_H
#define UKKONEN_H

#include <string>
#include "node.h"

class Ukkonen{
public:
    static Node* buildSuffixTree(const std::string& text);

private:
    static Node* buildTree(const std::string& text);
    static Reference update(Node* root, const Reference& reference, const int
        index);
    static Reference canonize(const Reference& reference);
    static bool testAndSplit(Reference& reference, const char character, Node*& s);
    static std::shared_ptr<std::string> s_text;
};

#endif