#include "TransitiveReductionBFS.h"
#include <vector>
#include <algorithm>

// ─────────────────────────────────────────────────────────────────────────────
// BFS iterativa (Otimizada sem alocações na busca)
// ─────────────────────────────────────────────────────────────────────────────

bool TransitiveReductionBFS::bfs(const Graph& g, int origem, int alvo,
                                 std::vector<int>& q, std::vector<bool>& visitado,
                                 long long& ops) {
    if (origem == alvo) return true;
 
    // Reseta o vetor de visitados
    std::fill(visitado.begin(), visitado.end(), false);
 
    int inicio = 0;
    int fim = 0;
 
    q[fim++] = origem;
    visitado[origem] = true;
 
    while (inicio < fim) {
        int u = q[inicio++];
        ops++; // Conta cada expansão de vértice na fila
 
        for (const Aresta& e : g.arestas(u)) {
            if (e.excluida) continue; // ignora arestas excluídas
            ops++;             // conta cada checagem de aresta ativa
 
            if (e.destino == alvo) return true; // alvo encontrado
 
            if (!visitado[e.destino]) {
                visitado[e.destino] = true;
                q[fim++] = e.destino;
            }
        }
    }
    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
// Algoritmo principal
// ─────────────────────────────────────────────────────────────────────────────

ReductionResult TransitiveReductionBFS::reducao(Graph& g) {
    int removido = 0;
    long long ops = 0;
    const int V = g.numVertices();
 
    // Estruturas de suporte pré-alocadas uma única vez
    std::vector<int> q(V);
    std::vector<bool> visitado(V);
 
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
 
            // Passo 2: BFS de u sem usar a aresta excluída
            bool alcancavel = bfs(g, u, v, q, visitado, ops);
 
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
