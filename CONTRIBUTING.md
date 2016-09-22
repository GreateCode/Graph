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
无const时, * 或&向右与标识符贴紧.
例子如下:

    int *p0 = nullptr;
    int const* p1 = nullptr;
    int const* const p2 = nullptr;
    int const* const* p3= nullptr;
    int const i = 0;
    int const& r = i;
