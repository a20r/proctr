
using JuMP
using LightGraphs
using DataStructures

Nv = 70
Nr = 100
max_region_time = 2000
caps = rand(1:10, Nv)
rates = 0.01 * rand(Nr)
costs = 10000 * rand(Nv, Nr)

N = 10
g = Grid([N, N])
state = floyd_warshall_shortest_paths(g)
paths = []

function constrained_shortest_paths(g, dists, src, dst)
    m = Model()
    for edge in edges(g)
        @variable(m, X[edge[1], edge[2]], Bin)
    end

    @constraints m begin
        sum{X[src, j], j = neighbors(g, src)} >= 1
    end

    for i = 1:N
        for j in neighbors(g, i)
        end
    end
end

constrained_shortest_paths(g, state.dists, 1, 8)
