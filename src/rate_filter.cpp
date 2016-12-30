
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

RateFilter::RateFilter(double max_rate, int n_rates,
        double vol, double resample_thresh, Prior prior,
        ptime starting_time) :
    n_rates(n_rates),
    vol(vol),
    resample_thresh(resample_thresh),
    probs(ArrayXd::Constant(n_rates, 1.0 / n_rates)),
    rates(ArrayXd::LinSpaced(n_rates, max_rate / n_rates, max_rate)),
    prior(prior),
    last_time(starting_time)
{
}

RateFilter::~RateFilter()
{
    // delete prior;
}

void RateFilter::observe(int n_obs, ptime time)
{
    resample();

    double secs = (time - last_time).total_seconds();
    for (int i = 0; i < n_rates; i++)
    {
        double prior_prob = prior.pdf(time, rates[i]);
        probs[i] *= pdf(poisson(rates[i] * secs), n_obs) * prior_prob;
    }

    normalize();
}

void RateFilter::normalize()
{
    probs /= probs.sum();
}

void RateFilter::evolve(double secs)
{
    double std = vol * sqrt(secs);
    normal brownian(0, std);

    ArrayXd new_probs = ArrayXd::Constant(n_rates, 0);

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
    double n_eff = 1.0 / probs.square().sum();
    if (n_eff < resample_thresh)
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
