#ifndef PROCTR_GRAPH_HPP
#define PROCTR_GRAPH_HPP

#include <list>
#include <iterator>
#include <boost/graph/edge_list.hpp>

using namespace std;

template<typename T>
using Edge = pair<T, T>;

template<typename T>
using EdgeVec = vector<Edge<T>>;

template<typename T>
class WeightedGraph : public boost::edge_list<typename EdgeVec<T>::iterator>
{
    public:
        WeightedGraph()
        {
        }

        WeightedGraph(vector<T> nodes, vector<Edge<T>> edges,
                vector<double> weights) :
            nodes(nodes), edges(edges), weights(weights),
            boost::edge_list<typename EdgeVec<T>::iterator>(
                    edges.begin(), edges.end())
        {
        }

        T get_node(int i)
        {
            return nodes[i];
        }

        vector<T> get_nodes()
        {
            return nodes;
        }

        Edge<T> get_edge(int i)
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

    private:
        vector<T> nodes;
        vector<double> weights;
        vector<pair<T, T>> edges;
};

#endif
