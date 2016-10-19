/* Copyright 2016, Gurobi Optimization, Inc. */

/* This example formulates and solves the following simple MIP model:

     maximize    x +   y + 2 z
     subject to  x + 2 y + 3 z <= 4
                 x +   y       >= 1
     x, y, z binary
*/

#include <vector>
#include <eigen3/Eigen/Dense>
#include "gurobi_c++.h"

using namespace std;
using namespace Eigen;

GRBModel create_model(
        GRBEnv *env,
        const MatrixXi& allowed_allocs,
        const MatrixXd& costs,
        const VectorXd& rates,
        const VectorXi& caps,
        double max_region_time,
        int Nv, int Nr)
{
    GRBModel model(*env);

    GRBVar vs[Nv][Nr];
    GRBVar times[Nr];

    GRBVar minivar = model.addVar(0, costs.maxCoeff() + max_region_time, 1,
            GRB_CONTINUOUS);

    for (int v = 0; v < Nv; v++)
    {
        for (int r = 0; r < Nr; r++)
        {
            if (allowed_allocs(v, r))
            {
                vs[v][r] = model.addVar(0, 1, 0, GRB_BINARY);
            }
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
            if (allowed_allocs(v, r))
            {
                total_demand += caps(v) * vs[v][r];
            }
        }
        model.addConstr(total_demand <= rates(r) * times[r]);
    }

    for (int v = 0; v < Nv; v++)
    {
        GRBLinExpr total_times_assigned = 0;
        for (int r = 0; r < Nr; r++)
        {
            if (allowed_allocs(v, r))
            {
                total_times_assigned += vs[v][r];
            }
        }
        model.addConstr(total_times_assigned == 1);
    }

    for (int v = 0; v < Nv; v++)
    {
        for (int r = 0; r < Nr; r++)
        {
            if (allowed_allocs(v, r))
            {
                model.addQConstr(
                        vs[v][r] * (times[r] + costs(v, r)) <= minivar);
            }
        }
    }

    model.update();

    return model;
}
