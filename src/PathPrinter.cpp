#include "PathPrinter.h"
#include <iostream>
#include <string>
#include "Dijkstra.h"
#include "Queue.h"
#include "Stack.h"

namespace {
// 초 -> "18분 30초" (0초면 "18분")
std::string formatTime(int seconds) {
    std::string s = std::to_string(seconds / 60) + "분";
    if (seconds % 60 != 0) s += " " + std::to_string(seconds % 60) + "초";
    return s;
}
}  // namespace

void printPath(const Graph& g, const std::vector<int>& dist,
               const std::vector<int>& prev, int target) {
    int n = g.size();
    if (target < 0 || target >= n || static_cast<int>(dist.size()) != n ||
        static_cast<int>(prev.size()) != n) {
        std::cout << "경로를 출력할 수 없습니다. (잘못된 입력)\n";
        return;
    }
    if (dist[target] >= INF) {
        std::cout << g.vertex(target).name << ": 갈 수 없는 역입니다.\n";
        return;
    }

    // 1. 도착역에서 prev를 따라 출발역까지 거꾸로 가며 스택에 넣는다.
    //    호선이 바뀌는 곳(같은 역의 다른 호선 정점)이 환승이다.
    Stack st;
    int transfers = 0;
    int start = target;
    for (int v = target; v != -1; v = prev[v]) {
        if (st.size() == n) {  // 경로는 V개를 넘을 수 없다. 넘으면 prev가 순환한다
            std::cout << "경로를 출력할 수 없습니다. (prev 오류)\n";
            return;
        }
        st.push(v);
        start = v;
        if (prev[v] != -1 && g.vertex(prev[v]).line != g.vertex(v).line) transfers++;
    }

    std::cout << g.vertex(start).name << " → " << g.vertex(target).name << " : "
              << formatTime(dist[target]) << ", 환승 " << transfers << "번\n";

    // 2. 스택에서 꺼내면 출발역부터 순서대로 나온다. 호선별로 끊어 출력한다.
    int cur = st.pop();
    std::cout << "  " << g.vertex(cur).line << "호선: " << g.vertex(cur).name;
    while (!st.empty()) {
        int next = st.pop();
        if (g.vertex(next).line != g.vertex(cur).line) {
            std::cout << "\n  (환승 " << g.vertex(cur).line << "호선 → "
                      << g.vertex(next).line << "호선)\n  "
                      << g.vertex(next).line << "호선: " << g.vertex(next).name;
        } else {
            std::cout << " → " << g.vertex(next).name;
        }
        cur = next;
    }
    std::cout << "\n";
}

bool checkConnected(const Graph& g) {
    int n = g.size();
    if (n == 0) {
        std::cout << "연결 검사: 정점이 없습니다.\n";
        return false;
    }

    // 0번 정점에서 BFS: 큐에서 꺼낸 정점의 이웃 중 처음 보는 정점을 표시하고 넣는다.
    DynamicArray<bool> visited;
    for (int v = 0; v < n; v++) visited.push_back(false);
    Queue q(n);
    visited[0] = true;
    q.push(0);
    int count = 1;
    while (!q.empty()) {
        int v = q.pop();
        for (Edge* e = g.edges(v); e; e = e->next) {
            if (!visited[e->to]) {
                visited[e->to] = true;
                q.push(e->to);
                count++;
            }
        }
    }
    if (count == n) return true;

    // 한 번도 표시되지 않은 정점 = 0번 정점과 이어지지 않은 정점
    std::cout << "연결 검사: 이어지지 않은 정점 " << n - count << "개\n";
    for (int v = 0; v < n; v++) {
        if (!visited[v]) {
            std::cout << "  " << g.vertex(v).name << " (" << g.vertex(v).line << "호선)\n";
        }
    }
    return false;
}
