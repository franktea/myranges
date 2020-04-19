#为gcc10的ranges写自定义view


gcc10是对C++20的特性支持得最多的一个编译器，支持包括ranges在内的诸多特性。gcc10的发布日期原本是在今年三四月份，现在看来有所延迟。不过很多特性在trunk中就能用，包括对ranges，我已试用一段时间。

我之前发过一篇ranges的入门文章，当时用的是range-v3库，range-v3是为那些不支持concept的编译器写的，因为要模拟concept，所以编译速度奇慢。gcc10中的ranges是第一个在译器中发布的官方ranges，没有编译速度慢的问题。

gcc10中的ranges头文件只有3000多行代码，包含了C++20中所规定的一些range/view类。ranges的目标是提供一套ranges/views/adaptors设施，让程序员用这些标准设施进行组合，来完成各种需求。用这种风格来写代码，可以省掉很多循环和边界判断，省时省力。所以ranges被包括C++之父在内的人称为下一代STL，其重要性可想而知。

有时候标准库中提供的ranges设施不够，需要程序员编写自己的view。可惜C++20连标注库中提供的view就太少，包括cycle、zip这样常见的都没有。本文就以尝试定义这两个东东，来演示自定义view的方法。

干支纪年法由10天干和12地支轮流组合而来，60年一个轮回。我用这个例子来演示cycle和zip的用法。我要的效果如下：

```
namespace vs = std::ranges::views;

int main() {
    vector v1 {"甲", "乙", "丙", "丁", "戊", "己", "庚", "辛", "壬", "癸"};
    vector v2 {"子", "丑", "寅", "卯", "辰", "巳", "午", "未", "申", "酉", "戌", "亥"};

    auto a = v1 | vs::cycle;
    auto b = v2 | vs::cycle;
    auto c = vs::zip(a, b) | vs::take(60);

    for(auto&& [x, y]: c)
    	cout<<x<<y<<" ";
    cout<<"\n";

    return 0;
}
```
上面代码代码中的cycle和zip就是本文要实现的自定义view(准确地说是view adaptor)。

自定义view大致分为四步：

1. 定义一个view类从`view_base`继承，标准库中定义了`view_base`的子类`view_interface`，直接从`view_interface`继承可以省一些事情；
2. 为view类定义iterator和setinel，实现`*`和`++`之类的运算符；
3. 为view定义begin()和end()函数；
4. 为view定义adaptor。

STL的标准库的编码风格有很多下划线，这种风格是故意的，叫做代码丑化(uglification)，例如以`_M_`开头的表示类成员，`_大写字母开头加小写字母`表示类名或者模板参数类型名，`__小写字母`表示局部变量，等等。这种风格刚开始看起来真是很丑，我尝试了一下，其实还好。

丑没关系，只要丑出风格丑出水平，那就是牛的东西。

先来看`cycle_view`吧，就是想要一个序列的无限循环，到了最后一个元素以后又绕回来到第一个。实现它的方法就是在iterator的`++`中做一个判断即可。可以这样来实现：

```
constexpr _Iterator& operator++()
{
	++_M_current;
	if(_M_current == ranges::end(_M_parent->_M_base)) // 到最后了，将迭代器放到第一个去
		_M_current = ranges::begin(_M_parent->_M_base);

	return *this;
}
```

`zip_view`没有什么特别的，就是在要保存两个序列的指针，每次要返回两个值，我用pair返回。

实现view以后需要定义adaptor，gcc10中定义adaptor很简单，只要定一个lambda即可。对于构造函数只有一个参数的view，返回`__adaptor::_RangeAdaptorClosure`类型；对于构造函数参数在一个以上的view，返回`__adaptor::_RangeAdaptor`类型。本文中的两个view刚好演示这两种情况。以`cycle_view`为例，只要如下定义即可：

```
template <input_range _Range>
cycle_view(_Range &&) -> cycle_view<views::all_t<_Range>>;

namespace views {
	inline constexpr __adaptor::_RangeAdaptorClosure cycle
	= [] <viewable_range _Range> (_Range&& __r)
	{
		return cycle_view { std::forward<_Range>(__r) };
	};
}
```

按照gcc10的ranges头文件中其它view的代码，来实现自己想要的view的功能非常简单。但是如果要优化，还是有很多事情要做的，比如需要实现更多的运算符；为了提高运行效率，需要对各种concept来进行重载；为了提高拷贝速度，要考虑如何减少成员变量。总之，自己动手实现一遍才会有更深的体会。[我的代码在这里](https://github.com/franktea/myranges)。

有兴趣的快自己试试吧。毕竟，ranges的时代要来了。

