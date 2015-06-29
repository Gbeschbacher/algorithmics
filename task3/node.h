#ifndef NODE_H
#define NODE_H

#include <memory>
#include <vector>
#include <iostream>
#include <queue>

class Node{
    public:
        Node(std::shared_ptr<std::string> text, bool isVirtualRoot = false);
        ~Node();

        void setStartIndex(int startIndex);
        int getStartIndex();

        void setEndIndex(int endIndex);
        int getEndIndex();

        void addChild(Node* child);
        void removeChild(unsigned int index);
        Node* getChild(unsigned int index);

        std::vector<Node*> getChildren();

        void setSuffixLink(Node* suffixLink);
        Node* getSuffixLink();

        std::string getLabel(int length = -1) const;

        bool isVirtualRoot() const;

        void print(Node* root) const;

    private:
        int m_startIndex;
        int m_endIndex;
        std::vector<Node*> m_children;
        bool m_isVirtualRoot;
        Node* m_suffixLink;
        std::shared_ptr<std::string> m_text;
};

struct Reference{
    public:
        Reference() : m_node(NULL)
        , m_startIndex(0)
        , m_endIndex(-1){}
        Node* m_node;
        int m_startIndex;
        int m_endIndex;
};

#endif