#include "StationBST.h"

StationBST::~StationBST() { destroy(root_); }

void StationBST::insert(const std::string& name, int vertex) {
    // 내려갈 자식 포인터 자체를 가리켜 루트도 같은 방식으로 연결한다.
    Node** link = &root_;
    while (*link != nullptr) {
        Node* node = *link;
        if (name < node->name) {
            link = &node->left;
        } else if (name > node->name) {
            link = &node->right;
        } else {
            node->vertices.push_back(vertex);
            return;
        }
    }
    *link = new Node{name, {vertex}, nullptr, nullptr};
}

const std::vector<int>* StationBST::find(const std::string& name) const {
    const Node* node = root_;
    while (node != nullptr) {
        if (name < node->name) {
            node = node->left;
        } else if (name > node->name) {
            node = node->right;
        } else {
            return &node->vertices;
        }
    }
    return nullptr;
}

void StationBST::destroy(Node* node) {
    // 왼쪽 자식을 위로 올리며 제거하여 편향 트리에서도 재귀 스택을 쓰지 않는다.
    while (node != nullptr) {
        if (node->left != nullptr) {
            Node* left = node->left;
            node->left = left->right;
            left->right = node;
            node = left;
        } else {
            Node* next = node->right;
            delete node;
            node = next;
        }
    }
}
