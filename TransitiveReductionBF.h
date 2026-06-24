#pragma once

#include "Graph.h"
#include <vector>

/**
 * Redução transitiva por Bellman-Ford adaptado.
 *
 * O Bellman-Ford é usado aqui como detector de alcançabilidade por caminhos
 * alternativos — não para otimização de pesos negativos. Todas as arestas
 * têm peso 1; buscamos apenas saber se dist[v] < INF após V-1 relaxamentos.
 *
 * Para cada aresta (u → v):
 *   1. Exclui temporariamente a aresta via flag.
 *   2. Inicializa dist[u] = 0, dist[w] = INF para w ≠ u.
 *   3. Executa V-1 rodadas de relaxamento sobre todas as arestas ativas.
 *   4. Se dist[v] < INF → aresta é redundante → remove permanentemente.
 *   5. Caso contrário → restaura a aresta.
 *
 * Complexidade: O(E · V · E) = O(V · E²)
 *   - E iterações (uma por aresta)
 *   - Cada Bellman-Ford custa O(V · E)
 */
class TransitiveReductionBF {
public:
    /**
     * Aplica a redução transitiva in-place sobre o grafo g.
     * Retorna o número de arestas removidas.
     */
    static int reduce(Graph& g);

private:
    static constexpr int INF = 1e9;

    /**
     * Executa Bellman-Ford a partir de `source` no grafo g.
     * Retorna vetor de distâncias (INF = inalcançável).
     */
    static std::vector<int> bellmanFord(const Graph& g, int source);
};
