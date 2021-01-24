#pragma once
#include <memory>
#include <stack>
#include <queue>
#include <set>
#include "Graph.hpp"
#include "Traverser.hpp"


class Strategy {
public:
    Strategy() = delete;
    explicit Strategy(const Graph& graph, std::shared_ptr<Traverser>& traverser) : graph_(graph) { traverser_ = traverser; }

    virtual void execute(const Graph::vertex& first) = 0;

    virtual ~Strategy() = default;
protected:
    void begin(const Graph::vertex& first) const { traverser_->begin(first); }
    void end() const { traverser_->end(); }
	
    void visit_vertex(const Graph::vertex& v) const { traverser_->visit_vertex(v); }
    void visit_edge(const Graph::vertex& src, const Graph::vertex& dest) const { traverser_->visit_edge(src, dest); }
	
	const bool is_finished() const { return traverser_->is_finished(); }

    const Graph& graph_;
    std::shared_ptr<Traverser> traverser_;
    std::map<Graph::vertex, Graph::vertex> prev_{};
};


class DFS final : public Strategy {
public:
    DFS() = delete;
    explicit DFS(const Graph& graph, std::shared_ptr<Traverser> traverser) : Strategy(graph, traverser) {}

    void execute(const Graph::vertex& first) override {
    	if (this->graph_.vertices_count() == 0) {
            return;
    	}
        this->prev_.clear();
        std::stack<std::pair<Graph::vertex, bool>> s;
        this->begin(first);

        s.push(std::make_pair(first, false));
        this->prev_[first] = first;
        while (!s.empty()) {
            auto& [v, discovered] = s.top();

            if (discovered == false) {
                this->visit_vertex(v);
                this->visit_edge(this->prev_.at(v), v);
                discovered = true;

                if (this->is_finished() == true) {
                    break;
                }

                const auto& adj_v = this->graph_.adj_vertices(v);
                for (auto i = adj_v.crbegin(); i != adj_v.crend(); ++i) {
                    if (*i != this->prev_[v]) {
                        s.push(std::make_pair(*i, false));
                        this->prev_[*i] = v;
                    }
                }
            }
            else {
                s.pop();
            }
        }
        this->end();
    }
};


class BFS final : public Strategy {
public:
    BFS() = delete;
    explicit BFS(const Graph& graph, std::shared_ptr<Traverser> traverser) : Strategy(graph, traverser) {}

    void execute(const Graph::vertex& first) override {
        if (this->graph_.vertices_count() == 0) {
            return;
        }
        this->prev_.clear();
        std::queue<Graph::vertex> q;
        this->begin(first);

        q.push(first);
        this->prev_[first] = first;
        while (!q.empty()) {
            auto v = q.front();
            q.pop();

            this->visit_vertex(v);
            this->visit_edge(this->prev_.at(v), v);

            if (this->is_finished() == true) {
                break;
            }

            const auto& adj_v = this->graph_.adj_vertices(v);
            for (auto i = adj_v.crbegin(); i != adj_v.crend(); ++i) {
                if (*i != this->prev_[v]) {
                    q.push(*i);
                    this->prev_[*i] = v;
                }
            }
        }
        this->end();
    }
};