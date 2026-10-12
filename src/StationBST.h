#pragma once
#include <string>
#include <vector>

// ③ 담당: 성원 — 역 이름 검색(이진 탐색 트리)
// 키: 역 이름, 값: 그 역의 모든 호선 정점 번호

class StationBST {
public:
    ~StationBST();
    // 같은 이름이 이미 있으면 정점 번호만 추가
    void insert(const std::string& name, int vertex);
    // 못 찾으면 nullptr
    const std::vector<int>* find(const std::string& name) const;

private:
    struct Node {
        std::string name;
        std::vector<int> vertices;
        Node* left;
        Node* right;
    };
    Node* root_ = nullptr;
    void destroy(Node* node);
};
