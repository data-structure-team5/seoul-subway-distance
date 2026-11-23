// 서울 지하철 약속장소 추천 및 경로 안내 프로그램
// 빌드: g++ -std=c++17 -O2 -static src/*.cpp -o subway   (macOS: -static 대신 -liconv, README 참고)
// 실행: ./subway   (data 폴더가 있는 위치에서)

#include <iostream>
#include <string>
#include "Graph.h"
#include "StationBST.h"
#include "Dijkstra.h"
#include "PathPrinter.h"
#include "Meeting.h"
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace {
const int MIN_PEOPLE = 2;
const int MAX_PEOPLE = 10;
const int TOP_K = 5;

std::string trim(const std::string& text) {
    const std::string whitespace = " \t\r\n";
    std::size_t first = text.find_first_not_of(whitespace);
    if (first == std::string::npos) return "";
    std::size_t last = text.find_last_not_of(whitespace);
    return text.substr(first, last - first + 1);
}

// 한 줄 읽기. 입력이 끝나면 false
bool readLine(const std::string& prompt, std::string& line) {
    std::cout << prompt << std::flush;
#ifdef _WIN32
    // Windows 콘솔은 한글을 CP949로 주므로, UTF-16으로 읽어 UTF-8로 바꾼다. (역 이름은 UTF-8)
    HANDLE in = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    if (GetConsoleMode(in, &mode)) {
        wchar_t buf[256];
        DWORD n = 0;
        if (!ReadConsoleW(in, buf, 255, &n, nullptr) || n == 0) return false;
        int len = WideCharToMultiByte(CP_UTF8, 0, buf, static_cast<int>(n),
                                      nullptr, 0, nullptr, nullptr);
        line.assign(len, '\0');
        if (len > 0) {
            WideCharToMultiByte(CP_UTF8, 0, buf, static_cast<int>(n),
                                &line[0], len, nullptr, nullptr);
        }
        line = trim(line);
        return true;
    }
#endif
    if (!std::getline(std::cin, line)) return false;
    line = trim(line);
    return true;
}

// 숫자가 아니면 -1
int toNumber(const std::string& text) {
    if (text.empty() || text.size() > 6) return -1;
    int x = 0;
    for (char c : text) {
        if (c < '0' || c > '9') return -1;
        x = x * 10 + (c - '0');
    }
    return x;
}

// 역 이름 -> 그 역의 모든 호선 정점. "강남역"처럼 끝에 "역"을 붙여도 찾는다.
const StationBST::VertexList* findStation(const StationBST& bst, const std::string& name) {
    if (name == "총신대입구" || name == "총신대입구역") return bst.find("이수");
    const StationBST::VertexList* found = bst.find(name);
    const std::string suffix = "역";
    if (found == nullptr && name.size() > suffix.size() &&
        name.compare(name.size() - suffix.size(), suffix.size(), suffix) == 0) {
        found = bst.find(name.substr(0, name.size() - suffix.size()));
    }
    return found;
}
}  // namespace

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

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

    // 3. 입력: 출발역 k곳, 도착역(선택)
    std::string line;
    int k = -1;
    while (k < MIN_PEOPLE || k > MAX_PEOPLE) {
        if (!readLine("몇 명이 만나나요? (" + std::to_string(MIN_PEOPLE) + "~" +
                          std::to_string(MAX_PEOPLE) + "): ", line)) return 0;
        k = toNumber(line);
    }

    DynamicArray<std::string> startNames;
    DynamicArray<const StationBST::VertexList*> starts;
    for (int p = 0; p < k; p++) {
        const StationBST::VertexList* found = nullptr;
        while (found == nullptr) {
            if (!readLine(std::to_string(p + 1) + "번째 사람 출발역: ", line)) return 0;
            found = findStation(bst, line);
            if (found == nullptr) std::cout << "  역을 찾지 못했습니다. 다시 입력해 주세요.\n";
        }
        startNames.push_back(g.vertex((*found)[0]).name);
        starts.push_back(found);
    }

    const StationBST::VertexList* fixedTarget = nullptr;
    while (true) {
        if (!readLine("도착역을 정했으면 입력하고, 추천을 받으려면 Enter: ", line)) return 0;
        if (line.empty()) break;
        fixedTarget = findStation(bst, line);
        if (fixedTarget != nullptr) break;
        std::cout << "  역을 찾지 못했습니다. 다시 입력해 주세요.\n";
    }

    // 4. 사람마다 다익스트라 (할리오나)
    DynamicArray<DynamicArray<int>> dists, prevs;
    for (int p = 0; p < k; p++) {
        DynamicArray<int> dist, prev;
        dijkstra(g, *starts[p], dist, prev);
        dists.push_back(dist);
        prevs.push_back(prev);
    }

    // 5. 사람마다 도착할 정점 정하기
    DynamicArray<int> targets;
    if (fixedTarget != nullptr) {
        // 도착역이 정해져 있으면: 사람마다 그 역의 호선 정점 중 가장 빨리 닿는 곳
        for (int p = 0; p < k; p++) {
            int best = (*fixedTarget)[0];
            for (int v : *fixedTarget) {
                if (dists[p][v] < dists[p][best]) best = v;
            }
            targets.push_back(best);
        }
    } else {
        // 도착역이 없으면 약속장소 순위 (원용)
        DynamicArray<Candidate> top = rankMeetingPlaces(g, dists, TOP_K);
        if (top.empty()) {
            std::cout << "모두가 갈 수 있는 역이 없습니다.\n";
            return 0;
        }
        std::cout << "\n추천 약속장소 (가장 오래 걸리는 사람의 시간이 짧은 순)\n";
        for (int i = 0; i < static_cast<int>(top.size()); i++) {
            std::cout << "  " << i + 1 << ". " << top[i].name
                      << "  최대 " << formatTime(top[i].maxTime)
                      << ", 합계 " << formatTime(top[i].sumTime) << "\n";
        }
        int choice = -1;
        while (choice < 1 || choice > static_cast<int>(top.size())) {
            if (!readLine("몇 번 역으로 갈까요? (Enter면 1번): ", line)) return 0;
            choice = line.empty() ? 1 : toNumber(line);
        }
        targets = top[choice - 1].arriveVertex;
    }

    // 6. 사람별 경로 출력 (성규)
    std::cout << "\n약속장소: " << g.vertex(targets[0]).name << "\n";
    for (int p = 0; p < k; p++) {
        std::cout << "\n[" << p + 1 << "번째 사람: " << startNames[p] << "]\n";
        printPath(g, dists[p], prevs[p], targets[p]);
    }
    return 0;
}
