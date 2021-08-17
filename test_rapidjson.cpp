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

    // 正常返回
    return 0;
}
