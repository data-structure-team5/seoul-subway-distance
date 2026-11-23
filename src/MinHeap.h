#pragma once
#include "DynamicArray_vector.h"

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
    DynamicArray<HeapItem> a_;  // 앞의 n_칸만 힙으로 쓴다
    int n_ = 0;                 // 힙에 들어 있는 개수
    void siftUp(int i);
    void siftDown(int i);
};
