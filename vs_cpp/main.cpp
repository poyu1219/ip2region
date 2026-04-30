#include <windows.h>

#include <memory>
#include <string>

#include "../binding/cpp/src/search.h"

namespace {

HINSTANCE g_hInst = nullptr;
HWND g_hXdbPath = nullptr;
HWND g_hIp = nullptr;
HWND g_hVersion = nullptr;
HWND g_hPolicy = nullptr;
HWND g_hResult = nullptr;

std::unique_ptr<xdb::search_t> g_searcher;
std::string g_cachedXdb;
int g_cachedVersion = -1;
int g_cachedPolicy = -1;

constexpr int IDC_XDB_PATH = 1001;
constexpr int IDC_IP = 1002;
constexpr int IDC_VERSION = 1003;
constexpr int IDC_POLICY = 1004;
constexpr int IDC_SEARCH = 1005;
constexpr int IDC_RESULT = 1006;

std::string wide_to_utf8(const std::wstring& w) {
    if (w.empty()) return "";
    int len = WideCharToMultiByte(CP_UTF8, 0, w.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (len <= 0) return "";

    std::string out(len - 1, '\0');
    WideCharToMultiByte(CP_UTF8, 0, w.c_str(), -1, out.data(), len, nullptr, nullptr);
    return out;
}

std::wstring utf8_to_wide(const std::string& s) {
    if (s.empty()) return L"";
    int len = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
    if (len <= 0) return L"";

    std::wstring out(len - 1, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, out.data(), len);
    return out;
}

std::wstring get_text(HWND hWnd) {
    int len = GetWindowTextLengthW(hWnd);
    std::wstring text(len, L'\0');
    GetWindowTextW(hWnd, text.data(), len + 1);
    return text;
}

void set_result(const std::wstring& text) {
    SetWindowTextW(g_hResult, text.c_str());
}

int version_from_combo() {
    int idx = static_cast<int>(SendMessageW(g_hVersion, CB_GETCURSEL, 0, 0));
    return idx == 0 ? xdb::ipv4 : xdb::ipv6;
}

int policy_from_combo() {
    int idx = static_cast<int>(SendMessageW(g_hPolicy, CB_GETCURSEL, 0, 0));
    if (idx == 0) return xdb::policy_content;
    if (idx == 1) return xdb::policy_vector;
    return xdb::policy_file;
}

void do_search() {
    std::wstring xdbW = get_text(g_hXdbPath);
    std::wstring ipW = get_text(g_hIp);

    if (xdbW.empty() || ipW.empty()) {
        set_result(L"請先輸入 xdb 路徑與 IP");
        return;
    }

    std::string xdb = wide_to_utf8(xdbW);
    std::string ip = wide_to_utf8(ipW);
    int version = version_from_combo();
    int policy = policy_from_combo();

    try {
        if (!g_searcher || g_cachedXdb != xdb || g_cachedVersion != version || g_cachedPolicy != policy) {
            g_searcher = std::make_unique<xdb::search_t>(xdb, version, policy);
            g_cachedXdb = xdb;
            g_cachedVersion = version;
            g_cachedPolicy = policy;
        }

        std::string region = g_searcher->search(ip);
        set_result(utf8_to_wide(region));
    } catch (const std::exception& ex) {
        set_result(L"查詢失敗: " + utf8_to_wide(ex.what()));
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE: {
            CreateWindowW(L"STATIC", L"XDB 路徑:", WS_CHILD | WS_VISIBLE, 20, 20, 90, 24, hWnd, nullptr, g_hInst, nullptr);
            g_hXdbPath = CreateWindowW(L"EDIT", L"..\\data\\ip2region_v6.xdb", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
                                       110, 20, 560, 24, hWnd, reinterpret_cast<HMENU>(IDC_XDB_PATH), g_hInst, nullptr);

            CreateWindowW(L"STATIC", L"IP:", WS_CHILD | WS_VISIBLE, 20, 60, 90, 24, hWnd, nullptr, g_hInst, nullptr);
            g_hIp = CreateWindowW(L"EDIT", L"2001:200:124::", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
                                  110, 60, 320, 24, hWnd, reinterpret_cast<HMENU>(IDC_IP), g_hInst, nullptr);

            CreateWindowW(L"STATIC", L"版本:", WS_CHILD | WS_VISIBLE, 450, 60, 50, 24, hWnd, nullptr, g_hInst, nullptr);
            g_hVersion = CreateWindowW(L"COMBOBOX", nullptr, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
                                       500, 60, 80, 200, hWnd, reinterpret_cast<HMENU>(IDC_VERSION), g_hInst, nullptr);
            SendMessageW(g_hVersion, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"IPv4"));
            SendMessageW(g_hVersion, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"IPv6"));
            SendMessageW(g_hVersion, CB_SETCURSEL, 1, 0);

            CreateWindowW(L"STATIC", L"快取:", WS_CHILD | WS_VISIBLE, 590, 60, 50, 24, hWnd, nullptr, g_hInst, nullptr);
            g_hPolicy = CreateWindowW(L"COMBOBOX", nullptr, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
                                      640, 60, 130, 200, hWnd, reinterpret_cast<HMENU>(IDC_POLICY), g_hInst, nullptr);
            SendMessageW(g_hPolicy, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"content(全量)"));
            SendMessageW(g_hPolicy, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"vector(部分)"));
            SendMessageW(g_hPolicy, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"file(不快取)"));
            SendMessageW(g_hPolicy, CB_SETCURSEL, 0, 0);

            CreateWindowW(L"BUTTON", L"查詢", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                          680, 18, 90, 30, hWnd, reinterpret_cast<HMENU>(IDC_SEARCH), g_hInst, nullptr);

            CreateWindowW(L"STATIC", L"結果:", WS_CHILD | WS_VISIBLE, 20, 105, 70, 24, hWnd, nullptr, g_hInst, nullptr);
            g_hResult = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
                                      20, 130, 750, 260, hWnd, reinterpret_cast<HMENU>(IDC_RESULT), g_hInst, nullptr);
            return 0;
        }
        case WM_COMMAND:
            if (LOWORD(wParam) == IDC_SEARCH) {
                do_search();
                return 0;
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            break;
    }

    return DefWindowProcW(hWnd, message, wParam, lParam);
}

}  // namespace

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
    g_hInst = hInstance;

    const wchar_t CLASS_NAME[] = L"Ip2RegionGuiWindow";
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    RegisterClassW(&wc);

    HWND hWnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"ip2region Visual Studio 2025 GUI (含快取)",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        820,
        480,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (hWnd == nullptr) return 0;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return static_cast<int>(msg.wParam);
}
