# Redução Transitiva de Grafos

Implementação e comparação experimental de dois algoritmos de redução transitiva
baseados em caminhamento em grafos direcionados.

---

## Problema

Dado um grafo direcionado G = (V, E), sua **redução transitiva** G' = (V, E') é o grafo
com o menor conjunto de arestas E' ⊆ E tal que a atingibilidade entre quaisquer dois
vértices é preservada: para todo par (u, v), existe caminho de u até v em G se e somente
se existe caminho de u até v em G'.

Em outras palavras, uma aresta u → v é **redundante** se existe um caminho alternativo
de comprimento ≥ 2 que já conecta u a v. A redução transitiva remove exatamente essas
arestas redundantes.

### Exemplo

```
Grafo original G:          Redução transitiva G':

  0 → 1 → 2 → 3             0 → 1 → 2 → 3
  0 -------→ 2  (redundante, pois 0→1→2)
  1 -------→ 3  (redundante, pois 1→2→3)
```

---

## Representação do Grafo

O grafo é representado por uma **lista de adjacência** estruturada (`vector<vector<Aresta>>`), onde cada elemento `Aresta` contém:
- `int destino`: Vértice de destino da aresta.
- `bool excluida`: Flag booleano indicando se a aresta está temporariamente desativada/excluída.

**Justificativa:**

- A redução transitiva exige múltiplas travessias — uma busca de alcançabilidade por aresta. Cada travessia custa $O(V + E)$ com lista de adjacência, contra $O(V^2)$ se usássemos matriz de adjacência.
- Grafos reais tendem a ser esparsos: a lista consome $O(V + E)$ de memória, enquanto a matriz consumiria $O(V^2)$.
- A flag `excluida` na própria estrutura `Aresta` permite que os algoritmos desativem/ativem arestas de forma muito eficiente e **in-place** durante os testes de alcançabilidade, evitando o custo de copiar o grafo ou reconstruí-lo a cada iteração.


### Grafos não-direcionados

A implementação foi projetada para grafos **direcionados**. Em grafos não-direcionados,
toda aresta (u, v) implica a aresta inversa (v, u), formando um ciclo de comprimento 2.
Isso faz com que qualquer aresta pareça redundante sob o critério de alcançabilidade
puro, tornando a redução transitiva mal definida nesse contexto sem adaptações
adicionais na lógica de detecção de caminhos alternativos.

---

## Algoritmos Implementados

Ambos os algoritmos operam **in-place** sobre o grafo original e compartilham a mesma
ideia central:

> Uma aresta (u → v) é redundante se, ao removê-la temporariamente, v ainda é
> alcançável a partir de u.

A remoção temporária é feita via **flag de exclusão** na estrutura do grafo, evitando
cópias desnecessárias.

### 1. DFS — Busca em Profundidade

Para cada aresta (u → v):

1. Marca a aresta como excluída temporariamente.
2. Executa DFS a partir de u.
3. Se v for visitado → aresta é redundante → remove permanentemente.
4. Caso contrário → restaura a aresta.

**Complexidade:** O(E · (V + E))

### 2. BFS — Busca em Largura

Para cada aresta (u → v):

1. Marca a aresta como excluída temporariamente.
2. Executa uma Busca em Largura (BFS) a partir de u.
3. Se v for alcançado durante a busca → aresta é redundante → remove permanentemente.
4. Caso contrário → restaura a aresta.

A BFS foi otimizada para evitar overhead de alocação de memória dinâmica (`std::queue` e `std::vector<bool>`) em loops internos de exclusão, pré-alocando a fila e o vetor de visitados apenas uma vez.

**Complexidade:** O(E · (V + E))

---

## Estrutura do Projeto

```
├── Graph.h                      Declaração da classe Graph e estrutura Aresta
├── Graph.cpp                    Implementação do Grafo (lista de adjacência + exclusão in-place)
├── TransitiveReductionDFS.h     Declaração do algoritmo de redução por DFS
├── TransitiveReductionDFS.cpp   Implementação da redução DFS com contagem de visitas recursivas
├── TransitiveReductionBFS.h     Declaração do algoritmo de redução por BFS
├── TransitiveReductionBFS.cpp   Implementação da redução BFS otimizada
├── Benchmark.h                  Declaração do gerador aleatório e orquestrador do benchmark
├── Benchmark.cpp                Orquestração do benchmark (Erdős-Rényi e medições)
├── main.cpp                     Ponto de entrada (validação de sanidade e execução)
└── README.md                    Este arquivo de documentação
```

---

## Benchmark

Os algoritmos são comparados experimentalmente em grafos aleatórios gerados pelo modelo **Erdős–Rényi direcionado**: cada aresta (u, v) com u ≠ v existe com probabilidade p independentemente.

### Variáveis testadas

| Parâmetro | Valores |
|-----------|---------|
| Vértices (V) | 50, 100, 250, 500, 1000 |
| Densidade (p) | 20%, 50%, 80% |

Para cada combinação (V, p), os dois algoritmos recebem **o mesmo grafo** (mesma seed aleatória) e são cronometrados individualmente. Métricas coletadas:

- Tempo de execução (ms)
- Número de arestas removidas
- Número de operações internas (visitas recursivas na DFS / expansões de vértices e checagens na BFS)

### Hipóteses esperadas

- Em grafos **esparsos** ($p = 20$\%): ambos os algoritmos performam de forma próxima, com a BFS apresentando vantagem de tempo devido à implementação iterativa livre de overhead de pilha (sendo cerca de 8,7x mais rápida na maior escala).
- Em grafos **densos** ($p = 50$\% a $80$\%): a BFS se torna massivamente mais rápida do que a DFS (atingindo velocidades até 31x superiores em grafos com $V=1000$). Isso ocorre porque, em grafos muito densos, o menor caminho alternativo é quase sempre de comprimento 2. A BFS, ao expandir em largura (nível por nível), encontra esse caminho curto instantaneamente no segundo nível. A DFS, por outro lado, tenta caminhos mais profundos primeiro, gerando um número excessivo de operações e chamadas recursivas antes de encontrar o destino.
- A diferença entre os dois algoritmos escala com a dimensão $V$, consolidando a BFS como a abordagem prática mais eficiente para a redução.

---

## Como compilar e executar

### Compilação (GCC com suporte a C++17)

```bash
g++ -std=c++17 -O2 -o transitive_reduction Graph.cpp TransitiveReductionDFS.cpp TransitiveReductionBFS.cpp Benchmark.cpp main.cpp
```

### Execução

**No Linux/macOS:**
```bash
./transitive_reduction
```

**No Windows (PowerShell/CMD):**
```powershell
.\transitive_reduction.exe
```

### Saída de Resultados
Ao executar o programa:
1. Um **teste de sanidade** é executado inicialmente em um grafo pequeno para verificar e certificar que ambos os algoritmos produzem exatamente a mesma redução transitiva.
2. A bateria completa de testes experimentais é executada para todas as configurações de $(V, p)$.
3. As médias de tempo e operações de cada configuração são salvas automaticamente em um arquivo CSV chamado `benchmark_results.csv` na raiz do diretório.

---

## Dependências

- Compilador C++ com suporte ao padrão **C++17** (como o `g++` ou `clang++`)
- Apenas componentes da biblioteca padrão do C++ (STL), como `<vector>`, `<chrono>`, `<random>`, `<algorithm>`, `<iostream>`, `<fstream>` e `<iomanip>`
- Nenhuma biblioteca ou dependência externa é requerida.

