
#include <vector>
#include "gurobi_c++.h"

using namespace std;

template<typename T>
using vec2 = vector<vector<T>>;

template<typename T>
using vec3 = vector<vector<vector<T>>>;

class ActiveRebalancingModel : public GRBModel
{
    public:
        ActiveRebalancingModel(GRBEnv *env, GRBVar *xs,
                vec2<int>& vehicle_to_paths);
        ~ActiveRebalancingModel();
        void solve(vector<int>& assignments);

    private:
        GRBVar *xs;
        vec2<int>& v_to_ps;
};

ActiveRebalancingModel create_model(
        GRBEnv *env,
        vec2<double>& times,
        vec2<double>& rates,
        vec2<int>& max_vecs,
        vec3<int>& paths_in_region,
        vec2<int>& vehicle_to_paths,
        int num_paths);
