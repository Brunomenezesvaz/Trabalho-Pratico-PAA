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

O grafo é representado por uma **lista de adjacência** estruturada (`vector<vector<Edge>>`), onde cada elemento `Edge` contém:
- `int to`: Vértice de destino da aresta.
- `bool excluded`: Flag booleano indicando se a aresta está temporariamente desativada/excluída.

**Justificativa:**

- A redução transitiva exige múltiplas travessias — uma busca de alcançabilidade por aresta. Cada travessia custa $O(V + E)$ com lista de adjacência, contra $O(V^2)$ se usássemos matriz de adjacência.
- Grafos reais tendem a ser esparsos: a lista consome $O(V + E)$ de memória, enquanto a matriz consumiria $O(V^2)$.
- A flag `excluded` na própria estrutura `Edge` permite que os algoritmos desativem/ativem arestas de forma muito eficiente e **in-place** durante os testes de alcançabilidade, evitando o custo de copiar o grafo ou reconstruí-lo a cada iteração.


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
├── Graph.h                      Declaração da classe Graph e estrutura Edge
├── Graph.cpp                    Implementação do Grafo (lista de adjacência + exclusão in-place)
├── TransitiveReductionDFS.h     Declaração do algoritmo de redução por DFS
├── TransitiveReductionDFS.cpp   Implementação da redução DFS com contagem de visitas recursivas
├── TransitiveReductionBF.h      Declaração do algoritmo por Bellman-Ford Adaptado
├── TransitiveReductionBF.cpp    Implementação da redução Bellman-Ford com contagem de relaxamentos
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
- Número de operações internas (visitas recursivas na DFS / relaxamentos de aresta no Bellman-Ford)

### Limite de Complexidade e Proteção

Devido à complexidade de pior caso de $O(V \cdot E^2)$ do algoritmo Bellman-Ford Adaptado, a execução desse método em grafos de grande escala e densos exige dezenas de bilhões de operações, o que causaria o travamento/congelamento dos testes por horas.

Para contornar isso, o benchmark possui um **limite de proteção de $10^{10}$ operações estimadas**, pulando automaticamente a execução do Bellman-Ford nas seguintes configurações (sendo exibido como `PULADO` no console e nos arquivos de resultados):
- $V \ge 250$ com probabilidade $p \ge 50$\%
- $V \ge 500$ em todas as densidades ($p \in$ {20%, 50%, 80%})

O algoritmo DFS, devido à sua eficiência assintótica ($O(E \cdot (V + E))$), é executado em todas as configurações sem restrições.

### Hipóteses esperadas

- Em grafos **esparsos** ($p = 20$\%): ambos os algoritmos performam de forma relativamente próxima, com a DFS levando vantagem.
- Em grafos **densos** ($p = 50$\% a $80$\%): o custo $O(V \cdot E^2)$ do Bellman-Ford cresce a taxas críticas — o número de operações internas no Bellman-Ford explode, enquanto na DFS a alta densidade de caminhos alternativos acelera o término da busca, tornando-a ordens de grandeza mais rápida.
- A diferença entre os dois algoritmos cresce com a escala $V$, confirmando as previsões da análise de complexidade teórica.

---

## Como compilar e executar

### Compilação (GCC com suporte a C++17)

```bash
g++ -std=c++17 -O2 -o transitive_reduction Graph.cpp TransitiveReductionDFS.cpp TransitiveReductionBF.cpp Benchmark.cpp main.cpp
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

