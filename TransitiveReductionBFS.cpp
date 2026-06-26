#include "TransitiveReductionBFS.h"
#include <vector>
#include <algorithm>

// ─────────────────────────────────────────────────────────────────────────────
// BFS iterativa (Otimizada sem alocações na busca)
// ─────────────────────────────────────────────────────────────────────────────

bool TransitiveReductionBFS::bfs(const Graph& g, int start, int target,
                                 std::vector<int>& q, std::vector<bool>& visited,
                                 long long& operations) {
    if (start == target) return true;

    // Reseta o vetor de visitados
    std::fill(visited.begin(), visited.end(), false);

    int head = 0;
    int tail = 0;

    q[tail++] = start;
    visited[start] = true;

    while (head < tail) {
        int u = q[head++];
        operations++; // Conta cada expansão de vértice na fila

        for (const Edge& e : g.edges(u)) {
            if (e.excluded) continue; // ignora arestas excluídas
            operations++;             // conta cada checagem de aresta ativa

            if (e.to == target) return true; // alvo encontrado

            if (!visited[e.to]) {
                visited[e.to] = true;
                q[tail++] = e.to;
            }
        }
    }
    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
// Algoritmo principal
// ─────────────────────────────────────────────────────────────────────────────

ReductionResult TransitiveReductionBFS::reduce(Graph& g) {
    int removed = 0;
    long long operations = 0;
    const int V = g.numVertices();

    // Estruturas de suporte pré-alocadas uma única vez
    std::vector<int> q(V);
    std::vector<bool> visited(V);

    for (int u = 0; u < V; ++u) {
        // Coleta destinos das arestas de u antes de iterar
        // (evita invalidar iteradores durante remoções)
        std::vector<int> targets;
        for (const Edge& e : g.edges(u))
            targets.push_back(e.to);

        for (int v : targets) {
            // Verifica se a aresta ainda existe (pode ter sido removida)
            if (!g.hasEdge(u, v)) continue;

            // Passo 1: exclui temporariamente u → v
            g.excludeEdge(u, v);

            // Passo 2: BFS de u sem usar a aresta excluída
            bool reachable = bfs(g, u, v, q, visited, operations);

            // Passo 3: decide
            if (reachable) {
                // Aresta redundante — remove permanentemente
                g.removeEdge(u, v);
                ++removed;
            } else {
                // Aresta necessária — restaura
                g.restoreEdge(u, v);
            }
        }
    }

    return {removed, operations};
}
