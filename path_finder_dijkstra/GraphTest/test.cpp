#include "pch.h"
#include "Graph.hpp"

TEST(EmptyGraph, GetVerticesList) {
    const Graph graph;
    EXPECT_TRUE(graph.GetVerticesList().empty());
}

TEST(EmptyGraph, GetAdjList) {
    const Graph graph;
    EXPECT_TRUE(graph.GetAdjList("Moscow").empty());
}

TEST(OneEdge, GetVerticesList) {
    Graph graph;
    graph.AddEdge("Moscow", 7, "Novosibirsk");
    auto vertices_list = graph.GetVerticesList();
    EXPECT_EQ("Moscow", vertices_list.front());
    vertices_list.pop_front();
    EXPECT_EQ("Novosibirsk", vertices_list.front());
    vertices_list.pop_front();
    EXPECT_TRUE(vertices_list.empty());
}

TEST(NonExsistVertex, GetAdjList) {
    Graph graph;
    graph.AddEdge("Moscow", 7, "Novosibirsk");
    EXPECT_TRUE(graph.GetAdjList("Omsk").empty());
}

TEST(ExistVertex, GetAdjList) {
    Graph graph;
    graph.AddEdge("Moscow", 7, "Novosibirsk");

    auto adj_list_first = graph.GetAdjList("Moscow");
    EXPECT_EQ("Novosibirsk", adj_list_first.front().first);
    EXPECT_EQ(7, adj_list_first.front().second);
    adj_list_first.pop_front();
    EXPECT_TRUE(adj_list_first.empty());

    auto adj_list_second = graph.GetAdjList("Novosibirsk");
    EXPECT_EQ("Moscow", adj_list_second.front().first);
    EXPECT_EQ(7, adj_list_second.front().second);
    adj_list_second.pop_front();
    EXPECT_TRUE(adj_list_second.empty());
}