#include <iostream>
#include <string>

#include "../src/search.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "用法: " << argv[0] << " <xdb檔案路徑> [ip_version(4|6)] [policy(1:file 2:vector 3:content)]\n";
        return 1;
    }

    std::string db_path = argv[1];
    int version = (argc >= 3 && std::string(argv[2]) == "6") ? xdb::ipv6 : xdb::ipv4;
    int policy = argc >= 4 ? std::stoi(argv[3]) : xdb::policy_vector;

    xdb::search_t searcher(db_path, version, policy);

    std::cout << "================ IP 城市查詢 UI ================\n";
    std::cout << "輸入 IP 後按 Enter 查詢，輸入 q 退出。\n\n";

    std::string ip;
    while (true) {
        std::cout << "請輸入 IP: ";
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

    return 0;
}
