#include <iostream>

int main() {
    std::cout << "__cplusplus: " << __cplusplus << std::endl;
    #ifdef _MSVC_LANG
    std::cout << "_MSVC_LANG: " << _MSVC_LANG << std::endl;
    #endif
    return 0;
}