
using JuMP;

m = Model()
Nv = 70
Nr = 100
max_region_time = 2000
caps = rand(1:10, Nv)
rates = 0.01 * rand(Nr)
costs = 10000 * rand(Nv, Nr)

@variables m begin
    X[1:Nv, 1:Nr], Bin
    0 <= T[1:Nr] <= max_region_time
    mini
end

for r = 1:Nr
    @constraint(m, sum{caps[v] * X[v, r], v = 1:Nv} <= rates[r] * T[r])
end

for v = 1:Nv
    @constraint(m, sum{X[v, r], r = 1:Nr} == 1)
end

for v = 1:Nv
    for r = 1:Nr
        @constraint(m, X[v, r] * (T[r] + costs[v, r]) <= mini)
    end
end

@objective(m, Min, mini)

status = solve(m)

for v = 1:Nv
    for r = 1:Nr
        if getvalue(X[v, r]) == 1
            println("$(v) --> $(r) for $(getvalue(T[r])) secs")
        end
    end
end
