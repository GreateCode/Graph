#ifndef GRAPH_VERTEX_ID_H
#define GRAPH_VERTEX_ID_H

#include <cstddef>    //for std::size_t
#include "Graph/include/stock_id.h"

namespace lzhlib
{

    class vertex_id
    {
        friend struct invalid_vertex;

        template<class, class>
        friend
        class graph_base;

    private:
        vertex_id(std::size_t i)
            : id_{i}
        {}
        vertex_id(stock_id i)
            : id_(i)
        {}
    public:
        vertex_id() = default;
        bool operator<(vertex_id rhs) const
        {
            return id_ < rhs.id_;
        }
        bool operator==(vertex_id rhs) const
        {
            return id_ == rhs.id_;
        }
    private:
        auto id() const noexcept
        {
            return id_;
        }
        stock_id id_{static_cast<std::size_t>(-1)};
    };

    struct invalid_vertex
    {
        static constexpr vertex_id id{};
    };
    constexpr vertex_id invalid_vertex_id = invalid_vertex::id;

}
#endif //GRAPH_VERTEX_ID_H
