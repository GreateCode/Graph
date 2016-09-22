#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <vector>
#include <set>
#include <string>
#include <exception>
#include <cassert>
#include "Graph/include/repository.h"
#include "Graph/include/vertex_id.h"
#include "Graph/include/edge_id.h"
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

    namespace exceptions
    {
        class require_edge_that_does_not_exist : public std::logic_error
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
    namespace detail
    {
        class edge_ref
        {
        public:
            edge_id associated_edge() const
            {
                return edge_;
            }
            vertex_id opposite_vertex() const
            {
                return vertex_;
            }
            bool is_connected(vertex_id v) const
            {
                return vertex_ == v;
            }

            bool operator<(edge_ref rhs) const  //默认忽略opposite_vertex()
            {
                return edge_ < rhs.edge_;
            }
            bool operator==(edge_ref rhs) const
            {
                return edge_ == rhs.edge_;
            }
            bool operator!=(edge_ref rhs) const
            {
                return !(*this == rhs);
            }

        private:
            edge_id edge_;
            vertex_id vertex_;
        };

        bool operator<(edge_ref r, edge_id rhs)
        {
            return r.associated_edge() < rhs;
        }
        bool operator<(edge_id rhs, edge_ref r)
        {
            return rhs < r.associated_edge();
        }
        bool operator<(edge_ref r, vertex_id rhs) //仍可依据opposite_vertex()来比较
        {
            return r.opposite_vertex() < rhs;
        }
        bool operator<(vertex_id rhs, edge_ref r) //仍可依据opposite_vertex()来比较
        {
            return rhs < r.opposite_vertex();
        }


        template<class VertexValueT = null_value_tag>
        class vertex;

        template<>
        class vertex<null_value_tag>
        {

        public:
            using vertex_value_t = null_value_tag;

//            bool is_associated(edge_id i)
//            {
//                return edges.find(i)!=edges.end();            //a possibly more efficent way :ask edge for this
//            }
            auto const& associated_edges() const
            {
                return edges;
            }
            void add_associated_edge(edge_ref i)
            {
                edges.insert(i);
            }
            void remove_associated_edge(edge_ref i)           //i.asociated_edge()对应的边将会移除.忽略i.opposite_vertex的值.
            {
                edges.erase(i);
            }

            bool adjacent(vertex_id v) const
            {
                auto const& edges = associated_edges();
                if (edges.find(v) != edges.end())
                    return true;
                return false;
            }
            bool associated(edge_id e) const
            {
                auto const& edges = associated_edges();
                if (edges.find(e) != edges.end())
                    return true;
                return false;
            }

            edge_id associated_edge(vertex_id y) const
            {
                auto const& edges = associated_edges();
                for (edge_ref e : edges)
                {
                    if (e.is_connected(y))
                        return e.associated_edge();
                }
                exceptions::throw_exception_require_edge_that_does_not_exist();
            }

        private:
            std::set<edge_ref, std::less<void>> edges;  //for std::less<void>::is_transparent  --> for comparision between differnet types
        };

        template<class VertexValueT>
        class vertex : public vertex<null_value_tag>
        {
        public:
            using vertex_value_t = VertexValueT;

            template<class ...Args>
            vertex(Args&& ... args)
                    : value(std::forward<Args>(args)...)
            {

            }

            vertex_value_t& vertex_value()&
            {
                return value;
            }
            vertex_value_t&& vertex_value()&&
            {
                return std::move(value);
            }
            vertex_value_t const& vertex_value() const&
            {
                return value;
            }
            vertex_value_t const&& vertex_value() const&&
            {
                return std::move(value);
            }

        private:
            vertex_value_t value;
        };

        template<class EdgeValueT = null_value_tag>
        class edge;

        template<>
        class edge<null_value_tag>
        {
        public:
            using edge_value_t = null_value_tag;
            using pair_t = std::pair<vertex_id, vertex_id>;

            bool is_associated(vertex_id i) const
            {
                return i == vertices.first || i == vertices.second;
            }
            pair_t associated_vertices() const
            {
                return vertices;
            }
            void set_associated_vertices(vertex_id v1, vertex_id v2)
            {
                vertices.first = v1;
                vertices.second = v2;
            }
            void detach()
            {
                vertices.first = vertices.second = invalid_vertex_id;
            }

        private:
            pair_t vertices;   //对于 undirected graph,vertices.first 与vertices.second 的地位相同.对于directed graph,vertices.first为边的起点,vertices.second为边的终点.
        };

        template<class EdgeValueT>
        class edge : public edge<null_value_tag>
        {
        public:
            using edge_value_t = EdgeValueT;

            template<class ...Args>
            edge(Args&& ...args)
                    : value(std::forward<Args>(args)...)
            {
            }

            edge_value_t& edge_value()&
            {
                return value;
            }
            edge_value_t&& edge_value()&&
            {
                return std::move(value);
            }
            edge_value_t const& edge_value() const&
            {
                return value;
            }
            edge_value_t const&& edge_value() const&&
            {
                return std::move(value);
            }

        private:
            edge_value_t value;
        };
    }

    template<class VertexValueT, class EdgeValueT>
    class graph_base
    {
    public:
        using vertex_t = detail::vertex<VertexValueT>;
        using vertex_value_t = typename vertex_t::vertex_value_t;
        using edge_ref_t = typename detail::edge_ref;
        using edge_t = detail::edge<EdgeValueT>;
        using edge_value_t = typename edge_t::edge_value_t;
        using pair_t = typename edge_t::pair_t;

        bool associated(vertex_id v, edge_id e)
        {
            return get_vertex(v).associated(e);
        }
        std::vector<vertex_id> neighbors(vertex_id v) const              //不如让用户 auto const& s = graph.associated_edges(v);
        {
            auto const& edges = associated_edges(v);     //在 for(auto e : s)中
            std::vector<vertex_id> ret;                                  //再     vertex_id v = e.opposite_vertex();
            ret.reserve(edges.size());
            for (edge_ref_t e : edges)
                ret.push_back(e.opposite_vertex());
            return ret;
        }
        auto const& associated_edges(vertex_id v) const  //比neighbors更建议使用(出于效率考虑)
        {
            return get_vertex(v).associated_edges();
        }

        template<class ...Args>
        vertex_id add_vertex(Args&& ...args)
        {
            return vertex_repository.add_stock(std::forward<Args>(args)...);
        }

        pair_t associated_vertices(edge_id e) const
        {
            return get_edge(e).associated_vertices();
        }

        vertex_value_t& value(vertex_id v)
        {
            return get_vertex(v).vertex_value();
        }
        vertex_value_t const& value(vertex_id v) const
        {
            return get_vertex(v).vertex_value();
        }
        edge_value_t& value(edge_id e)
        {
            return get_edge(e).edge_value();
        }
        edge_value_t const& value(edge_id e) const
        {
            return get_edge(e).edge_value();
        }

        vertex_id first_vertex() const
        {
            return vertex_id{vertex_repository.first_stock()};
        }

        bool vertex_end(vertex_id id) const
        {
            return vertex_repository.stock_end(id.id());
        }

        vertex_id next_vertex(vertex_id id) const
        {
            return vertex_repository.next_stock(id.id());
        }

    private:

        vertex_t& get_vertex(vertex_id v)
        {
            return vertex_repository.get_stock(v.id());
        }
        vertex_t const& get_vertex(vertex_id v) const
        {
            return vertex_repository.get_stock(v.id());
        }
        edge_t& get_edge(edge_id e)
        {
            return edge_repository.get_stock(e.id());
        }
        edge_t const& get_edge(edge_id e) const
        {
            return edge_repository.get_stock(e.id());
        }

        repository<vertex_t> vertex_repository;
        repository<edge_t> edge_repository;
    };


    template<class VertexValueT, class EdgeValueT>
    class undirected_graph : public graph_base<VertexValueT, EdgeValueT>
    {
    public:
        using base = graph_base<VertexValueT, EdgeValueT>;
        using vertex_t = detail::vertex<VertexValueT>;
        using vertex_value_t = typename vertex_t::vertex_value_t;
        using edge_ref_t = typename detail::edge_ref;
        using edge_t = detail::edge<EdgeValueT>;
        using edge_value_t = typename edge_t::edge_value_t;
        using pair_t = typename edge_t::pair_t;

        bool adjacent(vertex_id x, vertex_id y) const
        {
            if (base::get_vertex(x).associated_edges().size() <
                base::get_vertex(y).associated_edges().size())
                return base::get_vertex(x).adjacent(y);
            else
                return base::get_vertex(y).adjacent(x);
        }

        void remove_vertex(vertex_id v)
        {
            auto const& edges = base::get_vertex(v).associated_edges();
            for (edge_ref_t e : edges)
            {
                base::get_vertex(e.opposite_vertex()).remove_associated_edge(e);
                base::edge_repository.remove_stock(e.associated_edge().id());
            }
            base::vertex_repository.remove_stock(v.id());
        }

        template<class ...Args>
        edge_id add_edge(vertex_id x, vertex_id y, Args&& ...args)
        {
            edge_id result = base::edge_repository.add_stock(std::forward<Args>(args)...);
            base::get_edge(result).set_associated_vertices(x, y);
            base::get_vertex(x).add_associated_edge({result, y});
            base::get_vertex(y).add_associated_edge({result, x});
            return result;
        }
        void remove_edge(vertex_id x, vertex_id y)
        {
            remove_edge(get_edge(x, y));
        }
        void remove_edge(edge_id e)
        {
            pair_t vertices = base::get_edge(e).associated_vertices();
            base::get_vertex(vertices.first).remove_associated_edge({e, vertices.second});
            base::get_vertex(vertices.second).remove_associated_edge({e, vertices.first});
            base::edge_repository.remove_stock(e.id());
        }

        edge_id get_edge(vertex_id x, vertex_id y) const
        {
            assert(adjacent(x, y));
            if (base::get_vertex(x).associated_edges().size() <
                base::get_vertex(y).associated_edges().size())
                return base::get_vertex(x).associated_edge(y);
            else
                return base::get_vertex(y).associated_edge(x);
        }
    };


    template<class VertexValueT, class EdgeValueT>
    class directed_graph : public graph_base<VertexValueT, EdgeValueT>
    {
    public:
        using base = graph_base<VertexValueT, EdgeValueT>;
        using vertex_t = detail::vertex<VertexValueT>;
        using vertex_value_t = typename vertex_t::vertex_value_t;
        using edge_ref_t = typename detail::edge_ref;
        using edge_t = detail::edge<EdgeValueT>;
        using edge_value_t = typename edge_t::edge_value_t;
        using pair_t = typename edge_t::pair_t;


        bool adjacent(vertex_id x, vertex_id y) const
        {
            return base::get_vertex(x).adjacent(y);
        }

        void remove_vertex(vertex_id v)
        {
            auto const& edges = base::get_vertex(v).associated_edges();
            for (edge_ref_t e : edges)
            {
                base::edge_repository.remove_stock(e.associated_edge().id());
            }
            base::vertex_repository.remove_stock(v.id());
        }

        template<class ...Args>
        edge_id add_edge(vertex_id x, vertex_id y, Args&& ...args)
        {
            edge_id result = base::edge_repository.add_stock(std::forward<Args>(args)...);
            base::get_edge(result).set_associated_vertices(x, y);
            base::get_vertex(x).add_associated_edge({result, y});
            return result;
        }
        void remove_edge(vertex_id x, vertex_id y)
        {
            remove_edge(get_edge(x, y));
        }
        void remove_edge(edge_id e)
        {
            pair_t vertices = base::get_edge(e).associated_vertices();
            base::get_vertex(vertices.first).remove_associated_edge({e, vertices.second});
            base::edge_repository.remove_stock(e.id());
        }

        edge_id get_edge(vertex_id x, vertex_id y) const
        {
            assert(adjacent(x, y));
            return base::get_vertex(x).associated_edge(y);
        }
    };
}


#endif // GRAPH_H_INCLUDED
