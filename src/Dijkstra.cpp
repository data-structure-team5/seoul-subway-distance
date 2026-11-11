
#include "Dijkstra.h"
#include "MinHeap.h"

void dijkstra(const Graph& g, const std::vector<int>& starts,
              std::vector<int>& dist, std::vector<int>& prev) {
    const int n = g.size();

    dist.assign(n, INF);
    prev.assign(n, -1);

    MinHeap heap;

    // Бүх эхлэх өртөө 0 секундээс эхэлнэ.
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

    // Хамгийн бага хугацаатай өртөөг эхэлж боловсруулна.
    while (!heap.empty()) {
        HeapItem current = heap.pop();

        int d = current.dist;
        int u = current.vertex;

        // Хуучирсан heap элементийг алгасна.
        if (d != dist[u]) {
            continue;
        }

        // u өртөөнөөс гарах бүх холбоосыг шалгана.
        for (Edge* e = g.edges(u); e != nullptr; e = e->next) {
            int v = e->to;
            int cost = e->cost;

            if (v < 0 || v >= n || cost < 0) {
                continue;
            }

            // INF-ээс хэтрэх тооцооллоос хамгаална.
            if (d > INF - cost) {
                continue;
            }

            int newDist = d + cost;

            // Илүү богино зам олдвол утгуудыг шинэчилнэ.
            if (newDist < dist[v]) {
                dist[v] = newDist;
                prev[v] = u;
                heap.push({newDist, v});
            }
        }
    }
}
