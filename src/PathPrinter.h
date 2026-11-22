#pragma once
#include <string>
#include "DynamicArray_vertor.h"
#include "Graph.h"

// ④ 담당: 성규 — 경로 출력(스택), 연결 검사(큐, BFS)

// target까지의 경로를 prev로 거꾸로 따라가 스택으로 뒤집어 출력
// 소요시간(dist[target])과 환승 횟수도 함께 출력
void printPath(const Graph& g, const DynamicArray<int>& dist,
               const DynamicArray<int>& prev, int target);

// 초 -> "18분 30초" (0초면 "18분")
std::string formatTime(int seconds);

// BFS로 모든 정점이 이어져 있는지 검사. 끊긴 정점이 있으면 출력하고 false
bool checkConnected(const Graph& g);
