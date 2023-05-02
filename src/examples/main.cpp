#include <iostream>
#include "../Doc_NB.h"

int main(int argc, char** argv){
    // Инициализация распределений по умолчанию и с параметрами.
    NB_distr d0, d1(0.84, 11);

    // Название распределения.
    std::cout << d0.name_of_distr() << "\n";

    // Инициализация метода моделирования Бернулли. 
    Sample_Bernulli sam_ber(100, &d0);

    // Моделирование выборки.
    sam_ber.simulate();

    // Название метода.
    std::cout << sam_ber.get_name() << ": ";

    // Выборка.
    for (size_t i = 0; i < sam_ber.get_n(); ++i)
        std::cout << sam_ber[i] << " ";
    
    std::cout << "\n";

    // Изменение размера выборки.
    sam_ber.change_param(10);

    sam_ber.simulate();

    std::cout << sam_ber.get_name() << ": ";

    for (size_t i = 0; i < sam_ber.get_n(); ++i)
        std::cout << sam_ber[i] << " ";
    
    std::cout << "\n";

    // Инициализация табличного метода моделирования. 
    Sample_Table sam_table(70, &d0);

    sam_table.simulate();

    std::cout << sam_table.get_name() << ": ";

    for (size_t i = 0; i < sam_table.get_n(); ++i)
        std::cout << sam_table[i] << " ";
    
    std::cout << "\n";

    // Инициализация класса моделирования выборок p-value.
    Doc_NB doc;

    // Изменение параметров класса моделирования выборок p-value.
    doc.change_param(d0, d1, 100, 120, 0.1);

    // Установка в качестве распределения альтернативной гипотезы.
    doc.set_hyp_d1();
    // Установка в качестве метода моделирования метода Бернулли.
    doc.set_bernulli_method();

    // Моделирование выборки p-value.
    doc.make_p_value();

    // Получение доступа к классу хи квадрат.
    ChiSqHist* chi_sq = doc.get_chi_sq();

    // Вывод вычисленного p-value.
    std::cout << "P-value: " << chi_sq->get_p_value() << "\n";

    // Вычисления таблицы теоретичесикх вероятностей.
    chi_sq->calc_th_freq();

    std::cout << "Теоретические вероятности: ";

    // Вывод теоретических вероятностей.
    for (size_t i = 0; i < chi_sq->get_num_freq(); ++i)
        std::cout << chi_sq->get_th_freq()[i] << " ";
    
    std::cout << "\n";

    // Вычисление таблицы эмперичесикх частот.
    chi_sq->calc_exp_freq();

    std::cout << "Эмперические частоты: ";

    // Вывод эмперических частот.
    for (size_t i = 0; i < chi_sq->get_num_freq(); ++i)
        std::cout << chi_sq->get_exp_freq()[i] << " ";
    
    std::cout << "\n";

    std::cout << "Выборка p-value: ";

    // Вывод выборки p-value.
    for (size_t i = 0; i < doc.get_num_p_value(); ++i)
        std::cout << doc.get_p_value(i) << " ";

    std::cout << "\n";

    // Установка в качестве распределения нулевой гипотезы.
    doc.set_hyp_d0();
    // Установка в качестве метода моделирования табличного метода.
    doc.set_table_method();

    doc.make_p_value();

    std::cout << "Выборка p-value: ";

    for (size_t i = 0; i < doc.get_num_p_value(); ++i)
        std::cout << doc.get_p_value(i) << " ";

    std::cout << "\n";

    return 0;
}