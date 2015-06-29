#include "./ukkonen.h"
#include <string>

int main()
{
    std::string text = "test";
    Ukkonen tmp;
    Node* tmp1;
    tmp1 = tmp.buildSuffixTree(text);

/*
    suffix_tree t;
    for (std::size_t i = 1; i <= 5; ++i)
    {
        std::stringstream ss;
        ss << "book" << i << ".txt";
        std::string s = msc::load_file(ss.str());
        t.extend(s);
    }
*/
}
