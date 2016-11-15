
#ifndef RATE_FILTER_HPP
#define RATE_FILTER_HPP

#include <eigen3/Eigen/Dense>

using namespace std;
using namespace Eigen;
using namespace boost::posix_time;

class RateFilter
{
    private:
        ArrayXd rates;
        ArrayXd probs;
        double vol;
        double resample_thresh;
        int n_rates;
        Prior *prior;

    public:
        RateFilter();
        RateFilter(double max_rate, int n_rates, double vol,
                double resample_thresh, Prior *prior);
        ~RateFilter();
        void observe(int n_obs, double secs, tm time);
        double get_probability(int i);
        double get_rate(int i);
        void normalize();
        double get_predicted_rate();
        void resample();
        void evolve(double secs);
};

#endif
