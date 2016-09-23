
#ifndef GRAPH_DIRECTED_GRAPH_H_H
#define GRAPH_DIRECTED_GRAPH_H_H

#include "Graph/include/graph_base.h"

namespace lzhlib
{
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
                base::edge_repository.remove_stock(base::to_stock_id(e.edge()));
            }
            base::vertex_repository.remove_stock(base::to_stock_id(v));
        }

        template<class ...Args>
        edge_id add_edge(vertex_id x, vertex_id y, Args&& ...args)
        {
            edge_id result = base::to_edge_id(base::edge_repository
                                                  .add_stock(std::forward<Args>(args)...));
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
            base::get_vertex(vertices.first).remove_associated_edge(e);
            base::edge_repository.remove_stock(base::to_stock_id(e));
        }

        edge_id get_edge(vertex_id x, vertex_id y) const
        {
            assert(adjacent(x, y));
            return base::get_vertex(x).associated_edge(y);
        }
    };
}
#endif //GRAPH_DIRECTED_GRAPH_H_H
