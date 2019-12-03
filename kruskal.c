#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

// Estrutura da aresta
struct Edge { 
	int from;
    int to;
    int weight; 
};  

// Estrutura para um grafo não-direcionado
struct Graph { 

	int numberOfVertices;
    int numberOfEdges; 
	struct Edge* edge; 
}; 

struct Subset { 
	int parent; 
	int rank; 
}; 

struct Graph* createGraph(int numberOfVertices, int numberOfEdges) { 
	struct Graph* graph = (struct Graph*) malloc( sizeof(struct Graph) );
	graph->numberOfVertices = numberOfVertices; 
	graph->numberOfEdges = numberOfEdges; 

	graph->edge = (struct Edge*) malloc( graph->numberOfEdges * sizeof( struct Edge ) );

	return graph; 
} 

/* 
	- FUNÇÃO AUXILIAR - 
	Algoritmo de Busca-União:
	Procura sub-conjuntos disjuntos e transforma dois sub-conjuntos em um.
	Utilizado para verificar se um grafo não-direcionado possui ciclos.
	Faz uso do Path Compression, que transforma o nó de maior
	rank no pai. A técnica ajuda a evitar que todos os nós intermediários
	sejam visitados novamente.
	Complexidade: O(Log n)
 */
int findSubset(struct Subset subsets[], int setIndex) { 
	if (subsets[setIndex].parent != setIndex) {
		subsets[setIndex].parent = findSubset(subsets, subsets[setIndex].parent); 
    }

	return subsets[setIndex].parent; 
} 

void findUnion(struct Subset subsets[], int setA, int setB) { 
	int rootSetA = findSubset(subsets, setA); 
	int rootSetB = findSubset(subsets, setB); 

	if (subsets[rootSetA].rank < subsets[rootSetB].rank){
		subsets[rootSetA].parent = rootSetB; 
	} else if  (subsets[rootSetA].rank > subsets[rootSetB].rank) {
		subsets[rootSetB].parent = rootSetA; 
	} else {
		subsets[rootSetB].parent = rootSetA; 
		subsets[rootSetA].rank++; 
	}
} 

/*  
	- FUNÇÃO AUXILIAR - 
	Função auxiliar para comparar arestas de acordo com seus pesos.
	Utilizado no quickSort para ordenar as arestas.
*/
int compareEdges(const void* comparedFirstEdge, const void* comparedSecondEdge) { 
	struct Edge* firstEdge = (struct Edge*)comparedFirstEdge; 
	struct Edge* secondEdge = (struct Edge*)comparedSecondEdge; 
	return firstEdge->weight > secondEdge->weight; 
} 

// Algoritmo de Kruskal
void buildKruskal(struct Graph* graph) { 
	int vertices = graph->numberOfVertices; 
	struct Edge result[vertices]; // Resultado da árvore geradora mínima
	int indexForResults = 0; // Índice para o `result` 
	int indexforEdges = 0; // índice para as arestas já ordenadas

	/*
		Ordernar todos as arestas  por ordem crescente
		utilizando quick-sort
	  */
	qsort(graph->edge, graph->numberOfEdges, sizeof(graph->edge[0]), compareEdges); 

	// Cria subconjutos
	struct Subset *subsets = (struct Subset*) malloc( vertices * sizeof(struct Subset) ); 

	// Cria subconjuntos com um único elemento
	for (int i = 0; i < vertices; ++i) { 
		subsets[i].parent = i; 
		subsets[i].rank = 0; 
	} 

	/* 
		Verifica as arestas com ciclos
		até que a árvore geradora mínima
		possua o mínimo de arestas possíveis,
		isto é, V - 1, onde V é o número de vértices totais
		do grafo original.

	 */
	while (indexForResults < vertices - 1 && indexforEdges < graph->numberOfEdges) { 
		/* Seleciona a aresta de menor peso e a incrementa
		para a próxima iteração */
		struct Edge nextEdge = graph->edge[indexforEdges++]; 

		int subsetA = findSubset(subsets, nextEdge.from); 
		int subsetB = findSubset(subsets, nextEdge.to); 

		/* 
			Se os sub-conjuntos formados de uma aresta não
			ocasionarem em um ciclo, inclua esta aresta e incremente
			seu índice para buscar sub-conjunto da próxima aresta,
			senão, discarte-a
		 */

		if (subsetA != subsetB) { 
			result[indexForResults++] = nextEdge; 
			findUnion(subsets, subsetA, subsetB); 
		} 

	} 

	printf("Edges: \n"); 
	for (indexforEdges = 0; indexforEdges < indexForResults; ++indexforEdges){
        printf("%d -- %d == %d\n", result[indexforEdges].from, result[indexforEdges].to, result[indexforEdges].weight); 
    } 
	return; 
} 


int main() { 

	int numberOfVertices = 4; // Number of vertices in graph 
	int numberOfEdges = 5; // Number of edges in graph 
	struct Graph* graph = createGraph(numberOfVertices, numberOfEdges); 


	graph->edge[0].from = 0; 
	graph->edge[0].to = 1; 
	graph->edge[0].weight = 3; 

	graph->edge[1].from = 0; 
	graph->edge[1].to = 2; 
	graph->edge[1].weight = 4; 

	graph->edge[2].from = 0; 
	graph->edge[2].to = 3; 
	graph->edge[2].weight = 10; 

	graph->edge[3].from = 1; 
	graph->edge[3].to = 2; 
	graph->edge[3].weight = 11; 


	graph->edge[4].from = 2; 
	graph->edge[4].to = 3; 
	graph->edge[4].weight = 4; 

	buildKruskal(graph); 

	return 0; 
} 
