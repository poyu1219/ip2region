#include <iostream>
#include <string>

#include "../src/search.h"

int main() {
    const std::string default_db_path = R"(..\..\data\ip2region_v4.xdb)";

    std::cout << "==== IP2Region VS Console UI ====\n";
    std::cout << "Input xdb path (press Enter to use default): ";

    std::string db_path;
    std::getline(std::cin, db_path);
    if (db_path.empty()) {
        db_path = default_db_path;
    }

    int version = xdb::ipv4;
    int policy = xdb::policy_vector;

    xdb::search_t searcher(db_path, version, policy);
    std::cout << "\nEnter an IP to search, type q to quit.\n\n";

    std::string ip;
    while (true) {
        std::cout << "IP> ";
        if (!std::getline(std::cin, ip)) {
            std::cout << "\nBye.\n";
            break;
        }

        if (ip == "q" || ip == "quit" || ip == "exit") {
            std::cout << "Bye.\n";
            break;
        }

        if (ip.empty()) {
            continue;
        }

        std::string region = searcher.search(ip);
        if (region.empty()) {
            std::cout << "No result.\n\n";
        } else {
            std::cout << "Region: " << region << "\n";
            std::cout << "I/O count: " << searcher.get_io_count()
                      << ", elapsed: " << searcher.get_cost_time() << " us\n\n";
        }
    }

    return 0;
}
