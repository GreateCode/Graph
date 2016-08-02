#ifndef REPOSITORY_H_INCLUDED
#define REPOSITORY_H_INCLUDED
#include <exception>
#include <stdexcept>
#include <vector>
#include <set>
namespace lzhlib
{

    namespace traits
    {
        template <class StockT>
        struct stock_traits
        {
        };
    }
    template <class StockT>
    class repository
    {
    public:
        using stock_t = StockT;
        using id_t = typename traits::stock_traits<stock_t>::id_t;

        class attempt_to_use_unassigned_stock: public std::out_of_range
        {
        public:
            attempt_to_use_unassigned_stock(id_t id)
                : out_of_range(std::string("Attempt to use unassigned stock which id is ") + std::to_string(id.id) + "!")
            {

            }
        };
        class attempt_to_reuse_unreusable_stock: public std::out_of_range
        {
        public:
            attempt_to_reuse_unreusable_stock(id_t id)
                : out_of_range(std::string("Attempt to reuse unreusable stock which id is ") + std::to_string(id.id) + "!")
            {
            }
        };

        stock_t& get_stock(id_t id)
        {
#ifndef NDEBUG
            if(ids_of_living_stocks.find(id) == ids_of_living_stocks.end())
            {
                throw attempt_to_use_unassigned_stock(id);
            }
#endif // NDEBUG
            return stocks[id.id];
        }
        stock_t const& get_stock(id_t id) const
        {
#ifndef NDEBUG
            if(ids_of_living_stocks.find(id) == ids_of_living_stocks.end())
            {
                throw attempt_to_use_unassigned_stock(id);
            }
#endif // NDEBUG
            return stocks[id.id];
        }
        template <class ...Args>
        id_t add_stock(Args&&... args)
        {
            if(ids_of_reusable_stocks.empty())
            {
                return allocate_stock(std::forward<Args>(args)...);
            }
            else
            {
                return reuse_stock(std::forward<Args>(args)...);
            }
        }
        void remove_stock(id_t id)
        {
            get_stock(id).~stock_t();
            ids_of_living_stocks.erase(id);
            ids_of_reusable_stocks.insert(id);
        }
        id_t first_stock() const
        {
            return *ids_of_living_stocks.begin();
        }
        bool is_last_stock(id_t current_stock) const
        {
            return ++(ids_of_living_stocks.find(current_stock)) == ids_of_living_stocks.end();
        }
        id_t next_stock(id_t current_stock) const
        {
            return *++(ids_of_living_stocks.find(current_stock));
        }
    private:
        template <class ...Args>
        id_t allocate_stock(Args&&... args)
        {
            stocks.emplace_back(std::forward<Args>(args)...);
            id_t ret{stocks.size() - 1};
            ids_of_living_stocks.insert(ret);
            return ret;
        }
        template <class ...Args>
        id_t reuse_stock(Args&&... args)
        {
            typename std::set<id_t>::iterator i = iterator_of_a_reusable_stock();
            id_t ret = *i;
            new(&get_reusable_stock(ret)) stock_t(std::forward<Args>(args)...);
            ids_of_reusable_stocks.erase(i);
            ids_of_living_stocks.insert(ret);
            return ret;
        }
        typename std::set<id_t>::iterator iterator_of_a_reusable_stock() const
        {
            return ids_of_reusable_stocks.begin();
        }
        stock_t& get_reusable_stock(id_t id)
        {
#ifndef NDEBUG
            if(ids_of_reusable_stocks.find(id) == ids_of_reusable_stocks.end())
            {
                throw attempt_to_reuse_unreusable_stock(id);
            }
#endif // NDEBUG
            return stocks[id.id];
        }
    private:
        std::vector<stock_t> stocks;
        std::set<id_t> ids_of_living_stocks;
        std::set<id_t> ids_of_reusable_stocks;
    };

}

#endif // REPOSITORY_H_INCLUDED
