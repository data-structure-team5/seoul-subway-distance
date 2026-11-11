
#include "MinHeap.h"

void MinHeap::push(HeapItem item) {
    a_.push_back(item);
    siftUp(static_cast<int>(a_.size()) - 1);
}

HeapItem MinHeap::pop() {
    // Dijkstra нь хоосон heap-ээс pop хийхгүй.
    HeapItem result = a_.front();

    if (a_.size() == 1) {
        a_.pop_back();
        return result;
    }

    a_.front() = a_.back();
    a_.pop_back();
    siftDown(0);

    return result;
}

bool MinHeap::empty() const {
    return a_.empty();
}

int MinHeap::size() const {
    return static_cast<int>(a_.size());
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
    int n = static_cast<int>(a_.size());

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
