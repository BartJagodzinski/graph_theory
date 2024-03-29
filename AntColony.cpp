#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <random>
#include <functional>
#include <math.h>
#include <tuple>
#include <map>

#define N 5 // No. of vertex
#define evaporate 0.2 // evaporation rate
#define phoeromone 2 // phoeromone add value
#define MAX 5		// No. of iterations

class Vertex {
	unsigned int _id;
	// pointer, weight, pheromone
	std::vector<std::tuple<Vertex*, int, double>> _edges;
public:

	Vertex(unsigned int id) : _id(id) {}

	unsigned int getId() { return _id; }

	void print() {
		std::cout << _id << ": " << std::endl;
		for (auto el : _edges)
			std::cout << std::get<0>(el)->getId() << "  |   #_" << std::get<1>(el) << "  |   $_" << std::get<2>(el) << std::endl;
		
		std::cout << std::endl;
	}

	std::vector<std::tuple<Vertex*, int, double>> getEdges() { return _edges; }

	void AddEdge(Vertex* V, int weight, double p = 0.0) { _edges.push_back(std::tuple<Vertex*, int, double>(V, weight, p)); }

	void AddNeighbour(Vertex* V, int weight, bool dir, double p = 0.0) {
		_edges.push_back(std::tuple<Vertex*, int, double>(V, weight, p));

		if (!dir) V->AddEdge(this, weight, p);
	}

	double getPheromone(Vertex* V) { for (auto el : _edges) if (std::get<0>(el) == V) return std::get<2>(el); }
		
	void addPhoeromone(Vertex* V, double p) { 
		for (auto &el : _edges) {
			if (std::get<0>(el)->getId() == V->getId()) {
				std::get<2>(el) += p;
				std::get<0>(el)->addPhoeromone_2(this, p);
			}
		}	
	}

	void addPhoeromone_2(Vertex* V, double p) {
		for (auto &el : _edges)
			if (std::get<0>(el)->getId() == V->getId()) { std::get<2>(el) += p; }
	}

	void evaporatePheromone() { for (auto &el : _edges) { if(std::get<2>(el) > 0.3) std::get<2>(el) -= evaporate; } }
};

class Graph {
	std::vector<Vertex*> _G;
public:
	Graph(std::vector<Vertex*>& G) { _G = G; }

	void print() { std::cout << "id | weight  | phoeromone" << std::endl;	for (auto el : _G)	el->print(); }

	void evaporateP() {
		for (auto el : _G)
			el->evaporatePheromone();
	}
};

bool sortPairs(const std::pair<Vertex*, double>& lhs, const std::pair<Vertex*, double>& rhs) { return (lhs.second < rhs.second); }

class Ant {
	Graph *_G;
	Vertex* _position;
	std::vector<Vertex*> _visited;
	std::vector<std::pair<Vertex*, double>> _probabilities;
public:
	Ant(Graph *G) : _G(G) {}

	void addVisited(Vertex* V) { _visited.reserve(_visited.size() + 1); _visited.push_back(V); }

	void setPosition(Vertex* V) { _position = V; /*_visited.push_back(V);*/ }

	Vertex* getPosition() { return _position; }

	std::vector<Vertex*> getVisited() { return _visited; }	

	void probability() {
		std::vector<std::tuple<Vertex*, int, double>> e = _position->getEdges(); _probabilities.clear();
		std::vector<std::tuple<Vertex*, int, double>> edges;

		for (size_t i = 0; i < e.size(); i++) {
			bool b = true;
			for (size_t j = 0; j < _visited.size(); j++)
				if (std::get<0>(e[i]) == _visited[j]) b = false;
			
			if(b)edges.push_back(e[i]);
		}

		// remove _visited from edges

		for (size_t i = 0; i < edges.size(); i++) {
			double sum = 0.0;
			for (size_t j = 0; j < edges.size(); j++)
				sum += std::get<2>(edges[j]) / std::get<1>(edges[j]);

			_probabilities.push_back(std::make_pair(std::get<0>(edges[i]), 100 * (std::get<2>(edges[i]) / std::get<1>(edges[i])) / sum));
		}

		std::sort(_probabilities.begin(), _probabilities.end(), sortPairs);
		
		//for (auto el : _probabilities) std::cout << el.first << " " << el.second << std::endl;
	}

	double Rand(double from, double to) {
		std::random_device rd;
		std::mt19937 gen(rd());
		//std::uniform_int_distribution<> dis(from, to);
		std::uniform_real_distribution<> dis(from, to);

		return dis(gen);
	}

	void makeMove() {	
		_visited.push_back(_position);
		probability();
		if (_probabilities.size() != 0) {
			double r = 0.0, sum = 0.0, lhs = 0.0;
			Vertex* next = nullptr;

			r = Rand(0.0, 100.0);

			for (size_t i = 0; i < _probabilities.size(); i++) {
				sum += _probabilities[i].second;
				if (r > lhs && r < sum) next = _probabilities[i].first;
				lhs = sum;
			}

			_position->addPhoeromone(next, phoeromone);
			_position = next;
		}		
	}

	void lastMove() {
		_position->addPhoeromone(_visited.at(0), phoeromone);		
		_position = _visited.at(0);
		_visited.push_back(_position);
	}

	void run() {
		while (_visited.size() != N) {			
			//std::cout << _position->getId() << std::endl;
			makeMove();
			_G->evaporateP();
		}
		lastMove();
		//showPath();
	}

	void showPath() {
		for (auto el : _visited)
			std::cout << el->getId() << " -> ";
		std::cout << std::endl;
	}

	void reset() {
		_visited.clear();
		_probabilities.clear();
	}
};

class AntColony {
	std::vector<Ant*> _ants;	
	Graph* _G;
	std::vector<std::vector<Vertex*>> _paths;
public:

	AntColony(Graph* G) : _G(G) {}

	void add(Ant* a) { _ants.reserve(_ants.size() + 1); _ants.push_back(a); }

	void run() {
		for (auto ant : _ants)
			ant->run();
		_G->print();
	}

	void resetAnts() {	for (auto ant : _ants)	ant->reset(); }
};

int main() {
	Vertex* a = new Vertex(1); Vertex* b = new Vertex(2); Vertex* c = new Vertex(3);
	Vertex* d = new Vertex(4); Vertex* e = new Vertex(5);
	//	neighbour weight pointed pheromone
	a->AddNeighbour(b, 38, false, 3.0); a->AddNeighbour(c, 74, false, 2.0); a->AddNeighbour(d, 59, false, 2.0); a->AddNeighbour(e, 45, false, 2.0);
	b->AddNeighbour(c, 46, false, 1.0);	b->AddNeighbour(d, 61, false, 1.0); b->AddNeighbour(e, 72, false, 1.0);
	c->AddNeighbour(d, 49, false, 2.0);	c->AddNeighbour(e, 85, false, 2.0);
	d->AddNeighbour(e, 42, false, 1.0);	
	std::vector<Vertex*> G; G.reserve(N);
	G = { a, b, c, d, e };
	Graph graph(G); Graph* ptrG = &graph;
	//
	AntColony ants(ptrG);
	Ant *a1 = new Ant(ptrG), *a2 = new Ant(ptrG), *a3 = new Ant(ptrG), *a4 = new Ant(ptrG), *a5 = new Ant(ptrG);
	std::vector<Ant*> A; A.reserve(N);
	A = { a1, a2, a3, a4, a5 };
	ants.add(a1); ants.add(a2); ants.add(a3); ants.add(a4); ants.add(a5);
	
	size_t iter = 0;

	while (iter < MAX) {
		for (size_t i = 0; i < N; i++)
			A[i]->setPosition(G[i]);

		ants.run();

		for (auto ant : A)
			ant->reset();

		iter++;
	}

	return EXIT_SUCCESS;
}
