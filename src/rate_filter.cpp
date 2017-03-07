
#include <iostream>
#include <cmath>
#include <boost/math/distributions/poisson.hpp>
#include <boost/math/distributions/normal.hpp>
#include <vector>
#include <eigen3/Eigen/Dense>
#include "proctr/prior.hpp"
#include "proctr/rate_filter.hpp"

using namespace boost::math;

RateFilter::RateFilter()
{
}

RateFilter::RateFilter(int n_rates, double resample_thresh, Prior *prior) :
    n_rates(n_rates),
    vol(prior->get_volatility()),
    resample_thresh(resample_thresh),
    // probs(ArrayXd::Constant(n_rates, 1.0 / n_rates)),
    // rates(ArrayXd::LinSpaced(n_rates, prior->get_max_rate() / n_rates,
    //             prior->get_max_rate())),
    prior(prior)
{
    probs = VectorXd::Constant(n_rates, 1.0 / n_rates);
    rates = VectorXd::LinSpaced(n_rates, prior->get_max_rate() / n_rates,
                prior->get_max_rate());
}

RateFilter::~RateFilter()
{
    // delete prior;
}

void RateFilter::observe(int n_obs, ptime time, int secs)
{
    resample();

    for (int i = 0; i < n_rates; i++)
    {
        double rate = rates[i];
        double prior_prob = prior->pdf(time, rate) + 1e-8;
        // double prior_prob = 1.0;
        double update_prob = pdf(poisson(rate * secs), n_obs) + 1e-8;
        probs[i] *= update_prob * prior_prob;
    }

    normalize();
}


void RateFilter::normalize()
{
    double p_sum = probs.sum();
    if (p_sum > 0)
    {
        probs /= p_sum;
    }
}

void RateFilter::evolve(double secs)
{
    double std = vol * sqrt(secs);
    normal brownian(0, std);

    VectorXd new_probs = VectorXd::Constant(n_rates, 0);

    for (int i = 0; i < n_rates; i++)
    {
        double old_prob = probs[i];

        for (int j = 0; j < n_rates; j++)
        {
            bool gt = rates[j] >= rates[i] - 2 * std;
            bool lt = rates[j] <= rates[i] + 2 * std;
            if (gt and lt)
            {
                double bottom = rates[j] - rates[0] / 2;
                double top = rates[j] + rates[0] / 2;
                double bottom_cdf = cdf(brownian, bottom - rates[i]);
                double top_cdf = cdf(brownian, top - rates[i]);
                double cont = old_prob * (top_cdf - bottom_cdf);
                new_probs[j] += cont;
            }
        }
    }

    probs = new_probs;
    normalize();
}

void RateFilter::resample()
{
    double square_sum = 0.0;

    for (int i = 0; i < n_rates; i++)
    {
        square_sum += probs[i] * probs[i];
    }

    if (1.0 / square_sum < resample_thresh)
    {
        probs = VectorXd::Constant(n_rates, 1.0 / n_rates);
    }
}

double RateFilter::get_probability(int i)
{
    return probs[i];
}

double RateFilter::get_rate(int i)
{
    return rates[i];
}

double RateFilter::get_predicted_rate()
{
    double pred_rate = 0.0;
    for (int i = 0; i < n_rates; i++)
    {
        pred_rate += rates[i] * probs[i];
    }
    return pred_rate;
}

RateFilter *create_rate_filters(int n_rates, double resample_thresh,
        int n_filters, Prior *priors)
{
    RateFilter *rfs = new RateFilter[n_filters];
    for (int i = 0; i < n_filters; i++)
    {
        rfs[i] = RateFilter(n_rates, resample_thresh, &priors[i]);
    }

    return rfs;
}
