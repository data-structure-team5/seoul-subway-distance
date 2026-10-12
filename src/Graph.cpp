#include "Graph.h"

Graph::~Graph() {
    // TODO: 모든 간선 메모리 해제
}

bool Graph::load(const std::string& stationCsv, const std::string& transferCsv) {
    // TODO: 역간거리 CSV 읽기 (CP949 -> UTF-8, "mm:ss" -> 초)
    // TODO: 순환선(2호선 시청, 6호선 응암), 지선 3곳 처리
    // TODO: 환승 CSV 읽기 (1~8호선만, 양방향 간선)
    (void)stationCsv; (void)transferCsv;
    return false;
}

int Graph::size() const { return static_cast<int>(vertices_.size()); }

const Vertex& Graph::vertex(int v) const { return vertices_[v]; }

Edge* Graph::edges(int v) const { return vertices_[v].head; }

int Graph::addVertex(const std::string& name, int line) {
    vertices_.push_back({name, line, nullptr});
    return size() - 1;
}

void Graph::addEdge(int from, int to, int cost, bool transfer) {
    vertices_[from].head = new Edge{to, cost, transfer, vertices_[from].head};
}
