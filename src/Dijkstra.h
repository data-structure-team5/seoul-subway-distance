#pragma once
#include <vector>
#include "Graph.h"

// ② 담당: 할리오나 — 다익스트라

const int INF = 1000000000;  // 갈 수 없음

// starts: 출발역의 모든 호선 정점 번호 (모두 0초에서 시작)
// 결과: dist[v] = v까지 최소 시간(초), 못 가면 INF
//       prev[v] = 직전 정점 번호, 출발 정점이면 -1
void dijkstra(const Graph& g, const std::vector<int>& starts,
              std::vector<int>& dist, std::vector<int>& prev);
