#pragma once
 
#include <vector>
#include <iostream>

// Representa uma aresta direcionada u → v com flag de exclusão temporária
struct Aresta {
    int destino; // vértice de destino
    bool excluida; // true = exclusão temporária
 
    explicit Aresta(int destino) : destino(destino), excluida(false) {}
};
 
struct ReductionResult {
    int arestasRemovidas;
    long long ops;
};
 
class Graph {
public:
    // Constrói um grafo sem arestas com `numVertices` vértices
    explicit Graph(int numVertices);
 
    //Retorna o número de vértices
    int numVertices() const;
 
    // Retorna o número de arestas não excluídas permanentemente
    int numArestas() const;
 
    // Adiciona a aresta u → v
    void adicionaAresta(int u, int v);
 
    // Remove permanentemente a aresta direcionada u → v
    bool removeAresta(int u, int v);
 
    // Retorna true se existe a aresta u → v
    bool temAresta(int u, int v) const;
 
    // Marca a aresta u → v como excluída temporariamente
    bool excluiAresta(int u, int v);
 
    // Restaura a aresta u → v
    bool refazAresta(int u, int v);
 
    // Retorna a lista de arestas do vértice u
    const std::vector<Aresta>& arestas(int u) const;
 
    // Impressão
    void print(std::ostream& out = std::cout) const;
 
private:
    int _numVertices;
    int _numArestas;
 
    // _adj[u] contém as arestas saindo de u
    std::vector<std::vector<Aresta>> _adj;
};
