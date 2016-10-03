
#ifndef ROUTE_HPP
#define ROUTE_HPP

class Route
{
    public:
        Route();
        Route(int source, int sink);
        ~Route();
        int get_source();
        int get_sink();

    private:
        int source, sink;
};

#endif
