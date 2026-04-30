#include <iostream>
#include <string>
#include "../binding/cpp/src/search.h"

int main(int argc, char* argv[]) {
    // 用法: vs_cpp.exe <xdb_path> <ip> [v4|v6] [file|vector|content]
    // 默认: v6 + content（全量缓存，避免频繁文件扫描）
    std::string xdbPath = argc > 1 ? argv[1] : "../data/ip2region_v6.xdb";
    std::string ip = argc > 2 ? argv[2] : "2001:200:124::";
    std::string verArg = argc > 3 ? argv[3] : "v6";
    std::string policyArg = argc > 4 ? argv[4] : "content";

    int version = (verArg == "v4") ? xdb::ipv4 : xdb::ipv6;
    int policy = xdb::policy_content;
    if (policyArg == "file") policy = xdb::policy_file;
    if (policyArg == "vector") policy = xdb::policy_vector;

    try {
        xdb::search_t searcher(xdbPath, version, policy);
        std::string region = searcher.search(ip);
        std::cout << "IP: " << ip << "\n";
        std::cout << "Region: " << region << "\n";
        std::cout << "Version: " << (version == xdb::ipv4 ? "IPv4" : "IPv6") << "\n";
        std::cout << "Policy: "
                  << (policy == xdb::policy_file ? "file(不缓存)"
                      : policy == xdb::policy_vector ? "vector(部分缓存)"
                                                     : "content(全量缓存)")
                  << "\n";
    } catch (const std::exception& ex) {
        std::cerr << "查询失败: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
