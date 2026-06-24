#include "TransitiveReductionDFS.h"

// ─────────────────────────────────────────────────────────────────────────────
// DFS recursiva
// ─────────────────────────────────────────────────────────────────────────────

bool TransitiveReductionDFS::dfs(const Graph& g, int current, int target,
                                  std::vector<bool>& visited) {
    // Explora cada aresta ativa saindo de current
    for (const Edge& e : g.edges(current)) {
        if (e.excluded) continue;          // ignora arestas excluídas
        if (e.to == target) return true;   // alvo encontrado
        if (visited[e.to]) continue;       // evita revisitar

        visited[e.to] = true;
        if (dfs(g, e.to, target, visited))
            return true;
    }
    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
// Algoritmo principal
// ─────────────────────────────────────────────────────────────────────────────

int TransitiveReductionDFS::reduce(Graph& g) {
    int removed = 0;
    const int V = g.numVertices();

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

            // Passo 2: DFS de u sem usar a aresta excluída
            std::vector<bool> visited(V, false);
            visited[u] = true;
            bool reachable = dfs(g, u, v, visited);

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

    return removed;
}
