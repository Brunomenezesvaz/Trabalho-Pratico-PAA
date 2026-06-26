#include "Graph.h"

#include <algorithm>
#include <stdexcept>

// ─────────────────────────────────────────────────────────────────────────────
// Helpers internos
// ─────────────────────────────────────────────────────────────────────────────

static void validateVertex(int v, int n) {
    if (v < 0 || v >= n)
        throw std::out_of_range("Vertice fora do intervalo [0, " +
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
        throw std::invalid_argument("Numero de vertices nao pode ser negativo");
}

// ─────────────────────────────────────────────────────────────────────────────
// Consultas básicas
// ─────────────────────────────────────────────────────────────────────────────

int Graph::numVertices() const { return _numVertices; }
int Graph::numEdges()    const { return _numEdges;    }

bool Graph::hasEdge(int u, int v) const {
    validateVertex(u, _numVertices);
    validateVertex(v, _numVertices);
    for (const Edge& e : _adj[u]) {
        if (e.to == v && !e.excluded) {
            return true;
        }
    }
    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
// Modificações
// ─────────────────────────────────────────────────────────────────────────────

void Graph::addEdge(int u, int v) {
    validateVertex(u, _numVertices);
    validateVertex(v, _numVertices);
    _adj[u].push_back(Edge(v));
    ++_numEdges;
}

bool Graph::removeEdge(int u, int v) {
    validateVertex(u, _numVertices);
    validateVertex(v, _numVertices);

    auto& row = _adj[u];
    auto it = std::find_if(row.begin(), row.end(), [v](const Edge& e) {
        return e.to == v;
    });

    if (it == row.end()) return false;

    row.erase(it);
    --_numEdges;
    return true;
}

bool Graph::excludeEdge(int u, int v) {
    validateVertex(u, _numVertices);
    validateVertex(v, _numVertices);

    for (Edge& e : _adj[u]) {
        if (e.to == v) {
            e.excluded = true;
            return true;
        }
    }
    return false;
}

bool Graph::restoreEdge(int u, int v) {
    validateVertex(u, _numVertices);
    validateVertex(v, _numVertices);

    for (Edge& e : _adj[u]) {
        if (e.to == v) {
            e.excluded = false;
            return true;
        }
    }
    return false;
}

const std::vector<Edge>& Graph::edges(int u) const {
    validateVertex(u, _numVertices);
    return _adj[u];
}

// ─────────────────────────────────────────────────────────────────────────────
// Impressão
// ─────────────────────────────────────────────────────────────────────────────

void Graph::print(std::ostream& out) const {
    for (int u = 0; u < _numVertices; ++u) {
        out << u << ":";
        for (const Edge& e : _adj[u]) {
            if (!e.excluded) {
                out << " " << e.to;
            }
        }
        out << "\n";
    }
}
