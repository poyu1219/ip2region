#include <iostream>
#include <string>

#include "../src/search.h"

int main() {
    const std::string default_db_path = R"(..\..\data\ip2region_v4.xdb)";

    std::cout << "==== IP2Region Visual Studio UI (Console) ====\n";
    std::cout << "請輸入 xdb 路徑（直接 Enter 使用預設）: ";

    std::string db_path;
    std::getline(std::cin, db_path);
    if (db_path.empty()) {
        db_path = default_db_path;
    }

    int version = xdb::ipv4;
    int policy = xdb::policy_vector;

    try {
        xdb::search_t searcher(db_path, version, policy);
        std::cout << "\n輸入 IP 後按 Enter 查詢，輸入 q 退出。\n\n";

        std::string ip;
        while (true) {
            std::cout << "IP> ";
            if (!std::getline(std::cin, ip)) {
                std::cout << "\n已結束。\n";
                break;
            }

            if (ip == "q" || ip == "quit" || ip == "exit") {
                std::cout << "已退出。\n";
                break;
            }

            if (ip.empty()) {
                continue;
            }

            std::string region = searcher.search(ip);
            if (region.empty()) {
                std::cout << "查無結果。\n\n";
            } else {
                std::cout << "城市/區域結果: " << region << "\n";
                std::cout << "I/O 次數: " << searcher.get_io_count()
                          << ", 耗時: " << searcher.get_cost_time() << " μs\n\n";
            }
        }
    } catch (...) {
        std::cerr << "初始化查詢器失敗，請確認 xdb 路徑正確。\n";
        return 1;
    }

    return 0;
}
