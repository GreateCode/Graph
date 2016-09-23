# Graph
这个项目提供 数据结构--[图](https://en.wikipedia.org/wiki/Graph_(discrete_mathematics)) 的一个实现,同时提供一部分图的算法.

Graph强调封装与资源安全.

目前测试文件在./test下 ,头文件在./include下.
###接口简介
该项目目前有两个表示图的模板类:`lzhlib::undirected_graph`和`lzhlib::directed_graph`,分别表示无向图和有向图.
####`lzhlib::undirected_garph`.
这是一个表示无向图的模板类.

该类接受两个模板参数,分别是图中顶点和边存储的数据的类型.

例:

    undirected_graph<std::string, std::string> g;
    vertex_id v0 = g.add_vertex("v0");
    vertex_id v1 = g.add_vertex("v1");
    {
        edge_id e0 = g.add_edge(v0, v1,"e0");
        assert(g.get_edge(v0, v1) == e0);
        assert(g.associated(v0, e0));
        assert(g.associated(v1, e0));
        assert(g.adjacent(v0, v1));
        assert(g.adjacent(v1, v0));
        assert(g.get_edge(v0, v1) == e0);
        assert(g.get_edge(v1, v0) == e0);
        assert(g.value(v0) == "v0");
        assert(g.value(v1) == "v1");
        assert(g.value(e0) == "e0");
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
            auto vertices = g.associated_vertices(e0);
            assert(vertices.first == v0 && vertices.second == v1);
            vector<vertex_id> vertices0 = {vertices.first, vertices.second};
            vector<vertex_id> vertices1;
            for (auto v = g.first_vertex(); !g.vertex_end(v); v = g.next_vertex(v))
                vertices1.push_back(v);
            assert(vertices0 == vertices1);
        }
        g.remove_edge(e0);//可选
        assert(!g.associated(v0, e0));
        assert(!g.associated(v1, e0));
        assert(!g.adjacent(v0, v1));
        assert(!g.adjacent(v1, v0));
    }
    g.remove_vertex(v0);//可选
    g.remove_vertex(v1);//可选

####`directed_graph`
这个模板类表示有向图.

模板参数的意义与`undirected_graph`的相同.

例:

    directed_graph<std::string, std::string> g;
    vertex_id v0 = g.add_vertex("v0");
    vertex_id v1 = g.add_vertex("v1");
    {
        edge_id e0 = g.add_edge(v0, v1);        //不提供值初始化std::string(可以选择有)
        assert(g.value(e0) == "");              //因而e0关联的值为空字符串
        assert(g.get_edge(v0, v1) == e0);
        assert(g.associated(v0, e0));
        assert(!g.associated(v1, e0));
        assert(g.adjacent(v0, v1));
        assert(!g.adjacent(v1, v0));
        assert(g.get_edge(v0, v1) == e0);
        assert(g.value(v0) == "v0");
        assert(g.value(v1) == "v1");
        g.value(e0) = "e0";
        assert(g.value(e0) == "e0");
        {
            auto vertices = g.neighbors(v0);
            assert(vertices.size() == 1);
            assert(vertices[0] == v1);
        }
        {
            auto vertices = g.neighbors(v1);
            assert(vertices.empty());
        }

        {
            auto vertices = g.associated_vertices(e0);
            assert(vertices.first == v0 && vertices.second == v1);
            vector<vertex_id> vertices0 = {vertices.first, vertices.second};
            vector<vertex_id> vertices1;
            for (auto v = g.first_vertex(); !g.vertex_end(v); v = g.next_vertex(v)) //遍历所有vertex
                vertices1.push_back(v);
            assert(vertices0 == vertices1);
        }
        g.remove_edge(e0);  //可选
        assert(!g.associated(v0, e0));
        assert(!g.associated(v1, e0));
        assert(!g.adjacent(v0, v1));
        assert(!g.adjacent(v1, v0));
    }
    g.remove_vertex(v0);    //可选
    g.remove_vertex(v1);    //可选

###要求:

* 使用GCC 支持C++14 或以上的编译器,并添加编译选项-std=c++14或更高.

* 环境变量C_PLUS_INCLUDE_PATH 值:Graph文件夹所在的父目录的路径.(否则可能找不到头文件)  
  或者使用GCC编译时添加-I选项后接Graph文件夹所在父目录的路径.



###贡献:

期望各位为此供献出自己的力量.更多信息参见 [CONTRIBUTING](./CONTRIBUTING.md)和 [TODO](./TODO.md).
