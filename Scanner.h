#ifndef SCANNER_H
#define SCANNER_H
#include <string>
#include <vector>
#include <mutex>

class Scanner {
public:
    std::string target;
    std::string targetIP;
    std::mutex report_mutex;

    Scanner(std::string t) : target(t) {}
    bool resolveTarget();
    void runScan(const std::vector<int>& portList, int threads);
    bool tcpConnect(int port);
};
#endif
