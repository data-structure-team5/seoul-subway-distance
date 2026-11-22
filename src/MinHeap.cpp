
#include "MinHeap.h"

void MinHeap::push(HeapItem item) {
    // 꺼내서 비워 둔 칸이 있으면 다시 쓰고, 없으면 뒤에 붙인다.
    if (n_ < static_cast<int>(a_.size())) a_[n_] = item;
    else a_.push_back(item);
    n_++;
    siftUp(n_ - 1);
}

HeapItem MinHeap::pop() {
    // 다익스트라는 빈 힙에서 꺼내지 않는다.
    HeapItem result = a_[0];

    n_--;
    if (n_ == 0) {
        return result;
    }

    a_[0] = a_[n_];  // 마지막 원소를 루트로 옮기고 아래로 내린다
    siftDown(0);

    return result;
}

bool MinHeap::empty() const {
    return n_ == 0;
}

int MinHeap::size() const {
    return n_;
}

void MinHeap::siftUp(int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;

        if (a_[parent].dist <= a_[i].dist) {
            break;
        }

        HeapItem temp = a_[parent];
        a_[parent] = a_[i];
        a_[i] = temp;

        i = parent;
    }
}

void MinHeap::siftDown(int i) {
    int n = n_;

    while (true) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (left < n && a_[left].dist < a_[smallest].dist) {
            smallest = left;
        }

        if (right < n && a_[right].dist < a_[smallest].dist) {
            smallest = right;
        }

        if (smallest == i) {
            break;
        }

        HeapItem temp = a_[i];
        a_[i] = a_[smallest];
        a_[smallest] = temp;

        i = smallest;
    }
}
