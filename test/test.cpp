#include <iostream>
#include <cassert>
#define private public
#include "include\Graph.h"
using namespace std;
using namespace lzhlib;
using namespace lzhlib::detail;

//void test_vertex()
//{
//    vertex<int> v{};
//    edge_id e1{1}, e2{2};
//    v.add_associated_edge(e1);
//    std::set<edge_id> s = v.get_associated_edges();
//    assert(s.size() == 1);
//    assert(s.find(e1) != s.end() && s.find(e2) == s.end());
//
//    v.add_associated_edge(e2);
//    s = v.get_associated_edges();
//    assert(s.size() == 2);
//    assert(s.find(e1) != s.end() && s.find(e2) != s.end());
//
//    v.remove_associated_edge(e1);
//    s = v.get_associated_edges();
//    assert(s.size() == 1);
//    assert(s.find(e1) == s.end() && s.find(e2) != s.end());
//
//    v.remove_associated_edge(e2);
//    s = v.get_associated_edges();
//    assert(s.empty());
//    assert(s.find(e1) == s.end() && s.find(e2) == s.end());
//
//    v.vertex_value() = 1;
//    assert(v.vertex_value() == 1);
//    v.vertex_value() = 2;
//    assert(v.vertex_value() == 2);
//}
//void test_edge()
//{
//    edge<int> e;
//    vertex_id v1{0}, v2{1};
//    e.set_associated_vertices(v1, v2);
//    assert(e.is_associated(v1) && e.is_associated(v2));
//
//    edge<int>::pair_t p1 = e.get_associated_vertices();
//    assert((p1.first == v1 && p1.second == v2) || (p1.first == v2 && p1.first == v1));
//
//    e.detach();
//    p1 = e.get_associated_vertices();
//    assert(p1.first == invalid_vertex_id && p1.second == invalid_vertex_id);
//    assert(!e.is_associated(v1) && !e.is_associated(v2));
//
//    e.edge_value() = 1;
//    assert(e.edge_value() == 1);
//    e.edge_value() = 2;
//    assert(e.edge_value() == 2);
//}
void test_vertex_repository()
{
    using t = traits<int,null_value_tag>;
    repository<vertex<t>> r0;
    auto i0 = r0.add_stock(1);
    assert(r0.get_stock(i0).vertex_value() == 1);

    r0.remove_stock(i0);

    auto i1 = r0.add_stock(2);
//    assert(r0.ids_of_living_stocks.size() == 1);
//    assert(r0.ids_of_reusable_stocks.size() == 0);
    assert(r0.get_stock(i1).vertex_value() == 2);

    auto i2 = r0.add_stock(3);
//    assert(r0.ids_of_living_stocks.size() == 2);
//    assert(r0.ids_of_reusable_stocks.size() == 0);
    assert(r0.get_stock(i1).vertex_value() == 2);
    assert(r0.get_stock(i2).vertex_value() == 3);

    r0.remove_stock(i1);
//    assert(r0.ids_of_living_stocks.size() == 1);
//    assert(r0.ids_of_reusable_stocks.size() == 1);
    assert(r0.get_stock(i2).vertex_value() == 3);

    r0.remove_stock(i2);
//    assert(r0.ids_of_living_stocks.size() == 0);
//    assert(r0.ids_of_reusable_stocks.size() == 2);
}
void test_vertex_edge_and_repository()
{
    using t = traits<string,string>;
    repository<vertex<t>> rv;
    repository<edge<t>> re;

    auto v0 = rv.add_stock("a");
    auto v1 = rv.add_stock("b");
    auto e0 = re.add_stock("a and b");

    rv.get_stock(v0).add_associated_edge(e0);
    rv.get_stock(v1).add_associated_edge(e0);
    re.get_stock(e0).set_associated_vertices(v0, v1);

    assert(re.get_stock(e0).is_associated(v0));
    assert(re.get_stock(e0).is_associated(v1));

    auto const& vector0 = rv.get_stock(v0).get_associated_edges();
    cout<<vector0.size()<<endl;;
    assert(find(vector0.begin(),vector0.end(),e0) != vector0.end());

    auto const& vector1 = rv.get_stock(v1).get_associated_edges();
    assert(find(vector1.begin(),vector1.end(),e0) != vector1.end());
}
class test_undirected_graph
{
public:
    void operator()()
    {
        {
            using t = traits<string,string>;
            undirected_graph<t> g;
            auto v0 = g.add_vertex("v0");
            auto v1 = g.add_vertex("v1");
            {
                auto e0 = g.add_edge(v0, v1);
                assert(g.value(e0) == "");
                g.value(e0) = "e0";
                assert(g.adjacent(v0, v1));
                assert(g.adjacent(v1, v0));
                assert(g.get_edge(v0, v1) == e0);
                assert(g.get_edge(v1, v0) == e0);
                assert(g.value(v0) == "v0");
                assert(g.value(v1) == "v1");
                assert(g.value(e0) == "e0");

                g.remove_edge(e0);
                assert(!g.adjacent(v0, v1));
                assert(!g.adjacent(v1, v0));
            }
            {
                auto e1 = g.add_edge(v0, v1, "e1");
                assert(g.value(e1) == "e1");
                assert(g.adjacent(v0, v1));
                assert(g.adjacent(v1, v0));
                assert(g.get_edge(v0, v1) == e1);
                assert(g.get_edge(v1, v0) == e1);

                g.remove_edge(e1);
                assert(!g.adjacent(v0, v1));
                assert(!g.adjacent(v1, v0));
            }
        }
    }
private:
};
int main()
{
    test_vertex_repository();
    test_vertex_edge_and_repository();
    test_undirected_graph()();
    cout << "Success!成功" << endl;
    return 0;
}
