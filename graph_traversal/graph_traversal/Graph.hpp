#pragma once

#include <map>
#include <vector>


class Graph final {
public:
	Graph() = default;
	using vertex = int;
	
	void add_edge(const vertex& src, const vertex& dest) {
		data_[src].push_back(dest);
		data_[dest];
	}

	const std::vector<vertex>& adj_vertices(const vertex& v) const {
		return data_.at(v);
	}

	const size_t vertices_count() const {
		return data_.size();
	}
private:
	std::map<vertex, std::vector<vertex>> data_ {};
};