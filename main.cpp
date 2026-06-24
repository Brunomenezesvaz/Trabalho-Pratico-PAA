#include "Graph.h"
#include "TransitiveReductionDFS.h"
#include "TransitiveReductionBF.h"
#include <iostream>
 
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
    // ── Teste DFS ─────────────────────────────────────────────────────────────
    printSection("Grafo original");
    Graph g1 = buildTestGraph();
    g1.print();
    std::cout << "Arestas: " << g1.numEdges() << "\n";
 
    printSection("Redução transitiva — DFS");
    int removedDFS = TransitiveReductionDFS::reduce(g1);
    g1.print();
    std::cout << "Arestas removidas: " << removedDFS << "\n";
    std::cout << "Arestas restantes: " << g1.numEdges() << "\n";
 
    // ── Teste Bellman-Ford ────────────────────────────────────────────────────
    printSection("Grafo original (novo)");
    Graph g2 = buildTestGraph();
    g2.print();
    std::cout << "Arestas: " << g2.numEdges() << "\n";
 
    printSection("Redução transitiva — Bellman-Ford");
    int removedBF = TransitiveReductionBF::reduce(g2);
    g2.print();
    std::cout << "Arestas removidas: " << removedBF << "\n";
    std::cout << "Arestas restantes: " << g2.numEdges() << "\n";
 
    // ── Sanidade: os dois devem produzir o mesmo resultado ────────────────────
    printSection("Verificação");
    bool same = (removedDFS == removedBF) && (g1.numEdges() == g2.numEdges());
    std::cout << (same ? "OK — ambos produziram o mesmo resultado.\n"
                       : "ERRO — resultados divergem!\n");
 
    return 0;
}
