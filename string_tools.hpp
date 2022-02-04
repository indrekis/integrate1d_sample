
#ifndef TEST_STRING_TOOLS_HPP
#define TEST_STRING_TOOLS_HPP

#include <string>
#include <sstream>

std::string trim(std::string arg);

//! Slow but universal
template<typename T>
auto from_str(const std::string& arg){
    T res = T{};
    std::stringstream ss{arg};
    ss >> res;
    if( std::string temp; ss >> temp ){
        throw std::runtime_error{"Wrong numerical value: " + arg};
    }
    return res;
}

//! Optimised -- for ull only
//! Спеціалізація шаблонів функцій -- річ підозріла і небезпечна, хоч іноді й потрібна.
//! Мені видається, що тут якраз рідкісний виняток, коли вона корисна. (Inline тут необхідний).
template<>
inline auto from_str<unsigned long long>(const std::string& arg){
    unsigned long long res = 0;
    size_t last_sym = 0;
    res = std::stoull(arg, &last_sym);
    if( last_sym != arg.size() ){
        throw std::runtime_error{"Wrong numerical value: " + arg};
    }
    return res;
}

#endif //TEST_STRING_TOOLS_HPP
