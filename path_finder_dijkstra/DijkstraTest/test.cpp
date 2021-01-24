#include "pch.h"
#include "PathFinderDijkstra.hpp"

TEST(EmptyGraph, GetPath) {
	const Graph graph;
	PathFinderDijkstra finder;
	finder.Find(graph, "Moscow");
	const auto& vertices_list = finder.GetShortestPath("Novosibirsk");
	EXPECT_TRUE(vertices_list.empty());
}

TEST(EmptyGraph, GetDistance) {
	const Graph graph;
	PathFinderDijkstra finder;
	finder.Find(graph, "Moscow");
	EXPECT_EQ(finder.GetShortestDistance("Novosibirsk"), ~((size_t)0));
}

TEST(NonExistSrcVertex, GetPath) {
	Graph graph;
	graph.AddEdge("Moscow", 7, "Novosibirsk");
	PathFinderDijkstra finder;
	finder.Find(graph, "Omsk");
	const auto& vertices_list = finder.GetShortestPath("Novosibirsk");
	EXPECT_TRUE(vertices_list.empty());
}

TEST(NonExistSrcVertex, GetDistance) {
	Graph graph;
	graph.AddEdge("Moscow", 7, "Novosibirsk");
	PathFinderDijkstra finder;
	finder.Find(graph, "Omsk");
	EXPECT_EQ(finder.GetShortestDistance("Novosibirsk"), ~((size_t)0));
}

TEST(NonExistDestVertex, GetPath) {
	Graph graph;
	graph.AddEdge("Moscow", 7, "Novosibirsk");
	PathFinderDijkstra finder;
	finder.Find(graph, "Novosibirsk");
	const auto& vertices_list = finder.GetShortestPath("Omsk");
	EXPECT_TRUE(vertices_list.empty());
}

TEST(NonExistDestVertex, GetDistance) {
	Graph graph;
	graph.AddEdge("Moscow", 7, "Novosibirsk");
	PathFinderDijkstra finder;
	finder.Find(graph, "Novosibirsk");
	EXPECT_EQ(finder.GetShortestDistance("Omsk"), ~((size_t)0));
}

TEST(SimpleTest, GetDistance) {
	Graph graph;
	graph.AddEdge("Moscow", 7, "Novosibirsk");
	graph.AddEdge("Moscow", 9, "Toronto");
	graph.AddEdge("Moscow", 14, "Krasnoyarsk");
	graph.AddEdge("Novosibirsk", 10, "Toronto");
	graph.AddEdge("Novosibirsk", 15, "Omsk");
	graph.AddEdge("Omsk", 11, "Toronto");
	graph.AddEdge("Toronto", 2, "Krasnoyarsk");
	graph.AddEdge("Krasnoyarsk", 9, "Kiev");
	graph.AddEdge("Kiev", 6, "Omsk");

	PathFinderDijkstra finder;
	finder.Find(graph, "Moscow");
	EXPECT_EQ(0, finder.GetShortestDistance("Moscow"));
	EXPECT_EQ(7, finder.GetShortestDistance("Novosibirsk"));
	EXPECT_EQ(9, finder.GetShortestDistance("Toronto"));
	EXPECT_EQ(11, finder.GetShortestDistance("Krasnoyarsk"));
	EXPECT_EQ(20, finder.GetShortestDistance("Omsk"));
	EXPECT_EQ(20, finder.GetShortestDistance("Kiev"));
}

TEST(SimpleTest, GetPath) {
	Graph graph;
	graph.AddEdge("Moscow", 7, "Novosibirsk");
	graph.AddEdge("Moscow", 9, "Toronto");
	graph.AddEdge("Moscow", 14, "Krasnoyarsk");
	graph.AddEdge("Novosibirsk", 10, "Toronto");
	graph.AddEdge("Novosibirsk", 15, "Omsk");
	graph.AddEdge("Omsk", 11, "Toronto");
	graph.AddEdge("Toronto", 2, "Krasnoyarsk");
	graph.AddEdge("Krasnoyarsk", 9, "Kiev");
	graph.AddEdge("Kiev", 6, "Omsk");

	PathFinderDijkstra finder;
	finder.Find(graph, "Moscow");

	auto zero_path = finder.GetShortestPath("Moscow");
	EXPECT_TRUE(zero_path.empty());

	auto first_path = finder.GetShortestPath("Novosibirsk");
	EXPECT_EQ("Moscow", first_path.front());
	first_path.pop_front();
	EXPECT_TRUE(first_path.empty());

	auto second_path = finder.GetShortestPath("Toronto");
	EXPECT_EQ("Moscow", second_path.front());
	second_path.pop_front();
	EXPECT_TRUE(second_path.empty());

	auto third_path = finder.GetShortestPath("Krasnoyarsk");
	EXPECT_EQ("Moscow", third_path.front());
	third_path.pop_front();
	EXPECT_EQ("Toronto", third_path.front());
	third_path.pop_front();
	EXPECT_TRUE(third_path.empty());

	auto fourth_path = finder.GetShortestPath("Omsk");
	EXPECT_EQ("Moscow", fourth_path.front());
	fourth_path.pop_front();
	EXPECT_EQ("Toronto", fourth_path.front());
	fourth_path.pop_front();
	EXPECT_TRUE(fourth_path.empty());

	auto fifth_path = finder.GetShortestPath("Kiev");
	EXPECT_EQ("Moscow", fifth_path.front());
	fifth_path.pop_front();
	EXPECT_EQ("Toronto", fifth_path.front());
	fifth_path.pop_front();
	EXPECT_EQ("Krasnoyarsk", fifth_path.front());
	fifth_path.pop_front();
	EXPECT_TRUE(fifth_path.empty());
}