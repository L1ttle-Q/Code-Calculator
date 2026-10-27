# Code-Calculator

递归统计目录中的源码文件、非空行、空行，以及 Python 注释和函数规模的 C++ 命令行工具。

## 环境与构建

需要 Git、CMake 3.16+ 和支持 C++11 的编译器。下面的 Windows 命令使用已加入 PATH 的 MinGW-w64（`g++`、`mingw32-make`）。

```powershell
git clone https://github.com/L1ttle-Q/Code-Calculator.git
cd Code-Calculator
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

Linux/macOS 可将配置命令换为 `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release`。当前 CMake 将程序输出到仓库的 `build/`；使用 Visual Studio 生成器时通常在 `build/Release/`。

## 运行

从仓库根目录执行：

```powershell
# 扫描指定目录；目录含空格时加引号
.\build\calc_code.exe "C:\path\to\source"
# 不传参数会扫描当前工作目录
.\build\calc_code.exe
# 输出结构化格式
.\build\calc_code.exe "C:\path\to\source" json
```

Linux/macOS 对应 `./build/calc_code /path/to/source`。语法为 `calc_code [directory [json]]`，第三个参数只有精确写为 `json` 才启用结构化输出。结构化格式目前键名没有完整加引号，**不能直接当作标准 JSON 解析**。

## 统计范围与含义

| 输出 | 含义 |
| --- | --- |
| `.cpp`、`.c`、`.h`、`.java`、`.py` | 当前识别的五类扩展名；不包含 `.hpp`、`.cu` 等 |
| `line` | 总行数减去空白行数，包含注释行 |
| `blank` | 空行或仅含空格、制表符的行 |
| `comment` | 当前只对 Python 按源码中的简单规则判断 `#` 注释；其他语言为 0 |
| `file` | 对应类型的文件数量，包含递归子目录 |
| `func_line` / `func_cnt` | 当前只分析 Python，按 `def` / `async def` 和缩进估算 |
| 行末四元组 | 函数行数的整数平均值、最大值、最小值、中位数 |

以 `.` 开头的文件和目录会跳过。函数规模是启发式统计，不等价于语言解析器的结果，注释识别也不能正确处理所有字符串和行内注释。

## 最小验证

在仓库之外准备一个只含以下 `example.py` 的小目录，再对该目录运行工具：

```python
# comment

def demo():
    return 1
```

预期 `.py` 行为 `line=3, blank=1, comment=1, file=1, func_line=1, func_cnt=1`；其余类型为 0。本仓库目前没有自动测试模块。

## 目录与限制

- `src/main.cpp`：参数和程序入口；`src/calc_code.cpp`：递归汇总及输出。
- `src/cmd_parser.cpp`：通过系统命令枚举目录；`src/text_parser.cpp`：逐行统计。
- 枚举时会将文件名转为小写。在区分大小写的文件系统上，大写文件名可能无法读取；Unix 枚举命令也不能可靠处理含空格的名称。
- 不会读取 `.gitignore`，也没有循环链接检测。优先扫描独立源码目录，避免把大型构建目录或循环链接纳入统计。
- `Fail to open file` / `Cannot access directory` 表示该部分统计不完整，应先检查路径、权限和大小写。

## 验证记录与许可证

2026-10-26：Windows、MinGW-w64 GCC 8.1、CMake 3.31 下构建通过，以上 Python 小目录示例通过。Linux/macOS 命令未在本轮实测。

使用 [Apache License 2.0](LICENSE)。
