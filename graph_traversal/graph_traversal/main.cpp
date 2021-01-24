#include "Strategy.hpp"
#include "iostream"

/*int main() {
	Graph<int> g;
	g.add_edge(Edge(1, 2));
	g.add_edge(Edge(1, 3));
	g.add_edge(Edge(3, 4));
	g.add_edge(Edge(3, 5));
	g.add_edge(Edge(3, 6));
	g.add_edge(Edge(4, 7));
	g.add_edge(Edge(6, 8));

	const std::shared_ptr<Traverser<int>> tr = std::make_shared<FindPathToEdge<int>>(Edge(4, 7));
	BFS s(g, tr);
	s.execute(Vertex(1));
	for(auto i = tr->get_path().begin(); i != tr->get_path().end(); ++i) {
		std::cout << i->data();
	}
	
	return 0;
}*/