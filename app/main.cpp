#include "main.hpp"

int main(int argc, char **argv) {
    Controllers contr4llrs;
    auto connectedControllers = contr4llrs.get_connected();
    for (const auto& ctrl : connectedControllers) {
        contr4llrs.open_device(ctrl);
        auto& data = contr4llrs.read_from_device();
        std::cout << "read data size: " << data.size << std::endl;
        contr4llrs.close_device();
    }
    return 0;
}

