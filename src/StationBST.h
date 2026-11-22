#pragma once
#include <string>
#include "DynamicArray_vertor.h"

// ③ 담당: 성원 — 역 이름 검색(AVL 트리)
// 키: 역 이름, 값: 그 역의 모든 호선 정점 번호

class StationBST {
public:
    using VertexList = DynamicArray<int>;

    StationBST() = default;
    ~StationBST();
    // 노드의 소유권이 중복되어 이중 해제되지 않도록 복사를 금지한다.
    StationBST(const StationBST&) = delete;
    StationBST& operator=(const StationBST&) = delete;

    // 같은 이름이 이미 있으면 정점 번호만 추가
    void insert(const std::string& name, int vertex);
    // 못 찾으면 nullptr. 반환된 목록은 이 트리가 살아 있는 동안만 유효하다.
    // 서로 다른 역 이름이 n개일 때 삽입과 검색은 O(log n)번 노드를 비교한다.
    const VertexList* find(const std::string& name) const;

private:
    struct Node {
        std::string name;
        VertexList vertices;
        Node* left;
        Node* right;
        int height;
        Node(const std::string& stationName, int vertex)
            : name(stationName), left(nullptr), right(nullptr), height(1) {
            vertices.push_back(vertex);
        }
    };
    Node* root_ = nullptr;
    static int height(const Node* node);
    static void updateHeight(Node* node);
    static Node* rotateLeft(Node* node);
    static Node* rotateRight(Node* node);
    static Node* insertNode(Node* node, const std::string& name, int vertex);
    void destroy(Node* node);
};
