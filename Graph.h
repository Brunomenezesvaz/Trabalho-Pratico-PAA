#pragma once
 
#include <vector>
#include <iostream>
 
/**
 * Grafo direcionado representado por lista de adjacência.
 *
 * Escolha da representação:
 *   - Lista de adjacência (vector<vector<int>>) é preferível à matriz de
 *     adjacência para grafos esparsos, pois consome O(V + E) de memória
 *     contra O(V²) da matriz.
 *   - A redução transitiva exige múltiplas travessias (uma DFS/BF por aresta),
 *     e cada travessia custa O(V + E) com lista — ideal para grafos esparsos.
 *   - Inserção e remoção de arestas são operações simples sobre vetores.
 *
 * Observação sobre grafos não-direcionados:
 *   - Em grafos não-direcionados, toda aresta (u, v) forma implicitamente
 *     um ciclo de comprimento 2, tornando qualquer aresta "redundante" sob
 *     o critério de alcançabilidade puro. O algoritmo de redução transitiva
 *     baseado em caminhamento precisaria de adaptações (ex.: ignorar a aresta
 *     inversa durante a DFS) e o resultado seria não-trivial. A definição
 *     clássica de redução transitiva é mais bem comportada em grafos
 *     direcionados (e especialmente em DAGs).
 */
 
/** Representa uma aresta direcionada u → v com flag de exclusão temporária. */
struct Edge {
    int to;           // vértice de destino
    bool excluded;    // true = ignorada durante travessias (exclusão temporária)
 
    explicit Edge(int to) : to(to), excluded(false) {}
};
 
class Graph {
public:
    /**
     * Constrói um grafo com `numVertices` vértices e sem arestas.
     * Os vértices são indexados de 0 a numVertices-1.
     */
    explicit Graph(int numVertices);
 
    /** Retorna o número de vértices. */
    int numVertices() const;
 
    /** Retorna o número de arestas ativas (não excluídas permanentemente). */
    int numEdges() const;
 
    /**
     * Adiciona a aresta direcionada u → v.
     * Não verifica duplicatas.
     */
    void addEdge(int u, int v);
 
    /**
     * Remove permanentemente a aresta direcionada u → v, se existir.
     * Retorna true se a aresta existia e foi removida.
     */
    bool removeEdge(int u, int v);
 
    /**
     * Retorna true se existe a aresta u → v (ativa e não excluída).
     */
    bool hasEdge(int u, int v) const;
 
    /**
     * Marca a aresta u → v como excluída temporariamente.
     * Durante travessias, arestas excluídas são ignoradas.
     * Retorna true se a aresta foi encontrada e marcada.
     */
    bool excludeEdge(int u, int v);
 
    /**
     * Restaura a aresta u → v (desfaz exclusão temporária).
     * Retorna true se a aresta foi encontrada e restaurada.
     */
    bool restoreEdge(int u, int v);
 
    /**
     * Retorna a lista de arestas do vértice u (incluindo excluídas).
     * Os algoritmos devem checar edge.excluded antes de usar.
     */
    const std::vector<Edge>& edges(int u) const;
 
    /**
     * Imprime o grafo no formato:
     *   vértice: vizinho1 vizinho2 ...
     * Arestas excluídas temporariamente são omitidas.
     */
    void print(std::ostream& out = std::cout) const;
 
private:
    int _numVertices;
    int _numEdges;
 
    // _adj[u] contém as arestas saindo de u
    std::vector<std::vector<Edge>> _adj;
};
