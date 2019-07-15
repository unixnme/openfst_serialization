//
// Created by ykang7 on 2019-07-14.
//

#include <iostream>
#include <vector>
#include <string>
#include "fst/fstlib.h"

int main(int argc, const char **argv) {
    std::ios::sync_with_stdio(false);
    std::ifstream ifs;
    if (argc >= 2) {
        ifs = std::ifstream{argv[1]};
        std::cin.rdbuf(ifs.rdbuf());
    }

    for(std::string buffer; std::cin >> buffer;) {
        std::cout << buffer << std::endl;
    }
    return 0;
}