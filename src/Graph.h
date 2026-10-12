#pragma once
#include <string>
#include <vector>

// ① 담당: 칸 — CSV 읽기 + 노선도 그래프(인접 리스트)
// 시간 단위는 모두 초(정수)로 통일한다. 예) "02:30" -> 150

struct Edge {
    int to;         // 도착 정점 번호
    int cost;       // 걸리는 시간(초)
    bool transfer;  // 환승 간선이면 true
    Edge* next;     // 다음 간선 (연결 리스트)
};

struct Vertex {
    std::string name;  // 역 이름 (앞뒤 공백 제거, 총신대입구 -> 이수 통일)
    int line;          // 호선 (1~8)
    Edge* head;        // 이 정점에서 나가는 첫 간선
};

class Graph {
public:
    ~Graph();
    // 두 CSV를 읽어 그래프를 만든다. 실패하면 false
    bool load(const std::string& stationCsv, const std::string& transferCsv);
    int size() const;                   // 정점 개수 V
    const Vertex& vertex(int v) const;  // 정점 v의 정보
    Edge* edges(int v) const;           // 정점 v의 첫 간선 (없으면 nullptr)

private:
    std::vector<Vertex> vertices_;
    int addVertex(const std::string& name, int line);
    void addEdge(int from, int to, int cost, bool transfer);
};
