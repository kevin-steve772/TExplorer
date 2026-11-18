#include "i18n.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

namespace i18n {
namespace {

Lang g_lang = Lang::SimplifiedChinese;

struct Entry {
    Key key;
    const char* value[4];
};

const Entry kEntries[] = {
    {Key::Usage, {
        "Usage: %s [options] [directory]\n",
        "用法: %s [选项] [目录]\n",
        "用法: %s [選項] [目錄]\n",
        "用法: %s [选项] [目录]（给终端一点方向）\n",
    }},
    {Key::AppDescription, {
        "TExplorer - a simple TUI file explorer (mouse supported).\n",
        "TExplorer - 简易 TUI 文件资源管理器（支持鼠标）。\n",
        "TExplorer - 簡易 TUI 檔案總管（支援滑鼠）。\n",
        "TExplorer - 会动的终端文件管理器，鼠标也能用；sudo 不负责你的选择。\n",
    }},
    {Key::HelpOption, {
        "  -h, --help                Show this help\n",
        "  -h, --help                显示此帮助\n",
        "  -h, --help                顯示此說明\n",
        "  -h, --help                看说明，比 man 短\n",
    }},
    {Key::LangOption, {
        "  --lang <en|zh|zh_TW|meme> Set interface language\n",
        "  --lang <en|zh|zh_TW|meme> 设置界面语言\n",
        "  --lang <en|zh|zh_TW|meme> 設定介面語言\n",
        "  --lang <en|zh|zh_TW|meme> 切换语言，不用重开终端\n",
    }},
    {Key::LangRequired, {
        "Missing value for --lang\n",
        "--lang 缺少参数\n",
        "--lang 缺少參數\n",
        "--lang 少了参数，终端在等\n",
    }},
    {Key::UnknownLanguage, {
        "Unknown language: %s\n",
        "未知语言: %s\n",
        "未知語言: %s\n",
        "这是你自创的语言吗？: %s\n",
    }},
    {Key::ErrorPrefix, {
        "Error: %s\n",
        "错误: %s\n",
        "錯誤: %s\n",
        "牌有问题！: %s\n",
    }},

    {Key::Ok, {"OK", "确定", "確定", "好喔"}},
    {Key::Cancel, {"Cancel", "取消", "取消", "算了"}},
    {Key::Yes, {"Yes", "是", "是", "冲了"}},
    {Key::No, {"No", "否", "否", "不要"}},
    {Key::Unknown, {"Unknown", "未知", "未知", "谁知道呢"}},

    {Key::Home, {"Home", "主页", "主目錄", "家目录（回家，回家...）"}},
    {Key::Desktop, {"Desktop", "桌面", "桌面", "桌面（鼠标的停机坪）"}},
    {Key::Documents, {"Documents", "文档", "文件", "文件（不是 /dev）"}},
    {Key::Downloads, {"Downloads", "下载", "下載", "下载（你塞了多少东西进去？）"}},
    {Key::Pictures, {"Pictures", "图片", "圖片", "图片（好康的东西~）"}},
    {Key::Music, {"Music", "音乐", "音樂", "音乐（耳朵的数据流）"}},
    {Key::Videos, {"Videos", "视频", "影片", "视频（会动的像素）"}},
    {Key::Root, {"Root", "根目录", "根目錄", "根目录（`/`，不是 tree 的根）"}},
    {Key::Trash, {"Trash", "回收站", "資源回收筒", "垃圾桶（文件的黑洞）"}},
    {Key::QuickAccess, {"Quick Access", "快速访问", "快速存取", "快捷方式（不是符号链接）"}},

    {Key::ColName, {"Name", "名称", "名稱", "名字"}},
    {Key::ColDate, {"Date modified", "修改日期", "修改日期", "上次动过"}},
    {Key::ColType, {"Type", "类型", "類型", "什么玩意"}},
    {Key::ColSize, {"Size", "大小", "大小", "磁盘红温程度"}},

    {Key::SortByName, {"Sort by name", "按名称排序", "依名稱排序", "照名字排，像 ls"}},
    {Key::SortByDate, {"Sort by date", "按日期排序", "依日期排序", "照时间排，像 ls -lt"}},
    {Key::SortByType, {"Sort by type", "按类型排序", "依類型排序", "照类型排，像 file"}},
    {Key::SortBySize, {"Sort by size", "按大小排序", "依大小排序", "照大小排，像 du"}},

    {Key::Folder, {"Folder", "文件夹", "資料夾", "文件夹（cd进去康康？）"}},
    {Key::File, {"File", "文件", "檔案", "文件（inode 的肉体）"}},
    {Key::Shortcut, {"Shortcut", "快捷方式", "捷徑", "符号链接（传送门）"}},
    {Key::FolderShortcut, {"Folder shortcut", "文件夹快捷方式", "資料夾捷徑", "文件夹符号链接（传送门）"}},
    {Key::TypeText, {"Text document", "文本文档", "文字文件", "文字文件（没有 markdown 语法）"}},
    {Key::TypeMarkdown, {"Markdown document", "Markdown 文档", "Markdown 文件", "Markdown（README 的语言）"}},
    {Key::TypePDF, {"PDF document", "PDF 文档", "PDF 文件", "PDF（打印机的梦魇）"}},
    {Key::TypeWord, {"Word document", "Word 文档", "Word 文件", "Word 文件（排版灾难）"}},
    {Key::TypeExcel, {"Excel worksheet", "Excel 工作表", "Excel 工作表", "Excel 工作表（会计的 Excel）"}},
    {Key::TypePowerPoint, {"PowerPoint presentation", "PowerPoint 演示文稿", "PowerPoint 簡報", "力量的点（?）"}},
    {Key::TypeImage, {"Image", "图像", "圖片", "图像（好康的~）"}},
    {Key::TypeVideo, {"Video", "视频", "影片", "视频（会动的像素）"}},
    {Key::TypeAudio, {"Audio", "音频", "音訊", "音频（耳朵的数据流）"}},
    {Key::TypeArchive, {"Archive", "压缩文件", "壓縮檔", "压缩文件（tar 的亲戚）"}},
    {Key::TypeCSource, {"C source", "C 源文件", "C 原始碼", "C 源码（野指针重灾区）"}},
    {Key::TypeCHeader, {"C header", "C 头文件", "C 標頭檔", "C 头文件（#include 的入口）"}},
    {Key::TypeCppSource, {"C++ source", "C++ 源文件", "C++ 原始碼", "C++ 源码（编译器会哭）"}},
    {Key::TypeCppHeader, {"C++ header", "C++ 头文件", "C++ 標頭檔", "C++ 头文件（template 地狱）"}},
    {Key::TypePython, {"Python source", "Python 源文件", "Python 原始碼", "Python 源码（缩进决定命运）"}},
    {Key::TypeJavaScript, {"JavaScript file", "JavaScript 文件", "JavaScript 檔案", "JS 文件（undefined 的故乡）"}},
    {Key::TypeTypeScript, {"TypeScript file", "TypeScript 文件", "TypeScript 檔案", "TS 文件（JavaScript 加类型止痛药）"}},
    {Key::TypeShell, {"Shell script", "Shell 脚本", "Shell 指令碼", "Shell 脚本（rm -rf 的温床）"}},
    {Key::TypeJSON, {"JSON file", "JSON 文件", "JSON 檔案", "JSON 文件（少一个逗号就再见）"}},
    {Key::TypeHTML, {"HTML document", "HTML 文档", "HTML 文件", "HTML 文档（标签汤）"}},
    {Key::TypeStylesheet, {"Stylesheet", "样式表", "樣式表", "样式表（CSS 的诅咒）"}},
    {Key::TypeApplication, {"Application", "应用程序", "應用程式", "应用程序（当心rm -rf！）"}},
    {Key::TypeDiskImage, {"Disk image", "光盘映像", "光碟映像", "光盘映像（不是 ISO 9001）"}},
    {Key::TypeGeneric, {"%s file", "%s 文件", "%s 檔案", "%s 文件（我不知道啊？）"}},

    {Key::TerminalTooSmall, {
        "Terminal window is too small; please resize.",
        "终端窗口太小，请调整大小。",
        "終端機視窗太小，請調整大小。",
        "终端太小，连 htop 都塞不下，请拉大。",
    }},
    {Key::NewTabStatus, {"New tab", "新建标签页", "新增分頁", "开新分页，像 tmux new-window"}},
    {Key::CloseTabStatus, {"Tab closed", "已关闭标签页", "已關閉分頁", "分页关了，像 tmux kill-pane"}},
    {Key::EmptyFolder, {"This folder is empty", "此文件夹为空", "此資料夾是空的", "欸？我的东西呢？"}},
    {Key::StatusTotal, {"%zu %s", "共 %zu %s", "共 %zu %s", "目前有 %zu %s"}},
    {Key::StatusSelected, {", %zu %s selected", "，已选择 %zu %s", "，已選取 %zu %s", "，你选了 %zu %s"}},
    {Key::StatusSelectedSize, {" (%s)", "（%s）", "（%s）", "（%s，磁盘在哭）"}},
    {Key::StatusClipboard, {"    Clipboard: %zu %s%s", "    剪贴板: %zu %s%s", "    剪貼簿: %zu %s%s", "    剪贴板: %zu %s%s"}},
    {Key::ClipboardCutSuffix, {" (cut)", "（剪切）", "（剪下）", "（准备搬家）"}},
    {Key::ClipboardCopySuffix, {" (copy)", "（复制）", "（複製）", "（复制，Ctrl+C 不是终止）"}},
    {Key::FreeSpace, {"Free space %s", "可用空间 %s", "可用空間 %s", "磁盘还有 %s，省着点用"}},
    {Key::Item, {"item", "个项目", "個項目", "个文件"}},
    {Key::Items, {"items", "个项目", "個項目", "个文件"}},
    {Key::ReadDirFailed, {"Cannot read directory: %s", "读取目录失败: %s", "讀取目錄失敗: %s", "读不到目录: %s（离线了？）"}},
    {Key::CannotOpen, {"Cannot open: %s", "无法打开: %s", "無法開啟: %s", "打不开: %s（信不信我rm你！）"}},
    {Key::SelectAllDone, {"Selected all %zu %s", "已全选 %zu %s", "已全選 %zu %s", "全选了 %zu %s，小孩子才做选择"}},
    {Key::NoSelection, {"No items selected", "没有选中的项目", "沒有選取的項目", "你没选东西，终端也帮不了你"}},
    {Key::NoDeletable, {"No deletable items", "没有可删除的项目", "沒有可刪除的項目", "没有能删的东西，世界暂时安全"}},
    {Key::TrashNoCopy, {"Cannot copy or cut from Trash; use Restore", "回收站中不能复制或剪切，请使用“还原”", "資源回收筒中不能複製或剪下，請使用「還原」", "垃圾桶里的东西不能复制或剪切，请用“还原”救它"}},
    {Key::TrashNoPaste, {"Cannot paste into Trash", "回收站中不支持粘贴", "資源回收筒中不支援貼上", "别往垃圾桶里贴东西，它不是 /dev/null"}},
    {Key::TrashNoNewFolder, {"Cannot create folders in Trash", "回收站中不支持新建文件夹", "資源回收筒中不支援新增資料夾", "垃圾桶里不适合 mkdir，真的"}},
    {Key::TrashNoRename, {"Cannot rename items in Trash", "回收站中不支持重命名", "資源回收筒中不支援重新命名", "垃圾桶里的名字是历史，不能改"}},
    {Key::ClipboardEmpty, {"Clipboard is empty", "剪贴板为空", "剪貼簿是空的", "剪贴板空的，像没装依赖的项目"}},
    {Key::CopySuffix, {" - copy", " - 副本", " - 副本", " - 副本"}},
    {Key::Copied, {"Copied %zu %s", "已复制 %zu %s", "已複製 %zu %s", "复制了 %zu %s，Ctrl+C 没有终止任何东西"}},
    {Key::CutDone, {"Cut %zu %s", "已剪切 %zu %s", "已剪下 %zu %s", "剪下了 %zu %s，准备搬家"}},
    {Key::PasteDoneStatus, {"Paste complete: %zu %s", "粘贴完成：%zu %s", "貼上完成：%zu %s", "贴完了：%zu %s，没有惊动 sudo"}},
    {Key::PasteSkipped, {", %zu skipped", "，跳过 %zu 个", "，略過 %zu 個", "，跳过 %zu 个（它们拒绝了）"}},
    {Key::PasteFailed, {", %zu failed", "，%zu 个失败", "，%zu 個失敗", "，%zu 个失败（像编译错误一样顽固）"}},
    {Key::MoveToTrashDone, {"Moved %zu %s to Trash", "已移入回收站 %zu %s", "已移入資源回收筒 %zu %s", "把 %zu %s 丢进黑洞了"}},
    {Key::PermanentDeleteDone, {"Permanently deleted %zu %s", "已永久删除 %zu %s", "已永久刪除 %zu %s", "已永久删除 %zu %s，rm -rf 的温柔版"}},
    {Key::RestoreDone, {"Restored %zu %s", "已还原 %zu %s", "已還原 %zu %s", "救回了 %zu %s，像从 git 恢复一样"}},
    {Key::TrashEmptied, {"Trash emptied", "回收站已清空", "資源回收筒已清空", "垃圾桶清空，没有 undo"}},
    {Key::CreatedFolder, {"Created folder: %s", "已新建文件夹: %s", "已新增資料夾: %s", "已 mkdir: %s"}},
    {Key::Renamed, {"Renamed: %s", "已重命名: %s", "已重新命名: %s", "已改名: %s，旧名字已登出"}},
    {Key::ShowHiddenStatus, {"Showing hidden items", "正在显示隐藏项目", "正在顯示隱藏項目", "已把 . 开头的秘密挖出来"}},
    {Key::HideHiddenStatus, {"Hiding hidden items", "正在隐藏隐藏项目", "正在隱藏隱藏項目", "已把 . 开头的秘密藏回去"}},
    {Key::LanguageChanged, {"Language changed", "语言已切换", "語言已切換", "语言切好了，不用重开终端"}},
    {Key::ConfigSaveFailed, {"Language changed, but config could not be saved.",
                             "语言已切换，但配置保存失败。",
                             "語言已切換，但設定儲存失敗。",
                             "语言切好了，但 config.json 没写进去（权限在挡你）。"}},
    {Key::NoRestore, {"Nothing to restore", "没有可还原的项目", "沒有可還原的項目", "空空如也的回收站"}},

    {Key::MoveToTrashTitle, {"Move to Trash", "移入回收站", "移入資源回收筒", "丢进黑洞"}},
    {Key::ConfirmReplaceTitle, {"Confirm Replace", "确认替换", "確認取代", "确认覆盖"}},
    {Key::SelectLanguage, {"Select Language", "选择语言", "選擇語言", "选语言，国际化一下"}},
    {Key::SelectLanguageHint, {
        "Choose the interface language.",
        "选择界面语言。",
        "選擇介面語言。",
        "选一个语言，终端不会因此学会 sudo。",
    }},
    {Key::EmptyTrashConfirm, {
        "Empty the Trash?\n\nThis cannot be undone.",
        "确定要清空回收站吗？\n\n此操作无法撤销。",
        "確定要清空資源回收筒嗎？\n\n此操作無法復原。",
        "清空垃圾桶？\n\n没有 undo，也没有 git revert。",
    }},
    {Key::MovedToTrashInfo, {
        "Moved %zu %s to Trash, %zu failed.",
        "已移入回收站 %zu %s，%zu 个失败。",
        "已移入資源回收筒 %zu %s，%zu 個失敗。",
        "已送 %zu %s 进黑洞，%zu 个拒绝进去。",
    }},
    {Key::PermanentDeleteInfo, {
        "Permanently deleted %zu %s, %zu failed.",
        "已永久删除 %zu %s，%zu 个删除失败。",
        "已永久刪除 %zu %s，%zu 個刪除失敗。",
        "已永久删除 %zu %s，%zu 个删不掉（sudo附体？）。",
    }},
    {Key::RestoreInfo, {
        "Restored %zu %s, %zu failed (missing trash info?).",
        "已还原 %zu %s，%zu 个失败（可能缺少回收站信息）。",
        "已還原 %zu %s，%zu 個失敗（可能缺少資源回收筒資訊）。",
        "救回 %zu %s，%zu 个失败（回收站信息可能被 rm 了）。",
    }},
    {Key::EmptyTrashFail, {
        "%zu %s failed to delete.",
        "有 %zu %s删除失败。",
        "有 %zu %s刪除失敗。",
        "有 %zu %s删除失败，像编译错误一样顽固。",
    }},
    {Key::ReplaceConfirm, {
        "\n%s\n\nAn item with the same name already exists.\nReplace it?",
        "\n%s\n\n已存在同名项目。\n是否替换？",
        "\n%s\n\n已存在同名項目。\n是否取代？",
        "\n%s\n\n同名项目已经在了。\n要覆盖它吗？（没有 undo）",
    }},
    {Key::PasteCompleted, {"Paste completed.\n\n", "粘贴完成。\n\n", "貼上完成。\n\n", "贴完了。\n\n"}},
    {Key::SuccessCount, {"Succeeded: %zu\n", "成功: %zu 个\n", "成功: %zu 個\n", "成功: %zu 个\n"}},
    {Key::SkippedCount, {"Skipped: %zu\n", "跳过: %zu 个\n", "略過: %zu 個\n", "跳过: %zu 个（它们拒绝了）\n"}},
    {Key::FailedCount, {"Failed: %zu\n", "失败: %zu 个\n", "失敗: %zu 個\n", "失败: %zu 个（像编译错误）\n"}},
    {Key::PermanentDeleteConfirm, {
        "\nPermanently delete %zu %s?\n\nThis cannot be undone.",
        "\n确定要永久删除 %zu %s吗？\n\n此操作无法撤销。",
        "\n確定要永久刪除 %zu %s嗎？\n\n此操作無法復原。",
        "\n要永久删除 %zu %s吗？\n\nrm -rf 之后没有 undo。",
    }},
    {Key::MoveToTrashConfirm, {
        "Move %zu %s to Trash?",
        "确定要将 %zu %s移到回收站吗？",
        "確定要將 %zu %s移到資源回收筒嗎？",
        "要把 %zu %s丢进黑洞吗？",
    }},
    {Key::FolderName, {"Folder name:", "文件夹名称:", "資料夾名稱:", "资料夹名字:"}},
    {Key::NameExists, {"An item with this name already exists.", "已存在同名项目。", "已存在同名項目。", "同名项目已经在了，换一个吧。"}},
    {Key::InvalidName, {"Invalid name.", "名称无效。", "名稱無效。", "名字不合法，连 ext4 都摇头。"}},
    {Key::CreateFailed, {"Cannot create folder: %s", "无法创建文件夹: %s", "無法新增資料夾: %s", "mkdir 失败: %s，请尝试为TExplorer sudo附体。"}},
    {Key::NewName, {"New name:", "新名称:", "新名稱:", "新名字:"}},
    {Key::RenameFailed, {"Cannot rename: %s", "重命名失败: %s", "重新命名失敗: %s", "改名失败: %s，inode 表示无奈。"}},
    {Key::PropertiesName, {"Name: %s", "名称: %s", "名稱: %s", "名字: %s"}},
    {Key::PropertiesLocation, {"Location: %s", "位置: %s", "位置: %s", "位置: %s（不是 /dev/null）"}},
    {Key::PropertiesType, {"Type: %s", "类型: %s", "類型: %s", "这是啥: %s"}},
    {Key::PropertiesSize, {"Size: %s", "大小: %s", "大小: %s", "占了多少: %s"}},
    {Key::PropertiesModified, {"Modified: %s", "修改时间: %s", "修改時間: %s", "上次动过: %s"}},
    {Key::PropertiesFullPath, {"Full path: %s", "完整路径: %s", "完整路徑: %s", "完整路径: %s（复制时小心空格）"}},

    {Key::HelpLine1, {
        "Action menu: press a or right-click; x at top-right exits.",
        "动作菜单：按 a 或右键打开；右上角 x 退出程序。",
        "動作選單：按 a 或右鍵開啟；右上角 x 結束程式。",
        "动作菜单：按 a 或右键；右上角 x 是逃离终端的传送门。",
    }},
    {Key::HelpLine2, {
        "Tabs: Ctrl+T new, Ctrl+W close, Tab / Ctrl+Tab next, Shift+Tab previous.",
        "标签页：Ctrl+T 新建，Ctrl+W 关闭，Tab / Ctrl+Tab 下一个，Shift+Tab 上一个。",
        "分頁：Ctrl+T 新增，Ctrl+W 關閉，Tab / Ctrl+Tab 下一個，Shift+Tab 上一個。",
        "标签页：Ctrl+T 开新坑，Ctrl+W 填坑，Tab / Ctrl+Tab 下一个，Shift+Tab 上一个。",
    }},
    {Key::HelpLine3, {
        "Tabs: click to switch; click x or middle-click to close.",
        "标签操作：点击标签切换，点击标签上的 x 或中键关闭。",
        "分頁操作：點擊分頁切換，點擊分頁上的 x 或中鍵關閉。",
        "标签操作：点标签切换；点 x 或中键关闭，像 tmux kill-pane。",
    }},
    {Key::HelpLine4, {
        "Navigation: up/down move, PgUp/PgDn page, Home/End jump, Enter open.",
        "导航：上下键移动，PgUp/PgDn 翻页，Home/End 首尾，Enter 打开。",
        "導覽：上下鍵移動，PgUp/PgDn 翻頁，Home/End 首尾，Enter 開啟。",
        "导航：上下键移动，PgUp/PgDn 翻页，Home/End 首尾，Enter 打开。",
    }},
    {Key::HelpLine5, {
        "Navigation: Backspace back, Ctrl+U up, Ctrl+B toggle sidebar.",
        "导航：Backspace 后退，Ctrl+U 上一级，Ctrl+B 显示/隐藏侧栏。",
        "導覽：Backspace 上一頁，Ctrl+U 上一層，Ctrl+B 顯示/隱藏側欄。",
        "导航：Backspace 后退，Ctrl+U 上一级，Ctrl+B 开关侧栏。",
    }},
    {Key::HelpLine6, {
        "Address bar: < back, > forward, ^ up; click breadcrumbs to jump.",
        "地址栏：< 后退，> 前进，^ 上一级；点击面包屑跳转。",
        "網址列：< 上一頁，> 下一頁，^ 上一層；點擊麵包屑跳轉。",
        "地址栏：< 后退，> 前进，^ 上一级；点面包屑跳转，比 cd 直观一点。",
    }},
    {Key::HelpLine7, {
        "Files: F1 help, F2 rename, F4 properties, F5 refresh, F7 new folder.",
        "文件：F1 帮助，F2 重命名，F4 属性，F5 刷新，F7 新建文件夹。",
        "檔案：F1 說明，F2 重新命名，F4 屬性，F5 重新整理，F7 新增資料夾。",
        "文件：F1 帮助，F2 改名，F4 看身世，F5 重新投胎，F7 mkdir 一下。",
    }},
    {Key::HelpLine8, {
        "Files: Del delete, F9 show hidden, F10 quit.",
        "文件：Del 删除，F9 显示隐藏，F10 退出。",
        "檔案：Del 刪除，F9 顯示隱藏，F10 結束。",
        "文件：Del 丢进黑洞，F9 显示隐藏，F10 离开终端（不是 exit，但差不多）。",
    }},
    {Key::HelpLine9, {
        "Edit: Ctrl+C copy, Ctrl+X cut, Ctrl+V paste, Ctrl+A select all.",
        "编辑：Ctrl+C 复制，Ctrl+X 剪切，Ctrl+V 粘贴，Ctrl+A 全选。",
        "編輯：Ctrl+C 複製，Ctrl+X 剪下，Ctrl+V 貼上，Ctrl+A 全選。",
        "编辑：Ctrl+C 复制（不是终止），Ctrl+X 剪切（不是剪网线），Ctrl+V 粘贴（不是 Vim），Ctrl+A 全选。",
    }},
    {Key::HelpLine10, {
        "Mouse: click select, double-click open, wheel scroll, right-click menu.",
        "鼠标：单击选择，双击打开，滚轮滚动，右键菜单。",
        "滑鼠：單擊選取，雙擊開啟，滾輪捲動，右鍵選單。",
        "鼠标：单击选择，双击打开，滚轮滚动，右键菜单。",
    }},
    {Key::HelpLine11, {
        "Multi-select: Shift+click range, Ctrl+click toggle.",
        "多选：Shift+单击 连续，Ctrl+单击 多选。",
        "多選：Shift+單擊 連續，Ctrl+單擊 多選。",
        "多选：Shift+单击 连续，Ctrl+单击 多选；小孩子才做选择。",
    }},
    {Key::HelpLine12, {
        "Open: folders enter; files use the system default app.",
        "打开：文件夹进入下一级；文件用系统默认程序打开。",
        "開啟：資料夾進入下一層；檔案用系統預設程式開啟。",
        "打开：文件夹进去；文件交给系统默认程序，Linux 下是 nano，不是 vim。",
    }},
    {Key::HelpLine13, {
        "Trash: deleted items go to the sidebar Trash; restore or delete permanently there.",
        "回收站：删除的项目移入左侧回收站；在其中可还原或永久删除。",
        "資源回收筒：刪除的項目移到左側資源回收筒；可在其中還原或永久刪除。",
        "回收站：删除的项目进左侧回收站；可在里面还原或永久删除，没有 undo。",
    }},
    {Key::HelpLine14, {
        "Esc clears status; type a filename prefix to jump quickly.",
        "Esc 清除状态；直接输入文件名前缀可快速定位。",
        "Esc 清除狀態；直接輸入檔名前綴可快速定位。",
        "Esc 清除状态；直接输入文件名前缀可快速定位，比 find 快，但只找当前目录。",
    }},

    {Key::Open, {"Open", "打开", "開啟", "让我康康！"}},
    {Key::Restore, {"Restore", "还原", "還原", "救回来"}},
    {Key::PermanentDelete, {"Delete Permanently", "永久删除", "永久刪除", "rm -rf（温柔版）"}},
    {Key::EmptyTrash, {"Empty Trash", "清空回收站", "清空資源回收筒", "清空垃圾桶"}},
    {Key::Properties, {"Properties", "属性", "屬性", "看身世"}},
    {Key::Refresh, {"Refresh", "刷新", "重新整理", "F5 重新投胎"}},
    {Key::SelectAll, {"Select All", "全选", "全選", "小孩子才做选择"}},
    {Key::ShowHiddenMenu, {"Show hidden items", "显示隐藏项目", "顯示隱藏項目", "挖出 . 开头的秘密"}},
    {Key::HideHiddenMenu, {"Hide hidden items", "隐藏隐藏项目", "隱藏隱藏項目", "藏起 . 开头的秘密"}},
    {Key::ShowSidebar, {"Show navigation pane", "显示导航窗格", "顯示導覽窗格", "打开侧栏（不是 tmux split）"}},
    {Key::HideSidebar, {"Hide navigation pane", "隐藏导航窗格", "隱藏導覽窗格", "收起侧栏"}},
    {Key::Copy, {"Copy", "复制", "複製", "复制（Ctrl+C 不是终止）"}},
    {Key::Cut, {"Cut", "剪切", "剪下", "剪切（不是剪网线）"}},
    {Key::Paste, {"Paste", "粘贴", "貼上", "粘贴（不是 Vim）"}},
    {Key::Rename, {"Rename", "重命名", "重新命名", "改名（inode 不变）"}},
    {Key::Delete, {"Delete", "删除", "刪除", "丢进黑洞"}},
    {Key::NewFolder, {"New Folder", "新建文件夹", "新增資料夾", "mkdir 一下"}},
    {Key::Back, {"Back", "后退", "上一頁", "回到过去"}},
    {Key::Forward, {"Forward", "前进", "下一頁", "前往未来"}},
    {Key::Up, {"Up", "上一级", "上一層", "cd .."}},
    {Key::Help, {"Help", "帮助", "說明", "help 一下"}},
    {Key::Quit, {"Quit", "退出", "結束", "离开终端"}},
    {Key::Language, {"Language", "语言", "語言", "国际化"}},

    {Key::RunDone, {"Ran %s", "已运行 %s", "已執行 %s", "已执行 %s，终端没有爆炸"}},
    {Key::OpenDone, {"Opened %s", "已打开 %s", "已開啟 %s", "已打开 %s，交给系统了"}},
    {Key::RunFailed, {"Cannot run: %s", "无法运行: %s", "無法執行: %s", "跑不起来: %s"}},
    {Key::ProcessStartFailed, {"Cannot start process", "无法启动进程", "無法啟動程序", "fork 失败，连 process 都不想生"}},
    {Key::NanoStartFailed, {"Cannot start nano", "无法启动 nano", "無法啟動 nano", "nano 离家出走了"}},
    {Key::NanoEdited, {"Edited %s with nano", "已用 nano 编辑 %s", "已用 nano 編輯 %s", "用 nano 编辑了 %s，Ctrl+O 救了世界"}},
};

std::string lowerAscii(const std::string& value) {
    std::string out = value;
    for (char& ch : out) {
        if (ch >= 'A' && ch <= 'Z') ch = static_cast<char>(ch - 'A' + 'a');
    }
    return out;
}

bool startsWith(const std::string& value, const char* prefix) {
    const std::size_t n = std::char_traits<char>::length(prefix);
    return value.size() >= n && value.compare(0, n, prefix) == 0;
}

} // namespace

void setLanguage(Lang lang) {
    g_lang = lang;
}

Lang language() {
    return g_lang;
}

const char* languageName(Lang lang) {
    switch (lang) {
    case Lang::English: return "English";
    case Lang::SimplifiedChinese: return "简体中文";
    case Lang::TraditionalChinese: return "繁體中文";
    case Lang::MemeChinese: return "梗体中文";
    }
    return "English";
}

const char* languageCode(Lang lang) {
    switch (lang) {
    case Lang::English: return "en";
    case Lang::SimplifiedChinese: return "zh";
    case Lang::TraditionalChinese: return "zh_TW";
    case Lang::MemeChinese: return "meme";
    }
    return "en";
}

bool parseLanguage(const std::string& value, Lang& out) {
    std::string v = lowerAscii(value);
    if (v.empty()) return false;
    for (char& ch : v) {
        if (ch == '-')
            ch = '_';
    }

    if (v == "en" || v == "english" || v == "eng" ||
        startsWith(v, "en_") || startsWith(v, "en.")) {
        out = Lang::English;
        return true;
    }
    if (v == "meme" || v == "zh_meme" || v == "meme_zh" ||
        v == "梗" || v == "梗体" || v == "梗体中文") {
        out = Lang::MemeChinese;
        return true;
    }
    if (v == "zh_tw" || v == "zh_hk" || v == "zh_mo" ||
        v == "zh_hant" || v == "traditional" || v == "tw" ||
        startsWith(v, "zh_tw") || startsWith(v, "zh_hk") ||
        startsWith(v, "zh_mo") || startsWith(v, "zh_hant")) {
        out = Lang::TraditionalChinese;
        return true;
    }
    if (v == "zh" || v == "zh_cn" || v == "zh_hans" || v == "chinese" ||
        v == "simplified" || v == "cn" || startsWith(v, "zh_cn") ||
        startsWith(v, "zh_hans") || startsWith(v, "zh.")) {
        out = Lang::SimplifiedChinese;
        return true;
    }
    return false;
}

bool detectLanguageOverride(Lang& out) {
    const char* explicitLang = std::getenv("TEXPLORER_LANG");
    return explicitLang && parseLanguage(explicitLang, out);
}

Lang detectLanguage(Lang fallback) {
    Lang lang;
    if (detectLanguageOverride(lang)) return lang;
    for (const char* name : {"LC_ALL", "LC_MESSAGES", "LANG"}) {
        const char* value = std::getenv(name);
        if (!value || !*value) continue;
        Lang lang;
        if (parseLanguage(value, lang)) return lang;
    }
    return fallback;
}

const char* text(Key key) {
    for (const Entry& entry : kEntries) {
        if (entry.key == key) return entry.value[static_cast<int>(g_lang)];
    }
    return "";
}

std::string tr(Key key) {
    return std::string(text(key));
}

std::string tf(Key key, ...) {
    const char* fmt = text(key);
    va_list args;
    va_start(args, key);
    va_list argsCopy;
    va_copy(argsCopy, args);
    const int needed = std::vsnprintf(nullptr, 0, fmt, args);
    va_end(args);
    if (needed < 0) {
        va_end(argsCopy);
        return std::string(fmt);
    }
    std::vector<char> buffer(static_cast<std::size_t>(needed) + 1);
    std::vsnprintf(buffer.data(), buffer.size(), fmt, argsCopy);
    va_end(argsCopy);
    return std::string(buffer.data(), static_cast<std::size_t>(needed));
}

std::string itemWord(std::size_t n) {
    return tr(n == 1 ? Key::Item : Key::Items);
}

} // namespace i18n
