#ifndef REPOSITORY_H_INCLUDED
#define REPOSITORY_H_INCLUDED
#include <cstddef>    //for std::size_t
#include <exception>
#include <stdexcept>
#include <vector>
#include <memory>
#include <set>
#include "Graph/include/stock_id.h"
namespace lzhlib
{
    template <class StockT>
    class repository
    {
    public:
        using stock_t = StockT;
        using id_t = stock_id;
        using pointer_t = std::unique_ptr<stock_t>;

        class attempt_to_use_unassigned_stock: public std::out_of_range
        {
        public:
            attempt_to_use_unassigned_stock(id_t id)
                : out_of_range(std::string("Attempt to use unassigned stock which id is ") + std::to_string(id.id()) + "!")
            {

            }
        };
        class attempt_to_remove_nonexistent_stock: public std::out_of_range
        {
        public:
            attempt_to_remove_nonexistent_stock(id_t id)
                : out_of_range(std::string("Attempt to use unremovable stock which id is ") + std::to_string(id.id()) + "!")
            {

            }
        };
        class attempt_to_reuse_unreusable_stock: public std::out_of_range
        {
        public:
            attempt_to_reuse_unreusable_stock(id_t id)
                : out_of_range(std::string("Attempt to reuse unreusable stock which id is ") + std::to_string(id.id()) + "!")
            {
            }
        };

        stock_t& get_stock(id_t id)
        {
#ifndef NDEBUG
            if(is_not_found_in_living_stocks(id))
            {
                throw attempt_to_use_unassigned_stock(id);
            }
#endif // NDEBUG
            return *stocks[id.id()];
        }
        stock_t const& get_stock(id_t id) const
        {
#ifndef NDEBUG
            if(is_not_found_in_living_stocks(id))
            {
                throw attempt_to_use_unassigned_stock(id);
            }
#endif // NDEBUG
            return *stocks[id.id()];
        }
        template <class ...Args>
        id_t add_stock(Args&&... args)
        {
            if(no_reusable_stocks())
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
#ifndef NDEBUG
            if(is_not_found_in_living_stocks(id))
                throw attempt_to_remove_nonexistent_stock(id);
#endif // NDEBUG
            stocks[id.id()].reset();
            ids_of_living_stocks.erase(id);
            ids_of_reusable_stocks.insert(id);
        }
        id_t first_stock() const
        {
            return *ids_of_living_stocks.begin();
        }
        bool stock_end(id_t current_stock) const
        {
            return ids_of_living_stocks.find(current_stock) == ids_of_living_stocks.end();
        }
        id_t next_stock(id_t current_stock) const
        {
            return *++(ids_of_living_stocks.find(current_stock));
        }
    private:
#ifndef NDEBUG
        bool is_not_found_in_living_stocks(id_t id) const
        {
            return ids_of_living_stocks.find(id) == ids_of_living_stocks.end();
        }
#endif // NDEBUG
        bool no_reusable_stocks()
        {
            return ids_of_reusable_stocks.empty();
        }
        template <class ...Args>
        id_t allocate_stock(Args&&... args)
        {
            stocks.push_back(std::make_unique<stock_t>(std::forward<Args>(args)...));
            id_t ret {stocks.size() - 1};    //the allocated stock is at the last position in the container.
            ids_of_living_stocks.insert(ret);
            return ret;
        }
        template <class ...Args>
        id_t reuse_stock(Args&&... args)
        {
            typename std::set<id_t>::iterator i = iterator_of_a_reusable_stock();
            id_t ret = *i;
            reusable_pointer(ret) = std::make_unique<stock_t>(std::forward<Args>(args)...);
            ids_of_reusable_stocks.erase(i);
            ids_of_living_stocks.insert(ret);
            return ret;
        }
        typename std::set<id_t>::iterator iterator_of_a_reusable_stock() const
        {
            return ids_of_reusable_stocks.begin();
        }
        pointer_t& reusable_pointer(id_t id)          //just a checker.The calling may be optimized out -- that's to say, may be inlined.
        {
#ifndef NDEBUG
            if(is_not_found_in_reusable_stocks(id))
            {
                throw attempt_to_reuse_unreusable_stock(id);
            }
#endif // NDEBUG
            return stocks[id.id()];
        }
#ifndef NDEBUG
        bool is_not_found_in_reusable_stocks(id_t id) const
        {
            return ids_of_reusable_stocks.find(id) == ids_of_reusable_stocks.end();
        }
#endif // NDEBUG
    private:
        std::vector<pointer_t> stocks;
        std::set<id_t> ids_of_living_stocks;
        std::set<id_t> ids_of_reusable_stocks;
    };

}

#endif // REPOSITORY_H_INCLUDED
