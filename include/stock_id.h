#ifndef GRAPH_STOCK_ID_H
#define GRAPH_STOCK_ID_H

#include <cstddef>    //for std::size_t

namespace lzhlib
{
    class stock_id
    {
    public:
        constexpr stock_id(std::size_t i)
            : id_(i)
        {}
        stock_id() = default;
        bool operator<(stock_id rhs) const
        {
            return id_ < rhs.id_;
        }

        bool operator==(stock_id rhs) const
        {
            return id_ == rhs.id_;
        }

        auto id() const noexcept
        {
            return id_;
        }

    private:
        std::size_t id_;
    };
}
#endif //GRAPH_STOCK_ID_H
