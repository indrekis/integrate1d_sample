// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <algorithm>
#include <cctype>

#include "string_tools.hpp"

std::string trim(std::string arg) { // Note: we need copy here -- let compiler create it for us
    constexpr auto is_space_priv = [](auto a) { return std::isspace(a); }; // Зокрема, про цей код спитаю -- а що він значить ;-)
    // std::isspace() has unsuitable prototype, int isspace( int ch ), while our find_xxx require "bool fn(char)".
    auto last_nonspace = std::find_if_not(arg.rbegin(), arg.rend(), is_space_priv ).base(); // І про цей.
    if(last_nonspace != arg.end())
        arg.erase(last_nonspace, arg.end());   // Зауважте, тут швидкодія залежить від порядку стирання.
    // Пам'ятаємо також про інвадідацію ітераторів
    auto first_nonspace = std::find_if_not(arg.begin(), arg.end(), is_space_priv );
    arg.erase(arg.begin(), first_nonspace);
    return arg;
}

