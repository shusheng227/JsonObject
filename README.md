该项目主要提供一个简单的 Json 辅助类，可以便捷地生成 Json 字符串。

以下面的 Json 字符串为例：

```json
{
    "name": "John Doe",
    "age": 43,
    "address": {
        "street": "10 Downing Street",
        "city": "London"
    },
    "phones": [
        "+44 1234567",
        "+44 2345678"
    ]
}
```

使用 `JsonValue` 可以这样生成：

```c++
#include "json.h"

int main() {
    JsonValue address;
    address["street"] = JsonValue("10 Downing Street");
    address["city"] = JsonValue("London");

    JsonValue root;
    root["name"] = JsonValue("John Doe");
    root["age"] = JsonValue(43);
    root["address"] = address;
    root["phones"] = JsonValue({JsonValue("+44 1234567"), JsonValue("+44 2345678")});

    std::cout << root.ToString() << std::endl;
}
```

