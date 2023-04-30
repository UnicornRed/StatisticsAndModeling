#pragma once

#include <iostream>
#include <random>
#include <chrono>

extern unsigned int seed;
extern std::default_random_engine generator;

/// @brief Класс, содержащий параметры отрицательно-биномиального распределения и вычисляющий его вероятности. 
class NB_distr
{
private:
    double p;
    double prob_now;
    size_t k, culc_n;
public:
    /// @brief Конструктор по параметрам распределения: вероятности успеха и количества успехов.
    /// @param _p Вероятность успеха.
    /// @param _k Количество успехов.
    NB_distr(double _p = 0.8, size_t _k = 10);

    /// @brief 
    /// @return Вероятность успеха.
    inline double get_p() const { return p; }

    /// @brief 
    /// @return Количество успехов.
    inline size_t get_k() const { return k; }

    /// @brief 
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

/// @brief Базовый класс моделирования распределени.
class Sample
{
protected:
    size_t n;
    NB_distr* d;
    size_t* sam;

    void swap(Sample& s);
public:
    /// @brief 
    /// @param _n 
    /// @param _d 
    Sample(size_t _n, NB_distr* _d);

    /// @brief 
    /// @param s 
    Sample(const Sample& s);

    /// @brief 
    /// @param s 
    Sample(Sample&& s);

    /// @brief 
    /// @return 
    inline size_t get_n() const { return n; }

    /// @brief 
    /// @return 
    virtual const char* get_name() const = 0;

    /// @brief 
    void simulate();

    /// @brief 
    /// @return 
    virtual size_t simulate_one() = 0;

    /// @brief 
    /// @param _n 
    void change_param(size_t _n);

    /// @brief 
    /// @param i 
    /// @return 
    size_t operator[] (int i) const;

    virtual ~Sample() = 0;
};

/// @brief 
class Sample_Table : public Sample
{
private:
    double* sum_distr;
    size_t num_sum_distr;

    /// @brief 
    /// @param d 
    /// @return 
    size_t max_num(NB_distr* d) const;

    /// @brief 
    void make_sum_distr();

    void swap(Sample_Table& s);
public:
    /// @brief 
    /// @param _n 
    /// @param _d 
    Sample_Table(size_t _n, NB_distr* _d);

    /// @brief 
    /// @param s 
    Sample_Table(const Sample_Table& s);

    /// @brief 
    /// @param s 
    Sample_Table(Sample_Table&& s);

    /// @brief 
    /// @param s 
    /// @return 
    Sample_Table& operator=(Sample_Table s);

    /// @brief 
    /// @return 
    virtual const char* get_name() const override;

    /// @brief 
    /// @param _n 
    void change_param(size_t _n);

    /// @brief 
    /// @return 
    virtual size_t simulate_one() override;

    ~Sample_Table();
};

/// @brief 
class Sample_Bernulli : public Sample
{
public:
    /// @brief 
    /// @param _n 
    /// @param _d 
    Sample_Bernulli(size_t _n, NB_distr* _d);

    /// @brief 
    /// @param s 
    Sample_Bernulli(const Sample_Bernulli& s);

    /// @brief 
    /// @param s 
    Sample_Bernulli(Sample_Bernulli&& s);

    /// @brief 
    /// @param s 
    /// @return 
    Sample_Bernulli& operator=(Sample_Bernulli s);

    /// @brief 
    /// @return 
    virtual const char* get_name() const override;

    /// @brief 
    /// @return 
    virtual size_t simulate_one() override;
};

class ChiSqHist
{
private:
    NB_distr* d;
    Sample* s;
    size_t df;
    double chi_sq_stat, p_value;

    size_t num_freq;
    size_t* exp_freq;
    double* th_freq;

    /// @brief 
    /// @param exp_freq_merge 
    /// @param th_freq_merge 
    /// @return 
    size_t merge(size_t* exp_freq_merge, double* th_freq_merge);
    /// @brief 
    /// @param new_num 
    /// @param exp_freq_merge 
    /// @param th_freq_merge 
    /// @return 
    double chi_square(size_t new_num, size_t* exp_freq_merge, double* th_freq_merge);

    void swap(ChiSqHist& c);
public:
    /// @brief 
    /// @param _d 
    /// @param _s 
    ChiSqHist(NB_distr* _d, Sample* _s);

    /// @brief 
    /// @param c 
    ChiSqHist(ChiSqHist& c);

    /// @brief 
    /// @param c 
    ChiSqHist(ChiSqHist&& c);

    /// @brief 
    /// @param c 
    /// @return 
    ChiSqHist& operator=(ChiSqHist c);

    /// @brief 
    /// @return 
    inline double get_p_value() const { return p_value; }

    /// @brief 
    /// @return 
    inline size_t get_num_freq() const { return num_freq; }

    /// @brief 
    /// @return 
    inline const size_t* get_exp_freq() const { return exp_freq; }
    /// @brief 
    /// @return 
    inline const double* get_th_freq() const { return th_freq; }

    /// @brief 
    /// @param _d 
    /// @param _s 
    void set_data(NB_distr* _d, Sample* _s);

    /// @brief 
    void calc_th_freq();
    /// @brief 
    void calc_exp_freq();

    /// @brief 
    void calc_chi_sq();

    ~ChiSqHist();
};

/// @brief 
class Doc_NB
{
private:
    NB_distr d0, d1;
    NB_distr* d_now;
    Sample* s;
    ChiSqHist *chisq;

    size_t num_p_value;
    double sign_lv;
    double* p_value_arr;

    void swap(Doc_NB& d);
public:
    /// @brief 
    Doc_NB();

    /// @brief 
    /// @param d 
    Doc_NB(Doc_NB &d);

    /// @brief 
    /// @param d 
    Doc_NB(Doc_NB &&d);

    /// @brief 
    /// @param d 
    /// @return 
    Doc_NB& operator=(Doc_NB d);

    /// @brief 
    /// @return 
    inline Sample* get_Sample() const { return s; }
    /// @brief 
    /// @return 
    inline size_t get_num_p_value() const { return num_p_value; }
    /// @brief 
    /// @return 
    inline double get_sign_lv() const { return sign_lv; }
    /// @brief 
    /// @return 
    inline ChiSqHist* get_chi_sq() { return chisq; }
    /// @brief 
    /// @return 
    inline const NB_distr* get_d0() const { return &d0; }
    /// @brief 
    /// @return 
    inline const NB_distr* get_d1() const { return &d1; }

    /// @brief 
    void make_p_value();

    /// @brief 
    /// @param i 
    /// @return 
    double get_p_value(size_t i) const;

    /// @brief 
    /// @param _d0 
    /// @param _d1 
    /// @param _num_p_value 
    /// @param _n 
    /// @param _sign_lv 
    void change_param(NB_distr _d0, NB_distr _d1, size_t _num_p_value, size_t _n, double _sign_lv);

    /// @brief 
    void set_table_method();
    /// @brief 
    void set_bernulli_method();

    /// @brief 
    void set_hyp_d0();
    /// @brief 
    void set_hyp_d1();

    ~Doc_NB();
};