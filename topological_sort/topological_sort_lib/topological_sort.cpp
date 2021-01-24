#include "topological_sort.hpp"
#include <functional>
#include <iostream>
#include <stack>

graph::vertices topological_sort::sort(const graph& g)
{
    visited_vertices visited;
    graph::vertices ordered;
    auto vertices = g.get_vertices();
    std::reverse(vertices.begin(), vertices.end());
	for (const auto& v : vertices) {
        visited[v] = colors::white;
	}
	
    for (const auto& v : vertices) {
        if (visited.empty()) {
            throw std::bad_function_call();
        }
    	if (visited[v] == colors::white) {
            depth_sorting_traversal(g, v, visited, ordered);
        }
    }
    std::reverse(ordered.begin(), ordered.end());
    return ordered;
}

void topological_sort::depth_sorting_traversal(const graph& g, const graph::vertex& v, visited_vertices& visited, graph::vertices& ordered)
{
    std::stack<std::pair<graph::vertex, bool>> stack;
    stack.push(std::make_pair(v, false));
    visited[v] = colors::gray;
    while (!stack.empty())
    {
        auto [vertex, detector] = stack.top();
        stack.pop();
    	
        if (detector) {
            visited[vertex] = colors::black;
            ordered.push_back(vertex);
        	continue;
        }
    	
        stack.push(std::make_pair(vertex, true));
        for (const auto& i : g.dependent_vertices(vertex)) {
            if (visited[i] == colors::white) {
                stack.push(std::make_pair(i, false));
                visited[i] = colors::gray;
            }
            else if (visited[i] == colors::gray){
                visited.clear();
                return;
            }
        }
    }
}