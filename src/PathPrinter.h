#pragma once
#include <vector>
#include "Graph.h"

// ④ 담당: 성규 — 경로 출력(스택), 연결 검사(큐, BFS)

// target까지의 경로를 prev로 거꾸로 따라가 스택으로 뒤집어 출력
// 소요시간(dist[target])과 환승 횟수도 함께 출력
void printPath(const Graph& g, const std::vector<int>& dist,
               const std::vector<int>& prev, int target);

// BFS로 모든 정점이 이어져 있는지 검사. 끊긴 정점이 있으면 출력하고 false
bool checkConnected(const Graph& g);
