#pragma once

#include "Graph.h"
#include <vector>

class TransitiveReductionBFS {
public:
    // Aplica a redução transitiva in-place sobre o grafo  g e retorna as arestas removidas e contagem de operações
    static ReductionResult reducao(Graph& g);

private:
    // BFS a partir do vertice origem no grafo g usando estruturas pré-alocadas
    static bool bfs(const Graph& g, int origem, int alvo,
                    std::vector<int>& q, std::vector<bool>& visitado,
                    long long& ops);
};
