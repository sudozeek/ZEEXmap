#ifndef DATABASE_H
#define DATABASE_H
#include <map>
#include <string>
#include <vector>

struct ServiceInfo {
    std::string name;
    std::string vuln;
    std::string payload;
    std::string tool;
};

inline std::map<int, ServiceInfo> lookup = {
    {21,   {"FTP", "Anon Login", "USER anonymous", "Metasploit"}},
    {22,   {"SSH", "Brute Force", "rockyou.txt", "Hydra"}},
    {23,   {"Telnet", "Cleartext", "Sniffing", "Wireshark"}},
    {25,   {"SMTP", "Enum Users", "VRFY root", "Nmap"}},
    {53,   {"DNS", "Zone Trans", "AXFR Query", "Dig"}},
    {80,   {"HTTP", "SQLi/XSS", "' OR 1=1 --", "Sqlmap"}},
    {443,  {"HTTPS", "SSL Vulns", "Heartbleed", "SSLScan"}},
    {445,  {"SMB", "EternalBlue", "ms17-010", "Metasploit"}},
    {3306, {"MySQL", "Unauth", "mysql -u root", "Sqlmap"}},
    {3389, {"RDP", "BlueKeep", "cve-2019-0708", "Metasploit"}},
    {8080, {"Apache", "Manager", "/manager/html", "Nikto"}}
};

inline std::vector<int> getTop1000() {
    std::vector<int> p;
    // Top 1024 ports
    for(int i=1; i<=1024; i++) p.push_back(i); 
    return p;
}
#endif
