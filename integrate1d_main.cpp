// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//! УВАГА! Оскільки ця реалізація сильно спрощена, вона відповідає не всім вимогам до практичної.
//! УВАГА! Якщо використовуєте код парсингу звідси, ви повинні твердо розуміти
//!        і могти розповісти, як він функціонує. Без цього робота може бути не зарахованою.
#include <iostream>
#include <fstream>
#include <cmath>
#include <memory> // For unique_ptr
#include <optional>

#include "time_measurements.hpp"
#include "integrate1d_conf.hpp"


double func_to_integrate(const double& x) {
    return x*x;
}

template<typename func_T>
double integrate(func_T func, double x1, const double& x2, size_t steps)
{
    double res = 0;
    double delta_x = (x2 - x1)/steps; //-V113 // PVS studio warning here is a false positive
    while(x1<x2)
    {
        res += func(x1);
        x1 += delta_x;
    }
    return res * delta_x;
}


//! Якщо передано ім'я файлу -- він використовується. Якщо ні -- шукається файл іменем integration_1d.cfg.
int main(int argc, char* argv[])
{
    std::string filename("integration_1d.cfg");
    if(argc == 2)
        filename = argv[1];
    if(argc > 2) {
        std::cerr << "Too many arguments. Usage: \n"
                "<program>\n"
                "or\n"
                "<program> <config-filename>\n" << std::endl;
        return 1;
    }
    std::ifstream config_stream(filename);
    if(!config_stream.is_open()) {
        std::cerr << "Failed to open configuration file " << filename << std::endl;
        return 2;
    }

    std::unique_ptr<integrate1d_conf> config_ptr; // Вказівник -- щоб забезпечити потрібну оброку помилок (return 3 тут),
                                                  // але не загортати в try-catch все тіло програми нижче.
                                                  // А розумний -- щоб потім не звільняти.
    try{
        config_ptr = std::make_unique<integrate1d_conf>(config_stream);
    }catch (std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 3;
    }

    std::cout << "0" << std::endl; // Вивести варіант

    auto before = get_current_time_fenced();

    size_t steps = config_ptr->initial_steps;
    double cur_res = integrate(func_to_integrate, config_ptr->x1, config_ptr->x2, steps);
    double prev_res = cur_res;
    bool to_continue = true;
    double abs_err = -1; // Just guard value
    double rel_err = -1; // Just guard value
    while( to_continue )
    {
#ifdef PRINT_INTERMEDIATE_STEPS
        std::cout << cur_res << " : " << steps << " steps" << std::endl;
#endif
        prev_res = cur_res;
        steps *= 2;
        cur_res = integrate(func_to_integrate, config_ptr->x1, config_ptr->x2, steps);
        abs_err = fabs(cur_res-prev_res);
        rel_err = fabs( (cur_res-prev_res)/cur_res );
#ifdef PRINT_INTERMEDIATE_STEPS
        std::cout << '\t' << "Abs err : rel err " << abs_err << " : " << rel_err << std::endl;
#endif

        to_continue = ( abs_err > config_ptr->abs_err );
        to_continue = to_continue && ( rel_err > config_ptr->rel_err );
        to_continue = to_continue && (steps < config_ptr->max_steps);
    }

    auto time_to_calculate = get_current_time_fenced() - before;

#ifdef PRINT_INTERMEDIATE_STEPS
    std::cout << "Result = " << cur_res << std::endl;
    std::cout << "Abs err = " << abs_err << std::endl;
    std::cout << "rel err = " << rel_err << std::endl;
    std::cout << "Time: " << to_us(time_to_calculate)  << std::endl;
#else
    std::cout << cur_res << std::endl;
    std::cout << abs_err << std::endl;
    std::cout << rel_err << std::endl;
    std::cout << to_us(time_to_calculate)  << std::endl;
#endif
    if( ( abs_err > config_ptr->abs_err ) && ( rel_err > config_ptr->rel_err ) ){
        std::cerr << "Requested errors not reached within allowed iterations limit" << std::endl;
        return 100;
    }
    return 0;
}
