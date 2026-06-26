#pragma once

#include "Graph.h"
#include <vector>

/**
 * Redução transitiva por DFS (Busca em Profundidade).
 *
 * Para cada aresta (u → v):
 *   1. Exclui temporariamente a aresta via flag.
 *   2. Executa DFS a partir de u.
 *   3. Se v for alcançado → aresta é redundante → remove permanentemente.
 *   4. Caso contrário → restaura a aresta.
 *
 * Complexidade: O(E · (V + E))
 *   - E iterações (uma por aresta)
 *   - Cada DFS custa O(V + E)
 */
class TransitiveReductionDFS {
public:
    /**
     * Aplica a redução transitiva in-place sobre o grafo g.
     * Retorna o resultado da redução (arestas removidas e contagem de operações).
     */
    static ReductionResult reduce(Graph& g);

private:
    /**
     * DFS a partir de `start` no grafo g.
     * Retorna true se `target` for alcançado.
     */
    static bool dfs(const Graph& g, int current, int target,
                    std::vector<bool>& visited, long long& operations);
};
