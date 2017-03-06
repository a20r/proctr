
#include <vector>
#include <unordered_map>
#include <eigen3/Eigen/Dense>
#include "gurobi_c++.h"

using namespace std;
using namespace Eigen;

class RebalancingModel : public GRBModel
{
    public:
        RebalancingModel(GRBEnv *env, GRBVar **vs, GRBVar *times,
                int Nv, int Nr);

        ~RebalancingModel();

        bool solve(vector<int>& assignments, unordered_map<int, double>& durs);

    private:
        GRBEnv *env;
        GRBVar **vs;
        GRBVar *times;
        MatrixXi allowed_allocs;
        int Nv, Nr;
};

RebalancingModel create_model(
        GRBEnv *env,
        const MatrixXd& costs,
        const VectorXd& rates,
        const VectorXi& caps,
        const VectorXi& enroute_caps,
        double max_region_time,
        int Nv, int Nr);
