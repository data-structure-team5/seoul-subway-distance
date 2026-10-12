#include "Dijkstra.h"
#include "MinHeap.h"

void dijkstra(const Graph& g, const std::vector<int>& starts,
              std::vector<int>& dist, std::vector<int>& prev) {
    dist.assign(g.size(), INF);
    prev.assign(g.size(), -1);
    // TODO: starts를 0으로 두고 힙에 넣기
    // TODO: 힙에서 꺼내고(낡은 항목은 건너뛰기) 이웃 간선 relax
    (void)starts;
}
