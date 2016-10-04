
#ifndef CORRELATIONS_HPP
#define CORRELATIONS_HPP

using namespace std;

class Correlations
{
    public:
        Correlations();
        Correlations(double *cors, int num_ts);
        ~Correlations();
        double get_correlation(Route r0, Route r1);
        size_t get_index(Route r0, Route r1);

    private:
        double *cors;
        int num_ts;
};

#endif
