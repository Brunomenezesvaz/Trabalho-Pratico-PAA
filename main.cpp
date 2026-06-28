#include "Graph.h"
#include "TransitiveReductionDFS.h"
#include "TransitiveReductionBFS.h"
#include "Benchmark.h"
#include <iostream>
#include <vector>

void printSection(const std::string& title) {
    std::cout << "\n=== " << title << " ===\n";
}

int main() {
    // Benchmark
    printSection("Benchmark Experimental");
    
    std::vector<int> vertexSizes = {50, 100, 250, 500, 1000};
    std::vector<double> densities = {0.2, 0.5, 0.8};
    
    // Executa o benchmark com 3 rodadas para média
    Benchmark::run(vertexSizes, densities, 3, "benchmark_results.csv");

    return 0;
}
