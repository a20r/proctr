
#ifndef PREDICTOR_HPP
#define PREDICTOR_HPP

#include <unordered_map>
#include "proctr/route.hpp"
#include "proctr/route_table.hpp"
#include "proctr/route_hash.hpp"

using namespace std;

template<size_t num_ts>
class Predictor
{
    template <typename T>
    using route_map = unordered_map<Route, T, RouteHash<num_ts>>;

    public:
        Predictor()
        {
        }

        Predictor(Correlations cors, Volumes vols, Stds stds)
        {
        }

        ~Predictor()
        {
        }

        /*
        * This function tracks the change in route demand over time therefore
        * calls to this function need to be ordered in time.
        *
        * This function maintains state. Beware!
        */
        void predict(route_map<int> &demands, route_map<int> &preds)
        {
            route_map<int> diffs;
            for (auto it : demands)
            {
                diffs[it->first] = it->second - last[it->first];
            }

            Route max_route(num_ts - 1, num_ts - 1);
            for (auto it : diffs)
            {
                Route r = it->first;
                int diff = it->second;
                double vol_perc = (double) diff / (double) vols.get(r);

                for (Route s(0, 0); s <= max_route; s.next(num_ts))
                {
                    double cor = cors.get(r, s);
                    double s_pred_vols = vols.get(s) * vol_perc * cor;
                }
            }

            last = demands;
        }

    private:
        Correlations cors;
        Volumes vols;
        Stds stds;
        route_map<int> last;
};

#endif
