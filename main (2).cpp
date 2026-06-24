#include "Graph.h"
#include <iostream>

int main() {
    // Grafo do exemplo da discussão:
    //   0 → 1 → 2
    //   0 → 2   (aresta redundante — será removida pela redução transitiva)
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(0, 2);   // redundante
    g.addEdge(2, 3);
    g.addEdge(1, 3);   // redundante (1→2→3)

    std::cout << "=== Grafo original ===\n";
    g.print();
    std::cout << "Vértices: " << g.numVertices()
              << "  Arestas: " << g.numEdges() << "\n\n";

    // Teste de consultas
    std::cout << "hasEdge(0,2): " << g.hasEdge(0, 2) << "\n";
    std::cout << "hasEdge(0,3): " << g.hasEdge(0, 3) << "\n";

    // Remoção manual de aresta redundante (placeholder — o algoritmo fará isso)
    g.removeEdge(0, 2);
    std::cout << "\n=== Após remover aresta 0→2 ===\n";
    g.print();
    std::cout << "Arestas: " << g.numEdges() << "\n";

    return 0;
}
