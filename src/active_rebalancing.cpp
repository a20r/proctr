
#include <vector>
#include "gurobi_c++.h"
#include "proctr/active_rebalancing.hpp"

using namespace std;


ActiveRebalancingModel::ActiveRebalancingModel(
        GRBEnv *env, GRBVar *xs, vec2<int>& v_to_ps) :
    GRBModel(*env),
    xs(xs),
    v_to_ps(v_to_ps)
{
}

ActiveRebalancingModel::~ActiveRebalancingModel()
{
    delete[] xs;
}

void ActiveRebalancingModel::solve(vector<int>& assignments)
{
    assignments = vector<int>(v_to_ps.size());
    this->optimize();
    for (int v = 0; v < v_to_ps.size(); v++)
    {
        for (int p = 0; p < v_to_ps[v].size(); p++)
        {
            if (xs[v_to_ps[v][p]].get(GRB_DoubleAttr_X) > 0.001)
            {
                assignments[v] = v_to_ps[v][p];
            }
        }
    }
}

ActiveRebalancingModel create_model(
        GRBEnv *env,
        vec2<double>& times,
        vec2<double>& rates,
        vec2<int>& max_vecs,
        vec3<int>& paths_in_region,
        vec2<int>& vehicle_to_paths,
        int num_paths)
{
    /*
     * paths_in_region[i][j] is a vector of indices for paths
     * that move through region i in time interval j
     *
     * vehicle_to_paths[i] is a vector of indices of paths
     * that belong to vehicle i
     */

    int Nr = rates.size();
    int Nv = vehicle_to_paths.size();

    GRBVar *xs = new GRBVar[num_paths];
    vec2<GRBVar> amounts(Nr, vector<GRBVar>());

    ActiveRebalancingModel model(env, xs, vehicle_to_paths);

    for (int p = 0; p < num_paths; p++)
    {
        xs[p] = model.addVar(0, 1, 0, GRB_BINARY);
    }

    for (int r = 0; r < Nr; r++)
    {
        for (int i = 0; i < max_vecs[r].size(); i++)
        {
            GRBVar a_var = model.addVar(0, max_vecs[r][i], 0, GRB_INTEGER);
            amounts[r].push_back(a_var);
        }
    }

    model.update();

    for (int r = 0; r < Nr; r++)
    {
        for (int i = 0; i < paths_in_region[r].size(); i++)
        {
            GRBLinExpr total_paths_in_region = 0;
            for (int p = 0; p < paths_in_region[r][i].size(); i++)
            {
                total_paths_in_region += xs[p];
            }
            model.addConstr(total_paths_in_region == amounts[r][i]);
        }
    }

    for (int v = 0; v < Nr; v++)
    {
        GRBLinExpr paths_assigned_to_vec = 0;
        for (int p = 0; p < vehicle_to_paths[v].size(); p++)
        {
            paths_assigned_to_vec += xs[vehicle_to_paths[v][p]];
        }
        model.addConstr(paths_assigned_to_vec == 1);
    }

    model.update();

    GRBLinExpr obj = 0;
    for (int r = 0; r < Nr; r++)
    {
        for (int i = 0; i < times[r].size(); i++)
        {
            double dur = times[r][i + 1] - times[r][i];
            obj += dur * rates[r][i] * amounts[r][i];
        }
    }
    model.setObjective(obj);

    return model;
}
