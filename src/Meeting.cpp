#include "Meeting.h"
#include <algorithm>
#include <utility>
#include "Dijkstra.h"
#include "StationBST.h"

namespace {
// 가장 오래 걸리는 사람의 시간이 짧은 순, 같으면 시간 합이 짧은 순
bool comesBefore(const Candidate& a, const Candidate& b) {
    if (a.maxTime != b.maxTime) return a.maxTime < b.maxTime;
    return a.sumTime < b.sumTime;
}

// 정렬된 [lo, mid)와 [mid, hi)를 합친다.
// 순위가 같으면 왼쪽을 먼저 넣어 원래 순서를 유지한다(안정 정렬).
void merge(std::vector<Candidate>& a, std::vector<Candidate>& tmp,
           int lo, int mid, int hi) {
    int i = lo, j = mid, k = lo;
    while (i < mid && j < hi) {
        if (comesBefore(a[j], a[i])) {
            tmp[k++] = std::move(a[j++]);
        } else {
            tmp[k++] = std::move(a[i++]);
        }
    }
    while (i < mid) tmp[k++] = std::move(a[i++]);
    while (j < hi) tmp[k++] = std::move(a[j++]);
    for (k = lo; k < hi; k++) a[k] = std::move(tmp[k]);
}

// [lo, hi)를 절반씩 나눠 정렬한 뒤 합친다. O(n log n)
void mergeSort(std::vector<Candidate>& a, std::vector<Candidate>& tmp,
               int lo, int hi) {
    if (hi - lo <= 1) return;
    int mid = lo + (hi - lo) / 2;
    mergeSort(a, tmp, lo, mid);
    mergeSort(a, tmp, mid, hi);
    merge(a, tmp, lo, mid, hi);
}
}  // namespace

std::vector<Candidate> rankMeetingPlaces(const Graph& g,
                                         const std::vector<std::vector<int>>& dist,
                                         int topK) {
    if (dist.empty() || topK <= 0) return {};

    // 1. 역 이름별로 정점 묶기 (같은 이름의 호선 정점들이 한 노드에 모인다)
    StationBST bst;
    std::vector<std::string> names;  // 처음 나온 순서대로 고유한 역 이름
    for (int v = 0; v < g.size(); v++) {
        const std::string& name = g.vertex(v).name;
        if (bst.find(name) == nullptr) names.push_back(name);
        bst.insert(name, v);
    }

    // 2. 역마다 사람별 최소 시간 -> max, sum 계산
    std::vector<Candidate> candidates;
    for (const std::string& name : names) {
        const std::vector<int>& vertices = *bst.find(name);
        Candidate c{name, 0, 0, {}};
        bool reachable = true;
        for (const std::vector<int>& d : dist) {
            // 이 사람이 이 역의 여러 호선 정점 중 가장 빨리 닿는 곳
            int best = INF, bestVertex = -1;
            for (int v : vertices) {
                if (d[v] < best) {
                    best = d[v];
                    bestVertex = v;
                }
            }
            if (best == INF) {  // 한 명이라도 못 가면 후보에서 뺀다
                reachable = false;
                break;
            }
            c.maxTime = std::max(c.maxTime, best);
            c.sumTime += best;
            c.arriveVertex.push_back(bestVertex);
        }
        if (reachable) candidates.push_back(std::move(c));
    }

    // 3. 병합 정렬 후 상위 topK개
    std::vector<Candidate> tmp(candidates.size());
    mergeSort(candidates, tmp, 0, static_cast<int>(candidates.size()));
    if (static_cast<int>(candidates.size()) > topK) candidates.resize(topK);
    return candidates;
}
