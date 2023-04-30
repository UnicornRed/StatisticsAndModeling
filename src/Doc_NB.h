#pragma once

#include <iostream>
#include <random>
#include <chrono>

extern unsigned int seed;
extern std::default_random_engine generator;

/// @class NB_distr
/// @brief Класс, содержащий параметры отрицательно-биномиального распределения и вычисляющий его вероятности. 
class NB_distr
{
private:
    /// @brief Параметр вероятности успеха.
    double p;
    double prob_now;
    size_t k, culc_n;
public:
    /// @brief Конструктор по параметрам вероятности успеха и количество успехов.
    /// @param _p Вероятность успеха.
    /// @param _k Количество успехов.
    NB_distr(double _p = 0.8, size_t _k = 10);

    /// @brief 
    /// @return 
    inline double get_p() const { return p; }

    /// @brief 
    /// @return 
    inline size_t get_k() const { return k; }

    /// @brief 
    /// @return 
    inline double get_prob_now() const { return prob_now; }


    double next_prob();

    const char* name_of_distr() const;

    void reset();
};

class Sample
{
protected:
    size_t n;
    NB_distr* d;
    size_t* sam;

    void swap(Sample& s);
public:
    Sample(size_t _n, NB_distr* _d);

    Sample(const Sample& s);

    Sample(Sample&& s);

    inline size_t get_n() const { return n; }

    virtual const char* get_name() const = 0;

    void simulate();

    virtual size_t simulate_one() = 0;

    void change_param(size_t _n);

    size_t operator[] (int i) const;

    virtual ~Sample() = 0;
};

class Sample_Table : public Sample
{
private:
    double* sum_distr;
    size_t num_sum_distr;

    size_t max_num(NB_distr* d) const;
    void make_sum_distr();

    void swap(Sample_Table& s);
public:
    Sample_Table(size_t _n, NB_distr* _d);

    Sample_Table(const Sample_Table& s);

    Sample_Table(Sample_Table&& s);

    Sample_Table& operator=(Sample_Table s);

    virtual const char* get_name() const override;

    void change_param(size_t _n);

    virtual size_t simulate_one() override;

    ~Sample_Table();
};

class Sample_Bernulli : public Sample
{
public:
    Sample_Bernulli(size_t _n, NB_distr* _d);

    Sample_Bernulli(const Sample_Bernulli& s);

    Sample_Bernulli(Sample_Bernulli&& s);

    Sample_Bernulli& operator=(Sample_Bernulli s);

    virtual const char* get_name() const override;

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

    size_t merge(size_t* exp_freq_merge, double* th_freq_merge);
    double chi_square(size_t new_num, size_t* exp_freq_merge, double* th_freq_merge);

    void swap(ChiSqHist& c);
public:
    ChiSqHist(NB_distr* _d, Sample* _s);

    ChiSqHist(ChiSqHist& c);

    ChiSqHist(ChiSqHist&& c);

    ChiSqHist& operator=(ChiSqHist c);

    inline double get_p_value() const { return p_value; }

    inline size_t get_num_freq() const { return num_freq; }

    inline const size_t* get_exp_freq() const { return exp_freq; }
    inline const double* get_th_freq() const { return th_freq; }

    void set_data(NB_distr* _d, Sample* _s);

    void calc_th_freq();
    void calc_exp_freq();

    void calc_chi_sq();

    ~ChiSqHist();
};

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
public:
    Doc_NB();

    inline Sample* get_Sample() const { return s; }
    inline size_t get_num_p_value() const { return num_p_value; }
    inline double get_sign_lv() const { return sign_lv; }
    inline ChiSqHist* get_chi_sq() { return chisq; }
    inline const NB_distr* get_d0() const { return &d0; }
    inline const NB_distr* get_d1() const { return &d1; }

    void make_p_value();

    double get_p_value(size_t i) const;

    void change_param(NB_distr _d0, NB_distr _d1, size_t _num_p_value, size_t _n, double _sign_lv);

    void set_table_method();
    void set_bernulli_method();

    void set_hyp_d0();
    void set_hyp_d1();

    ~Doc_NB();
};