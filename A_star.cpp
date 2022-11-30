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
	unsigned int _h;
	unsigned int _g;
	std::string _name;
	bool _visited;
	Vertex* _parent;
	std::map<Vertex*, int> _edges;
public:
	Vertex(unsigned id, unsigned int h, std::string name) : _id(id), _h(h), _name(name), _visited(false), _parent(nullptr), _g(9999) {}
		
	std::string Name() { return _name; }
	unsigned int Id() { return _id; }
	unsigned int H() { return _h; }
	unsigned int G() { return _g; }
	unsigned int F() { return _g + _h; }

	void setG(unsigned int g) { _g = g; }
	bool IsVisited() { return _visited; }
	void SetVisited(bool v) { _visited = v; }
	Vertex* GetParent() { return _parent; }
	void SetParent(Vertex* newParent) { _parent = newParent; }

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

	int CostOfPath(const std::vector<Vertex*>& path) {		
		int cost = 0;
		for (unsigned int i = 0; i < path.size() - 1; i++)
			cost += path[i]->GetPathCost(path[i + 1]);		
		return cost;
	}

	bool PathExist(const std::vector<Vertex*>& path) {
		bool ret = true;

		for (unsigned int i = 0; i < N - 1; i++)
			if (!path.at(i)->IsConnected(path.at(i + 1))) ret = false;
		return ret;
	}

	bool Contain(std::vector<Vertex*>& vec, Vertex* value) {
		if (std::find(vec.begin(), vec.end(), value) != vec.end()) return true;
		else return false;
	}

	Vertex* GetVertexMin_Fcost(std::vector<Vertex*>& open) {
		unsigned int f_cost = INT_MAX; Vertex* min = open.at(0);
		for (auto it : open)
			if (it->F() < f_cost) { f_cost = it->F(); min = it; }
		return min;
	}

	void A_star(Vertex* start, Vertex* goal) {
		std::vector<Vertex*> path, open, closed; Vertex* current = start;
		open.push_back(start);

		while (open.size() > 0) {
			current = GetVertexMin_Fcost(open);

			if (current == goal) {	MakePath(start, goal, path); ShowPath(path); return; }

			for (auto it : current->GetEdges()) {
				unsigned int cost = current->G() + current->GetPathCost(it.first);
				if (cost < it.first->G()) {	it.first->setG(cost); it.first->SetParent(current);	}
				if (!Contain(open, it.first) && !Contain(closed, it.first)) open.push_back(it.first);
				//else it.first->SetParent(current);
			}			
			closed.push_back(current);			
			open.erase(std::remove(open.begin(), open.end(), current), open.end());
		}
	}

	void MakePath(Vertex* start, Vertex* goal, std::vector<Vertex*>& path) {
		Vertex* current = goal;

		while (current != nullptr) { 
			path.push_back(current); 
			current = current->GetParent();	}	
		std::reverse(path.begin(), path.end());
	}
	void ShowPath(std::vector<Vertex*>& path) {				
		for (auto el : path)
			std::cout << el->Name() << "->";	
		std::cout <<"\nCost of path: " << CostOfPath(path) << std::endl;
	}
};

int main() {

	//*
	Vertex* Sz = new Vertex(0, 600, "Sz"); Vertex* Gd = new Vertex(1, 546, "Gd"); Vertex* Ol = new Vertex(2, 468, "Ol"); Vertex* Bi = new Vertex(3, 470, "Bi");
	Vertex* Lu = new Vertex(4, 259, "Lu"); Vertex* Kr = new Vertex(5, 0, "Kr"); Vertex* Ka = new Vertex(6, 75, "Ka"); Vertex* Wr = new Vertex(7, 254, "Wr");
	Vertex* Zg = new Vertex(8, 409, "Zg"); Vertex* Po = new Vertex(9, 378, "Po"); Vertex* By = new Vertex(10, 408, "By"); Vertex* Wa = new Vertex(11, 282, "Wa");
	Vertex* Ki = new Vertex(12, 115, "Ki"); Vertex* L = new Vertex(13, 217, "L");	

	Sz->AddNeighbour(Gd, 362, false); Sz->AddNeighbour(By, 200, false); Sz->AddNeighbour(Po, 235, false); Sz->AddNeighbour(Zg, 217, false);
	Gd->AddNeighbour(By, 165, false); Gd->AddNeighbour(Wa, 377, false); Gd->AddNeighbour(Ol, 170, false);
	Ol->AddNeighbour(Wa, 212, false); Ol->AddNeighbour(Bi, 200, false);
	Bi->AddNeighbour(Wa, 193, false); Bi->AddNeighbour(Lu, 200, false);
	Lu->AddNeighbour(Wa, 166, false); Lu->AddNeighbour(Ki, 200, false); Lu->AddNeighbour(Kr, 200, false);
	Kr->AddNeighbour(Ki, 116, false); Kr->AddNeighbour(Ka, 80, false);
	Ka->AddNeighbour(L, 193, false); Ka->AddNeighbour(Wr, 195, false);
	Wr->AddNeighbour(L, 209, false); Wr->AddNeighbour(Zg, 156, false); Wr->AddNeighbour(Po, 186, false);
	Zg->AddNeighbour(Po, 193, false); Zg->AddNeighbour(L, 200, false);
	Po->AddNeighbour(By, 200, false); Po->AddNeighbour(Wa, 319, false); Po->AddNeighbour(L, 203, false);
	By->AddNeighbour(Wa, 270, false); By->AddNeighbour(Ol, 200, false); By->AddNeighbour(L, 227, false);
	Wa->AddNeighbour(L, 137, false); Wa->AddNeighbour(Ki, 178, false);
	Ki->AddNeighbour(L, 200, false);

	Gd->setG(0);
	//Sz->setG(0);

	std::vector<Vertex*> G; G.reserve(N);
	G = { Sz, Gd, Ol, Bi, Lu, Kr, Ka, Wr, Zg, Po, By, Wa, Ki, L };
	Graph graph(G);
	graph.A_star(Gd, Kr);
	//*/

	/*
	Vertex* s = new Vertex(0,5,"S"); Vertex* a = new Vertex(1,3, "A"); Vertex* b = new Vertex(2,4,"B");
	Vertex* c = new Vertex(3,2,"C"); Vertex* d = new Vertex(4,6, "D"); Vertex* g = new Vertex(5,0, "G");
	s->setG(0);

	std::vector<Vertex*> G; G.reserve(N);
	G = { s, a, b, c, d, g };

	s->AddNeighbour(a, 1, true); s->AddNeighbour(g, 10, true);
	a->AddNeighbour(b, 2, true); a->AddNeighbour(c, 1, true);
	b->AddNeighbour(d, 5, true); 
	c->AddNeighbour(g, 4, true);
	d->AddNeighbour(g, 2, true);

	Graph graph(G);
	graph.A_star(s,g);
	*/
	std::cout << std::endl;
	return EXIT_SUCCESS;
}
