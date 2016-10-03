
#ifndef ROUTE_HPP
#define ROUTE_HPP

template <typename T>
class Route
{
    public:
        Route()
        {
        }

        ~Route()
        {
        }

        Route(T source, T sink) : source(source), sink(sink)
        {
        }

        T get_source()
        {
            return source;
        }

        T get_sink()
        {
            return sink;
        }

    private:
        T source, sink;
};

#endif
