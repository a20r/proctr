
#ifndef ROUTE_HPP
#define ROUTE_HPP

class Route
{
    public:
        Route();
        Route(int source, int sink);
        ~Route();
        int get_source() const;
        int get_sink() const;
        bool operator<(const Route &rhs);
        void next(int num_ts);

    private:
        int source, sink;
};

#endif
