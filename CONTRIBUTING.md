#Contributing to this repository

首先,感谢对这个项目进行贡献的人.

###简介
Graph是一个强调封装与资源安全的图的实现.用户不必为了使用这个图而显式地动态分配内存.

用户也难以直接得到图内部的结构的指针从而做出一些令人意想不到的事情.

例如:

    directed_graph<std::string, std::string> graph;
    vertex_id v0 = graph.add_vertex();
    vertex_id v1 = graph.add_vertex();
    edge_id e0 = graph.add_edge(v0,v1);
    graph.value(v0) = "v0";
    graph.vaue(e0) = "e0";
    graph.remove_vertex(v1);

用户完全通过`vertex_id`和`edge_id`来和图进行交互.用户也没有访问权限来*非拷贝/移动地创建*和修改`vertex_id`和`edge_id`类型的对象.这将有助于封装.
###贡献方式
* 提出改进意见(欢迎!).
* 报告bug.
* 提出增强功能.
* 重构.
* 实现新的功能.
* 修补bug(万分感激).

###编码规范
以下规则只是建议各位遵守,若无暇顾及或觉得繁琐也可无视掉.
若觉得有不妥之处请联系我.
#####命名
默认的命名一律采用字母/数字+下划线的方式.建议字母均用小写.
特殊情况:模板形参一律采用驼峰命名法,单词首字母大写,其余字母小写,不使用下划线.
#####缩进
内层代码应当缩进一个4个空格.
#####花括号
所有scope的起始花括号应当放在独立的新的一行,与上一行对齐;下一行开始内层代码(缩进4个空格).
scope的结束花括号应当放在独立的新的一行,与对应的起始花括号对齐.

    void func()
    {
      std::cout << "This is code.\n";
    }

#####类
类的所有类型别名的声明均放在类成员的最前面,并使用alias-declaration(using关键字)而不是typedef declaration.
其余的成员按 public protected private的顺序出现,其中成员变量尽量放在成员函数之后.
#####指针,引用与const
最底层的const放置在右侧.
有const时,const右侧的 * 或&向左与const贴紧.
无const时, * 或&靠拢方向任意.
例子如下:

    int *p0 = nullptr;
    int const* p1 = nullptr;
    int const* const p2 = nullptr;
    int const* const* p3= nullptr;
    int const i = 0;
    int const& r = i;
