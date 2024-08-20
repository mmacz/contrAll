#include "main.hpp"

int main(int argc, char **argv) {
    Controllers contr4llrs;
    auto connectedControllers = contr4llrs.get_connected();
    for (const auto& ctrl : connectedControllers) {
        std::cout << ctrl.toString();
    }
    return 0;
}

