#include "Scanner.h"
#include "Database.h"
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <iomanip>
#include <atomic>

bool Scanner::resolveTarget() {
    struct hostent *he = gethostbyname(target.c_str());
    if (!he) return false;
    targetIP = inet_ntoa(*(struct in_addr *)he->h_addr);
    return true;
}

bool Scanner::tcpConnect(int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return false;
    struct timeval tv;
    tv.tv_sec = 2; tv.tv_usec = 0; 
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, targetIP.c_str(), &addr.sin_addr);
    int result = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    close(sock);
    return (result == 0);
}

void Scanner::runScan(const std::vector<int>& portList, int threadCount) {
    if(!resolveTarget()) { std::cout << "\n\033[1;31m[!] DNS Error\033[0m\n"; return; }
    std::atomic<int> open_found{0};
    std::cout << "\n\033[1;34m[*] TARGET: " << target << " (" << targetIP << ")\033[0m\n";
    std::cout << "------------------------------------------------------------------\n";
    std::cout << std::left << std::setw(8) << "PORT" << std::setw(12) << "SERVICE" << std::setw(18) << "VULNERABILITY" << "TOOL" << std::endl;
    std::cout << "------------------------------------------------------------------\n";

    size_t idx = 0; std::mutex mtx;
    auto worker = [&]() {
        while (true) {
            int p = -1;
            { std::lock_guard<std::mutex> lock(mtx); if (idx >= portList.size()) return; p = portList[idx++]; }
            if (tcpConnect(p)) {
                open_found++;
                std::lock_guard<std::mutex> lock(report_mutex);
                std::cout << "\033[1;32m" << std::left << std::setw(8) << p;
                if (lookup.count(p)) {
                    std::cout << std::setw(12) << lookup[p].name << std::setw(18) << lookup[p].vuln << lookup[p].tool << "\033[0m\n";
                    std::cout << "  \033[0;90m└─ Payload: " << lookup[p].payload << "\033[0m\n";
                } else { std::cout << std::setw(12) << "Active" << std::setw(18) << "Unknown" << "Ncat" << "\033[0m\n"; }
            }
        }
    };
    std::vector<std::thread> threads;
    for (int i = 0; i < threadCount; ++i) threads.emplace_back(worker);
    for (auto& t : threads) t.join();
    if (open_found == 0) std::cout << "\033[1;33m[!] No open ports detected.\033[0m\n";
    std::cout << "\033[1;34m[***] SCAN FINISHED [***]\033[0m\n";
}
