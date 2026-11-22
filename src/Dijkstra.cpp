
#include "Dijkstra.h"
#include "MinHeap.h"

void dijkstra(const Graph& g, const DynamicArray<int>& starts,
              DynamicArray<int>& dist, DynamicArray<int>& prev) {
    const int n = g.size();

    // 모든 정점을 "갈 수 없음"으로 시작한다.
    dist.clear();
    prev.clear();
    for (int v = 0; v < n; v++) {
        dist.push_back(INF);
        prev.push_back(-1);
    }

    MinHeap heap;

    // 출발역의 모든 정점은 0초에서 시작한다.
    for (int s : starts) {
        if (s < 0 || s >= n) {
            continue;
        }

        if (dist[s] != 0) {
            dist[s] = 0;
            prev[s] = -1;
            heap.push({0, s});
        }
    }

    // 시간이 가장 짧은 정점부터 꺼내 처리한다.
    while (!heap.empty()) {
        HeapItem current = heap.pop();

        int d = current.dist;
        int u = current.vertex;

        // 이미 더 짧은 시간으로 확정된 낡은 항목은 건너뛴다.
        if (d != dist[u]) {
            continue;
        }

        // u에서 나가는 모든 간선을 살핀다.
        for (Edge* e = g.edges(u); e != nullptr; e = e->next) {
            int v = e->to;
            int cost = e->cost;

            if (v < 0 || v >= n || cost < 0) {
                continue;
            }

            // 더한 값이 INF를 넘지 않게 막는다.
            if (d > INF - cost) {
                continue;
            }

            int newDist = d + cost;

            // 더 짧은 경로를 찾으면 dist, prev를 고치고 힙에 넣는다.
            if (newDist < dist[v]) {
                dist[v] = newDist;
                prev[v] = u;
                heap.push({newDist, v});
            }
        }
    }
}
