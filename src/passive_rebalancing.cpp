
#include <vector>
#include <unordered_map>
#include <eigen3/Eigen/Dense>
#include "gurobi_c++.h"
#include "proctr/passive_rebalancing.hpp"

using namespace std;
using namespace Eigen;

RebalancingModel::RebalancingModel(GRBEnv *env, GRBVar **vs, GRBVar *times,
        int Nv, int Nr) :
    GRBModel(*env), env(env), vs(vs), times(times),
    Nv(Nv), Nr(Nr)
{
}

RebalancingModel::~RebalancingModel()
{
    for (int i = 0; i < Nv; i++)
    {
        delete[] vs[i];
    }

    delete[] times;
}

bool RebalancingModel::solve(vector<int>& assignments,
        unordered_map<int, double>& durs)
{
    assignments = vector<int>(Nv);
    this->optimize();

    for (int v = 0; v < Nv; v++)
    {
        for (int r = 0; r < Nr; r++)
        {
            if (vs[v][r].get(GRB_DoubleAttr_X) > 0.001)
            {
                assignments[v] = r;
                durs[r] = times[r].get(GRB_DoubleAttr_X);
            }
        }
    }

    return true;
}


RebalancingModel create_model(
        GRBEnv *env,
        const MatrixXd& costs,
        const VectorXd& rates,
        const VectorXi& caps,
        const VectorXi& enroute_caps,
        double max_region_time,
        int Nv, int Nr)
{
    GRBVar **vs = new GRBVar *[Nv];
    for (int i = 0; i < Nv; i++)
    {
        vs[i] = new GRBVar[Nr];
    }

    GRBVar *times = new GRBVar[Nr];

    RebalancingModel model(env, vs, times, Nv, Nr);

    GRBVar minivar = model.addVar(0, costs.maxCoeff() + max_region_time, 1,
            GRB_CONTINUOUS);

    for (int v = 0; v < Nv; v++)
    {
        for (int r = 0; r < Nr; r++)
        {
            vs[v][r] = model.addVar(0, 1, 0, GRB_BINARY);
        }
    }

    for (int r = 0; r < Nr; r++)
    {
        times[r] = model.addVar(0, max_region_time, 0, GRB_CONTINUOUS);
    }

    model.update();

    for (int r = 0; r < Nr; r++)
    {
        GRBLinExpr total_demand = 0;
        for (int v = 0; v < Nv; v++)
        {
            total_demand += caps(v) * vs[v][r];
        }
        model.addConstr(total_demand + enroute_caps(r) <= rates(r) * times[r]);
    }

    for (int v = 0; v < Nv; v++)
    {
        GRBLinExpr total_times_assigned = 0;
        for (int r = 0; r < Nr; r++)
        {
            total_times_assigned += vs[v][r];
            GRBQuadExpr lhs = vs[v][r] * (times[r] + costs(v, r));
            model.addQConstr(lhs <= minivar);
        }
        model.addConstr(total_times_assigned == 1);
    }

    model.update();

    return model;
}
