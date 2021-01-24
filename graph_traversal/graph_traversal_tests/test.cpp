#include "pch.h"
#include "Strategy.hpp"


TEST(DFS, EmptyGraph) {
	const Graph g;
	const std::shared_ptr<Traverser> tr = std::make_shared<FindPathToVertex>(4);
	DFS s(g, tr);
	s.execute(1);
	EXPECT_TRUE(tr->get_path().empty());
}

TEST(DFS, VisitNonExistentVertex) {
	Graph g;
	g.add_edge(1, 2);
	g.add_edge(1, 3);

	const std::shared_ptr<Traverser> tr = std::make_shared<FindPathToVertex>(4);
	DFS s(g, tr);
	s.execute(1);
	EXPECT_TRUE(tr->get_path().empty());
}

TEST (DFS, VisitExistentVertex) {
	Graph g;
	g.add_edge(1, 2);
	g.add_edge(2, 1);
	g.add_edge(1, 3);
	g.add_edge(3, 1);
	g.add_edge(1, 4);
	g.add_edge(4, 1);
	g.add_edge(4, 5);
	g.add_edge(5, 4);
	g.add_edge(4, 6);
	g.add_edge(6, 4);
	g.add_edge(4, 7);
	g.add_edge(7, 4);
	g.add_edge(6, 8);
	g.add_edge(8, 6);
	g.add_edge(6, 9);
	g.add_edge(9, 6);

	std::shared_ptr<Traverser> tr = std::make_shared<FindPathToVertex>(4);
	DFS s(g, tr);
	s.execute(1);
	ASSERT_EQ(tr->get_path().size(), 2);
	EXPECT_EQ(tr->get_path().at(0), 1);
	EXPECT_EQ(tr->get_path().at(1), 4);

	const std::shared_ptr<Traverser> tr0 = std::make_shared<FindPathToVertex>(1);
	DFS s0(g, tr0);
	s0.execute(1);
	ASSERT_EQ(tr0->get_path().size(), 1);
	EXPECT_EQ(tr0->get_path().at(0), 1);
	
	const std::shared_ptr<Traverser> tr1 = std::make_shared<FindPathToVertex>(2);
	DFS s1(g, tr1);
	s1.execute(1);
	ASSERT_EQ(tr1->get_path().size(), 2);
	EXPECT_EQ(tr1->get_path().at(0), 1);
	EXPECT_EQ(tr1->get_path().at(1), 2);

	const std::shared_ptr<Traverser> tr2 = std::make_shared<FindPathToVertex>(3);
	DFS s2(g, tr2);
	s2.execute(1);
	ASSERT_EQ(tr2->get_path().size(), 2);
	EXPECT_EQ(tr2->get_path().at(0), 1);
	EXPECT_EQ(tr2->get_path().at(1), 3);

	const std::shared_ptr<Traverser> tr3 = std::make_shared<FindPathToVertex>(5);
	DFS s3(g, tr3);
	s3.execute(1);
	ASSERT_EQ(tr3->get_path().size(), 3);
	EXPECT_EQ(tr3->get_path().at(0), 1);
	EXPECT_EQ(tr3->get_path().at(1), 4);
	EXPECT_EQ(tr3->get_path().at(2), 5);

	const std::shared_ptr<Traverser> tr4 = std::make_shared<FindPathToVertex>(6);
	DFS s4(g, tr4);
	s4.execute(1);
	ASSERT_EQ(tr4->get_path().size(), 3);
	EXPECT_EQ(tr4->get_path().at(0), 1);
	EXPECT_EQ(tr4->get_path().at(1), 4);
	EXPECT_EQ(tr4->get_path().at(2), 6);

	const std::shared_ptr<Traverser> tr5 = std::make_shared<FindPathToVertex>(7);
	DFS s5(g, tr5);
	s5.execute(1);
	ASSERT_EQ(tr5->get_path().size(), 3);
	EXPECT_EQ(tr5->get_path().at(0), 1);
	EXPECT_EQ(tr5->get_path().at(1), 4);
	EXPECT_EQ(tr5->get_path().at(2), 7);
	
	const std::shared_ptr<Traverser> tr6 = std::make_shared<FindPathToVertex>(8);
	DFS s6(g, tr6);
	s6.execute(1);
	ASSERT_EQ(tr6->get_path().size(), 4);
	EXPECT_EQ(tr6->get_path().at(0), 1);
	EXPECT_EQ(tr6->get_path().at(1), 4);
	EXPECT_EQ(tr6->get_path().at(2), 6);
	EXPECT_EQ(tr6->get_path().at(3), 8);

	const std::shared_ptr<Traverser> tr7 = std::make_shared<FindPathToVertex>(9);
	DFS s7(g, tr7);
	s7.execute(1);
	ASSERT_EQ(tr7->get_path().size(), 4);
	EXPECT_EQ(tr7->get_path().at(0), 1);
	EXPECT_EQ(tr7->get_path().at(1), 4);
	EXPECT_EQ(tr7->get_path().at(2), 6);
	EXPECT_EQ(tr7->get_path().at(3), 9);
}

TEST(DFS, VisitNonExistentEdge) {
	Graph g;
	g.add_edge(1, 2);
	g.add_edge(1, 3);

	const std::shared_ptr<Traverser> tr = std::make_shared<FindPathToEdge>(1, 4);
	DFS s(g, tr);
	s.execute(1);
	EXPECT_TRUE(tr->get_path().empty());
}

TEST(DFS, VisitExistentEdge) {
	Graph g;
	g.add_edge(1, 2);
	g.add_edge(1, 3);
	g.add_edge(1, 4);
	g.add_edge(4, 5);
	g.add_edge(4, 6);
	g.add_edge(4, 7);
	g.add_edge(6, 8);
	g.add_edge(6, 9);

	std::shared_ptr<Traverser> tr = std::make_shared<FindPathToEdge>(1, 4);
	DFS s(g, tr);
	s.execute(1);
	ASSERT_EQ(tr->get_path().size(), 2);
	EXPECT_EQ(tr->get_path().at(0), 1);
	EXPECT_EQ(tr->get_path().at(1), 4);

	const std::shared_ptr<Traverser> tr1 = std::make_shared<FindPathToEdge>(1, 2);
	DFS s1(g, tr1);
	s1.execute(1);
	ASSERT_EQ(tr1->get_path().size(), 2);
	EXPECT_EQ(tr1->get_path().at(0), 1);
	EXPECT_EQ(tr1->get_path().at(1), 2);

	const std::shared_ptr<Traverser> tr2 = std::make_shared<FindPathToEdge>(1, 3);
	DFS s2(g, tr2);
	s2.execute(1);
	ASSERT_EQ(tr2->get_path().size(), 2);
	EXPECT_EQ(tr2->get_path().at(0), 1);
	EXPECT_EQ(tr2->get_path().at(1), 3);

	const std::shared_ptr<Traverser> tr3 = std::make_shared<FindPathToEdge>(4, 5);
	DFS s3(g, tr3);
	s3.execute(1);
	ASSERT_EQ(tr3->get_path().size(), 3);
	EXPECT_EQ(tr3->get_path().at(0), 1);
	EXPECT_EQ(tr3->get_path().at(1), 4);
	EXPECT_EQ(tr3->get_path().at(2), 5);

	const std::shared_ptr<Traverser> tr4 = std::make_shared<FindPathToEdge>(4, 6);
	DFS s4(g, tr4);
	s4.execute(1);
	ASSERT_EQ(tr4->get_path().size(), 3);
	EXPECT_EQ(tr4->get_path().at(0), 1);
	EXPECT_EQ(tr4->get_path().at(1), 4);
	EXPECT_EQ(tr4->get_path().at(2), 6);

	const std::shared_ptr<Traverser> tr5 = std::make_shared<FindPathToEdge>(4, 7);
	DFS s5(g, tr5);
	s5.execute(1);
	ASSERT_EQ(tr5->get_path().size(), 3);
	EXPECT_EQ(tr5->get_path().at(0), 1);
	EXPECT_EQ(tr5->get_path().at(1), 4);
	EXPECT_EQ(tr5->get_path().at(2), 7);

	const std::shared_ptr<Traverser> tr6 = std::make_shared<FindPathToEdge>(6, 8);
	DFS s6(g, tr6);
	s6.execute(1);
	ASSERT_EQ(tr6->get_path().size(), 4);
	EXPECT_EQ(tr6->get_path().at(0), 1);
	EXPECT_EQ(tr6->get_path().at(1), 4);
	EXPECT_EQ(tr6->get_path().at(2), 6);
	EXPECT_EQ(tr6->get_path().at(3), 8);

	const std::shared_ptr<Traverser> tr7 = std::make_shared<FindPathToEdge>(6, 9);
	DFS s7(g, tr7);
	s7.execute(1);
	ASSERT_EQ(tr7->get_path().size(), 4);
	EXPECT_EQ(tr7->get_path().at(0), 1);
	EXPECT_EQ(tr7->get_path().at(1), 4);
	EXPECT_EQ(tr7->get_path().at(2), 6);
	EXPECT_EQ(tr7->get_path().at(3), 9);
}

TEST(DFS, CyclicGraph) {
	Graph g;
	g.add_edge(1, 2);
	g.add_edge(1, 3);
	g.add_edge(1, 4);
	g.add_edge(4, 5);
	g.add_edge(4, 6);
	g.add_edge(4, 7);
	g.add_edge(6, 8);
	g.add_edge(6, 9);

	const std::shared_ptr<Traverser> tr = std::make_shared<CheckCycle>();
	DFS s(g, tr);
	s.execute(1);
	EXPECT_FALSE(tr->is_cyclic());

	g.add_edge(4, 8);
	s.execute(1);
	EXPECT_TRUE(tr->is_cyclic());

	Graph g1;
	g1.add_edge(1, 2);
	g1.add_edge(1, 3);
	g1.add_edge(1, 4);
	g1.add_edge(4, 5);
	g1.add_edge(4, 6);
	g1.add_edge(4, 7);
	g1.add_edge(6, 8);
	g1.add_edge(6, 9);
	g1.add_edge(1, 9);
	DFS s1(g1, tr);
	s1.execute(1);
	EXPECT_TRUE(tr->is_cyclic());
}

TEST(BFS, EmptyGraph) {
	const Graph g;
	const std::shared_ptr<Traverser> tr = std::make_shared<FindPathToVertex>(4);
	BFS s(g, tr);
	s.execute(1);
	EXPECT_TRUE(tr->get_path().empty());
}

TEST(BFS, VisitNonExistentVertex) {
	Graph g;
	g.add_edge(1, 2);
	g.add_edge(1, 3);

	const std::shared_ptr<Traverser> tr = std::make_shared<FindPathToVertex>(4);
	BFS s(g, tr);
	s.execute(1);
	EXPECT_TRUE(tr->get_path().empty());
}

TEST(BFS, VisitExistentVertex) {
	Graph g;
	g.add_edge(1, 2);
	g.add_edge(1, 3);
	g.add_edge(1, 4);
	g.add_edge(4, 5);
	g.add_edge(4, 6);
	g.add_edge(4, 7);
	g.add_edge(6, 8);
	g.add_edge(6, 9);

	std::shared_ptr<Traverser> tr = std::make_shared<FindPathToVertex>(4);
	BFS s(g, tr);
	s.execute(1);
	ASSERT_EQ(tr->get_path().size(), 2);
	EXPECT_EQ(tr->get_path().at(0), 1);
	EXPECT_EQ(tr->get_path().at(1), 4);

	const std::shared_ptr<Traverser> tr0 = std::make_shared<FindPathToVertex>(1);
	BFS s0(g, tr0);
	s0.execute(1);
	ASSERT_EQ(tr0->get_path().size(), 1);
	EXPECT_EQ(tr0->get_path().at(0), 1);

	const std::shared_ptr<Traverser> tr1 = std::make_shared<FindPathToVertex>(2);
	BFS s1(g, tr1);
	s1.execute(1);
	ASSERT_EQ(tr1->get_path().size(), 2);
	EXPECT_EQ(tr1->get_path().at(0), 1);
	EXPECT_EQ(tr1->get_path().at(1), 2);

	const std::shared_ptr<Traverser> tr2 = std::make_shared<FindPathToVertex>(3);
	BFS s2(g, tr2);
	s2.execute(1);
	ASSERT_EQ(tr2->get_path().size(), 2);
	EXPECT_EQ(tr2->get_path().at(0), 1);
	EXPECT_EQ(tr2->get_path().at(1), 3);

	const std::shared_ptr<Traverser> tr3 = std::make_shared<FindPathToVertex>(5);
	BFS s3(g, tr3);
	s3.execute(1);
	ASSERT_EQ(tr3->get_path().size(), 3);
	EXPECT_EQ(tr3->get_path().at(0), 1);
	EXPECT_EQ(tr3->get_path().at(1), 4);
	EXPECT_EQ(tr3->get_path().at(2), 5);

	const std::shared_ptr<Traverser> tr4 = std::make_shared<FindPathToVertex>(6);
	BFS s4(g, tr4);
	s4.execute(1);
	ASSERT_EQ(tr4->get_path().size(), 3);
	EXPECT_EQ(tr4->get_path().at(0), 1);
	EXPECT_EQ(tr4->get_path().at(1), 4);
	EXPECT_EQ(tr4->get_path().at(2), 6);

	const std::shared_ptr<Traverser> tr5 = std::make_shared<FindPathToVertex>(7);
	BFS s5(g, tr5);
	s5.execute(1);
	ASSERT_EQ(tr5->get_path().size(), 3);
	EXPECT_EQ(tr5->get_path().at(0), 1);
	EXPECT_EQ(tr5->get_path().at(1), 4);
	EXPECT_EQ(tr5->get_path().at(2), 7);

	const std::shared_ptr<Traverser> tr6 = std::make_shared<FindPathToVertex>(8);
	BFS s6(g, tr6);
	s6.execute(1);
	ASSERT_EQ(tr6->get_path().size(), 4);
	EXPECT_EQ(tr6->get_path().at(0), 1);
	EXPECT_EQ(tr6->get_path().at(1), 4);
	EXPECT_EQ(tr6->get_path().at(2), 6);
	EXPECT_EQ(tr6->get_path().at(3), 8);

	const std::shared_ptr<Traverser> tr7 = std::make_shared<FindPathToVertex>(9);
	BFS s7(g, tr7);
	s7.execute(1);
	ASSERT_EQ(tr7->get_path().size(), 4);
	EXPECT_EQ(tr7->get_path().at(0), 1);
	EXPECT_EQ(tr7->get_path().at(1), 4);
	EXPECT_EQ(tr7->get_path().at(2), 6);
	EXPECT_EQ(tr7->get_path().at(3), 9);
}

TEST(BFS, VisitNonExistentEdge) {
	Graph g;
	g.add_edge(1, 2);
	g.add_edge(1, 3);

	const std::shared_ptr<Traverser> tr = std::make_shared<FindPathToEdge>(1, 4);
	BFS s(g, tr);
	s.execute(1);
	EXPECT_TRUE(tr->get_path().empty());
}

TEST(BFS, VisitExistentEdge) {
	Graph g;
	g.add_edge(1, 2);
	g.add_edge(2, 1);
	g.add_edge(1, 3);
	g.add_edge(3, 1);
	g.add_edge(1, 4);
	g.add_edge(4, 1);
	g.add_edge(4, 5);
	g.add_edge(5, 4);
	g.add_edge(4, 6);
	g.add_edge(6, 4);
	g.add_edge(4, 7);
	g.add_edge(7, 4);
	g.add_edge(6, 8);
	g.add_edge(8, 6);
	g.add_edge(6, 9);
	g.add_edge(9, 6);

	std::shared_ptr<Traverser> tr = std::make_shared<FindPathToEdge>(1, 4);
	BFS s(g, tr);
	s.execute(1);
	ASSERT_EQ(tr->get_path().size(), 2);
	EXPECT_EQ(tr->get_path().at(0), 1);
	EXPECT_EQ(tr->get_path().at(1), 4);

	const std::shared_ptr<Traverser> tr1 = std::make_shared<FindPathToEdge>(1, 2);
	BFS s1(g, tr1);
	s1.execute(1);
	ASSERT_EQ(tr1->get_path().size(), 2);
	EXPECT_EQ(tr1->get_path().at(0), 1);
	EXPECT_EQ(tr1->get_path().at(1), 2);

	const std::shared_ptr<Traverser> tr2 = std::make_shared<FindPathToEdge>(1, 3);
	BFS s2(g, tr2);
	s2.execute(1);
	ASSERT_EQ(tr2->get_path().size(), 2);
	EXPECT_EQ(tr2->get_path().at(0), 1);
	EXPECT_EQ(tr2->get_path().at(1), 3);

	const std::shared_ptr<Traverser> tr3 = std::make_shared<FindPathToEdge>(4, 5);
	BFS s3(g, tr3);
	s3.execute(1);
	ASSERT_EQ(tr3->get_path().size(), 3);
	EXPECT_EQ(tr3->get_path().at(0), 1);
	EXPECT_EQ(tr3->get_path().at(1), 4);
	EXPECT_EQ(tr3->get_path().at(2), 5);

	const std::shared_ptr<Traverser> tr4 = std::make_shared<FindPathToEdge>(4, 6);
	BFS s4(g, tr4);
	s4.execute(1);
	ASSERT_EQ(tr4->get_path().size(), 3);
	EXPECT_EQ(tr4->get_path().at(0), 1);
	EXPECT_EQ(tr4->get_path().at(1), 4);
	EXPECT_EQ(tr4->get_path().at(2), 6);

	const std::shared_ptr<Traverser> tr5 = std::make_shared<FindPathToEdge>(4, 7);
	BFS s5(g, tr5);
	s5.execute(1);
	ASSERT_EQ(tr5->get_path().size(), 3);
	EXPECT_EQ(tr5->get_path().at(0), 1);
	EXPECT_EQ(tr5->get_path().at(1), 4);
	EXPECT_EQ(tr5->get_path().at(2), 7);

	const std::shared_ptr<Traverser> tr6 = std::make_shared<FindPathToEdge>(6, 8);
	BFS s6(g, tr6);
	s6.execute(1);
	ASSERT_EQ(tr6->get_path().size(), 4);
	EXPECT_EQ(tr6->get_path().at(0), 1);
	EXPECT_EQ(tr6->get_path().at(1), 4);
	EXPECT_EQ(tr6->get_path().at(2), 6);
	EXPECT_EQ(tr6->get_path().at(3), 8);

	const std::shared_ptr<Traverser> tr7 = std::make_shared<FindPathToEdge>(6, 9);
	BFS s7(g, tr7);
	s7.execute(1);
	ASSERT_EQ(tr7->get_path().size(), 4);
	EXPECT_EQ(tr7->get_path().at(0), 1);
	EXPECT_EQ(tr7->get_path().at(1), 4);
	EXPECT_EQ(tr7->get_path().at(2), 6);
	EXPECT_EQ(tr7->get_path().at(3), 9);
}

TEST(BFS, CyclicGraph) {
	Graph g;
	g.add_edge(1, 2);
	g.add_edge(1, 3);
	g.add_edge(1, 4);
	g.add_edge(4, 5);
	g.add_edge(4, 6);
	g.add_edge(4, 7);
	g.add_edge(6, 8);
	g.add_edge(6, 9);

	const std::shared_ptr<Traverser> tr = std::make_shared<CheckCycle>();
	BFS s(g, tr);
	s.execute(1);
	EXPECT_FALSE(tr->is_cyclic());

	g.add_edge(4, 8);
	s.execute(1);
	EXPECT_TRUE(tr->is_cyclic());

	
	Graph g1;
	g1.add_edge(1, 2);
	g1.add_edge(1, 3);
	g1.add_edge(1, 4);
	g1.add_edge(4, 5);
	g1.add_edge(4, 6);
	g1.add_edge(4, 7);
	g1.add_edge(6, 8);
	g1.add_edge(6, 9);
	g1.add_edge(1, 9);
	
	BFS s1(g1, tr);
	s1.execute(1);
	EXPECT_TRUE(tr->is_cyclic());
}