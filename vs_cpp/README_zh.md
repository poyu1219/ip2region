# ip2region Visual Studio 2025 C++ 專案（含快取）

這個目錄提供可直接用 Visual Studio 2025 開啟的 C++ 專案：
- 專案檔：`vs_cpp.sln`
- 預設使用 `content` 快取策略（全量載入 xdb 到記憶體），避免每次查詢都掃描檔案 I/O。

## 開啟方式
1. 用 Visual Studio 2025 開啟 `vs_cpp/vs_cpp.sln`。
2. 選擇 `x64` + `Debug` 或 `Release`。
3. 建置並執行。

## 命令列參數
執行檔用法：

```bash
vs_cpp.exe <xdb_path> <ip> [v4|v6] [file|vector|content]
```

- `xdb_path`：xdb 檔案路徑（預設 `../data/ip2region_v6.xdb`）
- `ip`：要查詢的 IP（預設 `2001:200:124::`）
- `v4|v6`：IP 版本（預設 `v6`）
- `file|vector|content`：快取策略（預設 `content`）

## 快取策略說明
- `file`：不快取，查詢時 I/O 最多。
- `vector`：部分快取（向量快取），減少部分 I/O。
- `content`：全量快取（建議），查詢時幾乎不需要再讀檔，速度最快。
