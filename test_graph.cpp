#include <iostream>
#include "src/Graph.h"

int main() {
    Graph g;

    if (!g.load("data/station_distance.csv", "data/transfer.csv")) {
        std::cout << "Load CSV failed\n";
        return 1;
    }

    std::cout << "Load CSV OK\n";
    std::cout << "Total vertices: " << g.size() << "\n";

    // In 5 đỉnh đầu tiên và các cạnh đi ra từ mỗi đỉnh.
    for (int v = 0; v < g.size() && v < 5; ++v) {
        const auto& station = g.vertex(v);
        std::cout << "\n[" << v << "] " << station.name
                  << " | Line " << station.line << "\n";

        for (Edge* e = g.edges(v); e; e = e->next) {
            const auto& next = g.vertex(e->to);
            std::cout << "  -> " << next.name
                      << " | Line " << next.line
                      << " | " << e->cost << " seconds"
                      << " | transfer=" << e->transfer << "\n";
        }
    }
}