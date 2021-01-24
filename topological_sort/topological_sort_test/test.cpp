#include "pch.h"
#include "topological_sort.hpp"
#include <functional>

TEST(NonCyclicGraph, Empty) {
    const graph g;
    const auto& ordered = topological_sort::sort(g);
    EXPECT_TRUE(ordered.empty());
}

TEST(NonCyclicGraph, OneVertex) {
    graph g;
    graph::vertices vector;
    graph::vertex vertex = "vertex";
    g.add_vertex(vertex, vector);
    auto ordered = topological_sort::sort(g);
    EXPECT_EQ(ordered.front(), vertex);
    ordered.pop_back();
    EXPECT_TRUE(ordered.empty());
}

TEST(NonCyclicGraph, TwoVertices) {
    graph g;
    graph::vertices vector;
    vector.push_back("first");
    graph::vertex vertex = "second";
    g.add_vertex(vertex, vector);
    auto ordered = topological_sort::sort(g);
    EXPECT_EQ(ordered.back(), "second");
    ordered.pop_back();
    EXPECT_EQ(ordered.back(), "first");
    ordered.pop_back();
    EXPECT_TRUE(ordered.empty());
}

TEST(NonCyclicGraph, SimpleTest) {
    graph g;
    graph::vertices vector;
	
    vector.push_back("1");
    graph::vertex vertex = "4";
    g.add_vertex(vertex, vector);
    vector.clear();

    vector.push_back("4");
    vertex = "3";
    g.add_vertex(vertex, vector);
    vector.clear();

    vector.push_back("4");
    vector.push_back("3");
    vertex = "2";
    g.add_vertex(vertex, vector);
    vector.clear();
	
    auto ordered = topological_sort::sort(g);
    EXPECT_EQ(ordered.back(), "2");
    ordered.pop_back();
    EXPECT_EQ(ordered.back(), "3");
    ordered.pop_back();
    EXPECT_EQ(ordered.back(), "4");
    ordered.pop_back();
    EXPECT_EQ(ordered.back(), "1");
    ordered.pop_back();
    EXPECT_TRUE(ordered.empty());
}

TEST(CyclicGraph, OneVertex) {
    graph g;
    graph::vertices vector;
    vector.push_back("first");
    graph::vertex vertex = "first";
    g.add_vertex(vertex, vector);
    graph::vertices ordered;
    try {
        ordered = topological_sort::sort(g);
    }
    catch (std::bad_function_call& ex) {
        EXPECT_TRUE(ordered.empty());
    }
}

TEST(CyclicGraph, SimpleTest) {
    graph g;
    graph::vertices vector;

    vector.push_back("1");
    graph::vertex vertex = "4";
    g.add_vertex(vertex, vector);
    vector.clear();

    vector.push_back("4");
    vertex = "3";
    g.add_vertex(vertex, vector);
    vector.clear();

    vector.push_back("4");
    vector.push_back("3");
    vertex = "2";
    g.add_vertex(vertex, vector);
    vector.clear();

    vector.push_back("2");
    vertex = "1";
    g.add_vertex(vertex, vector);
    vector.clear();
	
    graph::vertices ordered;
    try {
        ordered = topological_sort::sort(g);
    }
    catch (std::bad_function_call& ex) {
        EXPECT_TRUE(ordered.empty());
    }
}