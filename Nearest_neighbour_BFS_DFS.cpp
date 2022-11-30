#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <map>
#define N 4 /// No. of vertx

class Vertex {
	unsigned int _id;
	bool _visited;
	std::map<Vertex*, int> _edges;
public:
	Vertex(unsigned id) : _id(id), _visited(false) {}

	unsigned int Id() { return _id; }
	bool IsVisited() { return _visited; }
	void SetVisited(bool v) { _visited = v; }
	
	std::pair<Vertex*, int> GetMin() {
		auto it = std::min_element(std::begin(_edges), std::end(_edges),
			[](const auto& l, const auto& r) {	return !l.first->IsVisited() && (r.first->IsVisited() || l.second < r.second); });
		return *it;
	}

	std::map<Vertex*, int>& GetEdges() { return _edges; }

	void AddEdge(Vertex *V, int weight) { _edges.insert(std::make_pair(V, weight));	}

	void AddNeighbour(Vertex* V, int weight, bool dir) {
		_edges.insert(std::make_pair(V,weight));		
		if(!dir) V->AddEdge(this, weight);
	}

	void PrintNeighbours() {
		std::cout << _id << ": ";
		for (auto it = _edges.cbegin(); it != _edges.cend(); ++it)
			std::cout << it->first->_id << "#" << it->second << " ";
		std::cout << std::endl;
	}

};

class Graph {
	std::vector<Vertex*> _G;
public:
	Graph(std::vector<Vertex*> &G) { _G = G; }

	void NearestNeighbour(Vertex* V) {
		int cost = 0;
		std::vector<Vertex*> path; path.reserve(N);	

		NN(V, path, cost);

		std::cout << "Cost of path is: " << cost << std::endl;
		for (auto el : path) std::cout << el->Id() << "->";
	}	

	void NN(Vertex* V, std::vector<Vertex*> &path, int &cost) {
		if (path.size() == N) return;

		V->SetVisited(true);
		path.push_back(V);
		cost += V->GetMin().second;		
		NN(V->GetMin().first, path, cost);
	}

	void BreadthFirstSearch(Vertex* V)	{
		std::vector<Vertex*> path, queue; path.reserve(N); queue.reserve(N);
		queue.push_back(V);
		BFS(V, queue, path);

		for (auto el : path) std::cout << el->Id() << "->";
	}	
	
	void BFS(Vertex* V, std::vector<Vertex*>& queue, std::vector<Vertex*>& path) {
		
		V->SetVisited(true);		

		std::map<Vertex*, int> edges = V->GetEdges();
		for (auto it = edges.cbegin(); it != edges.cend(); ++it)
			if(!it->first->IsVisited())	queue.push_back(it->first);

		path.push_back(V);
		queue.erase(queue.begin());

		if (queue.size() == 0) return;
		else BFS(queue.at(0), queue, path);
	}
		
	void DepthFirstSearch(Vertex* V) {
		std::vector<Vertex*> path; path.reserve(N);
		DFS(V, path);

		for (auto el : path) std::cout << el->Id() << "->";
	}

	void DFS(Vertex* V, std::vector<Vertex*>& path)	{	
		V->SetVisited(true);

		path.push_back(V);

		std::map<Vertex*, int> edges = V->GetEdges();
		for (auto it = edges.cbegin(); it != edges.cend(); ++it)
			if (!it->first->IsVisited()) DFS(it->first, path);		
	}
};


int main() {
	// NearestNeighbour change N to 7
	//*
	Vertex* a = new Vertex(0); Vertex* b = new Vertex(1); Vertex* c = new Vertex(2);
	Vertex* d = new Vertex(3); Vertex* e = new Vertex(4); Vertex* f = new Vertex(5); Vertex* g = new Vertex(6);

	//      (vertex | weight | pointed)
	a->AddNeighbour(b, 10, false);	a->AddNeighbour(e, 1, false);	a->AddNeighbour(d, 4, false);

	b->AddNeighbour(e, 3, false);	b->AddNeighbour(c, 1, false);	b->AddNeighbour(f, 15, false);

	c->AddNeighbour(e, 2, false);	c->AddNeighbour(d, 1, false);

	d->AddNeighbour(e, 3, false);	d->AddNeighbour(f, 7, false);	d->AddNeighbour(g, 5, false);

	f->AddNeighbour(g, 6, false);

	std::vector<Vertex*> G; G.reserve(N);
	G = { a, b, c, d, e, f, g };

	std::cout << "Adjacency list" << std::endl;
	for (auto el : G) el->PrintNeighbours();
		
	Graph graph(G);
	graph.NearestNeighbour(f);
	//*/

	// BFS change N to 4
	/*
	Vertex* a = new Vertex(0); Vertex* b = new Vertex(1); Vertex* c = new Vertex(2); Vertex* d = new Vertex(3);

	c->AddNeighbour(a, 0, false); c->AddNeighbour(d, 0, true);
	a->AddNeighbour(b, 0, true); 
	b->AddNeighbour(c, 0, true);
	d->AddNeighbour(d, 0, true);

	std::vector<Vertex*> G; G.reserve(N);
	G = { a, b, c, d};
	Graph graph(G);
	graph.DepthFirstSearch(c);
	std::cout << std::endl;
	*/

	// DFS change N to 4
	/*
	Vertex* a = new Vertex(0); Vertex* b = new Vertex(1); Vertex* c = new Vertex(2); Vertex* d = new Vertex(3);

	a->AddNeighbour(b, 0, true); a->AddNeighbour(c, 0, true);
	b->AddNeighbour(c, 0, true);
	c->AddNeighbour(a, 0, true); c->AddNeighbour(d, 0, true);
	d->AddNeighbour(d, 0, true);

	std::vector<Vertex*> G; G.reserve(N);
	G = { a, b, c, d};
	Graph graph(G);

	graph.DepthFirstSearch(b);
	*/
	return EXIT_SUCCESS;
}
