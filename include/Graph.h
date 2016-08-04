#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include "repository.h"
#include <vector>
#include <set>
#include <algorithm>
#include <string>
#include <exception>
#include <cassert>
/** \file
 *  \brief 本文件提供与 [图](https://en.wikipedia.org/wiki/Graph_(abstract_data_type)) 有关的抽象.
 *
 * ##图 支持的操作:(摘自维基百科)
<ul>
<li> \code adjacent(G, x, y) \endcode tests whether there is an edge from the vertices x to y;</li>
<li> \code neighbors (G, x) \endcode lists all vertices y such that there is an edge from the vertices x to y;</li>
<li> \code add_vertex(G, x) \endcode adds the vertex x, if it is not there;</li>
<li> \code remove_vertex(G, x) \endcode removes the vertex x, if it is there;</li>
<li> \code add_edge(G, x, y) \endcode adds the edge from the vertices x to y, if it is not there;</li>
<li> \code remove_edge(G, x, y) \endcode removes the edge from the vertices x to y, if it is there;</li>
<li> \code get_vertex_value(G, x) \endcode returns the value associated with the vertex x;</li>
<li> \code set_vertex_value(G, x, v) \endcode sets the value associated with the vertex x to v.</li>
</ul>
Structures that associate values to the edges usually also provide:
<ul>
<li> \code get_edge_value(G, x, y) \endcode returns the value associated with the edge (x, y);</li>
<li> \code set_edge_value(G, x, y, v) \endcode sets the value associated with the edge (x, y) to v.</li>
</ul>
 *
 */
namespace lzhlib
{
//    namespace exceptions
//    {
//        class require_edge_that_does_not_exist;
//    }
//    class vertex_id
//    {
//        friend struct invalid_vertex;
//        template<class>
//        friend class repository;
//        friend class exceptions::require_edge_that_does_not_exist;
//        friend struct invalid_vertex;
//    private:
//        vertex_id(unsigned long long i)
//            : id(i)
//        {}
//    public:
//        vertex_id() = default;
//        bool operator<(vertex_id rhs) const
//        {
//            return id < rhs.id;
//        }
//        bool operator==(vertex_id rhs) const
//        {
//            return id == rhs.id;
//        }
//    private:
//        unsigned long long id = static_cast<unsigned long long>(-1);     //vector::size_type 为unsigned long long
//    };
    template <class vertex_t>
    constexpr id<vertex_t> invalid_vertex_id{};
//    class edge_id
//    {
//        template<class>
//        friend class repository;
//    private:
//        edge_id() = default;
//        edge_id(unsigned long long i)
//            : id(i)
//        {}
//    public:
//        bool operator<(edge_id rhs) const
//        {id<vertex>
//            return id < rhs.id;
//        }
//        bool operator==(edge_id rhs) const
//        {
//            return id == rhs.id;
//        }
//        bool operator!=(edge_id rhs) const
//        {
//            return !(*this == rhs);
//        }
//    private:
//        unsigned long long id;
//    };
//
    namespace exceptions
    {
        class require_edge_that_does_not_exist: public std::logic_error
        {
        public:
            require_edge_that_does_not_exist()
                : std::logic_error(std::string("require edge that doesn't exist!"))
            {

            }
        };
        [[noreturn]] void throw_exception_require_edge_that_does_not_exist()
        {
            throw exceptions::require_edge_that_does_not_exist();
        }
    }

    class null_value_tag;
    template<class VertexValueT = null_value_tag,class EdgeValueT = null_value_tag>
    struct traits
    {
        using vertex_value_t = VertexValueT;
        using edge_value_t = EdgeValueT;
    };
    namespace detail
    {
        template <class Traits = traits<>>
        class vertex;                                         //only used by graph_impl
        template <class Traits = traits<>>
        class edge;
        template <class Traits>
        struct typedef_base
        {
            using traits_t = Traits;

            using vertex_t = vertex<traits_t>;
            using vertex_value_t = typename traits_t::vertex_value_t;
            using vertex_id_t = id<vertex_t>;

            using edge_t = edge<traits_t>;
            using edge_value_t = typename traits_t::edge_value_t;
            using edge_id_t = id<edge_t>;
        };
        template <class EdgeValueT>          //only used by graph_impl
        class vertex<traits<null_value_tag,EdgeValueT>>: public typedef_base<traits<null_value_tag,EdgeValueT>>//强制使用traits
        {
        public:
//            bool is_associated(edge_id_t i)
//            {
//                return edges.find(i)!=edges.end();            //a possibly more efficent way :ask edge for this
//            }
            std::vector<typename vertex::edge_id_t>const& get_associated_edges() const
            {
                return edges;
            }
            void add_associated_edge(typename vertex::edge_id_t i)
            {
                if(find(edges.begin(),edges.end(),i) == edges.end())
                    edges.push_back(i);
            }
            void remove_associated_edge(typename vertex::edge_id_t i)
            {
                edges.erase(remove(edges.begin(),edges.end(),i),edges.end());
            }
        private:
            std::vector<typename vertex::edge_id_t> edges;
        };
        template <class VertexValueT,class EdgeValueT>
        class vertex<traits<VertexValueT,EdgeValueT>>: public typedef_base<traits<VertexValueT,EdgeValueT>> //强制使用traits
        {
        public:
            std::vector<typename vertex::edge_id_t>const& get_associated_edges() const
            {
                return edges;
            }
            void add_associated_edge(typename vertex::edge_id_t i)
            {
                if(find(edges.begin(),edges.end(),i) == edges.end())
                    edges.push_back(i);
            }
            void remove_associated_edge(typename vertex::edge_id_t i)
            {
                edges.erase(remove(edges.begin(),edges.end(),i),edges.end());
            }

            template<class ...Args>
            vertex(Args&&... args)
                : value(std::forward<Args>(args)...)
            {

            }
            typename vertex::vertex_value_t& vertex_value() &
            {
                return value;
            }
            typename vertex::vertex_value_t&& vertex_value() &&
            {
                return std::move(value);
            }
            typename vertex::vertex_value_t const& vertex_value() const&
            {
                return value;
            }
            typename vertex::vertex_value_t const&& vertex_value() const&&
            {
                return std::move(value);
            }
        private:
            std::vector<typename vertex::edge_id_t> edges;
            typename vertex::vertex_value_t value;
        };

        template <class VertexValueT>
        class edge<traits<VertexValueT,null_value_tag>>: public typedef_base<traits<VertexValueT,null_value_tag>>
        {
        public:
            using pair_t = std::pair<typename edge::vertex_id_t, typename edge::vertex_id_t>;
            bool is_associated(typename edge::vertex_id_t i) const
            {
                return i == vertices.first || i == vertices.second;
            }
            pair_t get_associated_vertices() const
            {
                return vertices;
            }
            void set_associated_vertices(typename edge::vertex_id_t v1, typename edge::vertex_id_t v2)
            {
                vertices.first = v1;
                vertices.second = v2;
            }
            void detach()
            {
                vertices.first = vertices.second = invalid_vertex_id<edge::vertex_t>;
            }
        private:
            pair_t vertices;
        };
        template <class VertexValueT,class EdgeValueT>
        class edge<traits<VertexValueT,EdgeValueT>>: public typedef_base<traits<VertexValueT,EdgeValueT>>
        {
        public:
            using pair_t = std::pair<typename edge::vertex_id_t, typename edge::vertex_id_t>;
            bool is_associated(typename edge::vertex_id_t i) const
            {
                return i == vertices.first || i == vertices.second;
            }
            pair_t get_associated_vertices() const
            {
                return vertices;
            }
            void set_associated_vertices(typename edge::vertex_id_t v1, typename edge::vertex_id_t v2)
            {
                vertices.first = v1;
                vertices.second = v2;
            }
            void detach()
            {
                vertices.first = vertices.second = invalid_vertex_id<edge::vertex_t>;
            }

            template <class ...Args>
            edge(Args&&...args)
                : value(std::forward<Args>(args)...)
            {
            }
            typename edge::edge_value_t& edge_value() &
            {
                return value;
            }
            typename edge::edge_value_t&& edge_value() &&
            {
                return std::move(value);
            }
            typename edge::edge_value_t const& edge_value() const&
            {
                return value;
            }
            typename edge::edge_value_t const&& edge_value() const&&
            {
                return std::move(value);
            }
        private:
            pair_t vertices;
            typename edge::edge_value_t value;
        };
    }


    template <class Traits>
    class undirected_graph: public detail::typedef_base<Traits>
    {
    public:
        using pair_t = typename undirected_graph::edge_t::pair_t;

        bool adjacent(typename undirected_graph::vertex_id_t x, typename undirected_graph::vertex_id_t y) const
        {
            if(get_vertex(x).get_associated_edges().size() < get_vertex(y).get_associated_edges().size())
                return adjacent_impl(x, y);
            else
                return adjacent_impl(y, x);
        }
        std::vector<typename undirected_graph::vertex_id_t> neighbors(typename undirected_graph::vertex_id_t v) const
        {
            auto const& edges = get_vertex(v).get_associated_edges();
            std::vector<typename undirected_graph::vertex_id_t> ret;
            ret.reserve(edges.size());
            for(auto e : edges)
            {
                ret.push_back(get_opposite_vertex(v, e));
            }
            return ret;
        }
        auto const& get_associated_edges(typename undirected_graph::vertex_id_t v) const
        {
            return get_vertex(v).get_associated_edges();
        }
        template <class ...Args>
        typename undirected_graph::vertex_id_t add_vertex(Args&&...args)
        {
            return vertex_repository.add_stock(std::forward<Args>(args)...);
        }
        void remove_vertex(typename undirected_graph::vertex_id_t v)
        {
            auto edges = get_vertex(v).get_associated_edges();
            for(auto e : edges)
            {
                remove_edge_from(e, v);
            }
            vertex_repository.remove_stock(v);
        }
        template <class ...Args>
        typename undirected_graph::edge_id_t add_edge(typename undirected_graph::vertex_id_t x, typename undirected_graph::vertex_id_t y,Args&& ...args)
        {
            typename undirected_graph::edge_id_t result = edge_repository.add_stock(std::forward<Args>(args)...);
            get_edge(result).set_associated_vertices(x,y);
            get_vertex(x).add_associated_edge(result);
            get_vertex(y).add_associated_edge(result);
            return result;
        }
        void remove_edge(typename undirected_graph::vertex_id_t x, typename undirected_graph::vertex_id_t y)
        {
            remove_edge(get_edge(x,y));
        }
        void remove_edge(typename undirected_graph::edge_id_t e)
        {
            pair_t vertices = get_edge(e).get_associated_vertices();
            get_vertex(vertices.first).remove_associated_edge(e);
            remove_edge_from(e,vertices.second);
        }

        typename undirected_graph::edge_id_t get_edge(typename undirected_graph::vertex_id_t x, typename undirected_graph::vertex_id_t y) const
        {
            assert(adjacent(x, y));
            if(get_vertex(x).get_associated_edges().size() < get_vertex(y).get_associated_edges().size())
                return get_edge_impl(x, y);
            else
                return get_edge_impl(y, x);
        }
        pair_t get_associated_vertices(typename undirected_graph::edge_id_t e) const
        {
            return get_edge(e).get_associated_vertices();
        }

        typename undirected_graph::vertex_value_t& value(typename undirected_graph::vertex_id_t v)
        {
            return get_vertex(v).vertex_value();
        }
        typename undirected_graph::vertex_value_t const& value(typename undirected_graph::vertex_id_t v) const
        {
            return get_vertex(v).vertex_value();
        }
        typename undirected_graph::edge_value_t& value(typename undirected_graph::edge_id_t e)
        {
            return get_edge(e).edge_value();
        }
        typename undirected_graph::edge_value_t const& value(typename undirected_graph::edge_id_t e) const
        {
            return get_edge(e).edge_value();
        }

        typename undirected_graph::vertex_id_t first_vertex() const
        {
            return vertex_repository.first_stock();
        }
        bool is_last_vertex(typename undirected_graph::vertex_id_t id) const
        {
            return vertex_repository.is_last_stock(id);
        }
        typename undirected_graph::vertex_id_t next_vertex(typename undirected_graph::vertex_id_t id) const
        {
            return vertex_repository.next_stock(id);
        }
    private:
        typename undirected_graph::vertex_t& get_vertex(typename undirected_graph::vertex_id_t v)
        {
            return vertex_repository.get_stock(v);
        }
        typename undirected_graph::vertex_t const& get_vertex(typename undirected_graph::vertex_id_t v) const
        {
            return vertex_repository.get_stock(v);
        }
        typename undirected_graph::edge_t& get_edge(typename undirected_graph::edge_id_t e)
        {
            return edge_repository.get_stock(e);
        }
        typename undirected_graph::edge_t const& get_edge(typename undirected_graph::edge_id_t e) const
        {
            return edge_repository.get_stock(e);
        }

        bool adjacent_impl(typename undirected_graph::vertex_id_t x, typename undirected_graph::vertex_id_t y) const
        {
            auto const& edges = get_vertex(x).get_associated_edges();
            for(auto e : edges)
            {
                if(get_edge(e).is_associated(y))
                    return true;
            }
            return false;
        }
        typename undirected_graph::vertex_id_t get_opposite_vertex(typename undirected_graph::vertex_id_t v, typename undirected_graph::edge_id_t e) const
        {
            pair_t vertices = get_edge(e).get_associated_vertices();
            if(vertices.first == v)
            {
                return vertices.second;
            }
            else
            {
                assert(vertices.second == v);
                return vertices.first;
            }
        }
        void remove_edge_from(typename undirected_graph::edge_id_t e, typename undirected_graph::vertex_id_t v)
        {
            get_vertex(get_opposite_vertex(v, e)).remove_associated_edge(e);
            edge_repository.remove_stock(e);
        }
        typename undirected_graph::edge_id_t get_edge_impl(typename undirected_graph::vertex_id_t x, typename undirected_graph::vertex_id_t y) const
        {
            auto const& edges = get_vertex(x).get_associated_edges();
            for(auto e : edges)
            {
                if(get_edge(e).is_associated(y))
                    return e;
            }
            exceptions::throw_exception_require_edge_that_does_not_exist();
        }

        repository<typename undirected_graph::vertex_t> vertex_repository;
        repository<typename undirected_graph::edge_t> edge_repository;
    };
}



#endif // GRAPH_H_INCLUDED

