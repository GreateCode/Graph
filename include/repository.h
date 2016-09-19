#ifndef REPOSITORY_H_INCLUDED
#define REPOSITORY_H_INCLUDED
#include <exception>
#include <stdexcept>
#include <vector>
#include <memory>
#include <set>
namespace lzhlib
{

    namespace traits
    {
        template <class StockT>
        struct stock_traits                  //特化提供type member: id_t
        //                     id_t的对象id需使以下代码合法:
        //                     static_cast<vector<StockT>::size_type>(id.id());
        {
        };
    }
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
        unsigned long long id_;
    };
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
        bool is_last_stock(id_t current_stock) const
        {
            return ++(ids_of_living_stocks.find(current_stock)) == ids_of_living_stocks.end();
        }
        id_t next_stock(id_t current_stock) const
        {
            return *++(ids_of_living_stocks.find(current_stock));
        }
    private:
        bool is_not_found_in_living_stocks(id_t id) const
        {
            return ids_of_living_stocks.find(id) == ids_of_living_stocks.end();
        }
        bool no_reusable_stocks()
        {
            return ids_of_reusable_stocks.empty();
        }
        template <class ...Args>
        id_t allocate_stock(Args&&... args)
        {
            stocks.push_back(std::make_unique<stock_t>(std::forward<Args>(args)...));
            id_t ret{stocks.size() - 1};
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
        pointer_t& reusable_pointer(id_t id)
        {
#ifndef NDEBUG
            if(is_not_found_in_reusable_stocks(id))
            {
                throw attempt_to_reuse_unreusable_stock(id);
            }
#endif // NDEBUG
            return stocks[id.id()];
        }
        bool is_not_found_in_reusable_stocks(id_t id) const
        {
            return ids_of_reusable_stocks.find(id) == ids_of_reusable_stocks.end();
        }
    private:
        std::vector<pointer_t> stocks;
        std::set<id_t> ids_of_living_stocks;
        std::set<id_t> ids_of_reusable_stocks;
    };

}

#endif // REPOSITORY_H_INCLUDED
