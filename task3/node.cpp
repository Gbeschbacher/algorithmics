#include "./node.h"

Node::Node(std::shared_ptr<std::string> text, bool isVirtualRoot) : m_isVirtualRoot(isVirtualRoot)
    , m_text(text)
    , m_suffixLink(NULL){}

Node::~Node(){}

void Node::setStartIndex(int startIndex){
    m_startIndex = startIndex;
}

int Node::getStartIndex(){
    return m_startIndex;
}

void Node::setEndIndex(int endIndex){
    m_endIndex = endIndex;
}

int Node::getEndIndex(){
    return m_endIndex;
}

void Node::addChild(Node* child){
    m_children.push_back(child);
}

void Node::removeChild(unsigned int index){

    if(index < m_children.size()){
        std::vector<Node*>::iterator it = m_children.begin() + index;
        m_children.erase(it);
    }
}

Node* Node::getChild(unsigned int index){

    if(index < m_children.size()){
        return m_children[index];
    }
}

std::vector<Node*> Node::getChildren(){
    return m_children;
}

void Node::setSuffixLink(Node* failureLink){
    m_suffixLink = failureLink;
}

Node* Node::getSuffixLink(){
    return m_suffixLink;
}

std::string Node::getLabel(int length) const{

    std::string result = "";
    if((m_endIndex - m_startIndex) >= 0 && m_startIndex < m_text->size()){
        if(length > 0){
            if(length < (m_endIndex - m_startIndex) && (m_startIndex + length)
                < m_text->size()){
                result = m_text->substr(m_startIndex, length);
            }

            else if(m_endIndex < m_text->size()){
                result = m_text->substr(m_startIndex, (m_endIndex - m_startIndex)
                    + 1);
            }

            else{
                result = m_text->substr(m_startIndex);
            }
        }
        else{
            if(m_endIndex >= m_text->size()){
                result = m_text->substr(m_startIndex);
            }
            else{
                result = m_text->substr(m_startIndex, (m_endIndex - m_startIndex)
                    + 1);
            }
        }
    }
    return result;
}

bool Node::isVirtualRoot() const{
    return m_isVirtualRoot;
}