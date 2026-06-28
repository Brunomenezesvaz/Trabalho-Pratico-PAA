#pragma once

#include "Graph.h"
#include <string>
#include <vector>

struct TestResult {
    int V;
    double p;
    int arestasIniciais;
    
    // DFS Results
    double tempoDfsMs;
    int dfsArestasRemovidas;
    long long dfsOps;
    
    // BFS Results
    double tempoBfsMs;
    int bfsArestasRemovidas;
    long long bfsOps;
};

class Benchmark {
public:
    /**
     * Gera um grafo direcionado usando o modelo de Erdős-Rényi.
     * Para cada par (u, v) com u != v, adiciona a aresta com probabilidade p.
     */
    static Graph geraErdosRenyi(int V, double p, unsigned int seed);

    /**
     * Executa a bateria de testes definida no enunciado.
     * V = {50, 100, 250, 500, 1000}
     * p = {0.2, 0.5, 0.8}
     * Salva os resultados em um arquivo CSV para o relatório.
     */
    static std::vector<TestResult> run(const std::vector<int>& vertexSizes,
                                      const std::vector<double>& densities,
                                      int numRuns = 3,
                                      const std::string& csvFilename = "benchmark_results.csv");

    /**
     * Imprime os resultados formatados em tabelas no terminal.
     */
    static void printResultsTable(const std::vector<TestResult>& results);
};
