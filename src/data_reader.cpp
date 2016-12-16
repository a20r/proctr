
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <cmath>
#include <nanoflann.hpp>
#include <boost/progress.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include "proctr/prior.hpp"
#include "proctr/data_reader.hpp"

using namespace boost::posix_time;
using namespace boost::filesystem;
using namespace std;
using namespace nanoflann;

const string data_fname = "/home/wallar/fast_data/nyc_taxi_data.csv";
const string stations_fname = "data/stations.csv";
const string ts_dir = "data/ts/";
const int num_rows = 165114362;

bool parse_data_line(string line, string &p_dt, string &d_dt,
        double &p_lng, double &p_lat,
        double &d_lng, double &d_lat)
{
    stringstream line_stream(line);
    string cell;
    int counter = 0;
    while (getline(line_stream, cell, ','))
    {
        try {
            switch (counter++)
            {
                case 1: p_dt = cell;
                case 2: d_dt = cell;
                case 5: p_lng = stod(cell);
                case 6: p_lat = stod(cell);
                case 9: d_lng = stod(cell);
                case 10: d_lat = stod(cell);
            }
        }
        catch (const invalid_argument &ia)
        {
            return false;
        }
    }

    return !(p_lng == 0 or p_lat == 0 or d_lng == 0 or d_lat == 0);
}

bool parse_stations_line(string line, double &lng, double &lat)
{
    stringstream line_stream(line);
    string cell;
    int counter = 0;
    while (getline(line_stream, cell, ','))
    {
        switch (counter++)
        {
            case 1: lng = stod(cell);
            case 2: lat = stod(cell);
        }
    }

    return true;
}

GeoPoints load_stations()
{
    GeoPoints gps;
    ifstream file(stations_fname);

    string line;
    for (int i = 0; getline(file, line); i++)
    {
        if (i > 0)
        {
            double lng, lat;
            parse_stations_line(line, lng, lat);
            gps.pts.push_back(GeoPoint(lng, lat));
        }
    }
    return gps;
}

size_t get_nearest(kd_tree_t &index, double lng, double lat)
{
    size_t ret_index;
    double out_dist_sqr;
    KNNResultSet<double> resultSet(1);
    resultSet.init(&ret_index, &out_dist_sqr);
    double query_pt[2] = {lng, lat};
    index.findNeighbors(resultSet, &query_pt[0], SearchParams(10));
    return ret_index;
}

string get_ts_dir(size_t p_st)
{
    ostringstream os;
    os << ts_dir << "pick-" << p_st;
    return os.str();
}

string get_ts_fname(size_t p_st, size_t d_st)
{
    ostringstream os;
    os << get_ts_dir(p_st) << "/drop-" << d_st << ".txt";
    return os.str();
}

void write_dt(size_t p_st, size_t d_st, string p_dt)
{
    path ts_dir(get_ts_dir(p_st));
    create_directories(ts_dir);

    ofstream fout;
    fout.open(get_ts_fname(p_st, d_st), ios_base::app);
    #pragma omp critical
    fout << p_dt << endl;
    fout.close();
}

void create_ts_files(kd_tree_t &index)
{
    ifstream file(data_fname);
    string l;
    getline(file, l);
    boost::progress_display show_progress(num_rows);

    #pragma omp parallel for
    for (int i = 2; i < num_rows; i++)
    {
        string line;
        #pragma omp critical
        getline(file, line);

        string p_dt, d_dt;
        double p_lng, p_lat, d_lng, d_lat;
        if (parse_data_line(line, p_dt, d_dt, p_lng, p_lat, d_lng, d_lat))
        {
            size_t p_st = get_nearest(index, p_lng, p_lat);
            size_t d_st = get_nearest(index, d_lng, d_lat);
            ptime pt = time_from_string(p_dt);
            ptime dt = time_from_string(d_dt);
            write_dt(p_st, d_st, p_dt);
        }
        ++show_progress;
    }
}

vector<ptime> parse_ts_file(int p_st, int d_st)
{
    vector<ptime> dts;
    string fname = get_ts_fname(p_st, d_st);
    ifstream file(fname);
    string line;

    while (getline(file, line))
    {
        ptime pt = time_from_string(line);
        dts.push_back(pt);
    }

    return dts;
}

void create_prior(vector<ptime>& times, Prior& prior)
{
    time_duration window_size = minutes(30);
    int start_index = 0

    for (int i = 1; i < times.size(); i++)
    {
        ptime cur_t = times[i];
        while (cur_t - times[start_index] >= window_size && start_index < i)
        {
            start_index++;
        }

        time_duration elapsed_time = cur_t - times[start_index];
        double rate = (i - start_time_index) / elapsed_time.total_seconds();
        prior.add_data(cur_t, rate);
    }
}

Prior **create_priors(int n_stations)
{
    Prior **priors = new int *[n_stations];
    for (int i = 0; i < n_stations; i++)
    {
        priors[i] = new Prior[n_stations];
    }

    for (int i = 0; i < n_stations; i++)
    {
        for (int j = 0; j < n_stations; j++)
        {
            vector<ptime> times = parse_ts_file(i, j);
            create_prior(times, priors[i][j]);
        }
    }

    return priors;
}
