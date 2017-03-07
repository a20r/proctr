
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
        VectorXd rates;
        VectorXd probs;
        double vol;
        double resample_thresh;
        int n_rates;
        Prior *prior;

    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        RateFilter();
        RateFilter(int n_rates, double resample_thresh, Prior *prior);
        ~RateFilter();
        void observe(int n_obs, ptime time, int secs);
        double get_probability(int i);
        double get_rate(int i);
        void normalize();
        double get_predicted_rate();
        void resample();
        void evolve(double secs);
};

RateFilter *create_rate_filters(int n_rates, double resample_thresh,
        int n_filters, Prior *priors);

#endif
