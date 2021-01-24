#include "graph.hpp"

void graph::add_vertex(const vertex& dest, const vertices& src)
{
	for (const auto& n : src){
		data_[n].push_back(dest);
		
		if (vertices_set_.find(n) == vertices_set_.end()){
			vertices_.push_back(n);
			vertices_set_.insert(n);
		}
	}
	vertices_set_.insert(dest);
	if (vertices_set_.find(dest) == vertices_set_.end()) {
		vertices_.push_back(dest);
		vertices_set_.insert(dest);
	}
}

const graph::vertices& graph::dependent_vertices(const vertex& dest) const
{
	const auto& found = data_.find(dest);
	if (found == data_.end()){
		return empty;
	}
	return found->second;
}

const graph::vertices& graph::get_vertices() const
{
	return vertices_;
}