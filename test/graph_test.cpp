
#include "gtest/gtest.h"
#include "proctr/graph.hpp"
#include "proctr/data_reader.hpp"
#include "proctr/graph_reader.hpp"

using namespace std;

TEST(GraphTest, ReaderTest)
{
    string points_fname = "data/nyc-graph/points.csv";
    string edges_fname = "data/nyc-graph/edges.csv";
    string durs_fname = "data/nyc-graph/week.csv";
    WeightedGraph<GeoPoint, GeoPointHash> graph = read_graph(
            points_fname, edges_fname, durs_fname);
    ASSERT_TRUE(graph.get_nodes().size() == 4091);
    ASSERT_TRUE(graph.get_edges().size() == 9452);
    ASSERT_TRUE(graph.get_weights().size() == 9452);
    cout << graph.shortest_dist(graph.get_node(0), graph.get_node(100)) << endl;
}
