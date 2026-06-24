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

O grafo é representado por **lista de adjacência** (`vector<vector<int>>`).

**Justificativa:**

- Redução transitiva exige múltiplas travessias — uma por aresta. Cada travessia
  custa O(V + E) com lista de adjacência, contra O(V²) com matriz de adjacência.
- Grafos do mundo real tendem a ser esparsos: a lista consome O(V + E) de memória,
  enquanto a matriz consumiria O(V²).
- A estrutura suporta uma **flag de exclusão de aresta** sem overhead significativo,
  necessária para o algoritmo de Bellman-Ford adaptado.

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

### 2. Bellman-Ford Adaptado

Para cada aresta (u → v):

1. Marca a aresta como excluída temporariamente.
2. Inicializa dist[u] = 0, dist[w] = ∞ para w ≠ u.
3. Executa V − 1 rodadas de relaxamento sobre todas as arestas restantes.
4. Se dist[v] < ∞ → aresta é redundante → remove permanentemente.
5. Caso contrário → restaura a aresta.

O Bellman-Ford é usado aqui como um detector de alcançabilidade por caminhos
alternativos, não para otimização de distâncias com pesos negativos.

**Complexidade:** O(E · V · E) = O(V · E²)

---

## Estrutura do Projeto

```
graph/
├── Graph.h                      Declaração da classe Graph
├── Graph.cpp                    Implementação da lista de adjacência + flag de exclusão
├── TransitiveReduction.h        Interface comum aos dois algoritmos
├── TransitiveReductionDFS.h     Declaração do algoritmo DFS
├── TransitiveReductionDFS.cpp   Implementação do algoritmo DFS
├── TransitiveReductionBF.h      Declaração do algoritmo Bellman-Ford adaptado
├── TransitiveReductionBF.cpp    Implementação do algoritmo Bellman-Ford adaptado
├── Benchmark.h                  Gerador de grafos aleatórios + cronômetro
├── Benchmark.cpp                Implementação do benchmark
├── main.cpp                     Orquestração dos testes e saída dos resultados
└── README.md                    Este arquivo
```

---

## Benchmark

Os algoritmos são comparados experimentalmente em grafos aleatórios gerados pelo
modelo **Erdős–Rényi direcionado**: cada aresta (u, v) com u ≠ v existe com
probabilidade p independentemente.

### Variáveis testadas

| Parâmetro | Valores |
|-----------|---------|
| Vértices (V) | 50, 100, 250, 500, 1000 |
| Densidade (p) | 20%, 50%, 80% |

Para cada combinação (V, p), os dois algoritmos recebem **o mesmo grafo** (mesma
seed aleatória) e são cronometrados individualmente. Métricas coletadas:

- Tempo de execução (ms)
- Número de arestas removidas
- Número de operações internas (visitas DFS / relaxamentos BF)

### Hipóteses esperadas

- Em grafos **esparsos** (p = 20%): ambos os algoritmos performam de forma próxima,
  com DFS levando leve vantagem.
- Em grafos **densos** (p = 50–80%): o custo O(V · E²) do Bellman-Ford se torna
  evidente — crescimento muito mais acentuado que o O(E · (V + E)) do DFS.
- A diferença entre os dois cresce com V, confirmando a análise assintótica.

---

## Como compilar e executar

```bash
# Compilar
g++ -std=c++17 -O2 -o transitive_reduction \
    Graph.cpp \
    TransitiveReductionDFS.cpp \
    TransitiveReductionBF.cpp \
    Benchmark.cpp \
    main.cpp

# Executar
./transitive_reduction
```

---

## Dependências

- C++17 ou superior
- Biblioteca padrão apenas (`<vector>`, `<chrono>`, `<random>`, `<iostream>`)
- Nenhuma dependência externa

---
