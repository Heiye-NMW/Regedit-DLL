# Regedit-DLL 
[[Chinese]](./README.md)   [[English]](./README_English.md)   [[Chinese Traditional]](./README_Traditional.md) 
>使用C语言编写的一个DLL，用于操作注册表，专给易语言（和C语言）调用（因为我觉得易语言的注册表操作不太好用，当然，我写的也不好，以后会继续更新）

## 函数
函数原型

```c
DLLIMPORT int GetRegValue(HKEY hKey, LPCSTR subKey, LPCSTR valueName, LPSTR buffer, LPDWORD bufferSize);
```

参数说明

1. `hKey`:
    - 类型：`HKEY`
    - 描述：根键的句柄，例如`HKEY_CURRENT_USER`或`HKEY_LOCAL_MACHINE`。

2. `subKey`:
    - 类型：`LPCSTR`
    - 描述：子键的路径，例如`"SOFTWARE\\MyApp"`。

3. `valueName`:
    - 类型：`LPCSTR`
    - 描述：要查询的值的名称，例如`"MyValue"`。

4. `buffer`:
    - 类型：`LPSTR`
    - 描述：指向一个缓冲区的指针，用于存储查询到的值的内容。确保缓冲区足够大，以容纳返回的数据。

5. `bufferSize`:
    - 类型：`LPDWORD`
    - 描述：指向一个变量的指针，该变量指定`buffer`的大小（以字节为单位）。函数返回时，此变量将包含实际返回的数据的大小。

返回值

`0`：操作成功。
`1`：键不存在。
`2`：访问被拒绝。
`3`：无效的句柄。
`4`：查询值时发生错误。
`5`：删除值时发生错误。
`7`：设置值时发生错误。

## 演示
### 安装
从 [Releases](https://github.com/Heiye-NMW/Regedit-DLL/releases/) 下载：
`https://github.com/Heiye-NMW/Regedit-DLL/releases/`

### 用法
将DLL文件放入您编译好的可执行文件的目录下，以下是一个C语言示例程序，展示如何调用`GetRegValue`函数：
```C
#include <windows.h>
#include <stdio.h>

// 声明DLL中的函数
typedef int (*GetRegValueFunc)(HKEY, LPCSTR, LPCSTR, LPSTR, LPDWORD);

int main() {
    HINSTANCE hDLL;
    GetRegValueFunc GetRegValue;

    // 加载DLL
    hDLL = LoadLibrary("library-regedit-hy.dll");
    if (!hDLL) {
        printf("Failed to load DLL.\n");
        return 1;
    }

    // 获取函数指针
    GetRegValue = (GetRegValueFunc)GetProcAddress(hDLL, "GetRegValue");
    if (!GetRegValue) {
        printf("Failed to get function address for GetRegValue.\n");
        FreeLibrary(hDLL);
        return 1;
    }

    const char *subKey = "SOFTWARE\\MyApp";
    const char *valueName = "MyValue";
    char buffer[256];
    DWORD bufferSize = sizeof(buffer);

    // 查询注册表值
    int status = GetRegValue(HKEY_CURRENT_USER, subKey, valueName, buffer, &bufferSize);
    switch (status) {
        case 0:
            printf("Queried value: %s\n", buffer);
            break;
        case 1:
            printf("Key does not exist.\n");
            break;
        case 2:
            printf("Access denied.\n");
            break;
        case 3:
            printf("Invalid handle.\n");
            break;
        case 4:
            printf("Error querying value.\n");
            break;
        default:
            printf("Unknown error.\n");
            break;
    }

    // 卸载DLL
    FreeLibrary(hDLL);
    return 0;
}
```
