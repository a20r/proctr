#ifndef PROCTR_GRAPH_HPP
#define PROCTR_GRAPH_HPP

#include <list>
#include <iterator>
#include <unordered_map>
#include <set>
#include "proctr/data_reader.hpp"

using namespace std;

template<typename T>
using Edge = pair<T, T>;

template<typename T>
using EdgeVec = vector<pair<T, T>>;

struct edge_t
{
    int to;
    double length;
};

inline double dijkstra(const vector<vector<edge_t>> &graph, int source, int target) {
    vector<double> min_distance( graph.size(), INT_MAX);
    min_distance[ source ] = 0;
    set< pair<int,int> > active_vertices;
    active_vertices.insert( {0,source} );

    while (!active_vertices.empty()) {
        int where = active_vertices.begin()->second;
        if (where == target)
        {
            return min_distance[where];
        }
        active_vertices.erase( active_vertices.begin() );
        for (auto ed : graph[where])
            if (min_distance[ed.to] > min_distance[where] + ed.length) {
                active_vertices.erase( { min_distance[ed.to], ed.to } );
                min_distance[ed.to] = min_distance[where] + ed.length;
                active_vertices.insert( { min_distance[ed.to], ed.to } );
            }
    }
    return INT_MAX;
};

template<typename T, typename T_HASH>
class WeightedGraph
{
    public:
        WeightedGraph()
        {
        }

        WeightedGraph(vector<T> nodes, EdgeVec<T> edges,
                vector<double> weights) :
            nodes(nodes), edges(edges), weights(weights),
            weight_map(vector<vector<double>>(nodes.size(),
                        vector<double>(nodes.size()))),
            adj(vector<vector<edge_t>>(nodes.size(), vector<edge_t>()))
        {

            for (int i = 0; i < nodes.size(); i++)
            {
                node_map[nodes[i]] = i;
            }

            for (int i = 0; i < edges.size(); i++)
            {
                Edge<T> edge = edges[i];
                int src = node_map[get<0>(edge)];
                int sink = node_map[get<1>(edge)];
                weight_map[src][sink] = weights[i];
                edge_t w_edge = {sink, weights[i]};
                adj[src].push_back(w_edge);
            }
        }

        T get_node(int i)
        {
            return nodes[i];
        }

        vector<T> get_nodes()
        {
            return nodes;
        }

        pair<T, T> get_edge(int i)
        {
            return edges[i];
        }

        EdgeVec<T> get_edges()
        {
            return edges;
        }

        double get_weight(int i)
        {
            return weights[i];
        }

        vector<double> get_weights()
        {
            return weights;
        }

        double shortest_dist(T source, T sink)
        {
            int src_id = node_map[source];
            int sink_id = node_map[sink];
            return dijkstra(adj, src_id, sink_id);
        }

        double shortest_dist(int source, int sink)
        {
            return dijkstra(adj, source, sink);
        }

    private:
        vector<T> nodes;
        vector<double> weights;
        vector<Edge<T>> edges;
        vector<vector<double>> weight_map;
        vector<vector<edge_t>> adj;
        unordered_map<T, int, T_HASH> node_map;
};

#endif
