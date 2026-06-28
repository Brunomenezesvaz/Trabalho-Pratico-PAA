#pragma once

#include "Graph.h"
#include <vector>

/**
 * Redução transitiva por BFS (Busca em Largura).
 *
 * Para cada aresta (u → v):
 *   1. Exclui temporariamente a aresta via flag.
 *   2. Executa BFS a partir de u.
 *   3. Se v for alcançado → aresta é redundante → remove permanentemente.
 *   4. Caso contrário → restaura a aresta.
 *
 * Complexidade: O(E · (V + E))
 *   - E iterações (uma por aresta)
 *   - Cada BFS custa O(V + E)
 */
class TransitiveReductionBFS {
public:
    /**
     * Aplica a redução transitiva in-place sobre o grafo g.
     * Retorna o resultado da redução (arestas removidas e contagem de operações).
     */
    static ReductionResult reducao(Graph& g);

private:
    /**
     * BFS a partir de `start` no grafo g usando estruturas pré-alocadas.
     * Retorna true se `target` for alcançado.
     */
    static bool bfs(const Graph& g, int origem, int alvo,
                    std::vector<int>& q, std::vector<bool>& visitado,
                    long long& ops);
};
