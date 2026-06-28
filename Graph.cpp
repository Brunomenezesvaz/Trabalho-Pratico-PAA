#include "Graph.h"

#include <algorithm>
#include <stdexcept>

// ─────────────────────────────────────────────────────────────────────────────
// Helpers internos
// ─────────────────────────────────────────────────────────────────────────────

static void validaVertice(int v, int n) {
    if (v < 0 || v >= n)
        throw std::out_of_range("Vertice fora do intervalo [0, " +
                                std::to_string(n - 1) + "]");
}

// ─────────────────────────────────────────────────────────────────────────────
// Construtor
// ─────────────────────────────────────────────────────────────────────────────

Graph::Graph(int numVertices)
    : _numVertices(numVertices), _numArestas(0),
      _adj(numVertices)          // inicializa numVertices listas vazias
{
    if (numVertices < 0)
        throw std::invalid_argument("Numero de vertices nao pode ser negativo");
}

// ─────────────────────────────────────────────────────────────────────────────
// Consultas básicas
// ─────────────────────────────────────────────────────────────────────────────

int Graph::numVertices() const { return _numVertices; }
int Graph::numArestas()   const { return _numArestas;    }

bool Graph::temAresta(int u, int v) const {
    validaVertice(u, _numVertices);
    validaVertice(v, _numVertices);
    for (const Aresta& e : _adj[u]) {
        if (e.destino == v && !e.excluida) {
            return true;
        }
    }
    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
// Modificações
// ─────────────────────────────────────────────────────────────────────────────

void Graph::adicionaAresta(int u, int v) {
    validaVertice(u, _numVertices);
    validaVertice(v, _numVertices);
    _adj[u].push_back(Aresta(v));
    ++_numArestas;
}

bool Graph::removeAresta(int u, int v) {
    validaVertice(u, _numVertices);
    validaVertice(v, _numVertices);

    auto& row = _adj[u];
    auto it = std::find_if(row.begin(), row.end(), [v](const Aresta& e) {
        return e.destino == v;
    });

    if (it == row.end()) return false;

    row.erase(it);
    --_numArestas;
    return true;
}

bool Graph::excluiAresta(int u, int v) {
    validaVertice(u, _numVertices);
    validaVertice(v, _numVertices);

    for (Aresta& e : _adj[u]) {
        if (e.destino == v) {
            e.excluida = true;
            return true;
        }
    }
    return false;
}

bool Graph::refazAresta(int u, int v) {
    validaVertice(u, _numVertices);
    validaVertice(v, _numVertices);

    for (Aresta& e : _adj[u]) {
        if (e.destino == v) {
            e.excluida = false;
            return true;
        }
    }
    return false;
}

const std::vector<Aresta>& Graph::arestas(int u) const {
    validaVertice(u, _numVertices);
    return _adj[u];
}

// ─────────────────────────────────────────────────────────────────────────────
// Impressão
// ─────────────────────────────────────────────────────────────────────────────

void Graph::print(std::ostream& out) const {
    for (int u = 0; u < _numVertices; ++u) {
        out << u << ":";
        for (const Aresta& e : _adj[u]) {
            if (!e.excluida) {
                out << " " << e.destino;
            }
        }
        out << "\n";
    }
}
