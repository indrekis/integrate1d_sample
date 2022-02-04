// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "integrate1d_conf.hpp"

#include <algorithm>
#include <iostream>

// auto integrate1d_conf::parse_string(std::string arg){ тут було б класно, але тоді -- з auto, не винесеш в cpp файл.
integrate1d_conf::parse_string_ret_t
integrate1d_conf::parse_string(std::string arg){ // Note: we need copy here -- let compiler create it for us
    constexpr char separator = '='; // Just for the readability
    constexpr char commenter = '#';

    auto comment_pos = arg.find(commenter);
    if(comment_pos != std::string::npos)
        arg.erase(comment_pos); //! Remove comment
    auto sep_pos = arg.find(separator);
    if(sep_pos == std::string::npos){
        return parse_string_ret_t{trim(arg), std::nullopt};
    }
    auto left_part = arg.substr(0, sep_pos);
    auto right_part = arg.substr(sep_pos+1, std::string::npos );
    return parse_string_ret_t{trim(left_part), trim(right_part)};
}

void integrate1d_conf::file_to_options_map(std::istream& cf){
    std::string line;
    while( std::getline(cf, line) ){
        auto pr = parse_string(line);
        if(pr.first.empty()) {
            if(!pr.second)
                continue;
            else
                throw wrong_option_line_error{"Wrong config line -- no option name: " + line}; // "=..."
        } else if(!pr.second){
            throw wrong_option_line_error{"Wrong config line -- no '=': " + line}; // "abc" -- no '='
        } else if(pr.second->empty()){
            throw wrong_option_arg_error{"Wrong config option value: " + line}; // "key="
        }
        if( options_map.count(pr.first) ){
            throw option_duplicated_error{"Duplicated option name: " + pr.first + " = "
                                          + *pr.second + "; prev. val: " + options_map[pr.first] };
        }
        options_map[pr.first] = *pr.second;
    }
}



void integrate1d_conf::read_conf(std::istream& cf)
{
    using namespace std::literals::string_literals; // Так using namespace - можна.
                                                    // Воно дає можливість створювати стрічкові літерали,
                                                    // "abc"s має тип std::string, а не const char*, як "abc".
    file_to_options_map(cf);
    //! І тут можу запитатися про подробиці синтаксису
    rel_err = get_option_from_map<decltype(rel_err)>("rel_err"s);
    abs_err = get_option_from_map<decltype(abs_err)>("abs_err"s);
    x1 = get_option_from_map<decltype(x1)>("x_start"s);
    x2 = get_option_from_map<decltype(x2)>("x_end"s);
    initial_steps = get_option_from_map<decltype(initial_steps)>("init_steps"s);
    max_iters = get_option_from_map<decltype(initial_steps)>("max_iter"s);
    max_steps = initial_steps * (2ull << max_iters); // ull тут важливий, без того (2 << ...) матиме тип int,
                                    // коректіше було б static_cast<size_t>(2) писати -- но то якось дуже вже канцелярит.

    validate_conf();
}

void integrate1d_conf::validate_conf(){
    if( x2 <= x1 ) {
        throw wrong_logical_option_arg_error("x1 should be <= x2");
    }
    if( initial_steps < 10 ) {
        throw wrong_logical_option_arg_error("Too few initial_steps");
    }
    if( max_iters < 3 ) {
        throw wrong_logical_option_arg_error("Too small max_iters");
    }
    if( max_iters > 30 ){
        throw wrong_logical_option_arg_error("Too large max_iters");
    }
    if( abs_err <=0 || rel_err <= 0 )
    {
        throw wrong_logical_option_arg_error("Errors should be positive");
    }
}

integrate1d_conf::integrate1d_conf(std::istream& fs){
    read_conf(fs);
    options_map = options_map_t{}; // Оптимізація -- оскільки конструювання одноразове, більше цей об'єкт не знадобиться.
                                   // options_map.clear() тут не звільнить внутрішні структури об'єкта.
}
