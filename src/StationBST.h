#pragma once
#include <string>
#include <vector>

// ③ 담당: 성원 — 역 이름 검색(이진 탐색 트리)
// 키: 역 이름, 값: 그 역의 모든 호선 정점 번호

class StationBST {
public:
    StationBST() = default;
    ~StationBST();
    // 노드의 소유권이 중복되어 이중 해제되지 않도록 복사를 금지한다.
    StationBST(const StationBST&) = delete;
    StationBST& operator=(const StationBST&) = delete;

    // 같은 이름이 이미 있으면 정점 번호만 추가
    void insert(const std::string& name, int vertex);
    // 못 찾으면 nullptr. 반환된 목록은 이 트리가 살아 있는 동안만 유효하다.
    // 삽입과 검색: O(h), h는 트리 높이 (편향되면 O(n)).
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
