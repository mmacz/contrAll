add_library(controller STATIC
    controller.cpp include/controller.hpp
)
target_include_directories(controller PUBLIC include)
target_link_libraries(controller
    PRIVATE
        libusb::libusb
)
