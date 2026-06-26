#include "TransitiveReductionBF.h"

// ─────────────────────────────────────────────────────────────────────────────
// Bellman-Ford adaptado (alcançabilidade por caminhos alternativos)
// ─────────────────────────────────────────────────────────────────────────────

std::vector<int> TransitiveReductionBF::bellmanFord(const Graph& g, int source, long long& operations) {
    const int V = g.numVertices();
    std::vector<int> dist(V, INF);
    dist[source] = 0;

    // V-1 rodadas de relaxamento
    for (int iter = 0; iter < V - 1; ++iter) {
        bool updated = false;

        for (int u = 0; u < V; ++u) {
            if (dist[u] == INF) continue;   // u ainda inalcançável, pula

            for (const Edge& e : g.edges(u)) {
                if (e.excluded) continue;    // ignora arestas excluídas

                operations++; // Incrementa cada relaxamento de aresta ativa

                // Relaxamento: todas as arestas têm peso 1
                if (dist[u] + 1 < dist[e.to]) {
                    dist[e.to] = dist[u] + 1;
                    updated = true;
                }
            }
        }

        // Otimização: para cedo se nenhuma distância foi atualizada
        if (!updated) break;
    }

    return dist;
}

// ─────────────────────────────────────────────────────────────────────────────
// Algoritmo principal
// ─────────────────────────────────────────────────────────────────────────────

ReductionResult TransitiveReductionBF::reduce(Graph& g) {
    int removed = 0;
    long long operations = 0;
    const int V = g.numVertices();

    for (int u = 0; u < V; ++u) {
        // Coleta destinos antes de iterar (evita invalidar iteradores)
        std::vector<int> targets;
        for (const Edge& e : g.edges(u))
            targets.push_back(e.to);

        for (int v : targets) {
            // Verifica se a aresta ainda existe
            if (!g.hasEdge(u, v)) continue;

            // Passo 1: exclui temporariamente u → v
            g.excludeEdge(u, v);

            // Passo 2: Bellman-Ford de u sem a aresta excluída
            std::vector<int> dist = bellmanFord(g, u, operations);

            // Passo 3: decide
            if (dist[v] < INF) {
                // Existe caminho alternativo — aresta redundante
                g.removeEdge(u, v);
                ++removed;
            } else {
                // Sem caminho alternativo — aresta necessária
                g.restoreEdge(u, v);
            }
        }
    }

    return {removed, operations};
}
