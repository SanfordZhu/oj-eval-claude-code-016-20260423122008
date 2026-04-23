#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include "bpt.h"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    BPTree tree;

    int n;
    std::cin >> n;
    std::cin.ignore();

    for (int i = 0; i < n; i++) {
        std::string line;
        std::getline(std::cin, line);
        std::istringstream iss(line);

        std::string command;
        iss >> command;

        if (command == "insert") {
            std::string index;
            int value;
            iss >> index >> value;
            tree.insert(index, value);
        } else if (command == "delete") {
            std::string index;
            int value;
            iss >> index >> value;
            tree.remove(index, value);
        } else if (command == "find") {
            std::string index;
            iss >> index;
            std::vector<int> values = tree.find(index);

            if (values.empty()) {
                std::cout << "null\n";
            } else {
                std::sort(values.begin(), values.end());
                for (int j = 0; j < values.size(); j++) {
                    if (j > 0) std::cout << " ";
                    std::cout << values[j];
                }
                std::cout << "\n";
            }
        }
    }

    return 0;
}