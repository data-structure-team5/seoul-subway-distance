#pragma once
#include <string>
#include <vector>
#include "Graph.h"

// ⑤ 담당: 원용 — 약속장소 순위(병합 정렬)

struct Candidate {
    std::string name;              // 역 이름
    int maxTime;                   // 가장 오래 걸리는 사람의 시간(초)
    int sumTime;                   // 모든 사람의 시간 합(초)
    std::vector<int> arriveVertex; // 사람별로 가장 빨리 도착하는 정점 (경로 출력용)
};

// dist[p][v]: p번 사람이 정점 v까지 가는 최소 시간
// maxTime이 짧은 순, 같으면 sumTime이 짧은 순으로 병합 정렬해 상위 topK개 반환
std::vector<Candidate> rankMeetingPlaces(const Graph& g,
                                         const std::vector<std::vector<int>>& dist,
                                         int topK = 5);
