#include "include/rapidjson/document.h"
#include <iostream>


int main(int argc, char** argv)
{
    // JSON字符串，为引号添加转义
    const char* json = "{ \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] }";
    
    // 将JSON字符串解析成一个Document，则其变成了一棵文件对象模型DOM树
    rapidjson::Document document;
    document.Parse(json);

    // 判断document的hello成员
    assert(document.HasMember("hello"));
    
    // 判断document["hello"]的类型为字符串
    // 其他类型判断方法类似如IsBool、IsNull、IsNumber、IsDouble、IsArray等
    assert(document["hello"].IsString());
    printf("[INFO] document[\"hello\"] = %s\n", document["hello"].GetString());
    
    // 使用引用来连续访问Array
    const rapidjson::Value& a = document["a"];
    assert(a.IsArray());
    for (rapidjson::SizeType i = 0; i < a.Size(); ++i)  // typedef unsigned SizeType;
    {
        printf("[INFO] a[%d] = %d\n", i, a[i].GetInt());
    }

    // 使用迭代器访问Array
    for (rapidjson::Value::ConstValueIterator it = a.Begin(); it != a.End(); ++it)
    {
        printf("[INFO] %d\n", it->GetInt());
    }

    // 使用for循环访问Array
    for (auto& v : a.GetArray())
    {
        printf("[INFO] %d\n", v.GetInt());
    }

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

    // 比较两个值，不相同
    assert(document["hello"] != document["n"]);

    // 比较键和值，相同
    assert(document["hello"] == "world");

    // 整数比较，相同
    assert(document["i"] == 123);

    // 浮点数比较，相同
    assert(document["pi"] == 3.1416);

    // 转移语义
    rapidjson::Value a_value(123);
    rapidjson::Value b_value(456);
    b_value = a_value;
    assert(a_value.IsNull());
    assert(b_value == 123);

    // PushBack
    rapidjson::Value a_pushback(rapidjson::kArrayType);
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    // 循环调用PushBack
    for (int i = 0; i < 5; ++i)
    {
        a_pushback.PushBack(i, allocator);
    }
    // 继续添加其他元素
    a_pushback.PushBack("Lua", allocator).PushBack("Mio", allocator);

    // AddMember
    rapidjson::Value contact(rapidjson::kObjectType);
    contact.AddMember("name", "Milo", allocator);
    contact.AddMember("married", true, allocator);

    // RemoveMember
    contact.RemoveMember("name");   // 根据键名移除单个成员
    // contact.RemoveAllMembers();     // 移除所有成员
    // contact.RemoveMember(const rapidjson::Value& name);   // 移除Value

    // CopyForm
    rapidjson::Value v2(rapidjson::kObjectType);
    v2.CopyFrom(document, allocator);   // 将整个document复制给v2
    
    // Swap
    rapidjson::Value a_swap(123);
    rapidjson::Value b_swap("hello");
    a_swap.Swap(b_swap);
    assert(a_swap.IsString());
    assert(b_swap.IsInt());

    // 正常返回
    return 0;
}
