add_executable(contrAll main.cpp)
target_include_directories(contrAll PRIVATE include)

target_link_libraries(contrAll
    PRIVATE
        controller
)

target_compile_definitions(contrAll
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

