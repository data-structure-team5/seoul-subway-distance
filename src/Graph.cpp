#include "Graph.h"
#include <fstream>
#include <limits>
#include <sstream>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

namespace {
// Convert one CP949-encoded CSV line to UTF-8 on Windows.
bool cp949_To_Utf8(const std::string& input, std::string& output) {
    output.clear();
    if (input.empty()) return true;
    if (input.size() > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
        return false;
    }
    int inputLength = static_cast<int>(input.size());

    int wideLength = MultiByteToWideChar(
        949, MB_ERR_INVALID_CHARS, input.data(), inputLength, nullptr, 0);
    if (wideLength == 0) return false;

    std::wstring wide(wideLength, L'\0');
    if (MultiByteToWideChar(949, MB_ERR_INVALID_CHARS, input.data(),
                            inputLength, wide.data(), wideLength) == 0) {
        return false;
    }

    int utf8Length = WideCharToMultiByte(
        CP_UTF8, WC_ERR_INVALID_CHARS, wide.data(), wideLength,
        nullptr, 0, nullptr, nullptr);
    if (utf8Length == 0) return false;

    output.resize(utf8Length);
    if (WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wide.data(),
                            wideLength, output.data(), utf8Length,
                            nullptr, nullptr) == 0) {
        output.clear();
        return false;
    }
    return true;
}

// Remove surrounding ASCII whitespace without changing the station name.
std::string trim(const std::string& text) {
    const std::string whitespace = " \t\r\n";
    std::size_t first = text.find_first_not_of(whitespace);
    if (first == std::string::npos) {
        return "";
    }
    std::size_t last = text.find_last_not_of(whitespace);
    return text.substr(first, last - first + 1);
}

// Call this after converting the CSV text to UTF-8.
std::string normalizeStationName(const std::string& text) {
    std::string name = trim(text);
    if (name == u8"총신대입구") {
        name = u8"이수";
    }
    return name;
}

// Split one CSV record, including quoted fields and escaped quotes.
// Invalid quoting returns false; columns should only be used on success.
bool splitCsvLine(const std::string& line, std::vector<std::string>& columns) {
    columns.clear();
    std::string field;
    bool inQuotes = false;
    bool quoteClosed = false;

    for (std::size_t i = 0; i < line.size(); ++i) {
        char ch = line[i];

        if (inQuotes) {
            if (ch == '"') {
                if (i + 1 < line.size() && line[i + 1] == '"') {
                    field += '"';
                    ++i;
                } else {
                    inQuotes = false;
                    quoteClosed = true;
                }
            } else {
                field += ch;
            }
        } else if (ch == ',') {
            columns.push_back(field);
            field.clear();
            quoteClosed = false;
        } else if (ch == '"') {
            if (!field.empty() || quoteClosed) return false;
            inQuotes = true;
        } else {
            if (quoteClosed) return false;
            field += ch;
        }
    }

    if (inQuotes) return false;
    columns.push_back(field);
    return true;
}

// Read six-column CP949 CSV data, skipping the header and empty lines.
bool readCsvFile(const std::string& path,
                 std::vector<std::vector<std::string>>& rows) {
    rows.clear();
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return false;

    std::string rawLine;
    if (!std::getline(file, rawLine)) return false;
    if (!rawLine.empty() && rawLine.back() == '\r') rawLine.pop_back();

    std::string utf8Line;
    std::vector<std::string> columns;
    if (!cp949_To_Utf8(rawLine, utf8Line) ||
        !splitCsvLine(utf8Line, columns) || columns.size() != 6) {
        return false;
    }

    while (std::getline(file, rawLine)) {
        if (!rawLine.empty() && rawLine.back() == '\r') rawLine.pop_back();
        if (trim(rawLine).empty()) continue;

        if (!cp949_To_Utf8(rawLine, utf8Line) ||
            !splitCsvLine(utf8Line, columns) || columns.size() != 6) {
            return false;
        }
        rows.push_back(columns);
    }

    if (file.bad() || !file.eof()) return false;
    return !rows.empty();
}

// Convert CSV time such as "02:30" to seconds. Invalid input returns -1.
int parseTime(const std::string& text) {
    std::istringstream input(text);
    int minutes;
    int seconds;
    char separator;

    if (!(input >> minutes >> separator >> seconds)) {
        return -1;
    }
    if (separator != ':' || minutes < 0 || seconds < 0 || seconds >= 60) {
        return -1;
    }

    input >> std::ws;
    if (!input.eof()) {
        return -1;
    }

    // Keep the result within the range of an int.
    if (minutes > (std::numeric_limits<int>::max() - seconds) / 60) {
        return -1;
    }
    return minutes * 60 + seconds;
}
}

Graph::~Graph() {
    for (Vertex& vertex : vertices_) {
        Edge* current = vertex.head;
        while (current != nullptr) {
            Edge* next = current->next;
            delete current;
            current = next;
        }
        vertex.head = nullptr;
    }
}

bool Graph::load(const std::string& stationCsv, const std::string& transferCsv) {
    Graph temporary;
    if (!temporary.buildFromCsv(stationCsv, transferCsv)) {
        return false;
    }
    vertices_.swap(temporary.vertices_);
    return true;
}

bool Graph::buildFromCsv(const std::string& stationCsv, const std::string& transferCsv) {
    std::vector<std::vector<std::string>> stationRows;
    std::vector<std::vector<std::string>> transferRows;
    if (!readCsvFile(stationCsv, stationRows) ||
        !readCsvFile(transferCsv, transferRows)) {
        return false;
    }

    std::vector<int> stationIds;
    std::vector<int> travelTimes;
    for (const std::vector<std::string>& row : stationRows) {
        std::string lineText = trim(row[1]);
        if (lineText.size() != 1 || lineText[0] < '1' || lineText[0] > '8') {
            return false;
        }
        int line = lineText[0] - '0';
        std::string name = normalizeStationName(row[2]);
        int seconds = parseTime(row[3]);
        if (name.empty() || seconds < 0) return false;

        int id = findVertex(name, line);
        if (id == -1) {
            id = addVertex(name, line);
        }
        stationIds.push_back(id);
        travelTimes.push_back(seconds);
    }

    bool line6LoopFinished = false;
    for (std::size_t i = 1; i < stationIds.size(); ++i) {
        int from = stationIds[i - 1];
        int to = stationIds[i];
        const Vertex& previous = vertex(from);
        const Vertex& current = vertex(to);

        // A new line starts a separate sequence of stations.
        if (previous.line != current.line) continue;

        // At a branch start, replace the previous CSV station with its junction.
        if (current.line == 2 && current.name == u8"용답") {
            from = findVertex(u8"성수", 2);
        } else if (current.line == 2 && current.name == u8"도림천") {
            from = findVertex(u8"신도림", 2);
        } else if (current.line == 5 && current.name == u8"둔촌동") {
            from = findVertex(u8"강동", 5);
        }
        if (from == -1) return false;

        int cost = travelTimes[i];
        if (cost <= 0 || from == to) return false;
        addEdge(from, to, cost, false);

        // Eungam loop follows CSV order in one direction only.
        // Set the flag after adding the last loop edge (Gusan -> Eungam).
        if (current.line == 6 && !line6LoopFinished) {
            if (current.name == u8"응암") line6LoopFinished = true;
            continue;
        }
        addEdge(to, from, cost, false);
    }

    for (const std::vector<std::string>& row : transferRows) {
        std::string sourceText = trim(row[1]);
        if (sourceText.size() != 1 || sourceText[0] < '1' || sourceText[0] > '8') {
            return false;
        }
        int sourceLine = sourceText[0] - '0';

        std::string targetText = trim(row[3]);
        if (targetText.size() != std::string(u8"1호선").size() ||
            targetText[0] < '1' || targetText[0] > '8' ||
            targetText.substr(1) != u8"호선") {
            continue;
        }
        int targetLine = targetText[0] - '0';

        // Same-line branch transfers cannot be represented by (name, line).
        if (sourceLine == targetLine) continue;

        std::string name = normalizeStationName(row[2]);
        int cost = parseTime(row[5]);
        if (name.empty() || cost < 0) return false;

        int from = findVertex(name, sourceLine);
        int to = findVertex(name, targetLine);
        // The station CSV does not cover every station on the wider network.
        if (from == -1 || to == -1) continue;

        bool alreadyAdded = false;
        for (Edge* edge = edges(from); edge != nullptr; edge = edge->next) {
            if (edge->to == to && edge->transfer) {
                if (edge->cost != cost) return false;
                alreadyAdded = true;
                break;
            }
        }
        if (alreadyAdded) continue;

        addEdge(from, to, cost, true);
        addEdge(to, from, cost, true);
    }
    return true;
}

int Graph::size() const { return static_cast<int>(vertices_.size()); }

const Vertex& Graph::vertex(int v) const { return vertices_[v]; }

Edge* Graph::edges(int v) const { return vertices_[v].head; }

int Graph::findVertex(const std::string& name, int line) const {
    for (int v = 0; v < size(); ++v) {
        if (vertices_[v].name == name && vertices_[v].line == line) {
            return v;
        }
    }
    return -1;
}

int Graph::addVertex(const std::string& name, int line) {
    vertices_.push_back({name, line, nullptr});
    return size() - 1;
}

void Graph::addEdge(int from, int to, int cost, bool transfer) {
    vertices_[from].head = new Edge{to, cost, transfer, vertices_[from].head};
}
