#include <iostream>
#include <cassert>

#define private public

#include "Graph/include/graph.h"

using namespace std;
using namespace lzhlib;
using namespace lzhlib::detail;

void test_vertex()
{
    vertex<int> v{};
    edge_ref e1{1, vertex_id()}, e2{2, vertex_id()};
    v.add_associated_edge(e1);
    auto s = v.associated_edges();
    assert(s.size() == 1);
    assert(s.find(e1) != s.end() && s.find(e2) == s.end());

    v.add_associated_edge(e2);
    s = v.associated_edges();
    assert(s.size() == 2);
    assert(s.find(e1) != s.end() && s.find(e2) != s.end());

    v.remove_associated_edge(e1.edge());
    s = v.associated_edges();
    assert(s.size() == 1);
    assert(s.find(e1) == s.end() && s.find(e2) != s.end());

    v.remove_associated_edge(e2.edge());
    s = v.associated_edges();
    assert(s.empty());
    assert(s.find(e1) == s.end() && s.find(e2) == s.end());

    v.vertex_value() = 1;
    assert(v.vertex_value() == 1);
    v.vertex_value() = 2;
    assert(v.vertex_value() == 2);
}

void test_edge()
{
    edge<int> e;
    vertex_id v1{0}, v2{1};
    e.set_associated_vertices(v1, v2);
    assert(e.is_associated(v1) && e.is_associated(v2));

    edge<int>::pair_t p1 = e.associated_vertices();
    assert((p1.first == v1 && p1.second == v2) || (p1.first == v2 && p1.first == v1));

    e.detach();
    p1 = e.associated_vertices();
    assert(p1.first == invalid_vertex_id && p1.second == invalid_vertex_id);
    assert(!e.is_associated(v1) && !e.is_associated(v2));

    e.edge_value() = 1;
    assert(e.edge_value() == 1);
    e.edge_value() = 2;
    assert(e.edge_value() == 2);
}

void test_vertex_repository()
{
    repository<vertex<int>> r0;
    auto i0 = r0.add_stock(1);
    assert(r0.get_stock(i0).vertex_value() == 1);

    r0.remove_stock(i0);
    try
    {
        r0.get_stock(i0);
    }
    catch (repository<vertex<int>>::attempt_to_use_unassigned_stock e)
    {
        assert(e.what() == std::string("Attempt to use unassigned stock which id is ") +
                           std::to_string(i0.id()) + "!");
    }

    auto i1 = r0.add_stock(2);
    assert(r0.ids_of_living_stocks.size() == 1);
    assert(r0.ids_of_reusable_stocks.size() == 0);
    assert(r0.get_stock(i1).vertex_value() == 2);

    auto i2 = r0.add_stock(3);
    assert(r0.ids_of_living_stocks.size() == 2);
    assert(r0.ids_of_reusable_stocks.size() == 0);
    assert(r0.get_stock(i1).vertex_value() == 2);
    assert(r0.get_stock(i2).vertex_value() == 3);

    r0.remove_stock(i1);
    assert(r0.ids_of_living_stocks.size() == 1);
    assert(r0.ids_of_reusable_stocks.size() == 1);
    assert(r0.get_stock(i2).vertex_value() == 3);

    r0.remove_stock(i2);
    assert(r0.ids_of_living_stocks.size() == 0);
    assert(r0.ids_of_reusable_stocks.size() == 2);
}

void test_vertex_edge_and_repository()
{
    repository<vertex<string>> rv;
    repository<edge<string>> re;

    auto v0 = rv.add_stock("a");
    auto v1 = rv.add_stock("b");
    auto e0 = re.add_stock("a and b");

    rv.get_stock(v0).add_associated_edge({e0, v1});
    rv.get_stock(v1).add_associated_edge({e0, v0});
    re.get_stock(e0).set_associated_vertices(v0, v1);

    assert(re.get_stock(e0).is_associated(v0));
    assert(re.get_stock(e0).is_associated(v1));

    auto const& s0 = rv.get_stock(v0).associated_edges();
    assert(s0.find({e0, v1}) != s0.end());

    auto const& s1 = rv.get_stock(v1).associated_edges();
    assert(s1.find({e0, v0}) != s1.end());
}

class test_undirected_graph
{
public:
    void operator()()
    {
        undirected_graph<std::string, std::string> g;
        vertex_id v0 = g.add_vertex("v0");
        vertex_id v1 = g.add_vertex("v1");
        {
            edge_id e0 = g.add_edge(v0, v1);
            assert(g.value(e0) == "");
            assert(g.get_edge(v0, v1) == e0);
            g.value(e0) = "e0";
            assert(g.associated(v0, e0));
            assert(g.associated(v1, e0));
            assert(g.adjacent(v0, v1));
            assert(g.adjacent(v1, v0));
            assert(g.get_edge(v0, v1) == e0);
            assert(g.get_edge(v1, v0) == e0);
            assert(g.value(v0) == "v0");
            assert(const_cast<decltype(g)const&>(g).value(v0) == "v0");
            assert(g.value(v1) == "v1");
            assert(const_cast<decltype(g)const&>(g).value(v1) == "v1");
            assert(g.value(e0) == "e0");
            assert(const_cast<decltype(g)const&>(g).value(e0) == "e0");
            {
                auto vertex = g.neighbors(v0);
                assert(vertex.size() == 1);
                assert(vertex[0] == v1);
            }
            {
                auto vertex = g.neighbors(v1);
                assert(vertex.size() == 1);
                assert(vertex[0] == v0);
            }
            {
                auto edge_refs = g.associated_edges(v0);
                assert(edge_refs.size() == 1);
                assert((*edge_refs.begin()).edge() == e0);
                assert((*edge_refs.begin()).opposite_vertex() == v1);
            }
            {
                auto edge_refs = g.associated_edges(v1);
                assert(edge_refs.size() == 1);
                assert((*edge_refs.begin()).edge() == e0);
                assert((*edge_refs.begin()).opposite_vertex() == v0);
            }
            auto vertices = g.associated_vertices(e0);
            assert(vertices.first == v0 && vertices.second == v1);
            vector<vertex_id> vertices0 = {vertices.first, vertices.second};
            vector<vertex_id> vertices1;
            for (auto v = g.first_vertex(); !g.vertex_end(v); v = g.next_vertex(v))
                vertices1.push_back(v);
            assert(vertices0 == vertices1);

            g.remove_edge(e0);
            assert(!g.associated(v0, e0));
            assert(!g.associated(v1, e0));
            assert(!g.adjacent(v0, v1));
            assert(!g.adjacent(v1, v0));
        }
        {
            edge_id e1 = g.add_edge(v0, v1, "e1");
            assert(g.value(e1) == "e1");
            assert(g.associated(v0, e1));
            assert(g.associated(v1, e1));
            assert(g.adjacent(v0, v1));
            assert(g.adjacent(v1, v0));
            assert(g.get_edge(v0, v1) == e1);
            assert(g.get_edge(v1, v0) == e1);

            g.remove_edge(v0, v1);
            assert(!g.associated(v0, e1));
            assert(!g.associated(v1, e1));
            assert(!g.adjacent(v0, v1));
            assert(!g.adjacent(v1, v0));
        }
        g.remove_vertex(v0);
        g.remove_vertex(v1);
    }

private:
};

void test_directed_graph()
{

    directed_graph<std::string, std::string> g;
    vertex_id v0 = g.add_vertex("v0");
    vertex_id v1 = g.add_vertex("v1");
    {
        edge_id e0 = g.add_edge(v0, v1);
        assert(g.value(e0) == "");
        assert(g.get_edge(v0, v1) == e0);
        g.value(e0) = "e0";
        assert(g.associated(v0, e0));
        assert(!g.associated(v1, e0));
        assert(g.adjacent(v0, v1));
        assert(!g.adjacent(v1, v0));
        assert(g.get_edge(v0, v1) == e0);
        //assert(g.get_edge(v1, v0) != e0);
        assert(g.value(v0) == "v0");
        assert(const_cast<decltype(g)const&>(g).value(v0) == "v0");
        assert(g.value(v1) == "v1");
        assert(const_cast<decltype(g)const&>(g).value(v1) == "v1");
        assert(g.value(e0) == "e0");
        assert(const_cast<decltype(g)const&>(g).value(e0) == "e0");
        {
            auto vertex = g.neighbors(v0);
            assert(vertex.size() == 1);
            assert(vertex[0] == v1);
        }
        {
            auto vertex = g.neighbors(v1);
            assert(vertex.empty());
        }

        {
            auto edge_refs = g.associated_edges(v0);
            assert(edge_refs.size() == 1);
            assert((*edge_refs.begin()).edge() == e0);
            assert((*edge_refs.begin()).opposite_vertex() == v1);
        }
        {
            auto edge_refs = g.associated_edges(v1);
            assert(edge_refs.empty());
        }

        {
            auto vertices = g.associated_vertices(e0);
            assert(vertices.first == v0 && vertices.second == v1);
            vector<vertex_id> vertices0 = {vertices.first, vertices.second};
            vector<vertex_id> vertices1;
            for (auto v = g.first_vertex(); !g.vertex_end(v); v = g.next_vertex(v))
                vertices1.push_back(v);
            assert(vertices0 == vertices1);
        }

        g.remove_edge(e0);
        assert(!g.associated(v0, e0));
        assert(!g.associated(v1, e0));
        assert(!g.adjacent(v0, v1));
        assert(!g.adjacent(v1, v0));
        {
            edge_id e1 = g.add_edge(v0, v1, "e1");
            assert(g.value(e1) == "e1");
            assert(g.adjacent(v0, v1));
            assert(!g.adjacent(v1, v0));
            assert(g.get_edge(v0, v1) == e1);
            //assert(g.get_edge(v1, v0) != e1);

            g.remove_edge(v0, v1);
            assert(!g.associated(v0, e0));
            assert(!g.associated(v1, e0));
            assert(!g.adjacent(v0, v1));
            assert(!g.adjacent(v1, v0));
        }
    }
    g.remove_vertex(v0);
    g.remove_vertex(v1);
}

int main()
{
    test_vertex();
    test_edge();
    test_vertex_repository();
    test_vertex_edge_and_repository();
    test_undirected_graph()();
    test_directed_graph();
    cout << "Success!成功" << endl;
    return 0;
}
