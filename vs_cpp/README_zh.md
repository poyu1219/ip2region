# ip2region Visual Studio 2025 C++ GUI 專案（含快取）

這個目錄提供可直接用 Visual Studio 2025 開啟的 **Windows UI** C++ 專案：
- 解決方案：`vs_cpp.sln`
- 專案：`vs_cpp.vcxproj`
- 介面：可輸入 xdb 路徑、IP、版本（IPv4/IPv6）、快取策略，按「查詢」顯示結果。

## 開啟方式
1. 用 Visual Studio 2025 開啟 `vs_cpp/vs_cpp.sln`。
2. 選擇 `x64` + `Debug` 或 `Release`。
3. 直接執行，會開啟 GUI 視窗。

## UI 功能
- `XDB 路徑`：預設 `..\data\ip2region_v6.xdb`
- `IP`：預設 `2001:200:124::`
- `版本`：IPv4 / IPv6
- `快取`：
  - `content(全量)`：全量快取（預設，查詢最快）
  - `vector(部分)`：部分快取
  - `file(不快取)`：不快取
- `結果`：顯示查詢到的地區字串

## 快取行為
程式會快取 `searcher` 實例；只有在 `xdb 路徑 / 版本 / 快取策略` 任一改變時，才會重新建立查詢器。一般連續查詢時不會重複掃描檔案。
