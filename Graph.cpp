#include "Graph.h"
 
#include <algorithm>  // std::find, std::remove
#include <stdexcept>
 
// ─────────────────────────────────────────────────────────────────────────────
// Helpers internos
// ─────────────────────────────────────────────────────────────────────────────
 
static void validateVertex(int v, int n) {
    if (v < 0 || v >= n)
        throw std::out_of_range("Vértice fora do intervalo [0, " +
                                std::to_string(n - 1) + "]");
}
 
// ─────────────────────────────────────────────────────────────────────────────
// Construtor
// ─────────────────────────────────────────────────────────────────────────────
 
Graph::Graph(int numVertices)
    : _numVertices(numVertices), _numEdges(0),
      _adj(numVertices)          // inicializa numVertices listas vazias
{
    if (numVertices < 0)
        throw std::invalid_argument("Número de vértices não pode ser negativo");
}
 
// ─────────────────────────────────────────────────────────────────────────────
// Consultas básicas
// ─────────────────────────────────────────────────────────────────────────────
 
int Graph::numVertices() const { return _numVertices; }
int Graph::numEdges()    const { return _numEdges;    }
 
const std::vector<int>& Graph::neighbors(int u) const {
    validateVertex(u, _numVertices);
    return _adj[u];
}
 
bool Graph::hasEdge(int u, int v) const {
    validateVertex(u, _numVertices);
    validateVertex(v, _numVertices);
    const auto& row = _adj[u];
    return std::find(row.begin(), row.end(), v) != row.end();
}
 
// ─────────────────────────────────────────────────────────────────────────────
// Modificações
// ─────────────────────────────────────────────────────────────────────────────
 
void Graph::addEdge(int u, int v) {
    validateVertex(u, _numVertices);
    validateVertex(v, _numVertices);
    _adj[u].push_back(v);
    ++_numEdges;
}
 
bool Graph::removeEdge(int u, int v) {
    validateVertex(u, _numVertices);
    validateVertex(v, _numVertices);
 
    auto& row = _adj[u];
    auto it = std::find(row.begin(), row.end(), v);
    if (it == row.end()) return false;
 
    // Erase-remove idiom: move o elemento para o fim e apaga
    row.erase(it);
    --_numEdges;
    return true;
}
 
// ─────────────────────────────────────────────────────────────────────────────
// Impressão
// ─────────────────────────────────────────────────────────────────────────────
 
void Graph::print(std::ostream& out) const {
    for (int u = 0; u < _numVertices; ++u) {
        out << u << ":";
        for (int v : _adj[u])
            out << " " << v;
        out << "\n";
    }
}
