#include <stdio.h>
#include <stdlib.h>

struct Edge {
    int from;
    int to;
    int weight;
};

struct Graph {
    int numberOfVertices;
    int numberOfEdges;
    struct Edge *edge;
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
};

int find(struct Subset subsets[], int n) { 

	if (subsets[n].parent != n)  {
        subsets[n].parent = find(subsets, subsets[n].parent); 

    }
	return subsets[n].parent; 
} 

void unionSets(struct Subset subsets[], int setA, int setB) { 
	int rootSetA = find(subsets, setA); 
	int rootSetB = find(subsets, setB); 

	if (subsets[rootSetA].rank < subsets[rootSetB].rank) {
        subsets[rootSetA].parent = rootSetB; 
    } else if (subsets[rootSetA].rank > subsets[rootSetB].rank)  {
        subsets[rootSetB].parent = rootSetA; 
    } else { 
		subsets[rootSetB].parent = rootSetA; 
		subsets[rootSetA].rank++; 
	} 
} 

int compareEdges(const void* comparedFirstEdge, const void* comparedSecondEdge) { 
	struct Edge* firstEdge = (struct Edge*)comparedFirstEdge; 
	struct Edge* secondEdge = (struct Edge*)comparedSecondEdge; 
	return firstEdge->weight > secondEdge->weight; 
} 

void buildKruskalMininumSpanningTree(struct Graph* graph) { 
	int vertices = graph->numberOfVertices; 
	struct Edge result[vertices];  
	int indexForResults = 0;  
	int indexForEdges = 0;
    
	qsort(graph->edge, graph->numberOfEdges, sizeof(graph->edge[0]), compareEdges); 

	struct Subset *subsets = (struct Subset*) malloc(vertices * sizeof(struct Subset)); 

    
	for (int i = 0; i < vertices; i++) { 
		subsets[i].parent = i; 
		subsets[i].rank = 0; 
	} 

	while (indexForResults < vertices - 1 && indexForEdges < graph->numberOfEdges) { 

		struct Edge nextEdge = graph->edge[indexForEdges++]; 

		int subsetA = find(subsets, nextEdge.from); 
		int subsetB = find(subsets, nextEdge.to); 

		if (subsetA != subsetB) 
		{ 
			result[indexForEdges++] = nextEdge; 
			unionSets(subsets, subsetA, subsetB); 
		} 
		
	} 
	printf("Edges:\n"); 
	for (indexForEdges = 0; indexForEdges < indexForResults; indexForEdges++)  {
		printf("%d -- %d == %d\n", result[indexForEdges].from, result[indexForEdges].to, result[indexForEdges].weight); 
    }
	return; 
} 


// Driver program to test above functions 
int main() { 

	int numberOfVertices = 5;
	int numberOfEdges = 5; 
	struct Graph* graph = createGraph(numberOfEdges, numberOfVertices); 

	graph->edge[0].from = 0; 
	graph->edge[0].to = 1; 
	graph->edge[0].weight = 2; 


	graph->edge[1].from = 1; 
	graph->edge[1].to = 2; 
	graph->edge[1].weight = 1; 

	graph->edge[2].from = 0; 
	graph->edge[2].to = 3; 
	graph->edge[2].weight = 5; 


	graph->edge[3].from = 2; 
	graph->edge[3].to = 4; 
	graph->edge[3].weight = 10; 


	graph->edge[4].from = 2; 
	graph->edge[4].to = 3; 
	graph->edge[4].weight = 4; 

	buildKruskalMininumSpanningTree(graph); 

	return 0; 
} 


 
