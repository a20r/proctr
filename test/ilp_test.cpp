
#include <random>
#include "gurobi_c++.h"
#include "gtest/gtest.h"
#include "proctr/ilp.hpp"

using namespace std;

MatrixXi random_binary_matrix(int Nv, int Nr)
{
    MatrixXi mat = MatrixXi::Zero(Nv, Nr);
    MatrixXd rvs = MatrixXd::Random(Nv, Nr);

    for (int v = 0; v < Nv; v++)
    {
        for (int r = 0; r < Nr; r++)
        {
            if (rvs(v, r) > 0)
            {
                mat(v, r) = 1;
            }
        }
    }

    return mat;
}

TEST(ILPTest, InitialTest)
{
    GRBEnv *env = new GRBEnv;
    int Nv = 200, Nr = 200;
    int max_region_time = 1000;
    MatrixXi allowed_allocs = random_binary_matrix(Nv, Nr);
    // not positive dummy
    MatrixXd costs = 1000 * (MatrixXd::Random(Nv, Nr) +
            MatrixXd::Constant(Nv, Nr, 1));
    VectorXd rates = VectorXd::Random(Nr);
    VectorXi caps = VectorXi::Constant(Nr, 10);
    try
    {
    GRBModel model = create_model(
            env,
            allowed_allocs,
            costs, rates, caps,
            max_region_time,
            Nv, Nr);
    model.optimize();
    }
    catch (GRBException e)
    {
        cout << "Error: " << e.getErrorCode() << endl;
    }
}
