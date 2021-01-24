#pragma once

#include "Graph.hpp"


class Traverser {
public:
    Traverser() = default;

    virtual void begin(const Graph::vertex& first) {
        first_ = first;
        is_finished_ = false;
        is_cyclic_ = false;
        path_.clear();
    }
	
    virtual void visit_vertex(const Graph::vertex& v) = 0;
    virtual void visit_edge(const Graph::vertex& src, const Graph::vertex& dest) = 0;
	
    virtual void end() = 0;

	const bool is_finished() const {
        return is_finished_;
    }

    const std::vector<Graph::vertex>& get_path() const {
        return path_;
    }

    const bool is_cyclic() const {
        return is_cyclic_;
	}

    virtual ~Traverser() = default;
protected:
    bool is_finished_ = false;
    bool is_cyclic_ = false;
    Graph::vertex first_{};
    std::vector<Graph::vertex> path_{};
};


class FindPath : public Traverser {
public:
    FindPath() = default;

	void visit_vertex(const Graph::vertex& v) override {
        this->is_finished_ = comp(v);
	}

	void visit_edge(const Graph::vertex& src, const Graph::vertex& dest) override {
        this->is_finished_ = comp(src, dest);
        prev_[dest] = src;
    }

	void end() override {
        if (this->is_finished_ == false) {
            this->path_.clear();
            return;
        }
        trace_path();
        std::reverse(this->path_.begin(), this->path_.end());
    }
	
	virtual ~FindPath() = default;
protected:
    std::map<Graph::vertex, Graph::vertex> prev_{};

    virtual bool comp(const Graph::vertex& vertex) = 0;
    virtual bool comp(const Graph::vertex& src, const Graph::vertex& dest) = 0;
    virtual void trace_path() = 0;
};


class FindPathToVertex final : public FindPath {
public:
    FindPathToVertex() = delete;
    explicit FindPathToVertex(const Graph::vertex& target) : FindPath(), target_(target) {}
	
private:
    const Graph::vertex target_;
	
    bool comp(const Graph::vertex& vertex) override {
        return vertex == target_;
    }
	
	bool comp(const Graph::vertex& src, const Graph::vertex& dest) override {
        return this->is_finished_;
    }

	void trace_path() override {
        auto v = target_;
        while (v != this->first_) {
            this->path_.emplace_back(v);
            v = prev_[v];
        }
        this->path_.emplace_back(v);
    }
};


class FindPathToEdge final : public FindPath {
public:
    FindPathToEdge() = delete;
    explicit FindPathToEdge(const Graph::vertex& src, const Graph::vertex& dest) : FindPath(), src_(src), dest_(dest) {}
private:
    const Graph::vertex src_, dest_;

    bool comp(const Graph::vertex&) override {
        return this->is_finished_;
    }

    bool comp(const Graph::vertex& src, const Graph::vertex& dest) override {
        return src_ == src && dest_ == dest;
    }

    void trace_path() override {
        auto v = dest_;
        while (v != this->first_) {
            this->path_.emplace_back(v);
            v = prev_[v];
        }
        this->path_.emplace_back(v);
    }
};


class CheckCycle final : public Traverser {
public:
    CheckCycle() = default;
    void visit_vertex(const Graph::vertex& v) override {
    	if (visited_.find(v) != visited_.end()) {
            this->is_finished_ = true;
            this->is_cyclic_ = true;
            return;
    	}
        visited_.emplace(v);
    }
	
    void visit_edge(const Graph::vertex& src, const Graph::vertex& dest) override {}

    void end() override { }
private:
    std::set<Graph::vertex> visited_;
    std::map<Graph::vertex, Graph::vertex> prev_;
};