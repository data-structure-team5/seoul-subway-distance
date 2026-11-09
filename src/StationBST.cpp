#include "StationBST.h"

StationBST::~StationBST() { destroy(root_); }

void StationBST::insert(const std::string& name, int vertex) {
    root_ = insertNode(root_, name, vertex);
}

int StationBST::height(const Node* node) {
    return node == nullptr ? 0 : node->height;
}

void StationBST::updateHeight(Node* node) {
    const int leftHeight = height(node->left);
    const int rightHeight = height(node->right);
    node->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

StationBST::Node* StationBST::rotateLeft(Node* node) {
    Node* right = node->right;
    node->right = right->left;
    right->left = node;
    updateHeight(node);
    updateHeight(right);
    return right;
}

StationBST::Node* StationBST::rotateRight(Node* node) {
    Node* left = node->left;
    node->left = left->right;
    left->right = node;
    updateHeight(node);
    updateHeight(left);
    return left;
}

StationBST::Node* StationBST::insertNode(Node* node, const std::string& name, int vertex) {
    if (node == nullptr) {
        return new Node(name, vertex);
    }
    if (name < node->name) {
        node->left = insertNode(node->left, name, vertex);
    } else if (name > node->name) {
        node->right = insertNode(node->right, name, vertex);
    } else {
        node->vertices.push_back(vertex);
        return node;
    }

    updateHeight(node);
    const int balance = height(node->left) - height(node->right);
    if (balance > 1) {
        if (name > node->left->name) {
            node->left = rotateLeft(node->left);
        }
        return rotateRight(node);
    }
    if (balance < -1) {
        if (name < node->right->name) {
            node->right = rotateRight(node->right);
        }
        return rotateLeft(node);
    }
    return node;
}

const StationBST::VertexList* StationBST::find(const std::string& name) const {
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
