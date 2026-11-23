#pragma once
#include <stdexcept>
#include "DynamicArray_vector.h"

// ④ 담당: 성규 — 스택 (배열 기반)
// 경로 출력: 도착역부터 거꾸로 넣고, 꺼내면 출발역부터 나온다. (후입선출)

class Stack {
public:
    // 맨 위에 넣기, O(1) (공간이 모자라면 DynamicArray가 늘린다)
    void push(int x) {
        if (top_ < static_cast<int>(a_.size())) a_[top_] = x;  // 꺼냈던 칸 다시 쓰기
        else a_.push_back(x);
        top_++;
    }

    // 맨 위에서 꺼내기, O(1)
    int pop() {
        if (empty()) throw std::underflow_error("Stack is empty");
        return a_[--top_];
    }

    bool empty() const { return top_ == 0; }
    int size() const { return top_; }

private:
    DynamicArray<int> a_;
    int top_ = 0;  // 다음에 넣을 칸 = 지금 들어 있는 개수
};
