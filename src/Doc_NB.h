/// @mainpage NB distribution
/// @author  Олейник Михаил
/// @date 03.05.2023
///
/// Основная страница набора классов по моделированию отрицательно-биномиального распределения.
/// Их возможности позволяют
/// 1. Моделировать выборки случайных величин, распределённых по отрицательно биномиальному закону, методами Бернулли и табличным; 
/// 2. Менять их параметры (такие как вероятность успеха, количество успехов, размер выборки);
/// 3. Получать теоретические вероятности и эмперические частоты;
/// 4. Считать критерий \f$ \chi ^2 \f$ и p-value;
/// 5. Моделировать выборку p-value и менять для этого метод моделирования, размер выборки, гипотезу, на основе которой моделируется выборка.
/// 
/// @ref NB_distr - класс распределения, хранящий параметры отрицательно-биномиального распределения.
///
/// @ref Sample - базовый класс для моделирования выборок.
///
/// @ref Sample_Table - класс моделирования выборок табличным методом.
/// 
/// @ref Sample_Bernulli - класс моделирования выборок методом Бернулли.
///
/// @ref ChiSqHist - класс критерия согласия \f$ \chi ^2 \f$.
///
/// @ref Doc_NB - класс моделирования выборок p-value.
///
/// Пример использования классов:
/// @include main.cpp
/// @example main.cpp

/// @file
/// @brief Моделирование отрицательно-биномиального распределения.
/// @details  файл содержит классы, используемые для моделирования выбоки отрицательно-биномиального распределения.
/// Они позволяют получать выборки, изменять параметры распределений, выбирать метод моделирования (табличный или Бернулли); 
/// вычислять эмперические частоты и теоретические вероятности, критерий согласия \f$ \chi ^2 \f$ и p-value на его основе; 
/// строить выборки из p-value.
#pragma once

#include <iostream>
#include <random>
#include <chrono>

/// @brief Инициация генератора случайных чисел.
extern unsigned int seed;
/// @brief Генератор псевдослучайных чисел.
extern std::default_random_engine generator;

/// @brief Класс отрицательно-биномиального распределения.
/// @details Класс, содержащий параметры отрицательно-биномиального распределения и вычисляющий его вероятности. 
class NB_distr
{
private:
    /// @brief Вероятность успеха.
    double p;
    /// @brief Текущая вероятность.
    double prob_now;
    /// @brief Количество успехов.
    size_t k;
    /// @brief Номер вычесленной вероятности.
    size_t culc_n;
public:
    /// @brief Конструктор по параметрам распределения: вероятности успеха и количества успехов.
    /// @param[in] _p Вероятность успеха.
    /// @param[in] _k Количество успехов.
    NB_distr(double _p = 0.8, size_t _k = 10);

    /// @brief Доступ к p.
    /// @return Вероятность успеха.
    inline double get_p() const { return p; }

    /// @brief Доступ к k.
    /// @return Количество успехов.
    inline size_t get_k() const { return k; }

    /// @brief Доступ к prob_now.
    /// @return Текущую вычисленную вероятность.
    inline double get_prob_now() const { return prob_now; }

    /// @brief Вычисляет следующую вероятность распределения.
    /// @return Следующую вероятность распределения.
    double next_prob();

    /// @brief Функция содержащая название распределения.
    /// @return Строку "Negative Binomial Distribution".
    const char* name_of_distr() const;

    /// @brief Обнуляет вычисление вероятностей.
    void reset();
};

/// @brief Класс моделирования распределений.
/// @details Базовый класс для моделирования распределений, содержащий размер выборки, указатель на распределение и массив выборки.
/// Позволяет генерировать выборку, изменять её размер и получать её параметры и название метода.
class Sample
{
protected:
    /// @brief Размер выборки.
    size_t n;
    /// @brief Указатель на класс распределения.
    NB_distr* d;
    /// @brief Массив с выборкой.
    size_t* sam;

    /// @brief Осуществляет обмен полями между объектом класса и переданным s.
    /// @param s Объект класса Sample.
    void swap(Sample& s);
public:
    /// @brief Конструктор модирования распределений по размеру выборки и распределению.
    /// @param[in] _n Размер выборки.
    /// @param[in] _d Указатель на распределение.
    Sample(size_t _n, NB_distr* _d);

    /// @brief Конструктор копирования.
    /// @param[in] s Объект класса Sample.
    Sample(const Sample& s);

    /// @brief Конструктор перемещения.
    /// @param[in] s Объект класса Sample.
    Sample(Sample&& s);

    /// @brief Доступ к размеру выборки.
    /// @return Размер выборки.
    inline size_t get_n() const { return n; }

    /// @brief Доступ к названию методу моделирования.
    /// @return Константную строку с названием метода моделирования.
    virtual const char* get_name() const = 0;

    /// @brief Симулирует выборку, записывая её во внутренний массив.
    void simulate();

    /// @brief Симулирует один элемент выборки.
    /// @return Значение элемента выборки.
    virtual size_t simulate_one() = 0;

    /// @brief Изменяет размер выборки.
    /// @param[in] _n Размер выборки.
    void change_param(size_t _n);

    /// @brief Доступ к элементам выборки по индексу.
    /// @param[in] i Индекс элемента выборки.
    /// @return Значение элемента выборки. 
    size_t operator[] (int i) const;

    /// @brief Деструктор Sample.
    virtual ~Sample() = 0;
};

/// @brief Класс моделирования распределения табличным методом.
/// @details Дочерний к Sample класс для моделирования распределений табличным методом, содержащий размер выборки, 
/// указатель на распределение, массив выборки и массив суммированных вероятностей.
/// Позволяет генерировать выборку, изменять её размер и получать её параметры и название метода.
class Sample_Table : public Sample
{
private:
    /// @brief Массив суммированных вероятностей.
    double* sum_distr;
    /// @brief Размер массива суммированных вероятностей.
    size_t num_sum_distr;

    /// @brief Вычисляет размер массива суммированных вероятностей до значений вероятностей, равных машинному нулю.
    /// @param d Указатель на распределение.
    /// @return Размер массива суммированных вероятностей.
    size_t max_num(NB_distr* d) const;

    /// @brief Создаёт таблицу для метода (массив суммированных вероятностей).
    void make_sum_distr();

    /// @brief Осуществляет обмен полями между объектом класса и переданным s.
    /// @param s Объект класса Sample_Table.
    void swap(Sample_Table& s);
public:
    /// @brief Конструктор модирования распределений табличным методом по размеру выборки и распределению.
    /// @param[in] _n Размер выборки.
    /// @param[in] _d Указатель на распределение.
    Sample_Table(size_t _n, NB_distr* _d);

    /// @brief Конструктор копирования.
    /// @param[in] s Объект класса Sample_Table.
    Sample_Table(const Sample_Table& s);

    /// @brief Конструктор перемещения.
    /// @param[in] s Объект класса Sample_Table.
    Sample_Table(Sample_Table&& s);

    /// @brief Оператор присваивания для класса Sample_Table.
    /// @param[in] s Объект класса Sample_Table.
    /// @return Результат присваивания, объект класса Sample_Table.
    Sample_Table& operator=(Sample_Table s);

    /// @brief Доступ к названию методу моделирования.
    /// @return Константную строку "Table Method".
    virtual const char* get_name() const override;

    /// @brief Изменяет размер выборки.
    /// @param[in] _n Размер выборки.
    void change_param(size_t _n);

    /// @brief Симулирует один элемент выборки.
    /// @return Значение элемента выборки.
    virtual size_t simulate_one() override;

    /// @brief Деструктор Sample_Table.
    ~Sample_Table();
};

/// @brief Класс моделирования распределения методом Бернулли.
/// @details Дочерний к Sample класс для моделирования распределений методом Бернулли, содержащий размер выборки, 
/// указатель на распределение и массив выборки.
/// Позволяет генерировать выборку, изменять её размер и получать её параметры и название метода.
class Sample_Bernulli : public Sample
{
public:
    /// @brief Конструктор модирования распределений методом Бернулли по размеру выборки и распределению.
    /// @param[in] _n Размер выборки.
    /// @param[in] _d Указатель на распределение.
    Sample_Bernulli(size_t _n, NB_distr* _d);

    /// @brief Конструктор копирования.
    /// @param[in] s Объект класса Sample_Bernulli.
    Sample_Bernulli(const Sample_Bernulli& s);

    /// @brief Конструктор перемещения.
    /// @param[in] s Объект класса Sample_Bernulli.
    Sample_Bernulli(Sample_Bernulli&& s);

    /// @brief Оператор присваивания для класса Sample_Bernulli.
    /// @param[in] s Объект класса Sample_Bernulli.
    /// @return Результат присваивания, объект класса Sample_Bernulli.
    Sample_Bernulli& operator=(Sample_Bernulli s);

    /// @brief Доступ к названию методу моделирования.
    /// @return Константную строку "Bernulli Method".
    virtual const char* get_name() const override;

    /// @brief Симулирует один элемент выборки.
    /// @return Значение элемента выборки.
    virtual size_t simulate_one() override;
};

/// @brief Класс критерия согласия.
/// @details Класс, который хранит вычисленные теоретические и эмперические вероятности распределения и выборки, вычисляет критерий \f$ \chi ^2 \f$ 
/// и значение p-value. Позволяет сменить распределение и метод моделирования.
class ChiSqHist
{
private:
    /// @brief Указатель на распределение.
    NB_distr* d;
    /// @brief Указатель на метод моделирования выборки.
    Sample* s;
    /// @brief Степени свободы.
    size_t df;
    /// @brief Значение критерия \f$ \chi ^2 \f$.
    double chi_sq_stat;
    /// @brief Значение p-value.
    double p_value;

    /// @brief Размер массива с вероятностями.
    size_t num_freq;
    /// @brief Массив эмперических частот.
    size_t* exp_freq;
    /// @brief Теоретические вероятности.
    double* th_freq;

    /// @brief Объединяет состояния, чтобы критерий был применим.
    /// @param[in, out] exp_freq_merge Массив эмперических частот для объединения.
    /// @param[in, out] th_freq_merge Массив теоретических вероятностей для объединения.
    /// @return Размер объединённых массивов.
    size_t merge(size_t* exp_freq_merge, double* th_freq_merge);
    /// @brief Вычисляет значение критерия \f$ \chi ^2 \f$.
    /// @param[in] new_num Размер объединённых массивов.
    /// @param[in] exp_freq_merge Объединённый массив эмперических частот.
    /// @param[in] th_freq_merge Объединённый массив теоретических вероятностей.
    /// @return Значение критерия для данной выборки.
    double chi_square(size_t new_num, size_t* exp_freq_merge, double* th_freq_merge);

    /// @brief Осуществляет обмен полями между объектом класса и переданным c.
    /// @param[in, out] c Объект класса ChiSqHist.
    void swap(ChiSqHist& c);
public:
    /// @brief Конструктор критерия согласия по указателям на распределение и метод моделирования.
    /// @param[in] _d Указатель на распределение.
    /// @param[in] _s Указатель на метод моделирования.
    ChiSqHist(NB_distr* _d, Sample* _s);

    /// @brief Конструктор копирования.
    /// @param[in] c Объект класса ChiSqHist.
    ChiSqHist(ChiSqHist& c);

    /// @brief Конструктор перемещения.
    /// @param[in] c Объект класса ChiSqHist.
    ChiSqHist(ChiSqHist&& c);

    /// @brief Оператор присваивания для класса ChiSqHist.
    /// @param[in] c Объект класса ChiSqHist.
    /// @return Результат присваивания, объект класса ChiSqHist.
    ChiSqHist& operator=(ChiSqHist c);

    /// @brief Доступ к p-value.
    /// @return Значение p-value.
    inline double get_p_value() const { return p_value; }

    /// @brief Доступ к размеру массива с вероятностями.
    /// @return Размер массива с вероятностями.
    inline size_t get_num_freq() const { return num_freq; }

    /// @brief Доступ к эмперическим частотам.
    /// @return Указатель на массив эмперичесикх частот.
    inline const size_t* get_exp_freq() const { return exp_freq; }
    /// @brief Доступ к теоретическим вероятностям.
    /// @return Указатель на массив теоретических вероятностей.
    inline const double* get_th_freq() const { return th_freq; }

    /// @brief Изменение распределения и метода моделирования, на основе, которых вычисляется критерий.
    /// @param[in] _d Указатель на распределение.
    /// @param[in] _s Указатель на метод моделирования.
    void set_data(NB_distr* _d, Sample* _s);

    /// @brief Составление таблицы теоретических вероятностей.
    void calc_th_freq();
    /// @brief Составление таблицы эмперических частот.
    void calc_exp_freq();

    /// @brief Вычисление критерия \f$ \chi ^2 \f$ и p-value.
    void calc_chi_sq();

    /// @brief Деструктор ChiSqHist.
    ~ChiSqHist();
};

/// @brief Класс моделирования и гипотез.
/// @details Класс, который хранит нулевую и альтернативную гипотезы, метод моделирования, объект критерия \f$ \chi ^2 \f$, выборку p_value, 
/// уровень значимости. Позволяет менять параметры распеределений, методы моделирования, критерий и размер выборки p-value.
/// Моделирует выборку p-value.
class Doc_NB
{
private:
    /// @brief Распределение нулевой гипотезы.
    NB_distr d0;
    /// @brief Распределение альтернативной гипотезы.
    NB_distr d1;
    /// @brief Указатель на гипотезу, на основе которой моделируется выборка.
    NB_distr* d_now;
    /// @brief Указатель на метод моделирования.
    Sample* s;
    /// @brief Указатель на критерий согласия \f$ \chi ^2 \f$.
    ChiSqHist *chisq;

    /// @brief Размер выборки p-value.
    size_t num_p_value;
    /// @brief Уровень значимости.
    double sign_lv;
    /// @brief Выборка p-value.
    double* p_value_arr;

    /// @brief Осуществляет обмен полями между объектом класса и переданным d.
    /// @param[in, out] c Объект класса Doc_NB.
    void swap(Doc_NB& d);
public:
    /// @brief Конструктор класса моделирования и гипотез.
    Doc_NB();

    /// @brief Конструктор копирования.
    /// @param[in] d Объект класса Doc_NB.
    Doc_NB(Doc_NB &d);

    /// @brief Конструктор перемещения.
    /// @param[in] d Объект класса Doc_NB.
    Doc_NB(Doc_NB &&d);

    /// @brief Оператор присваивания для класса Doc_NB.
    /// @param[in] d Объект класса Doc_NB.
    /// @return Результат присваивания, объект класса Doc_NB.
    Doc_NB& operator=(Doc_NB d);

    /// @brief Доступ к методу моделирования.
    /// @return Указатель на метод моделирования.
    inline Sample* get_Sample() const { return s; }
    /// @brief Доступ к размеру выборки p-value.
    /// @return Размер выборки p-value.
    inline size_t get_num_p_value() const { return num_p_value; }
    /// @brief Доступ к уровню значимости.
    /// @return Уровень значимости.
    inline double get_sign_lv() const { return sign_lv; }
    /// @brief Доступ к критерию согласию \f$ \chi ^2 \f$.
    /// @return Указатель на критерий согласия \f$ \chi ^2 \f$.
    inline ChiSqHist* get_chi_sq() { return chisq; }
    /// @brief Доступ к нулевой гипотезе.
    /// @return Указатель на нулевую гипотезу.
    inline const NB_distr* get_d0() const { return &d0; }
    /// @brief Доступ к альтернативной гипотезе.
    /// @return Указатель на альтернативную гипотезу.
    inline const NB_distr* get_d1() const { return &d1; }

    /// @brief Моделирование выборки p-value.
    void make_p_value();

    /// @brief Доступ к элементу выборки p-value.
    /// @param[in] i Индекс элемента выборки p-value.
    /// @return Значение элемента выборки p-value.
    double get_p_value(size_t i) const;

    /// @brief Позволяет изменить параметры гипотез, размер выборки p-value, размер выборки и уровень значимости.
    /// @param[in] _d0 Нулевая гипотеза.
    /// @param[in] _d1 Альтернативная гипотеза.
    /// @param[in] _num_p_value Размер выборки p-value.
    /// @param[in] _n Размер выборки.
    /// @param[in] _sign_lv Уровень значимости.
    void change_param(NB_distr _d0, NB_distr _d1, size_t _num_p_value, size_t _n, double _sign_lv);

    /// @brief Установка в качестве метода моделирования табличного метода.
    void set_table_method();
    /// @brief Установка в качестве метода моделирования метода Бернулли.
    void set_bernulli_method();

    /// @brief Установка для моделирования нулевую гипотезу.
    void set_hyp_d0();
    /// @brief Установка для моделирования альтернативную гипотезу.
    void set_hyp_d1();

    /// @brief Деструктор Doc_NB.
    ~Doc_NB();
};