#include <iostream>

#include "table.hpp"

int main() {
    RoutingTable rt;
    std::cin >> rt;
    
    while (true) {
        // std::cerr << "send_updates\n";
        rt.send_updates();
        // std::cerr << "recv_updates\n";
        rt.recv_updates();
        // std::cerr << "check_timeouts\n";
        rt.check_timeouts();
        std::cout << rt << std::endl;
    }

    return EXIT_SUCCESS;
}