#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <random>
#include <functional>
#include <math.h>
#define N 14 /// No. of vertx

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

	bool IsConnected(Vertex* V) {
		if (_edges.find(V) != _edges.end()) return true;
		else return false;
	}

	bool HasUnvisited() {
		bool ret = false;

		for (auto it = _edges.cbegin(); it != _edges.cend(); ++it)
			if (!it->first->IsVisited()) ret = true;

		return ret;
	}

	int GetPathCost(Vertex* V) {
		for (auto it = _edges.find(V); it != _edges.cend(); ++it)
			return it->second;
	}

	void AddEdge(Vertex* V, int weight) { _edges.insert(std::make_pair(V, weight));	}

	void AddNeighbour(Vertex* V, int weight, bool dir) {
		_edges.insert(std::make_pair(V, weight));

		if (!dir) V->AddEdge(this, weight);
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
	Graph(std::vector<Vertex*>& G) { _G = G; }

	int CostOfPath(const std::vector<Vertex*> &path) {
		int cost = 0;

		for (unsigned int i = 0; i < N - 1; i++)
			cost += path[i]->GetPathCost(path[i+1]);
			
		cost += path[N - 1]->GetPathCost(path[0]);
		return cost;
	}

	bool PathExist(const std::vector<Vertex*>& path) {
		bool ret = true;

		for (unsigned int i = 0; i < N - 1; i++)
			if (!path.at(i)->IsConnected(path.at(i + 1))) ret = false;
		return ret;
	}
		
	unsigned int Rand(unsigned int from, unsigned int to) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(from, to);

		return dis(gen);
	}

	void Rand_2(unsigned int&r1, unsigned int &r2, unsigned int from, unsigned int to) {
		r1 = Rand(from, to); r2 = Rand(from, to);
		if (r1 == r2) Rand_2(r1, r2, from, to);
	}

	void RandPath(std::vector<Vertex*>& path) {
		std::vector<Vertex*> rand(_G); unsigned int id;
		for (unsigned int i = 0; i <N; i++) {
			id = Rand(0, rand.size() - 1);
			path.push_back(rand.at(id));
			rand.erase(rand.begin() + id);
		}
		path.push_back(path.at(0));
	}

	bool Propability(int ds, double T) {	
		double P = 100 * exp(-ds / T);

		if (P > Rand(1,100)) return true;
		else return false;
	}

	void Combine(std::vector<Vertex*>& path, unsigned int &r1, unsigned int &r2) {
		Rand_2(r1, r2, 0, N - 1);
		bool swapped = false;

		if (r1 == 0) {
			Vertex* tmp = path.at(r1);
			path.at(r1) = path.at(r2);
			path.at(r2) = tmp;
			path.at(N) = path.at(r2);
			swapped = true;
		}
		
		if (r2 == 0 && !swapped) {
			Vertex* tmp = path.at(r1);	
			path.at(r1) = path.at(r2);
			path.at(r2) = tmp;
			path.at(N) = path.at(r1);
		}

		if (r1 != 0 && r2 != 0) {
			Vertex* tmp = path.at(r1);
			path.at(r1) = path.at(r2);
			path.at(r2) = tmp;
		}
		
	}
	
	void Swap(std::vector<Vertex*>& path,const unsigned int &r1,const unsigned int &r2) {
		Vertex* tmp = path.at(r1);
		path.at(r1) = path.at(r2);
		path.at(r2) = tmp;
	}
	
	void SimulatedAnnealing(double a, double T, unsigned int limit) {
		std::vector<Vertex*> path; 	RandPath(path);
		unsigned int cost = CostOfPath(path); unsigned int r1 , r2;	
		std::vector<std::vector<Vertex*>> earlier_paths;
		unsigned int iter = 0;

		earlier_paths.push_back(path);

		while (T > 0.1 && iter < limit) {
			bool swap = true, add = true;	

			Combine(path, r1, r2);			

			if (PathExist(path)) {
				for (auto it : earlier_paths)
					if (it == path) { add = false; swap = false; }

				if (add) earlier_paths.push_back(path);

				if (swap) {					
					if (CostOfPath(path) < cost) { cost = CostOfPath(path); std::cout << "cost = " << cost << " T = " << T << std::endl; T *= a; ShowPath(path);}
					else if (Propability(CostOfPath(path) - cost, T)) { cost = CostOfPath(path); std::cout << "cost = " << cost << " T = " << T << std::endl; T *= a; ShowPath(path);	}
					else Swap(path, r1, r2);					
				}
			}
			else Swap(path, r1, r2);	

			iter++;
		}		
		std::cout << "\n\n";
	}

	void ShowPath(std::vector<Vertex*> &path) {
		for (auto el : path)
			std::cout << el->Id() << "->";
		std::cout << "\n\n";
	}
};

int main() {	
	/*
	
	Vertex* a = new Vertex(0); Vertex* b = new Vertex(1); Vertex* c = new Vertex(2);
	Vertex* d = new Vertex(3); Vertex* e = new Vertex(4); Vertex* f = new Vertex(5);

	a->AddNeighbour(b, 19, false); a->AddNeighbour(c, 41, false); a->AddNeighbour(d, 39, false);
	a->AddNeighbour(e, 27, false); a->AddNeighbour(f, 20, false);

	b->AddNeighbour(c, 24, false); b->AddNeighbour(d, 31, false); b->AddNeighbour(e, 35, false);
	b->AddNeighbour(f, 13, false);

	c->AddNeighbour(d, 20, false); c->AddNeighbour(e, 41, false); c->AddNeighbour(f, 22, false);

	d->AddNeighbour(e, 26, false); d->AddNeighbour(f, 20, false);

	e->AddNeighbour(f, 23, false);
	std::vector<Vertex*> G; G.reserve(N);
	G = { e, c, d, f, b, a};
	*/

	/*
	Vertex* a = new Vertex(0); Vertex* b = new Vertex(1); Vertex* c = new Vertex(2);
	Vertex* d = new Vertex(3); Vertex* e = new Vertex(4); Vertex* f = new Vertex(5); Vertex* g = new Vertex(6);
	a->AddNeighbour(b, 10, false);	a->AddNeighbour(e, 1, false);	a->AddNeighbour(d, 4, false);

	b->AddNeighbour(e, 3, false);	b->AddNeighbour(c, 1, false);	b->AddNeighbour(f, 15, false);

	c->AddNeighbour(e, 2, false);	c->AddNeighbour(d, 1, false);

	d->AddNeighbour(e, 3, false);	d->AddNeighbour(f, 7, false);	d->AddNeighbour(g, 5, false);

	f->AddNeighbour(g, 6, false);
	std::vector<Vertex*> G; G.reserve(N);
	G = { e, c, d, g, f, b, a};
	*/
	
	/*
	Vertex* a = new Vertex(0); Vertex* b = new Vertex(1); Vertex* c = new Vertex(2);
	Vertex* d = new Vertex(3); Vertex* e = new Vertex(4);

	a->AddNeighbour(b, 132, false); a->AddNeighbour(c, 217, false); 
	a->AddNeighbour(d, 164, false);	a->AddNeighbour(e, 58, false);

	b->AddNeighbour(c, 290, false); b->AddNeighbour(d, 201, false);
	b->AddNeighbour(e, 79, false);
	

	c->AddNeighbour(d, 113, false); c->AddNeighbour(e, 303, false); 

	d->AddNeighbour(e, 196, false); 
	
	std::vector<Vertex*> G; G.reserve(N);
	G = { a,b,c,d,e };

	Graph graph(G);		
	*/
	
	//                     alfa | temp | iterations limit
	graph.SimulatedAnnealing(0.5, 100, 100000);
	std::cout << std::endl;
	return EXIT_SUCCESS;
}
