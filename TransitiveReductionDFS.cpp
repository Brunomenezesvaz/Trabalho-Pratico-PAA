#include "TransitiveReductionDFS.h"

// ─────────────────────────────────────────────────────────────────────────────
// DFS recursiva
// ─────────────────────────────────────────────────────────────────────────────

bool TransitiveReductionDFS::dfs(const Graph& g, int atual, int alvo,
                                  std::vector<bool>& visitado, long long& ops) {
    ops++; // Conta cada chamada de DFS (visita a vértice)
    
    // Explora cada aresta ativa saindo de atual
    for (const Aresta& e : g.arestas(atual)) {
        if (e.excluida) continue;          // ignora arestas excluídas
        if (e.destino == alvo) return true;   // alvo encontrado
        if (visitado[e.destino]) continue;       // evita revisitar

        visitado[e.destino] = true;
        if (dfs(g, e.destino, alvo, visitado, ops))
            return true;
    }
    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
// Algoritmo principal
// ─────────────────────────────────────────────────────────────────────────────

ReductionResult TransitiveReductionDFS::reducao(Graph& g) {
    int removido = 0;
    long long ops = 0;
    const int V = g.numVertices();

    for (int u = 0; u < V; ++u) {
        // Coleta destinos das arestas de u antes de iterar
        // (evita invalidar iteradores durante remoções)
        std::vector<int> alvos;
        for (const Aresta& e : g.arestas(u))
            alvos.push_back(e.destino);

        for (int v : alvos) {
            // Verifica se a aresta ainda existe (pode ter sido removida)
            if (!g.temAresta(u, v)) continue;

            // Passo 1: exclui temporariamente u → v
            g.excluiAresta(u, v);

            // Passo 2: DFS de u sem usar a aresta excluída
            std::vector<bool> visitado(V, false);
            visitado[u] = true;
            bool alcancavel = dfs(g, u, v, visitado, ops);

            // Passo 3: decide
            if (alcancavel) {
                // Aresta redundante — remove permanentemente
                g.removeAresta(u, v);
                ++removido;
            } else {
                // Aresta necessária — restaura
                g.refazAresta(u, v);
            }
        }
    }

    return {removido, ops};
}
