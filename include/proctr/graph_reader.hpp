#ifndef GRAPH_READER_HPP
#define GRAPH_READER_HPP

#include <string>
#include <boost/graph/edge_list.hpp>
#include "proctr/data_reader.hpp"
#include "proctr/graph.hpp"

using namespace std;

typedef Edge<GeoPoint> GeoEdge;

WeightedGraph<GeoPoint, GeoPointHash> read_graph(string points_fname,
        string edges_fname,
        string durs_fname);


#endif
