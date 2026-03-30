#include "Scanner.h"
#include "Database.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

void printBanner() {
    std::cout << "\033[1;32m";
    std::cout << "███████╗███████╗███████╗██╗  ██╗███╗   ███╗ █████╗ ██████╗ \n";
    std::cout << "╚══███╔╝██╔════╝██╔════╝╚██╗██╔╝████╗ ████║██╔══██╗██╔══██╗\n";
    std::cout << "  ███╔╝ █████╗  █████╗   ╚███╔╝ ██╔████╔██║███████║██████╔╝\n";
    std::cout << " ███╔╝  ██╔══╝  ██╔══╝   ██╔██╗ ██║╚██╔╝██║██╔══██║██╔═══╝ \n";
    std::cout << "███████╗███████╗███████╗██╔╝ ██╗██║ ╚═╝ ██║██║  ██║██║     \n";
    std::cout << "╚══════╝╚══════╝╚══════╝╚═╝  ╚═╝╚═╝     ╚═╝╚═╝  ╚═╝╚═╝     \n";
    std::cout << "\033[1;37m           [ Written by Zarak Afridi ]\033[0m\n\n";
}

int main() {
    while (true) {
        printBanner();
        std::string input;
        std::cout << "[?] Enter Target(s) (IP/URL) or '0' to exit:\n> ";
        std::getline(std::cin >> std::ws, input);
        if (input == "0") break;

        std::stringstream ss(input);
        std::string t;
        std::vector<std::string> targets;
        while (ss >> t) targets.push_back(t);

        std::cout << "\n1. Top 1000 Famous Ports\n2. Full Range (1-65535)\n3. Custom Port(s)\n0. Back\nChoice: ";
        int choice; 
        if (!(std::cin >> choice)) { std::cin.clear(); std::cin.ignore(1000, '\n'); continue; }
        if (choice == 0) continue;

        std::vector<int> ports;
        if (choice == 1) ports = getTop1000();
        else if (choice == 2) { for(int p=1; p<=65535; p++) ports.push_back(p); }
        else if (choice == 3) {
            std::string p_input;
            std::cout << "Enter ports (e.g., 80 443): ";
            std::cin.ignore(1000, '\n');
            std::getline(std::cin, p_input);
            std::stringstream ss_p(p_input);
            int p_val;
            while(ss_p >> p_val) ports.push_back(p_val);
        }

        int threads;
        std::cout << "Threads (1-500): ";
        std::cin >> threads;

        for (const auto& target : targets) {
            Scanner zeex(target);
            zeex.runScan(ports, (threads > 0 ? threads : 50)); 
        }
    }
    return 0;
}

