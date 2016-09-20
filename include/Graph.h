#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include "Graph/include/repository.h"
#include <vector>
#include <set>
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
	namespace exceptions
	{
		class require_edge_that_does_not_exist;
	}
	class vertex_id
	{
		friend struct invalid_vertex;
		friend class exceptions::require_edge_that_does_not_exist;
	private:
		vertex_id(unsigned long long i)
			: id_ {i}
		{}
		vertex_id(stock_id i)
			: id_(i)
		{}
	public:
		vertex_id() = default;
		bool operator<(vertex_id rhs) const
		{
			return id_ < rhs.id_;
		}
		bool operator==(vertex_id rhs) const
		{
			return id_ == rhs.id_;
		}
	private:
		auto id() const noexcept
		{
			return id_;
		}
		stock_id id_ {static_cast<unsigned long long>(-1)};    //vector::size_type 为unsigned long long
	};
	struct invalid_vertex
	{
		static constexpr vertex_id id {};
	};
	constexpr vertex_id invalid_vertex_id = invalid_vertex::id;
	class edge_id
	{
	private:
		edge_id() = default;
		edge_id(unsigned long long i)
			: id_ {i}
		{}
		edge_id(stock_id i)
			: id_(i)
		{}
	public:
		bool operator<(edge_id rhs) const
		{
			return id_ < rhs.id_;
		}
		bool operator==(edge_id rhs) const
		{
			return id_ == rhs.id_;
		}
		bool operator!=(edge_id rhs) const
		{
			return !(*this == rhs);
		}
	private:
		auto id() const noexcept
		{
			return id_;
		}
		stock_id id_;
	};

	namespace exceptions
	{
		class require_edge_that_does_not_exist: public std::logic_error
		{
		public:
			require_edge_that_does_not_exist(vertex_id x, vertex_id y)
				: std::logic_error(std::string("require edge that doesn't exist!The vertices ids are ") + std::to_string(x.id().id()) + " and " + std::to_string(y.id().id()) + ".")
			{

			}
		};
		[[noreturn]] void throw_exception_require_edge_that_does_not_exist(vertex_id x, vertex_id y)
		{
			throw exceptions::require_edge_that_does_not_exist(x, y);
		}
	}

	class null_value_tag;
	namespace detail
	{

		template<class VertexValueT = null_value_tag>
		class vertex;                                         //only used by graph_impl
		template <>
		class vertex<null_value_tag>                          //only used by graph_impl
		{

		public:
			using vertex_value_t = null_value_tag;
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

				bool operator<(edge_ref rhs) const
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
//            bool is_associated(edge_id i)
//            {
//                return edges.find(i)!=edges.end();            //a possibly more efficent way :ask edge for this
//            }
			std::set<edge_ref>const& associated_edges() const
			{
				return edges;
			}
			void add_associated_edge(edge_ref i)
			{
				edges.insert(i);
			}
			void remove_associated_edge(edge_ref i)
			{
				edges.erase(i);
			}
		private:
			std::set<edge_ref> edges;
		};
		template <class VertexValueT>
		class vertex: public vertex<null_value_tag>
		{
		public:
			using vertex_value_t = VertexValueT;
			template<class ...Args>
			vertex(Args&&... args)
				: value(std::forward<Args>(args)...)
			{

			}
			vertex_value_t& vertex_value() &
			{
				return value;
			}
			vertex_value_t&& vertex_value() &&
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

		template <class EdgeValueT = null_value_tag>
		class edge;
		template <>
		class edge<null_value_tag>
		{
		public:
			using edge_value_t = null_value_tag;
			using pair_t = std::pair<vertex_id, vertex_id>;
			bool is_associated(vertex_id i) const
			{
				return i == vertices.first || i == vertices.second;
			}
			pair_t get_associated_vertices() const
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
			pair_t vertices;
		};
		template <class EdgeValueT>
		class edge: public edge<null_value_tag>
		{
		public:
			using edge_value_t = EdgeValueT;
			template <class ...Args>
			edge(Args&&...args)
				: value(std::forward<Args>(args)...)
			{
			}
			edge_value_t& edge_value() &
			{
				return value;
			}
			edge_value_t&& edge_value() &&
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


	template <class VertexValueT, class EdgeValueT>
	class undirected_graph
	{
	public:
		using vertex_t = detail::vertex<VertexValueT>;
		using vertex_value_t = typename vertex_t::vertex_value_t;
		using edge_ref_t = typename vertex_t::edge_ref;
		using edge_t = detail::edge<EdgeValueT>;
		using edge_value_t = typename edge_t::edge_value_t;
		using pair_t = typename edge_t::pair_t;

		bool adjacent(vertex_id x, vertex_id y) const
		{
			if(get_vertex(x).associated_edges().size() < get_vertex(y).associated_edges().size())
				return adjacent_impl(x, y);
			else
				return adjacent_impl(y, x);
		}
		std::vector<vertex_id> neighbors(vertex_id v) const                           //不如让用户 auto const& s = r.associated_edges(v);
		{
			//在 for(auto e : s)中
			std::set<edge_ref_t> const& edges = get_vertex(v).get_associated_edges(); //再     vertex_id = e.opposite_vertex();
			std::vector<vertex_id> ret;
			ret.reserve(edges.size());
			for(edge_ref_t e : edges)
			{
				ret.push_back(e.opposite_vertex());
			}
			return ret;
		}
		std::set<edge_ref_t>const& associated_edges(vertex_id v) const
		{
			return get_vertex(v).get_associated_edges();
		}
		template <class ...Args>
		vertex_id add_vertex(Args&&...args)
		{
			return vertex_repository.add_stock(std::forward<Args>(args)...);
		}
		void remove_vertex(vertex_id v)
		{
			std::set<edge_ref_t> const& edges = get_vertex(v).get_associated_edges();
			for(edge_ref_t e : edges)
			{
				get_vertex(e.opposite_vertex()).remove_associated_edge(e);
				edge_repository.remove_stock(e.associated_edge());
			}
			vertex_repository.remove_stock(v);
		}
		template <class ...Args>
		edge_id add_edge(vertex_id x, vertex_id y, Args&& ...args)
		{
			edge_id result = edge_repository.add_stock(std::forward<Args>(args)...);
			get_edge(result).set_associated_vertices(x, y);
			get_vertex(x).add_associated_edge( {result, y});
			get_vertex(y).add_associated_edge( {result, x});
			return result;
		}
		void remove_edge(vertex_id x, vertex_id y)
		{
			remove_edge(get_edge(x, y));
		}
		void remove_edge(edge_id e)
		{
			pair_t vertices = get_edge(e).get_associated_vertices();
			get_vertex(vertices.first).remove_associated_edge( {e, vertices.second});
			get_vertex(vertices.second).remove_associated_edge( {e, vertices.first});
			edge_repository.remove_stock(e.id());
		}

		edge_id get_edge(vertex_id x, vertex_id y) const
		{
			assert(adjacent(x, y));
			if(get_vertex(x).associated_edges().size() < get_vertex(y).associated_edges().size())
				return get_edge_impl(x, y);
			else
				return get_edge_impl(y, x);
		}
		pair_t get_associated_vertices(edge_id e) const
		{
			return get_edge(e).get_associated_vertices();
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
			return vertex_repository.first_stock();
		}
		bool is_last_vertex(vertex_id id) const
		{
			return vertex_repository.is_last_stock(id);
		}
		vertex_id next_vertex(vertex_id id) const
		{
			return vertex_repository.next_stock(id);
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

		bool adjacent_impl(vertex_id x, vertex_id y) const
		{
			std::set<edge_ref_t> const& edges = get_vertex(x).associated_edges();
			for(edge_ref_t r : edges)
			{
				if(r.is_connected(y))
					return true;
			}
			return false;
		}
		vertex_id get_opposite_vertex(vertex_id v, edge_id e) const
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
		void remove_edge_from(edge_id e, vertex_id v)
		{
			get_vertex(get_opposite_vertex(v, e)).remove_associated_edge(e);
			edge_repository.remove_stock(e);
		}
		edge_id get_edge_impl(vertex_id x, vertex_id y) const
		{
			std::set<edge_ref_t> const& edges = get_vertex(x).associated_edges();
			for(edge_ref_t e : edges)
			{
				if(e.is_connected(y))
					return e.associated_edge();
			}
			exceptions::throw_exception_require_edge_that_does_not_exist(x, y);
		}

		repository<vertex_t> vertex_repository;
		repository<edge_t> edge_repository;
	};
}



#endif // GRAPH_H_INCLUDED
