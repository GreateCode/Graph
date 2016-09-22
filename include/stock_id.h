#ifndef GRAPH_STOCK_ID_H
#define GRAPH_STOCK_ID_H
#include <cstddef>
namespace lzhlib
{
    class stock_id
    {
    public:
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
