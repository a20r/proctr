
#ifndef RATE_FILTER_HPP
#define RATE_FILTER_HPP

#include <eigen3/Eigen/Dense>
#include <boost/date_time/posix_time/posix_time.hpp>

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
        ptime last_time;

    public:
        RateFilter();
        RateFilter(double max_rate, int n_rates, double vol,
                double resample_thresh, Prior *prior,
                ptime starting_time);
        ~RateFilter();
        void observe(int n_obs, ptime time);
        double get_probability(int i);
        double get_rate(int i);
        void normalize();
        double get_predicted_rate();
        void resample();
        void evolve(double secs);
};

#endif
