#include "./ukkonen.h"

#include <time.h>
#include <iostream>

std::shared_ptr<std::string> Ukkonen::s_text;

Node* Ukkonen::buildSuffixTree(const std::string& text){
    s_text = std::make_shared<std::string>(text);
    Node* root = buildTree(text);
    return root;
}

Node* Ukkonen::buildTree(const std::string& text){
    Node* root = new Node(s_text);
    Node* virtualRoot = new Node(s_text, true);
    root->setSuffixLink(virtualRoot);
    virtualRoot->addChild(root);
    int k = 1;
    root->setStartIndex(0);
    root->setEndIndex(-1);
    Reference ref;
    ref.m_node = root;
    ref.m_startIndex = k;
    ref.m_endIndex = 0;
    ref = update(root, ref, 0);
    ref.m_node = root;

    for(unsigned int i = 1; i < text.size(); i++){
        ref.m_endIndex = i-1;
        ref = update(root, ref, i);
    }
    return root;
}

Reference Ukkonen::update(Node* root, const Reference& reference, const int index){
    Node* r = NULL;
    Node* old_r = root;
    Reference ref = canonize(reference);
    bool done = testAndSplit(ref, s_text->at(index), r);
    while(done == false){
        Node* newNode = new Node(s_text, false);
        newNode->setStartIndex(index);
        newNode->setEndIndex(s_text->size()+1);
        r->addChild(newNode);
        if(old_r != root){
            old_r->setSuffixLink(r);
        }
        old_r = r;
        ref.m_node = ref.m_node->getSuffixLink();
        ref = canonize(ref);
        done = testAndSplit(ref, s_text->at(index), r);
    }
    if(old_r != root){
        old_r->setSuffixLink(ref.m_node);
    }
    return ref;
}

Reference Ukkonen::canonize(const Reference& reference){
    int k = reference.m_startIndex;
    int p = reference.m_endIndex;
    Node* s = reference.m_node;
    Reference result;
    result.m_node = s;
    result.m_startIndex = k;
    result.m_endIndex = p;
    while((p - k) >= 0){
        bool found = false;
        std::vector<Node*> children = result.m_node->getChildren();
        for(unsigned int i = 0; i < children.size(); i++){
            std::string label = "";
            int labelSize = 0;
            if(result.m_node->isVirtualRoot()){
                label = s_text->at(result.m_startIndex);
                labelSize = 1;
            }
            else{
                label = children[i]->getLabel(1);
                labelSize = (children[i]->getEndIndex() - children[i]->getStartIndex()) + 1;;
            }
            if(label.size() > 0 && label[0] == s_text->at(result.m_startIndex)){
                if(children[i]->getChildren().size() <= 0 || labelSize-1 >
                    (p - k)){
                    return result;
                }

                result.m_node = children[i];
                result.m_startIndex += labelSize;
                found = true;
                break;
            }
        }
        if(found){
            k = result.m_startIndex;
            p = result.m_endIndex;
        }
        else{
            return result;
        }
    }
    return result;
}

bool Ukkonen::testAndSplit(Reference& reference, const char character, Node*& s){
    if(reference.m_node->isVirtualRoot()){
        s = reference.m_node;
        return true;
    }

    if((reference.m_endIndex - reference.m_startIndex) < 0){
        std::vector<Node*> children = reference.m_node->getChildren();
        for(unsigned int i = 0; i < children.size(); i++){
            std::string label = children[i]->getLabel(1);
            if(label.size() > 0 && label[0] == character){
                s = reference.m_node;
                return true;
            }
        }
        s = reference.m_node;
        return false;
    }
    else{
        std::vector<Node*> children = reference.m_node->getChildren();
        for(unsigned int i = 0; i < children.size(); i++){
            std::string label = children[i]->getLabel(1);
            int labelLength = (children[i]->getEndIndex() - children[i]->getStartIndex()) + 1;
            if(labelLength > 0 && label[0] == s_text->at(reference.m_startIndex)){
                int length = reference.m_endIndex - reference.m_startIndex;
                if(labelLength > length+1){
                    label = children[i]->getLabel(length+2);
                    if(label[length+1] == character){
                        s = reference.m_node;
                        return true;
                    }
                    else{
                        Node* child = children[i];
                        int startIndex = child->getStartIndex();
                        int endIndex = child->getEndIndex();
                        child->setStartIndex(startIndex + length+1);
                        Node* node = new Node(s_text, false);
                        node->setStartIndex(startIndex);
                        node->setEndIndex(startIndex + length);
                        node->addChild(child);
                        reference.m_node->removeChild(i);
                        reference.m_node->addChild(node);
                        s = node;
                        return false;
                    }
                }
            }
        }
    }
}