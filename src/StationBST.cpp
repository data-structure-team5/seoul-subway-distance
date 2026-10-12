#include "StationBST.h"

StationBST::~StationBST() { destroy(root_); }

void StationBST::insert(const std::string& name, int vertex) {
    // TODO
    (void)name; (void)vertex;
}

const std::vector<int>* StationBST::find(const std::string& name) const {
    // TODO
    (void)name;
    return nullptr;
}

void StationBST::destroy(Node* node) {
    if (!node) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}
