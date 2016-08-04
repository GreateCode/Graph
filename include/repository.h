#ifndef REPOSITORY_H_INCLUDED
#define REPOSITORY_H_INCLUDED
#include <exception>
#include <stdexcept>
#include <list>
#include <string>
namespace lzhlib
{

    template <typename StockT>
    class id
    {
        template <class>
        friend class repository;
    public:
        using stock_t = StockT;

        bool operator==(id rhs) const
        {
            return pointer == rhs.pointer;
        }
        bool operator!=(id rhs) const
        {
            return !(*this<rhs);
        }
    private:
        typename std::list<stock_t>::iterator pointer;
    };
    template <class StockT>
    class repository
    {
    public:
        using stock_t = StockT;
        using id_t = id<stock_t>;


        stock_t& get_stock(id_t id)
        {
            return *id.pointer;
        }
        stock_t const& get_stock(id_t id) const
        {
            return *id.pointer;
        }
        template <class ...Args>
        id_t add_stock(Args&&... args)
        {
            return {stocks.emplace(stocks.begin(),std::forward<Args>(args)...)};
        }
        void remove_stock(id_t id)
        {
            stocks.erase(id.pointer);
        }
        id_t first_stock() const
        {
            return {stocks.begin()};
        }
        bool is_last_stock(id_t current_stock) const
        {
            return current_stock == stocks.end();
        }
        id_t next_stock(id_t current_stock) const
        {
            id_t next = current_stock;
            ++next.pointer;
            return next;
        }
    private:
        std::list<stock_t> stocks;
    };

}

#endif // REPOSITORY_H_INCLUDED
