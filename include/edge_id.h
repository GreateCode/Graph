#ifndef GRAPH_EDGE_ID_H
#define GRAPH_EDGE_ID_H

#include <cstddef>     //for std::size_t
#include "Graph/include/stock_id.h"

namespace lzhlib
{
    class edge_id
    {
        template<class, class>
        friend
        class graph_base;

    private:
        edge_id() = default;
        edge_id(std::size_t i)
            : id_{i}
        {}
        edge_id(stock_id i)
            : id_(i)
        {}
    public:
        bool operator<(edge_id rhs) const
        {
            return id_ < rhs.id_;
        }
        bool operator==(edge_id rhs) const
        {
            return id_ == rhs.id_;
        }
        bool operator!=(edge_id rhs) const
        {
            return !(*this == rhs);
        }
    private:
        auto id() const noexcept
        {
            return id_;
        }
        stock_id id_;
    };

}
#endif //GRAPH_EDGE_ID_H
