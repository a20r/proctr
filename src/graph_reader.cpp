
#include <sstream>
#include <fstream>
#include "proctr/graph.hpp"
#include "proctr/graph_reader.hpp"

void read_edge_line(string edge_line, int& from, int& to)
{
    stringstream line_stream(edge_line);
    string cell;

    int counter = 0;
    while (getline(line_stream, cell, ','))
    {
        switch (counter++)
        {
            case 1: from = stod(cell) - 1;
            case 2: to = stod(cell) - 1;
        }
    }
}

double read_durs_line(string durs_line)
{
    stringstream line_stream(durs_line);
    string cell;
    double dur_sum = 0;
    getline(line_stream, cell, ',');

    while (getline(line_stream, cell, ','))
    {
        dur_sum += stod(cell);
    }

    return dur_sum / 24;
}

vector<GeoPoint> read_points(string points_fname)
{
    ifstream points_file(points_fname);
    vector<GeoPoint> nodes;
    string line;

    while(getline(points_file, line))
    {
        nodes.push_back(GeoPoint(line));
    }

    return nodes;
}

vector<GeoEdge> read_edges_file(string edges_fname, vector<GeoPoint>& nodes)
{
    ifstream edges_file(edges_fname);
    string line;
    int from, to;
    vector<GeoEdge> edges;

    while(getline(edges_file, line))
    {
        read_edge_line(line, from, to);
        edges.push_back(GeoEdge(nodes[from], nodes[to]));
    }

    return edges;
}

vector<double> read_durs_file(string durs_fname)
{
    ifstream points_file(durs_fname);
    vector<double> durs;
    string line;

    while(getline(points_file, line))
    {
        durs.push_back(read_durs_line(line));
    }

    return durs;
}

WeightedGraph<GeoPoint> read_graph(string points_fname, string edges_fname,
        string durs_fname, kd_tree_t **index)
{
    vector<GeoPoint> nodes = read_points(points_fname);
    vector<GeoEdge> edges = read_edges_file(edges_fname, nodes);
    vector<double> durs = read_durs_file(durs_fname);
    GeoPoints gps;
    gps.pts = nodes;
    *index = create_kd_tree(gps);
    return WeightedGraph<GeoPoint>(nodes, edges, durs);
}
