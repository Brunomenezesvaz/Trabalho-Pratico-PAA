#include "Benchmark.h"
#include "TransitiveReductionDFS.h"
#include "TransitiveReductionBFS.h"

#include <chrono>
#include <random>
#include <fstream>
#include <iostream>
#include <iomanip>

Graph Benchmark::geraErdosRenyi(int V, double p, unsigned int seed) {
    Graph g(V);
    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (int u = 0; u < V; ++u) {
        for (int v = 0; v < V; ++v) {
            if (u == v) continue;
            if (dist(rng) < p) {
                g.adicionaAresta(u, v);
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
        csv << "V,p,arestasIniciais,tempoDfsMs,dfsArestasRemovidas,dfsOps,tempoBfsMs,bfsArestasRemovidas,bfsOps\n";
    }

    std::cout << "\nIniciando Benchmark...\n";
    std::cout << "--------------------------------------------------------------------------------\n";
    std::cout << std::left << std::setw(6) << "V"
              << std::setw(6) << "p"
              << std::setw(12) << "Arestas"
              << std::setw(15) << "DFS (ms)"
              << std::setw(15) << "DFS Ops"
              << std::setw(15) << "BFS (ms)"
              << std::setw(15) << "BFS Ops" << "\n";
    std::cout << "--------------------------------------------------------------------------------\n";

    unsigned int seedBase = 42;

    for (int V : vertexSizes) {
        for (double p : densities) {
            double totalDfsTime = 0;
            double totalBfsTime = 0;
            long long totalDfsRemoved = 0;
            long long totalBfsRemoved = 0;
            long long totalDfsOps = 0;
            long long totalBfsOps = 0;
            int arestasIniciais = 0;

            for (int run = 0; run < numRuns; ++run) {
                unsigned int seed = seedBase + run * 1000 + V + static_cast<int>(p * 100);
                Graph g = geraErdosRenyi(V, p, seed);
                if (run == 0) {
                    arestasIniciais = g.numArestas();
                }

                // Testar DFS
                {
                    Graph gDfs = g;
                    auto start = std::chrono::high_resolution_clock::now();
                    ReductionResult res = TransitiveReductionDFS::reducao(gDfs);
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> duration = end - start;
                    
                    totalDfsTime += duration.count();
                    totalDfsRemoved += res.arestasRemovidas;
                    totalDfsOps += res.ops;
                }

                // Testar BFS
                {
                    Graph gBfs = g;
                    auto start = std::chrono::high_resolution_clock::now();
                    ReductionResult res = TransitiveReductionBFS::reducao(gBfs);
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> duration = end - start;

                    totalBfsTime += duration.count();
                    totalBfsRemoved += res.arestasRemovidas;
                    totalBfsOps += res.ops;
                }
            }

            double avgDfsTime = totalDfsTime / numRuns;
            double avgBfsTime = totalBfsTime / numRuns;
            int avgDfsRemoved = static_cast<int>(totalDfsRemoved / numRuns);
            int avgBfsRemoved = static_cast<int>(totalBfsRemoved / numRuns);
            long long avgDfsOps = totalDfsOps / numRuns;
            long long avgBfsOps = totalBfsOps / numRuns;

            TestResult res{V, p, arestasIniciais, avgDfsTime, avgDfsRemoved, avgDfsOps, avgBfsTime, avgBfsRemoved, avgBfsOps};
            results.push_back(res);

            if (csv.is_open()) {
                csv << V << "," << p << "," << arestasIniciais << ","
                    << avgDfsTime << "," << avgDfsRemoved << "," << avgDfsOps << ","
                    << avgBfsTime << "," << avgBfsRemoved << "," << avgBfsOps << "\n";
            }

            std::cout << std::left << std::setw(6) << V
                       << std::setw(6) << p
                       << std::setw(12) << arestasIniciais
                       << std::setw(15) << std::fixed << std::setprecision(2) << avgDfsTime
                       << std::setw(15) << avgDfsOps
                       << std::setw(15) << std::fixed << std::setprecision(2) << avgBfsTime
                       << std::setw(15) << avgBfsOps << "\n";
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
    // Apenas informativo
}
