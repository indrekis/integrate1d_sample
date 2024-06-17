#ifndef TEST_INTEGRATE2D_CONF_HPP
#define TEST_INTEGRATE2D_CONF_HPP

#include <string>
#include <optional>
#include <map>
#include <iosfwd>       // iostream здоровенний, тому його тільки в .cpp включаю.
#include <exception>
#include <stdexcept>

// #define PRINT_INTERMEDIATE_STEPS

#ifdef PRINT_INTERMEDIATE_STEPS
#include <iostream>
#endif

#include "string_tools.hpp"

struct config_error : public std::runtime_error { // Тут теж запитаюся -- навіщо цей базовий клас
    using runtime_error::runtime_error;
};

struct wrong_option_line_error : public config_error{
    using config_error::config_error;
};

struct wrong_option_arg_error : public config_error{
    using config_error::config_error;
};

struct option_duplicated_error : public config_error{
    using config_error::config_error;
};

struct option_not_found_error : public config_error{
    using config_error::config_error;
};

struct wrong_logical_option_arg_error : public wrong_option_arg_error{
    using wrong_option_arg_error::wrong_option_arg_error;
};



class integrate1d_conf {
public:
    // Це порушення вимог ООП -- зробити ці поля публічними. Але для наукових обчислень,
    // у деяких випадках, прийнятно і часто -- достатньо хороше рішення.
    // Не робіть так без дуже вагомих аргументів!
    double rel_err = 0.0, abs_err = 0.0;
    double x1 = 0.0, x2 = 0.0;
    size_t initial_steps = 0;
    size_t max_steps = 0;
    size_t max_iters = 0;

    //! Конструювання свідомо одноразове -- заради простоти, "reset" -- перечинтування, не передбачені.
    integrate1d_conf(std::istream& fs);
private:
    // Зауважте, публічне лише те, що потрібно користувачам -- і що з точки зору коду
    // чисельних розрахунків немає сенсу ізолювати за геттерами.
    using parse_string_ret_t = std::pair<std::string, std::optional<std::string>>;
    using options_map_t = std::map<std::string, std::string>;

    static parse_string_ret_t parse_string(std::string arg);
    void file_to_options_map(std::istream& cf);

    template<typename T>
    T get_option_from_map(const std::string& option_name) const;

    void read_conf(std::istream& cf);
    void validate_conf();

    options_map_t options_map;
};


//=Template implementation -- should be in headers================================
#ifdef PRINT_INTERMEDIATE_STEPS
template<typename T>
std::ostream& operator<<(std::ostream& os, const std::optional<T>& arg)
{
    if(arg) os << *arg;
    return os;
}
#ifdef __GNUC__ // Clang too
template<typename T>
void print_type() { std::cout << __PRETTY_FUNCTION__ << '\n'; }
#endif
#endif

// Насправді, цей метод приватний, використовується лише в реалізації цього класу,
// і її варто в .cpp файл -- але студенти ж копіювати почнуть...
template<typename T>
T integrate1d_conf::get_option_from_map(const std::string& option_name) const{
    if( options_map.count(option_name) == 0 ){
        throw option_not_found_error("Option not found: " + option_name); // Тут можу запитатися -- чого воно працює?
    }
    try {
        auto elem_itr = options_map.find(option_name);
        if( elem_itr != options_map.end() ) {
            return from_str<T>(elem_itr->second);
        } else {
            throw wrong_option_arg_error{"Option " + option_name + " not found"};
        }
        // Так було б гарно -- але якщо find поверне end()...
        // return from_str<T>(options_map.find(option_name)->second);
    }catch( std::runtime_error& ex ){
        throw wrong_option_arg_error{ex.what()}; // Convert error type to our internal exception
    }
}




#endif //TEST_INTEGRATE2D_CONF_HPP
