#pragma once
#include <vector>

// ② 담당: 할리오나 — 최소 힙

struct HeapItem {
    int dist;    // 지금까지 구한 시간(초)
    int vertex;  // 정점 번호
};

class MinHeap {
public:
    void push(HeapItem item);  // 넣기
    HeapItem pop();            // 가장 작은 dist 꺼내기
    bool empty() const;
    int size() const;

private:
    std::vector<HeapItem> a_;
    void siftUp(int i);
    void siftDown(int i);
};
