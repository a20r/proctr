#ifndef GRAPH_READER_HPP
#define GRAPH_READER_HPP

#include <string>
#include <boost/graph/edge_list.hpp>
#include "proctr/graph.hpp"

using namespace std;

class LatLngNode
{
    public:
        LatLngNode();
        LatLngNode(double lat, double lng);
        LatLngNode(string csv_line);

    private:
        double lat, lng;
};

typedef Edge<LatLngNode> GeoEdge;

WeightedGraph<LatLngNode> read_graph(string points_fname, string edges_fname,
        string durs_fname);

#endif
