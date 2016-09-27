#ifndef REPOSITORY_H_INCLUDED
#define REPOSITORY_H_INCLUDED

#include <stdexcept>  //for std::out_of_range
#include <vector>     //for std::vector
#include <memory>     //for std::unique_ptr
#include <set>        //for std::set
#include "Graph/include/stock_id.h"

namespace lzhlib
{
    template<class StockT>
    class repository
    {
    public:
        using stock_t = StockT;
        using id_t = stock_id;
        using pointer_t = std::unique_ptr<stock_t>;

        class attempt_to_use_unassigned_stock : public std::out_of_range
        {
        public:
            attempt_to_use_unassigned_stock(id_t id)
                : out_of_range(std::string("Attempt to use unassigned stock which id is ") +
                               std::to_string(id.id()) + "!")
            {

            }
        };

        class attempt_to_remove_nonexistent_stock : public std::out_of_range
        {
        public:
            attempt_to_remove_nonexistent_stock(id_t id)
                : out_of_range(std::string("Attempt to use unremovable stock which id is ") +
                               std::to_string(id.id()) + "!")
            {

            }
        };

        class attempt_to_reuse_unreusable_stock : public std::out_of_range
        {
        public:
            attempt_to_reuse_unreusable_stock(id_t id)
                : out_of_range(std::string("Attempt to reuse unreusable stock which id is ") +
                               std::to_string(id.id()) + "!")
            {
            }
        };

        stock_t& get_stock(id_t id)
        {
#ifndef NDEBUG
            if (is_not_valid(id))
            {
                throw attempt_to_use_unassigned_stock(id);
            }
#endif // NDEBUG
            return *stocks[id.id()];
        }
        stock_t const& get_stock(id_t id) const
        {
#ifndef NDEBUG
            if (is_not_valid(id))
            {
                throw attempt_to_use_unassigned_stock(id);
            }
#endif // NDEBUG
            return *stocks[id.id()];
        }
        template<class ...Args>
        id_t add_stock(Args&& ... args)
        {
            id_t ret = reusable_stock();
            if (ret == stocks.size())  //no reusable stock
            {
                return allocate_stock(std::forward<Args>(args)...);
            }
            else
            {
                return reuse_stock(ret,std::forward<Args>(args)...);
            }
        }
        void remove_stock(id_t id)
        {
#ifndef NDEBUG
            if (is_not_valid(id))
                throw attempt_to_remove_nonexistent_stock(id);
#endif // NDEBUG
            stocks[id.id()].reset();
        }
        id_t first_stock() const
        {
            return stock_at_or_after(id_t{0});
        }
        bool stock_end(id_t current) const
        {
            return current.id() == stocks.size();
        }
        id_t next_stock(id_t current) const        //precondition: current.id() < stocks.size()
        {
            return stock_at_or_after(++current);   //postcondition: 设返回值为next,则stocks[next.id()]为current代表的位置或current代表的位置之后的位置上的有效stock的左值,
        }                                          // 或者next.id() == stocks.size()(current代表的位置之后的位置上均无有效stock

    private:
#ifndef NDEBUG
        bool is_not_valid(id_t id) const
        {
            return stocks[id.id()] == nullptr;
        }
#endif // NDEBUG
        bool no_reusable_stocks()
        {

        }
        template<class ...Args>
        id_t allocate_stock(Args&& ... args)
        {
            stocks.push_back(std::make_unique<stock_t>(std::forward<Args>(args)...));
            id_t ret{stocks.size() -
                     1};    //the allocated stock is at the last position in the container.
            return ret;
        }
        template<class ...Args>
        id_t reuse_stock(id_t reused, Args&& ... args)//precondition:stocks[reused.id()]必须为一个空指针
        {
            reusable_pointer(reused) = std::make_unique<stock_t>(std::forward<Args>(args)...);
            return reused;
        }
        id_t reusable_stock() const               //postcondition: 设返回值为ret,则stocks[ret.id()]为一个空指针.
        {                                         //或者ret.id() == stocks.size()(current代表的位置及current代表的位置之后的位置上均无reusable stock
            id_t current{0};
            while (current.id() != stocks.size() && stocks[current.id()] != nullptr)
                ++current;
            return current;
        }
        pointer_t& reusable_pointer(id_t id)          //just a checker.The calling may be optimized out -- that's to say, may be inlined.
        {
#ifndef NDEBUG
            if(stocks[id.id()] != nullptr)
                throw attempt_to_reuse_unreusable_stock(id);
#endif // NDEBUG
            return stocks[id.id()];
        }
        id_t stock_at_or_after(id_t current) const //precondition: current.id() <= stocks.size()
        {
            while (current.id() != stocks.size() && stocks[current.id()] == nullptr)
                ++current;
            return current;                       //postcondition: 设返回值为next,则stocks[next.id()]为指向current代表的位置或current代表的位置之后的位置上的有效stock的指针的左值,
        }                                         //或者next.id() == stocks.size()(current代表的位置及current代表的位置之后的位置上均无有效stock
    private:
        std::vector<pointer_t> stocks;
    };

}

#endif // REPOSITORY_H_INCLUDED
