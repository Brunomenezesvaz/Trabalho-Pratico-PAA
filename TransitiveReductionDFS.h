#pragma once

#include "Graph.h"
#include <vector>

class TransitiveReductionDFS {
public:
    // Aplica a redução transitiva in-place sobre o grafo g retorna as arestas removidas e contagem de operações
    static ReductionResult reducao(Graph& g);

private:
    // DFS
    static bool dfs(const Graph& g, int atual, int alvo,
                    std::vector<bool>& visitado, long long& ops);
};
