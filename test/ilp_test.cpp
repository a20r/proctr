
#include <random>
#include <cmath>
#include <unordered_map>
#include "gurobi_c++.h"
#include "gtest/gtest.h"
#include "proctr/passive_rebalancing.hpp"

using namespace std;

MatrixXd random_real_matrix(int Nv, int Nr, double min_val, double max_val)
{
    default_random_engine gen;
    uniform_real_distribution<double> dist(min_val, max_val);
    MatrixXd mat = MatrixXd::Zero(Nv, Nr);

    for (int v = 0; v < Nv; v++)
    {
        for (int r = 0; r < Nr; r++)
        {
            mat(v, r) = dist(gen);
        }
    }

    return mat;
}

MatrixXi random_int_matrix(int Nv, int Nr, int min_val, int max_val)
{
    default_random_engine gen;
    uniform_int_distribution<int> dist(min_val, max_val);
    MatrixXi mat = MatrixXi::Zero(Nv, Nr);

    for (int v = 0; v < Nv; v++)
    {
        for (int r = 0; r < Nr; r++)
        {
            mat(v, r) = dist(gen);
        }
    }

    return mat;
}

TEST(ILPTest, InitialTest)
{
    GRBEnv *env = new GRBEnv;
    int Nv = 40, Nr = 100;
    double max_region_time = 2000;
    MatrixXd costs = random_real_matrix(Nv, Nr, 0, 1000);
    VectorXd rates = random_real_matrix(Nr, 1, 0, 0.01);
    VectorXi caps = random_int_matrix(Nv, 1, 1, 10);

    try
    {
        vector<int> assignments;
        unordered_map<int, double> durs;
        RebalancingModel model = create_model(
                env,
                costs, rates, caps,
                max_region_time,
                Nv, Nr);
        model.solve(assignments, durs);
        for (int v = 0; v < Nv; v++)
        {
            cout << v << " --> " << assignments[v];
            cout << " for " << durs[assignments[v]] << " secs" << endl;
        }
    }
    catch (GRBException e)
    {
        cout << "Error: " << e.getErrorCode() << endl;
    }
}
