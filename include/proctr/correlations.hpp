
#ifndef CORRELATIONS_HPP
#define CORRELATIONS_HPP

class Correlations
{
    public:
        Correlations();
        Correlations(double *cors, int num_ts);
        ~Correlations();
        get_correlation(Route r0, Route r1);

    private:
        double *cors;
        int num_ts;
}

#endif
