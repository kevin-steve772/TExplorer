# AGENTS.md

本文件面向参与本仓库开发的 Agent。先读本文件，再按需阅读 `README.md` 和源码。

## 项目概述

TExplorer 是一个 C++17 终端文件管理器，界面和操作方式参考 Windows 资源管理器。
支持目录浏览、标签页、面包屑导航、文件操作、回收站、鼠标操作、排序、隐藏文件、
窄终端自适应布局，以及 Linux / macOS / Windows 平台。

- Linux：ncursesw
- macOS：系统 curses
- Windows：PDCursesMod（宽字符 + UTF-8，通过 CMake FetchContent 获取）

## 依赖检查

不要假设依赖已经安装。开始构建前先自行检查当前环境，不要仅根据文档推断。

Linux / macOS：

```bash
cmake --version
c++ --version
pkg-config --version
pkg-config --exists ncursesw && echo "ncursesw found"
```

Windows：

```powershell
cmake --version
where.exe cl
where.exe g++
where.exe ninja
```

平台依赖：

- 所有平台：C++17 编译器、CMake >= 3.16
- Linux：ncursesw；pkg-config 可选，找不到时 CMake 会回退到 `find_package(Curses)`
- macOS：系统 curses；pkg-config 可选
- Windows：MinGW-w64 或 MSVC；首次构建需要网络下载 PDCursesMod
- Linux 普通文件编辑：`nano`
- 安装脚本：`bash`、`curl`；`install.sh` 还需要 `jq` 或 `python3`

如果依赖缺失，先说明缺少什么，再决定是否安装；不要静默修改系统环境。

## 目录结构

```text
.
├── CMakeLists.txt
├── README.md
├── AGENTS.md
├── install.sh
├── uninstall.sh
├── install.ps1
├── uninstall.ps1
└── src
    ├── main.cpp
    ├── platform.h
    └── platform.cpp
```

- `src/main.cpp`：UI、事件循环、布局、绘制、鼠标、标签页、文件操作、对话框。
- `src/platform.h`：平台抽象接口。
- `src/platform.cpp`：Linux / macOS / Windows 平台实现。
- `CMakeLists.txt`：跨平台构建配置。
- `README.md`：面向用户的文档；行为变化时同步更新。
- `install.sh` / `uninstall.sh`：Linux 安装与卸载。
- `install.ps1` / `uninstall.ps1`：Windows 安装与卸载。

## 架构要点

### App 类

`App` 定义在 `src/main.cpp`，核心成员和方法：

- 状态：`current`、`entries`、`selected`、`cursor`、`scroll`、`tabs`、`clipboard`。
- 标签页：`TabState` 保存每个标签的路径、历史、选择、光标、滚动位置和排序状态。
- 布局：`computeLayout()` 计算工具栏、地址栏、表头、列表、侧边栏、状态栏的矩形。
- 绘制：`draw()` 调用 `drawTabs()`、`drawAddress()`、`drawSidebar()`、`drawList()`、
  `drawScrollbar()`、`drawStatus()`。
- 鼠标：`handleMouse()` 处理点击、双击、右键、滚轮、悬停和滚动条拖动。
- 键盘：`processKey()` 处理功能键，`processChar()` 处理普通字符和快捷键。
- 操作：`navigate()`、`openCurrent()`、`copySel()`、`paste()`、`deleteSelection()`、
  `moveToTrash()`、`restoreSelection()` 等。
- 对话框：`showDialog()`、`inputDialog()`、`popupMenu()`，各自是独立窗口和事件循环。

### 平台抽象

文件系统、路径、时间、可执行权限、隐藏文件、主目录、回收站、外部打开等
平台相关逻辑应通过 `plat::` 接口实现，不要在主 UI 代码中直接写 `#ifdef _WIN32`。

常用接口：

- `plat::pathToUtf8()` / `plat::pathFromUtf8()` / `plat::joinPath()`
- `plat::statFile()` / `plat::isExecutable()` / `plat::isHidden()`
- `plat::homeDir()` / `plat::trashBaseDir()`
- `plat::openExternal()`
- `plat::charWidth()`、`plat::enableMouseMotion()` / `disableMouseMotion()`

### UTF-8 与终端宽度

项目大量使用中文、emoji 和宽字符。不要用 `std::string::size()` 计算显示宽度。

- 使用 `utf8Cols()` 计算列宽。
- 使用 `utf8Clip()` / `fitCols()` / `fitRight()` / `centerCols()` 截断或填充。
- 使用 `wput()` 写入固定宽度的文本，它会自动填充空格并应用属性。
- 内部路径使用 `fs::path`；与 UI/字符串交互时再转换为 UTF-8。
- 避免在源码中直接拼接设备相关的绝对路径。

### 布局与命中测试

- 所有布局矩形由 `computeLayout()` 统一计算。
- 绘制函数会填充用于点击/悬停的矩形列表，例如 `toolbarButtons`、`tabButtons`、
  `tabCloseButtons`、`crumbRects`、`sidebarRects`。
- 主界面的悬停高亮依赖 `hoverSignature()` 和 `mouseHover()`。
  新增或移动可交互控件时，必须同时更新绘制逻辑和 `hoverSignature()`，
  否则鼠标移动时可能不会重绘，或悬停状态残留。
- 滚动条是单列滑块：非活动 `⏐`，悬停/拖动 `┃`；拖动区域为最后两列。
- 对话框和右键菜单是独立窗口，通过 `REPORT_MOUSE_POSITION` 处理悬停；
  修改它们的按钮/菜单项布局时，同步更新悬停命中逻辑。

### 颜色与主题

- 256 色终端使用深灰主题：
  - `pair 1`：选中项
  - `pair 2`：界面框架
  - `pair 3`：普通悬停
  - `pair 4`：选中项悬停
- 非 256 色终端回退到 `A_REVERSE` / `A_BOLD`。
- 禁用控件不应显示悬停高亮。

### 标签页与全局设置

- 每个标签页独立保存路径、历史、选择、光标、滚动和排序。
- `showHidden`、`showSidebar` 等视图设置是全局的，所有标签共享。
- 新增标签状态字段时，检查 `TabState`、`snapshotTab()`、`saveActiveTab()`、`loadTab()`。

### 回收站

- Linux：遵循 freedesktop.org Trash 规范，位于 `~/.local/share/Trash`
  （可通过 `XDG_DATA_HOME` 修改）。
- Windows：位于 `%LOCALAPPDATA%\TExplorer\Trash`，由程序自行管理，不是系统回收站。
- 回收站中不支持复制、剪切、粘贴、重命名和新建文件夹。
- Linux 的 `uninstall.sh --purge` 只删除 `~/.texplorer/`，不会删除
  `~/.local/share/Trash` 中的回收站数据。

### 打开外部程序

- Linux：普通文件使用 `nano` 编辑，打开前暂停 TUI，退出后恢复；可执行文件直接运行。
- Windows：使用系统默认程序打开，不暂停 TUI。
- 打开外部程序前后要正确处理鼠标模式、终端状态和 `endwin()` / 恢复逻辑。

## 构建与运行

Linux / macOS：

```bash
cmake -S . -B build
cmake --build build -j
./build/TExplorer [目录]
```

Windows（MinGW-w64 + Ninja）：

```powershell
cmake -S . -B build -G Ninja
cmake --build build
.\build\TExplorer.exe [目录]
```

Windows（MSVC）：

```powershell
cmake -S . -B build
cmake --build build --config Release
```

无网络时可指定本地 PDCursesMod 源码：

```powershell
cmake -S . -B build -G Ninja -DTEXPLORER_PDCURSES_SOURCE=<相对或本地路径>
cmake --build build
```

注意：PDCursesMod 固定使用一个晚于 v4.5.4 的提交，因为 v4.5.4 的 CMake 文件
依赖 `CMAKE_SOURCE_DIR`，不适合作为子目录/FetchContent 使用。

## 验证

本项目目前没有自动化测试。修改后至少执行：

1. 全量构建，确认无警告：

   ```bash
   cmake --build build --clean-first -j
   ```

2. 基本启动：

   ```bash
   ./build/TExplorer --help
   ```

3. 手动检查受影响功能：
   - 键盘导航、标签页切换/关闭、地址栏和面包屑
   - 文件选择、多选、排序、隐藏文件、侧边栏
   - 右键菜单、对话框、输入框
   - 鼠标点击、双击、滚轮、悬停、滚动条拖动
   - 窄终端和窗口 resize
   - 中文、emoji、宽字符截断
   - 回收站操作
   - Linux 下 `nano` 打开/退出后的 TUI 恢复
   - Windows 下的路径、磁盘列表、默认程序打开（如环境可用）

无头环境下可使用 tmux 做 TUI 验证；tmux 中需要开启鼠标：

```tmux
set -g mouse on
```

测试鼠标时注意：ncurses 的 `MEVENT` 坐标是 0 基，而 X10 协议输入是 1 基。
不要依赖真实鼠标；必要时可用合成事件，但最终应尽量在真实终端中复核。

## 安装与卸载脚本

### Linux

`install.sh`：

- 从 GitHub Release 获取最新版本，或通过 `--version` 指定标签。
- 按 Linux / 架构自动选择资源，也支持 `--asset` 手动指定。
- 下载到 `~/.texplorer/TExplorer`。
- 创建 `/bin/TExplorer` 符号链接；无权限时使用 `sudo`。
- 支持 `GITHUB_TOKEN` / `GH_TOKEN` 避免 API 限流。
- 目前要求 Release 资源是裸可执行文件（ELF 或 shebang 脚本），不支持压缩包。

`uninstall.sh`：

- 删除 `/bin/TExplorer` 和 `~/.texplorer/TExplorer`。
- 仅当符号链接指向本程序时删除；`--force` 可强制删除。
- 目录为空时自动删除；`--purge` 递归删除整个 `~/.texplorer/`。
- `--name` 可匹配自定义链接名。

### Windows

`install.ps1`：

- 从 GitHub Release 下载 `.exe`。
- 安装到 `%LOCALAPPDATA%\Programs\TExplorer`。
- 将安装目录加入当前用户 `PATH`；`-NoPath` 可跳过。
- 支持 `-Version`、`-Asset`、`-Repo`、`-InstallDir`。
- 会检查下载文件是否为 Windows 可执行文件。

`uninstall.ps1`：

- 删除程序和用户 `PATH` 中的安装目录。
- 默认保留回收站数据；`-PurgeData` 删除 `%LOCALAPPDATA%\TExplorer\Trash`。
- 支持 `-InstallDir`。

修改安装目录、链接名、Release 资源命名或卸载行为时，必须同时更新对应平台的
安装/卸载脚本和 `README.md`。

## 修改约定

- 使用 C++17，保持现有代码风格。
- 不引入新依赖，除非确有必要并说明原因。
- 优先使用 `plat::` 抽象，不要把平台逻辑散落在 `main.cpp`。
- 所有显示文本按 UTF-8 处理，使用已有的宽度计算工具。
- 不要用设备相关的绝对路径写文档或示例；使用 `./build`、`~/.texplorer`、
  `%LOCALAPPDATA%` 等相对/平台路径。
- 注释保持克制，只在逻辑不直观时添加。
- 不要主动提交、amend、push 或创建 PR；只有用户明确要求时才执行。
- 修改用户可见行为后，同步更新 `README.md`；修改 Agent 工作方式后，更新本文件。

## 提交前检查

- `git status` 和 `git diff` 确认只包含预期修改。
- 完整构建通过且无警告。
- 至少做一次相关功能的手动验证。
- 确认没有写入设备相关的绝对路径或临时文件。
- 除非用户明确要求，否则不要提交。
