// 서울 지하철 약속장소 추천 및 경로 안내 프로그램
// 빌드: g++ -std=c++17 -O2 src/*.cpp -o subway
// 실행: ./subway   (data 폴더가 있는 위치에서)

#include <iostream>
#include <string>
#include <vector>
#include "Graph.h"
#include "StationBST.h"
#include "Dijkstra.h"
#include "PathPrinter.h"
#include "Meeting.h"

int main() {
    // 1. 그래프 만들기 (칸)
    Graph g;
    if (!g.load("data/station_distance.csv", "data/transfer.csv")) {
        std::cout << "데이터를 읽지 못했습니다.\n";
        return 1;
    }
    checkConnected(g);

    // 2. 역 이름 사전 (성원)
    StationBST bst;
    for (int v = 0; v < g.size(); v++) bst.insert(g.vertex(v).name, v);

    // 3. 입력 (성원): 출발역 k곳, 도착역(선택)
    // TODO

    // 4. 사람마다 다익스트라 (할리오나)
    // TODO

    // 5. 도착역이 없으면 약속장소 순위 (원용)
    // TODO

    // 6. 사람별 경로 출력 (성규)
    // TODO

    return 0;
}
