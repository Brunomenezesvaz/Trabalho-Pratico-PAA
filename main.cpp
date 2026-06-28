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
    g.adicionaAresta(0, 1);
    g.adicionaAresta(1, 2);
    g.adicionaAresta(2, 3);
    g.adicionaAresta(0, 2);
    g.adicionaAresta(1, 3);
    g.adicionaAresta(0, 3);
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
    std::cout << "Arestas: " << g1.numArestas() << "\n";

    printSection("Reducao transitiva - DFS");
    ReductionResult resDFS = TransitiveReductionDFS::reducao(g1);
    g1.print();
    std::cout << "Arestas removidas: " << resDFS.arestasRemovidas << "\n";
    std::cout << "Arestas restantes: " << g1.numArestas() << "\n";
    std::cout << "Operacoes (visitas): " << resDFS.ops << "\n";

    printSection("Grafo original (novo)");
    Graph g2 = buildTestGraph();
    g2.print();
    std::cout << "Arestas: " << g2.numArestas() << "\n";

    printSection("Reducao transitiva - BFS");
    ReductionResult resBFS = TransitiveReductionBFS::reducao(g2);
    g2.print();
    std::cout << "Arestas removidas: " << resBFS.arestasRemovidas << "\n";
    std::cout << "Arestas restantes: " << g2.numArestas() << "\n";
    std::cout << "Operacoes (expansoes/leituras): " << resBFS.ops << "\n";

    printSection("Verificacao de Sanidade");
    bool same = (resDFS.arestasRemovidas == resBFS.arestasRemovidas) && (g1.numArestas() == g2.numArestas());
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
