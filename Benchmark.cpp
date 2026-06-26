#include "Benchmark.h"
#include "TransitiveReductionDFS.h"
#include "TransitiveReductionBF.h"

#include <chrono>
#include <random>
#include <fstream>
#include <iostream>
#include <iomanip>

Graph Benchmark::generateErdosRenyi(int V, double p, unsigned int seed) {
    Graph g(V);
    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (int u = 0; u < V; ++u) {
        for (int v = 0; v < V; ++v) {
            if (u == v) continue;
            if (dist(rng) < p) {
                g.addEdge(u, v);
            }
        }
    }
    return g;
}

std::vector<TestResult> Benchmark::run(const std::vector<int>& vertexSizes,
                                      const std::vector<double>& densities,
                                      int numRuns,
                                      const std::string& csvFilename) {
    std::vector<TestResult> results;
    
    std::ofstream csv(csvFilename);
    if (csv.is_open()) {
        csv << "V,p,initialEdges,dfsTimeMs,dfsRemoved,dfsOps,bfTimeMs,bfRemoved,bfOps\n";
    }

    std::cout << "\nIniciando Benchmark...\n";
    std::cout << "--------------------------------------------------------------------------------\n";
    std::cout << std::left << std::setw(6) << "V"
              << std::setw(6) << "p"
              << std::setw(12) << "Arestas"
              << std::setw(15) << "DFS (ms)"
              << std::setw(15) << "DFS Ops"
              << std::setw(15) << "BF (ms)"
              << std::setw(15) << "BF Ops" << "\n";
    std::cout << "--------------------------------------------------------------------------------\n";

    unsigned int seedBase = 42;

    for (int V : vertexSizes) {
        for (double p : densities) {
            double totalDfsTime = 0;
            double totalBfTime = 0;
            long long totalDfsRemoved = 0;
            long long totalBfRemoved = 0;
            long long totalDfsOps = 0;
            long long totalBfOps = 0;
            int initialEdges = 0;

            // Decidir se o Bellman-Ford deve ser pulado por complexidade
            // Estimativa: V * E^2. Se V >= 250 e p >= 0.5, pulamos BF para evitar travar.
            bool skipBF = false;
            long long estimatedEdges = static_cast<long long>(V) * (V - 1) * p;
            if (V >= 250 && (p >= 0.5 || V > 250)) {
                skipBF = true;
            }

            for (int run = 0; run < numRuns; ++run) {
                unsigned int seed = seedBase + run * 1000 + V + static_cast<int>(p * 100);
                Graph g = generateErdosRenyi(V, p, seed);
                if (run == 0) {
                    initialEdges = g.numEdges();
                }

                // Testar DFS
                {
                    Graph gDfs = g;
                    auto start = std::chrono::high_resolution_clock::now();
                    ReductionResult res = TransitiveReductionDFS::reduce(gDfs);
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> duration = end - start;
                    
                    totalDfsTime += duration.count();
                    totalDfsRemoved += res.removedEdges;
                    totalDfsOps += res.operations;
                }

                // Testar Bellman-Ford
                if (!skipBF) {
                    Graph gBf = g;
                    auto start = std::chrono::high_resolution_clock::now();
                    ReductionResult res = TransitiveReductionBF::reduce(gBf);
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> duration = end - start;

                    totalBfTime += duration.count();
                    totalBfRemoved += res.removedEdges;
                    totalBfOps += res.operations;
                }
            }

            double avgDfsTime = totalDfsTime / numRuns;
            double avgBfTime = skipBF ? -1.0 : (totalBfTime / numRuns);
            int avgDfsRemoved = static_cast<int>(totalDfsRemoved / numRuns);
            int avgBfRemoved = skipBF ? -1 : static_cast<int>(totalBfRemoved / numRuns);
            long long avgDfsOps = totalDfsOps / numRuns;
            long long avgBfOps = skipBF ? -1 : (totalBfOps / numRuns);

            TestResult res{V, p, initialEdges, avgDfsTime, avgDfsRemoved, avgDfsOps, avgBfTime, avgBfRemoved, avgBfOps};
            results.push_back(res);

            if (csv.is_open()) {
                csv << V << "," << p << "," << initialEdges << ","
                    << avgDfsTime << "," << avgDfsRemoved << "," << avgDfsOps << ",";
                if (skipBF) {
                    csv << "N/A,N/A,N/A\n";
                } else {
                    csv << avgBfTime << "," << avgBfRemoved << "," << avgBfOps << "\n";
                }
            }

            std::cout << std::left << std::setw(6) << V
                      << std::setw(6) << p
                      << std::setw(12) << initialEdges
                      << std::setw(15) << std::fixed << std::setprecision(2) << avgDfsTime
                      << std::setw(15) << avgDfsOps;
            if (skipBF) {
                std::cout << std::setw(15) << "PULADO"
                          << std::setw(15) << "PULADO" << "\n";
            } else {
                std::cout << std::setw(15) << std::fixed << std::setprecision(2) << avgBfTime
                          << std::setw(15) << avgBfOps << "\n";
            }
        }
    }

    if (csv.is_open()) {
        csv.close();
    }
    std::cout << "--------------------------------------------------------------------------------\n";
    std::cout << "Resultados salvos em " << csvFilename << "\n";
    return results;
}

void Benchmark::printResultsTable(const std::vector<TestResult>& results) {
    // Apenas informativo, já imprimimos durante a execução do Benchmark::run
}
