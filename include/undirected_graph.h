
#ifndef GRAPH_UNDIRECTED_GRAPH_H
#define GRAPH_UNDIRECTED_GRAPH_H

#include "Graph/include/graph_base.h"

namespace lzhlib
{

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
                base::get_vertex(e.opposite_vertex()).remove_associated_edge(e.edge());
                base::edge_repository.remove_stock(base::to_stock_id(e.edge()));
            }
            base::vertex_repository.remove_stock(base::to_stock_id(v));
        }

        template<class ...Args>
        edge_id add_edge(vertex_id x, vertex_id y, Args&& ...args)
        {
            edge_id result = base::to_edge_id(base::edge_repository.add_stock(std::forward<Args>(args)...));
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
            base::get_vertex(vertices.first).remove_associated_edge(e);
            base::get_vertex(vertices.second).remove_associated_edge(e);
            base::edge_repository.remove_stock(base::to_stock_id(e));
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
}
#endif //GRAPH_UNDIRECTED_GRAPH_H
