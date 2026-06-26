#include "Graph.h"
#include "TransitiveReductionDFS.h"
#include "TransitiveReductionBFS.h"
#include "Benchmark.h"
#include <iostream>
#include <vector>

// Constrói o grafo de teste:
//   0 → 1 → 2 → 3
//   0 -------→ 2   (redundante: 0→1→2)
//   1 -------→ 3   (redundante: 1→2→3)
//   0 -------→ 3   (redundante: 0→1→2→3)
Graph buildTestGraph() {
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(0, 3);
    return g;
}

void printSection(const std::string& title) {
    std::cout << "\n=== " << title << " ===\n";
}

int main() {
    // ── Teste de Sanidade ─────────────────────────────────────────────────────
    printSection("Grafo original");
    Graph g1 = buildTestGraph();
    g1.print();
    std::cout << "Arestas: " << g1.numEdges() << "\n";

    printSection("Reducao transitiva - DFS");
    ReductionResult resDFS = TransitiveReductionDFS::reduce(g1);
    g1.print();
    std::cout << "Arestas removidas: " << resDFS.removedEdges << "\n";
    std::cout << "Arestas restantes: " << g1.numEdges() << "\n";
    std::cout << "Operacoes (visitas): " << resDFS.operations << "\n";

    printSection("Grafo original (novo)");
    Graph g2 = buildTestGraph();
    g2.print();
    std::cout << "Arestas: " << g2.numEdges() << "\n";

    printSection("Reducao transitiva - BFS");
    ReductionResult resBFS = TransitiveReductionBFS::reduce(g2);
    g2.print();
    std::cout << "Arestas removidas: " << resBFS.removedEdges << "\n";
    std::cout << "Arestas restantes: " << g2.numEdges() << "\n";
    std::cout << "Operacoes (expansoes/leituras): " << resBFS.operations << "\n";

    printSection("Verificacao de Sanidade");
    bool same = (resDFS.removedEdges == resBFS.removedEdges) && (g1.numEdges() == g2.numEdges());
    std::cout << (same ? "OK - ambos produziram o mesmo resultado de reducao.\n"
                       : "ERRO - resultados divergem!\n");

    // ── Benchmark Experimental Completo ───────────────────────────────────────
    printSection("Benchmark Experimental");
    
    std::vector<int> vertexSizes = {50, 100, 250, 500, 1000};
    std::vector<double> densities = {0.2, 0.5, 0.8};
    
    // Executa o benchmark com 3 rodadas para média
    Benchmark::run(vertexSizes, densities, 3, "benchmark_results.csv");

    return 0;
}
