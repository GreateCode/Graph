# Graph
这个项目提供 数据结构--[图](https://en.wikipedia.org/wiki/Graph_(discrete_mathematics)) 的一个实现,同时提供一部分图的算法.它被设计为在安全与效率中取得平衡.

目前源文件为./test/test.cpp ,头文件在./include下.

该项目目前有一个表示无向图的模板类:lzhlib::undirected_garph.

该类接受两个模板参数,分别是图中顶点和边存储的数据的类型.

例:

    lzhlib::undirected_graph<std::string, std::string> graph;
    vertex_id v0 = g.add_vertex("v0");
    vertex_id v1 = g.add_vertex("v1");
    {
        edge_id e0 = g.add_edge(v0, v1);
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
        edge_id e1 = g.add_edge(v0, v1, "e1");
        assert(g.value(e1) == "e1");
        assert(g.adjacent(v0, v1));
        assert(g.adjacent(v1, v0));
        assert(g.get_edge(v0, v1) == e1);
         assert(g.get_edge(v1, v0) == e1);

        g.remove_edge(e1);
        assert(!g.adjacent(v0, v1));
        assert(!g.adjacent(v1, v0));
    }


要求:

使用GCC 支持C++14 或以上的编译器.

环境变量Path包含Graph所在的父目录的路径.(否则可能找不到头文件)

贡献:

期望各位为此供献出自己的力量.更多信息参见 [CONTRIBUTING](./CONTRIBUTING.md)和 [TODO](./TODO.md).
