# 0. 前言
由于前段时间在处理 CPP 字符串时用到了 RapidJSON，这里做一个简单的笔记。首先，在 rapidjson 中，每个 JSON 值都储存为 `Value` 类，而 `Document` 类表示整个文件对象模型，DOM。

# 1. Value 和 Document
## 1.1 查询 Value
用到的字符串为官方提供的文档中的例子，官方文档 [地址](http://rapidjson.org/zh-cn/index.html)。
```json
{
    "hello": "world",
    "t": true ,
    "f": false,
    "n": null,
    "i": 123,
    "pi": 3.1416,
    "a": [1, 2, 3, 4]
}
```
首先将 JSON 字符串解析至 document，将其组织成一棵DOM树。
```cpp
#include "include/rapidjson/document.h"

rapidjson::Document document;
document.Parse(json_string);
```
判断 document 是否具有某个成员，以及判断该成员的类型。
```cpp
// 有 hello 成员
assert(document.HasMember("hello"));

// document["hello"] 的类型为字符串
assert(document["hello"].IsString());
```
其他的类型判断还有诸如`IsBool`、`IsNull`、`IsNumber`、`IsDouble`、`IsArray`等。
## 1.2 查询 Array
`Array` 与 CPP 中的数组类似，因此具有与数组相似的查询方案。
```cpp
// 使用索引
const rapidjson::Value& a = document["a"];
for (rapidjson::SizeType i = 0; i < a.Size(); ++i)  // typedef unsigned SizeType;
{
    printf("[INFO] a[%d] = %d\n", i, a[i].GetInt());
}

// 使用迭代器
for (rapidjson::Value::ConstValueIterator it = a.Begin(); it != a.End(); ++it)
{
    printf("[INFO] %d\n", it->GetInt());
}

// 使用C++11
for (auto& v : a.GetArray())
{
    printf("[INFO] %d\n", v.GetInt());
}
```
## 1.3 查询 Object
```cpp
// 使用迭代器访问Object
static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };
for (rapidjson::Value::ConstMemberIterator it = document.MemberBegin(); it != document.MemberEnd(); ++it)
{
    // GetType返回类型与kTypeNames的定义顺序相对应
    printf("[INFO] Type of member %s is %s.\n", it->name.GetString(), kTypeNames[it->value.GetType()]);
}

// 使用for循环访问Object
for (auto& m : document.GetObject())
{
    printf("[INFO] Type of member %s is %s.\n", m.name.GetString(), kTypeNames[m.value.GetType()]);
}
```
## 1.4 比较两个 Value
可以使用 `==` 或 `!=` 来比较两个 Value，当且仅当两个 Value 的类型及其内容相同时，它们才相等。
```cpp
// 比较两个值，不相同
assert(document["hello"] != document["n"]);

// 比较键和值，相同
assert(document["hello"] == "world");

// 整数比较，相同
assert(document["i"] == 123);

// 浮点数比较，相同
assert(document["pi"] == 3.1416);
```

# 2. 转移语义
在 RapidJSON 中，`Value` 的赋值并不是把源 `Value` 的内容复制到目的 `Value`，而是通过转移实现的。如
```cpp
Value a(123);
Value b(456);

// 转移后a为 Null，b为 456
b = a;  
```

# 3. 修改 Array
前面提到过 RapidJSON 中的 `Array` 与 STL 中的 `vector` 类似，因此有着相似的 API。注意，在调用 `Reverse(SizeType, Allocator&);` 和 `PushBack();` 等时可能涉及内存分配，所以需要一个 allocator。
```cpp
// PushBack
rapidjson::Value a_pushback(rapidjson::kArrayType);
rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

// 循环调用PushBack
for (int i = 0; i < 5; ++i)
{
    a_pushback.PushBack(i, allocator);
}

// 继续添加其他元素，PushBack返回的是 Array 本身，所以可以连续使用；类似于赋值运算符的重载
a_pushback.PushBack("Lua", allocator).PushBack("Mio", allocator);
```

# 4. 修改 Object
`Object` 是键值对的集合，每个键必须为字符串，其常用的方法是 `AddMember();` 和 `RemoveMember();`，其各种重载形式可在官方文档中查看。

# 5. 深复制 Value
```cpp
rapidjson::Value v2(rapidjson::kObjectType);

// 将整个document复制给v2，且document的内容不变
v2.CopyFrom(document, allocator);   
```

# 6. 交换 Value
```cpp
rapidjson::Value a_swap(123);
rapidjson::Value b_swap("hello");
a_swap.Swap(b_swap);
assert(a_swap.IsString());
assert(b_swap.IsInt());
```
无论两个 DOM 多么复杂，交换是在常数时间内进行的。

# 7. 待续......
后面是 RapidJSON 的进阶知识以及高级用法，现在提到的部分可以简单地处理 CPP 中的字符串了。
