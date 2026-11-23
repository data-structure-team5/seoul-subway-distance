#pragma once
#include <stdexcept>
#include "DynamicArray_vector.h"

// ④ 담당: 성규 — 큐 (원형 배열 기반)
// 연결 검사(BFS): 먼저 찾은 정점부터 차례로 꺼내 이웃을 살핀다. (선입선출)

class Queue {
public:
    // 최대 capacity개까지 넣을 수 있다. BFS는 정점마다 한 번만 넣으므로 V면 충분하다.
    explicit Queue(int capacity) {
        for (int i = 0; i < capacity; i++) a_.push_back(0);
    }

    // 맨 뒤에 넣기, O(1)
    void push(int x) {
        if (full()) throw std::overflow_error("Queue is full");
        a_[(front_ + count_) % a_.size()] = x;
        count_++;
    }

    // 맨 앞에서 꺼내기, O(1)
    int pop() {
        if (empty()) throw std::underflow_error("Queue is empty");
        int x = a_[front_];
        front_ = (front_ + 1) % a_.size();
        count_--;
        return x;
    }

    bool empty() const { return count_ == 0; }
    bool full() const { return count_ == static_cast<int>(a_.size()); }
    int size() const { return count_; }

private:
    DynamicArray<int> a_;
    int front_ = 0;  // 맨 앞 원소의 칸
    int count_ = 0;  // 들어 있는 개수
};
