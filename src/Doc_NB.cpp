#include <algorithm>
#include <cstring>
#include "probdist.h"
#include "Doc_NB.h"

unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator;
std::uniform_real_distribution<double> distribution(0.0, 1.0);

double fast_deg(double x, size_t alpha)
{
    double res = 1;

    while (alpha != 0)
    {
        if (alpha & 1)
            res *= x;
        
        x *= x;
        alpha >>= 1;
    }

    return res;
}

NB_distr::NB_distr(double _p, size_t _k) : k(_k), p(_p), culc_n(0), prob_now(fast_deg(_p, _k))
{
    if (p >= 1 || p <= 0)
        p = 0.5;
}

double NB_distr::next_prob()
{
    prob_now = prob_now * (k + culc_n) * (1 - p) / (culc_n + 1);
    ++culc_n;

    return prob_now;
}

const char* NB_distr::name_of_distr() const
{
    return "Negative Binomial Distribution";
}

void NB_distr::reset()
{
    prob_now = fast_deg(p, k);
    culc_n = 0;
}

void Sample::swap(Sample& s)
{
    size_t buff_n = n;
    n = s.n;
    s.n = buff_n;

    NB_distr* buff_d = d;
    d = s.d;
    s.d = buff_d;

    size_t* buff_sam = sam;
    sam = s.sam;
    s.sam = buff_sam;
}

Sample::Sample(size_t _n, NB_distr* _d) : n(_n), d(_d)
{
    sam = new size_t[n];
}

Sample::Sample(const Sample& s) : n(s.n), d(s.d)
{
    sam = new size_t[n];

    for (size_t i = 0; i < n; ++i)
        sam[i] = s.sam[i];
}

Sample::Sample(Sample&& s)
{
    this->swap(s);
}

void Sample::simulate()
{
    for (size_t i = 0; i < n; ++i)
        sam[i] = simulate_one();
}

void Sample::change_param(size_t _n)
{
    n = _n;

    delete[] sam;
    sam = new size_t[n];
}

size_t Sample::operator[] (int i) const
{
    return sam[i];
}

Sample::~Sample()
{
    delete[] sam;
}

size_t Sample_Table::max_num(NB_distr* d) const
{
    size_t j = 0;
    double v = fast_deg(d->get_p(), d->get_k());

    while (v + 1.0 != 1.0)
    {
        ++j;
        v = v * (d->get_k() + j) / j * (1 - d->get_p());
    }

    return j;
}

void Sample_Table::swap(Sample_Table& s)
{
    Sample::swap(s);

    size_t buff_num_sum_distr = num_sum_distr;
    num_sum_distr = s.num_sum_distr;
    s.num_sum_distr = buff_num_sum_distr;

    double* buff_sum_distr = sum_distr;
    sum_distr = s.sum_distr;
    s.sum_distr = buff_sum_distr;
}

void Sample_Table::make_sum_distr()
{
    num_sum_distr = max_num(d);

    sum_distr = new double[num_sum_distr];

    d->reset();
    sum_distr[0] = d->get_prob_now();

    d->reset();

    for (size_t i = 1; i < num_sum_distr; ++i)
        sum_distr[i] = sum_distr[i - 1] + d->next_prob();
}

Sample_Table::Sample_Table(size_t _n, NB_distr* _d) : Sample(_n, _d)
{
    make_sum_distr();
}

Sample_Table::Sample_Table(const Sample_Table& s) : Sample(s)
{
    sum_distr = new double[s.num_sum_distr];

    for (size_t i = 0; i < s.num_sum_distr; ++i)
        sum_distr[i] = s.sum_distr[i];
}

Sample_Table::Sample_Table(Sample_Table&& s) : Sample(s)
{
    this->swap(s);
}

Sample_Table& Sample_Table::operator=(Sample_Table s)
{
    this->swap(s);

    return *this;
}

const char* Sample_Table::get_name() const
{
    return "Table Method";
}

size_t Sample_Table::simulate_one()
{
    size_t j = 0;

    double alpha = distribution(generator);

    while (sum_distr[j] < alpha && j < num_sum_distr)
        ++j;

    return j;
}

void Sample_Table::change_param(size_t _n)
{
    Sample::change_param(_n);

    delete[] sum_distr;

    make_sum_distr();
}

Sample_Table::~Sample_Table()
{
    delete[] sum_distr;
}

Sample_Bernulli::Sample_Bernulli(size_t _n, NB_distr* _d) : Sample(_n, _d)
{

}

Sample_Bernulli::Sample_Bernulli(const Sample_Bernulli& s) : Sample(s)
{

}

Sample_Bernulli::Sample_Bernulli(Sample_Bernulli&& s) : Sample(s)
{
    
}

Sample_Bernulli& Sample_Bernulli::operator=(Sample_Bernulli s)
{
    this->swap(s);

    return *this;
}

const char* Sample_Bernulli::get_name() const
{
    return  "Bernulli Method";
}

size_t Sample_Bernulli::simulate_one()
{
    size_t l = 0, j = 0;

    while (l != d->get_k())
        distribution(generator) > d->get_p() ? ++j : ++l;

    return j;
}

void ChiSqHist::swap(ChiSqHist& c)
{
    NB_distr* buff_d = d;
    d = c.d, c.d = buff_d;
    Sample* buff_s = s;
    s = c.s, c.s = buff_s;
    size_t buff_df = df;
    df = c.df, c.df = buff_df;
    double buff_chi_sq_stat = chi_sq_stat, buff_p_value = p_value;
    chi_sq_stat = c.chi_sq_stat, c.chi_sq_stat = buff_chi_sq_stat;
    p_value = c.p_value, c.p_value = buff_p_value;

    size_t buff_num_freq = num_freq;
    num_freq = c.num_freq;
    size_t* buff_exp_freq = exp_freq;
    exp_freq = c.exp_freq, c.exp_freq = buff_exp_freq;
    double* buff_th_freq = th_freq;
    th_freq = c.th_freq, c.th_freq = buff_th_freq;
}

ChiSqHist::ChiSqHist(NB_distr* _d, Sample* _s) : d(_d), s(_s)
{
    exp_freq = new size_t[10];
    th_freq = new double[10];

    if (_d && _s)
    {
        calc_th_freq();
        calc_exp_freq();
    }
}

ChiSqHist::ChiSqHist(ChiSqHist& c) : d(c.d), s(c.s), df(c.df), chi_sq_stat(c.chi_sq_stat), p_value(c.p_value), num_freq(c.num_freq)
{
    exp_freq = new size_t[num_freq];
    th_freq = new double[num_freq];

    for (size_t i = 0; i < num_freq; ++i)
    {
        exp_freq[i] = c.exp_freq[i];
        th_freq[i] = c.th_freq[i];
    }
}

ChiSqHist::ChiSqHist(ChiSqHist&& c)
{
    this->swap(c);
}

ChiSqHist& ChiSqHist::operator=(ChiSqHist c)
{
    this->swap(c);

    return *this;
}

void ChiSqHist::calc_th_freq()
{
    d->reset();
    num_freq = 0;

    while (1.0 + d->next_prob() != 1.0)
        ++num_freq;

    delete[] th_freq;

    th_freq = new double[num_freq];

    d->reset();
    th_freq[0] = d->get_prob_now();

    for (size_t i = 1; i < num_freq; ++i)
        th_freq[i] = d->next_prob();
}

void ChiSqHist::calc_exp_freq()
{
    size_t inx = 0;

    delete[] exp_freq;

    exp_freq = new size_t[num_freq]{};

    for (size_t i = 0; i < s->get_n(); ++i)
    {   
        inx = (*s)[i];

        if (inx > num_freq)
            ++exp_freq[num_freq - 1];
        else
            ++exp_freq[inx];
    }
}

void ChiSqHist::set_data(NB_distr* _d, Sample* _s)
{
    d = _d;
    s = _s;

    calc_th_freq();
    calc_exp_freq();
}

size_t ChiSqHist::merge(size_t* exp_freq_merge, double* th_freq_merge)
{
    size_t j = 0;

    for (size_t i = 0; i < num_freq; ++j)
    {
        th_freq_merge[j] = th_freq_merge[i];
        exp_freq_merge[j] = exp_freq_merge[i];
        ++i;

        for (; i < num_freq; ++i)
        {
            if (th_freq_merge[j] * s->get_n() >= 5)
                break;
            else
            {
                th_freq_merge[j] += th_freq_merge[i];
                exp_freq_merge[j] += exp_freq_merge[i];
            }
        }

        if (i == num_freq && th_freq_merge[j] * s->get_n() < 5)
        {
            --j;
            th_freq_merge[j] += th_freq_merge[i - 1];
            exp_freq_merge[j] += exp_freq_merge[i - 1];
        }
    }

    return j;
}

double ChiSqHist::chi_square(size_t new_num, size_t* exp_freq_merge, double* th_freq_merge)
{
    double res = 0, na_p;

    for (size_t i = 0; i < new_num; ++i)
    {
        na_p = th_freq_merge[i] * s->get_n();
        res += (exp_freq_merge[i] - na_p) * (exp_freq_merge[i] - na_p) / na_p; 
    }

    return res;
}

void ChiSqHist::calc_chi_sq()
{
    double* th_freq_merge = new double[num_freq];
    size_t* exp_freq_merge = new size_t[num_freq];

    memcpy(th_freq_merge, th_freq, num_freq * sizeof(double));
    memcpy(exp_freq_merge, exp_freq, num_freq * sizeof(size_t));

    size_t new_num = merge(exp_freq_merge, th_freq_merge);

    chi_sq_stat = chi_square(new_num, exp_freq_merge, th_freq_merge);
    df = new_num - 1;
    p_value = 1 - pChi(chi_sq_stat, df);

    delete[] th_freq_merge;
    delete[] exp_freq_merge;
}

ChiSqHist::~ChiSqHist()
{
    delete[] exp_freq;
    delete[] th_freq;
}

void Doc_NB::swap(Doc_NB& d)
{
    NB_distr buff_d0 = d0, buff_d1 = d1;
    d0 = d.d0, d1 = d.d1;
    d.d0 = buff_d0, d.d1 = buff_d1;
    NB_distr* buff_d_now = d_now;
    d_now = d.d_now;
    d.d_now = buff_d_now;
    Sample* buff_s = s;
    s = d.s;
    d.s = buff_s;
    ChiSqHist *buff_chisq = chisq;
    chisq = d.chisq;
    d.chisq = buff_chisq;

    size_t buff_num_p_value = num_p_value;
    num_p_value = d.num_p_value;
    d.num_p_value = buff_num_p_value;
    double buff_sign_lv = sign_lv;
    sign_lv = d.sign_lv;
    d.sign_lv = buff_sign_lv;
    double* buff_p_value_arr = p_value_arr;
    p_value_arr = d.p_value_arr;
    d.p_value_arr = buff_p_value_arr;
}

Doc_NB::Doc_NB() : d0(), d1(), num_p_value(10000), d_now(&d0), sign_lv(0.05)
{
    s = new Sample_Bernulli(100, d_now);
    chisq = new ChiSqHist(d_now, s);
    p_value_arr = new double[num_p_value]{};
}

Doc_NB::Doc_NB(Doc_NB &d) : d0(d.d0), d1(d.d1), num_p_value(d.num_p_value), sign_lv(d.sign_lv), s(d.s), chisq(d.chisq)
{
    p_value_arr = new double[num_p_value]{};
    memcpy(p_value_arr, d.p_value_arr, num_p_value * sizeof(double));
}

Doc_NB::Doc_NB(Doc_NB &&d)
{
    this->swap(d);
}

Doc_NB& Doc_NB::operator=(Doc_NB d)
{
    this->swap(d);

    return *this;
}

double Doc_NB::get_p_value(size_t i) const
{
    if(i < num_p_value)
        return p_value_arr[i];
    else
        return -1;
}

int comp(const void *a, const void *b)
{
    if (*(const double*)a > *(const double*)b)
        return 1;
    else
        return -1;
}

void Doc_NB::make_p_value()
{
    for (size_t i = 0; i < num_p_value; ++i)
    {
        s->simulate();
        chisq->calc_exp_freq();
        chisq->calc_chi_sq();
        p_value_arr[i] = chisq->get_p_value();
    }

    qsort(p_value_arr, num_p_value, sizeof(double), comp);
}

void Doc_NB::change_param(NB_distr _d0, NB_distr _d1, size_t _num_p_value, size_t _n, double _sign_lv)
{
    d0 = _d0;
    d1 = _d1;
    num_p_value = _num_p_value;
    sign_lv = _sign_lv;

    delete[] p_value_arr;
    p_value_arr = new double[num_p_value]{};

    s->change_param(_n);

    chisq->set_data(&d0, s);
}

void Doc_NB::set_table_method()
{
    size_t n = s->get_n();

    delete s;

    s = new Sample_Table(n, d_now);
    chisq->set_data(&d0, s);
}

void Doc_NB::set_bernulli_method()
{
    size_t n = s->get_n();

    delete s;

    s = new Sample_Bernulli(n, d_now);
    chisq->set_data(&d0, s);
}

void Doc_NB::set_hyp_d0()
{
    d_now = &d0;
}

void Doc_NB::set_hyp_d1()
{
    d_now = &d1;
}

Doc_NB::~Doc_NB()
{
    if (p_value_arr)
        delete[] p_value_arr;

    delete s;
    delete chisq;
}