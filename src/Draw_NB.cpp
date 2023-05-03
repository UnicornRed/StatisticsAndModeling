#include "string.h"
#include "Draw_NB.h"

int comp_double (const void* a, const void* b)
{
    if (*(const double *)a > *(const double *)b)
        return 1;
    else
        return -1;
}

void My_Chart::draw()
{
    int x = this->x() + 2 * margin_w + text_graf_w;
    int y = this->y() + text_graf_h;
    int w = this->w() - 3 * margin_w - 2 * text_graf_w;
    int h = this->h() - 3 * margin_h - 2 * text_graf_h;

    Fl_Box::draw();
    fl_color(24);
    fl_line_style(FL_CAP_SQUARE, 2);
    fl_line(x, y - text_graf_h, x, y + h);
    fl_line(x, y - text_graf_h, x + 4, y - text_graf_h + 4);
    fl_line(x - 1, y - text_graf_h, x - 5, y - text_graf_h + 4);
    fl_line(x, y + h, x + w + text_graf_w, y + h);
    fl_line(x + w + text_graf_w, y + h - 1, x + w + text_graf_w - 4, y + h - 5);
    fl_line(x + w + text_graf_w, y + h, x + w + text_graf_w - 4, y + h + 4);
}

My_Chart::My_Chart(int x, int y, int w, int h, const char* s) : Fl_Box(x, y, w, h, s)
{
    labelsize(5);
}

void My_Graf::draw()
{
    int x = this->x() + 2 * margin_w + text_graf_w;
    int y = this->y() + text_graf_h;
    int w = this->w() - 3 * margin_w - 2 * text_graf_w;
    int h = this->h() - 3 * margin_h - 2 * text_graf_h;
    double dx = max_x - min_x, dy = max_y - min_y;

    My_Chart::draw();
    
    fl_font(FL_COURIER, 12);
    char str[10];

    for (size_t i = 0; i <= max_part; ++i)
    {
        int y_i = y + h - i * (double(h) / max_part);
        int x_i = x + i * (double(w) / max_part);

        sprintf(str, "%4.2f", min_x + dx * i / max_part);

        fl_line(x_i, y + h - 3, x_i, y + h + 3);
        fl_draw(str, x_i - text_graf_w / 2, y + h + text_graf_h + margin_h);

        sprintf(str, "%4.2f", min_y + dy * i / max_part);

        fl_line(x - 3, y_i, x + 3, y_i);
        fl_draw(str, x - text_graf_w - margin_w, y_i + text_graf_h / 2);
    }

    double pre_x = x + (x_point[0] - min_x) / dx * w, pre_y = y + (1 - (y_point[0] - min_y) / dy) * h;

    if (min_x == min_y && max_x == max_y)
        fl_line(x ,y + h, x + w, y);
        
    fl_color(255, 0, 0);

    if (num > 0)
        fl_line(pre_x, pre_y, pre_x, pre_y);

    for (size_t i = 1; i < num; ++i)
    {
        double now_x = x + (x_point[i] - min_x) / dx * w, now_y = y + (1 - (y_point[i] - min_y) / dy) * h;

        fl_line(pre_x, pre_y, now_x, now_y);

        pre_x = now_x;
        pre_y = now_y;
    }
}

My_Graf::My_Graf(int x, int y, int w, int h, const char* s) : My_Chart(x, y, w, h, s)
{
    x_point = y_point = nullptr;
    num = 0;
    labelsize(5);
}

void My_Graf::set_data(size_t _num, double* _x_point, double* _y_point)
{
    if (num != 0 && !_x_point)
        throw "My_Graf::set_data: If the number of elements is greater than 0, the array of x_points should not be empty.";

    if (num != 0 && !_y_point)
        throw "My_Graf::set_data: If the number of elements is greater than 0, the array of y_points should not be empty.";

    num = _num;

    if (x_point)
        delete[] x_point;

    if (y_point)
        delete[] y_point;

    x_point = new double[num];
    y_point = new double[num];
    
    memcpy(x_point, _x_point, num * sizeof(double));
    memcpy(y_point, _y_point, num * sizeof(double));

    qsort(x_point, num, sizeof(double), comp_double);
    qsort(y_point, num, sizeof(double), comp_double);
}

void My_Graf::auto_minmax()
{
    if (!x_point || !y_point)
        throw "My_Graf::auto_minmax: Array of points is not defined.";

    min_x = x_point[0];
    min_y = y_point[0];
    max_x = x_point[num - 1];
    max_y = y_point[num - 1];
}

void My_Graf::set_minmax(double _min_x, double _min_y, double _max_x, double _max_y)
{
    if (_min_x > _max_x)
        throw "My_Graf::set_minmax: min_x must be less than the max_x.";

    if (_min_y > _max_y)
        throw "My_Graf::set_minmax: min_y must be less than the max_y.";

    min_x = _min_x;
    min_y = _min_y;
    max_x = _max_x;
    max_y = _max_y;
}

My_Graf::~My_Graf()
{
    delete[] x_point;
    delete[] y_point;
}

void My_BarChart::draw()
{
    int x = this->x() + 2 * margin_w + text_graf_w;
    int y = this->y() + text_graf_h;
    int w = this->w() - 3 * margin_w - 2 * text_graf_w;
    int h = this->h() - 3 * margin_h - 2 * text_graf_h;
    int w_bar = w / num * 4 / 5;
    int w_bar_margin = w / num / 10;
    double dy = max_y - min_y;

    My_Chart::draw();
    
    fl_font(FL_COURIER, 12);
    char str[10];

    for (size_t i = 0; i <= max_part; ++i)
    {
        int y_i = y + h - i * (double(h) / max_part);

        sprintf(str, "%4.2f", min_y + dy * i / max_part);

        fl_line(x - 3, y_i, x + 3, y_i);
        fl_draw(str, x - text_graf_w - margin_w, y_i + text_graf_h / 2);

    }

    for (size_t i = 1; i <= num; ++i)
    {
        int x_i = x + i * (double(w) / num);

        sprintf(str, "%lu", i);

        fl_line(x_i, y + h - 3, x_i, y + h + 3);
        fl_draw(str, x_i - (text_graf_w / 2 + (double(w) / num)) / 2, y + h + text_graf_h + margin_h);
    }

    for (size_t i = 0; i < num; ++i)
    {
        double now_x = x + i * (double(w) / num);

        fl_color(20);
        fl_rectf(now_x + w_bar_margin, y + (dy - y1_point[i]) / dy * h, w_bar, y1_point[i] / dy * h);
        fl_color(24);
        fl_rectf(now_x + w_bar_margin + w_bar / 6, y + (dy - y2_point[i]) / dy * h, w_bar * 2 / 3, y2_point[i] / dy * h);
    }
}

My_BarChart::My_BarChart(int x, int y, int w, int h, const char* s) : My_Chart(x, y, w, h, s)
{
    y1_point = nullptr;
    y2_point = nullptr;
    num = 0;
    labelsize(5);
}

void My_BarChart::set_data(size_t _num, double* _y1_point, double* _y2_point)
{
    if (num != 0 && !_y1_point && !_y2_point)
        throw "My_Graf::set_data: If the number of elements is greater than 0, the array of y_points should not be empty.";

    num = _num;

    if (y1_point)
        delete[] y1_point;

    if (y2_point)
        delete[] y2_point;

    y1_point = new double[num];
    y2_point = new double[num];
    
    memcpy(y1_point, _y1_point, num * sizeof(double));
    memcpy(y2_point, _y2_point, num * sizeof(double));
}

void My_BarChart::set_minmax(double _min_y, double _max_y)
{
    if (_min_y > _max_y)
        throw "My_Graf::set_minmax: min_y must be less than the max_y.";

    min_y = _min_y;
    max_y = _max_y;
}

My_BarChart::~My_BarChart()
{
    delete[] y1_point;
    delete[] y2_point;
}

void My_Button::callback_function(Fl_Widget *w, void *user)
{
    static_cast<My_Button*>(w)->on_press(user);
}

My_Button::My_Button(int x, int y, int w, int h, const char* label, void* user) : Fl_Button(x, y, w, h, label)
{
    callback(callback_function, user);
};

My_Icon::My_Icon(int x, int y, int w, int h, Chart _c, Doc_NB* _data) : My_Button(x, y, w, h, "", (void*)(&c)), data(_data), c(_c)
{

}

void My_Icon_P_Value::draw()
{
    Fl_Button::draw();

    int x = this->x();
    int y = this->y();
    int h = this->h();
    int w = this->w();

    fl_line_style(FL_CAP_SQUARE, 2);
    fl_color(255, 0, 0);
    fl_line(x + 7, y + h - 7, x + 7 + (w - 14) / 3, y + 7 + (h - 14) / 3);
    fl_line(x + 7 + (w - 14) / 3, y + 7 + (h - 14) / 3, x + 7 + 2 * (w - 14) / 3, y + 7 + 2 * (h - 14) / 3);
    fl_line(x + 7 + 2 * (w - 14) / 3, y + 7 + 2 * (h - 14) / 3,  x + w - 7, y + 8);
    fl_color(24);
    fl_line(x + 7, y + 7, x + 7, y + h - 7);
    fl_line(x + 7, y + h - 7, x + w - 7, y + h - 7);
}

My_Icon_P_Value::My_Icon_P_Value(int x, int y, int w, int h, Chart _c, Doc_NB* _data) : My_Icon(x, y, w, h, _c, _data) {};

void My_Icon_P_Value::change_output()
{
    char s[150];

    sprintf(s, "Parametrs. H0: p = %.2f, k = %lu. H1: p = %.2f, k = %lu. Number of sample: %lu. Number of p-value: %lu. Method: %10s.",
            data->get_d0()->get_p(), data->get_d0()->get_k(), data->get_d1()->get_p(), data->get_d1()->get_k(), data->get_Sample()->get_n(),
            data->get_num_p_value(), data->get_Sample()->get_name());

    c.out_param->value(s);
}

void My_Icon_P_Value::on_press(void *user)
{
    c.b->hide();
    c.g_p_level->hide();
    c.g_power->hide();
    change_output();

    double *sort_p_value_arr = new double[21]{};
    double x_point[21];

    sort_p_value_arr[0] = 0;

    data->make_p_value();

    for (size_t i = 0, j = 0; i <= 20; ++i)
    {
        x_point[i] = 0.05 * i;

        while (data->get_p_value(j) < x_point[i] && data->get_p_value(j) != -1)
            ++j;
        
        sort_p_value_arr[i] = double(j) / data->get_num_p_value();
    }

    c.g_p_level->set_data(21, x_point, sort_p_value_arr);
    c.g_p_level->set_minmax(0, 0, 1, 1);
    c.g_p_level->show();

    delete[] sort_p_value_arr;
}

void My_Icon_Power::draw()
{
    Fl_Button::draw();

    int x = this->x();
    int y = this->y();
    int h = this->h();
    int w = this->w();

    fl_line_style(FL_CAP_SQUARE, 2);
    fl_color(0, 255, 0);
    fl_line(x + 7, y + h - 7, x + 7 + (w - 14) / 3, y + 7 + 2 * (h - 14) / 6);
    fl_line(x + 7 + (w - 14) / 3, y + 7 + 2 * (h - 14) / 6, x + 7 + 2 * (w - 14) / 3, y + 7 + (h - 14) / 6);
    fl_line(x + 7 + 2 * (w - 14) / 3, y + 7 + (h - 14) / 6,  x + w - 7, y + 8);
    fl_color(24);
    fl_line(x + 7, y + 7, x + 7, y + h - 7);
    fl_line(x + 7, y + h - 7, x + w - 7, y + h - 7);
}

My_Icon_Power::My_Icon_Power(int x, int y, int w, int h, Chart _c, Doc_NB* _data) : My_Icon(x, y, w, h, _c, _data) {};

void My_Icon_Power::change_output()
{
    char s[150];

    sprintf(s, "Parametrs. H0: p = %.2f, k = %lu. H1: p = %.2f, k = %lu. Significance level: %.2f. Method: %10s.",
            data->get_d0()->get_p(), data->get_d0()->get_k(), data->get_d1()->get_p(), data->get_d1()->get_k(),
            data->get_sign_lv(), data->get_Sample()->get_name());

    c.out_param->value(s);
}

void My_Icon_Power::on_press(void *user)
{
    c.b->hide();
    c.g_p_level->hide();
    c.g_power->hide();
    change_output();

    size_t num_n = 20, len_n = 5, start_n = 50, back_n = data->get_Sample()->get_n();
    double *power_arr = new double[num_n]{};
    double x_point[num_n], max_y = 0, min_y = 1;

    for (size_t i = 0; i < num_n; ++i)
    {
        size_t j = 0;
        x_point[i] = len_n * i + start_n;
        data->get_Sample()->change_param(x_point[i]);

        data->make_p_value();

        while (data->get_p_value(j) < data->get_sign_lv() && data->get_p_value(j) != -1)
            ++j;
        
        power_arr[i] = double(j) / data->get_num_p_value();

        if (max_y < power_arr[i])
            max_y = power_arr[i];

        if (min_y > power_arr[i])
            min_y = power_arr[i];
    }

    data->get_Sample()->change_param(back_n);
    c.g_power->set_data(num_n, x_point, power_arr);
    c.g_power->set_minmax(start_n, min_y, len_n * (num_n - 1) + start_n, max_y);
    c.g_power->show();

    delete[] power_arr;
}

void My_Icon_Bar::draw()
{
    Fl_Button::draw();

    int x = this->x();
    int y = this->y();
    int h = this->h();
    int w = this->w();

    fl_line_style(FL_CAP_SQUARE, 2);
    fl_color(255, 0, 0);
    fl_rectf(x + 9, y + 7 + 2 * (h - 14) / 3, 4, (h - 14) / 3);
    fl_color(0, 0, 255);
    fl_rectf(x + 7 + (w - 14) / 2, y + 7 + (h - 14) / 3, 4, 2 * (h - 14) / 3);
    fl_color(24);
    fl_line(x + 7, y + 7, x + 7, y + h - 7);
    fl_line(x + 7, y + h - 7, x + w - 7, y + h - 7);
}

My_Icon_Bar::My_Icon_Bar(int x, int y, int w, int h, Chart _c, Doc_NB* _data) : My_Icon(x, y, w, h, _c, _data) {};

void My_Icon_Bar::change_output()
{
    char s[150];

    sprintf(s, "Parametrs. H0: p = %.2f, k = %lu. H1: p = %.2f, k = %lu. Number of sample: %lu. Method: %10s.",
            data->get_d0()->get_p(), data->get_d0()->get_k(), data->get_d1()->get_p(), data->get_d1()->get_k(), data->get_Sample()->get_n(),
            data->get_Sample()->get_name());

    c.out_param->value(s);
}

void My_Icon_Bar::on_press(void *user)
{
    c.b->hide();
    c.g_p_level->hide();
    c.g_power->hide();
    change_output();

    double max_mean = 0;

    double *exp_freq = new double[data->get_chi_sq()->get_num_freq()];
    double *th_freq = new double[data->get_chi_sq()->get_num_freq()];

    data->get_Sample()->simulate();
    data->get_chi_sq()->calc_exp_freq();

    for (size_t i = 0; i < data->get_chi_sq()->get_num_freq(); ++i)
    {
        th_freq[i] = data->get_Sample()->get_n() * data->get_chi_sq()->get_th_freq()[i];
        exp_freq[i] = data->get_chi_sq()->get_exp_freq()[i];

        if (max_mean < th_freq[i])
            max_mean = th_freq[i];

        if (max_mean < exp_freq[i])
            max_mean = exp_freq[i];
    }

    c.b->set_data(15, th_freq, exp_freq);
    c.b->set_minmax(0, max_mean);
    c.b->show();

    delete[] exp_freq;
    delete[] th_freq;
}

void setting_callback(Fl_Widget *w, void* user)
{
    ((Fl_Window*)user)->show();
}

void bar_chart_callback(Fl_Widget *w, void* user)
{
    ((My_Icon_Bar*)user)->on_press(user);
}

void graf_p_value_callback(Fl_Widget *w, void* user)
{
    ((My_Icon_P_Value*)user)->on_press(user);
}

void graf_power_callback(Fl_Widget *w, void* user)
{
    ((My_Icon_P_Value*)user)->on_press(user);
}

Draw_NB::Draw_NB(Doc_NB *_data) : data(_data)
{

}

void exit_callback(Fl_Widget* w, void* user)
{
    Fl_Widget* step = w;

    while(step->parent() != NULL)
        step = step->parent();

    step->hide();
}

My_Button_Dialog_Ok::My_Button_Dialog_Ok(int x, int y, int w, int h, My_Dialog* _dialog) : My_Button(x, y, w, h, "Ok", (void*)_dialog) {};

void My_Button_Dialog_Ok::on_press(void *user)
{
    int d0_k, d1_k, np, ns;
    double d0_p, d1_p, ah;

    d0_k = std::stoi(((My_Dialog*)user)->ii_d0->value());
    d0_p = std::stod(((My_Dialog*)user)->if_d0->value());
    d1_k = std::stoi(((My_Dialog*)user)->ii_d1->value());
    d1_p = std::stod(((My_Dialog*)user)->if_d1->value());
    np = std::stoi(((My_Dialog*)user)->ii_np->value());
    ns = std::stoi(((My_Dialog*)user)->ii_ns->value());
    ah = std::stod(((My_Dialog*)user)->if_ah->value());

    if (d0_k <= 0 || d0_k > 50)
    {
        fl_alert("Param. k of hypothesis 0 must be greater than 0 and less 50!\nThe default value is set: 10");

        ((My_Dialog*)user)->ii_d0->value("10");

        return;
    }

    if (d0_p < 0 || d0_p > 1)
    {
        fl_alert("Param. p of hypothesis 0 must be greater than 0 and less 1!\nThe default value is set: 0.8");

        ((My_Dialog*)user)->if_d0->value("0.80");

        return;
    }

    if (d1_k <= 0 || d1_k > 50)
    {
        fl_alert("Param. k of hypothesis 1 must be greater than 0 and less 50!\nThe default value is set: 10");

        ((My_Dialog*)user)->ii_d1->value("10");

        return;
    }

    if (d1_p < 0 || d1_p > 1)
    {
        fl_alert("Param. p of hypothesis 1 must be greater than 0 and less 1!\nThe default value is set: 0.8");

        ((My_Dialog*)user)->if_d1->value("0.80");

        return;
    }

    if (np <= 0 || np > 100000)
    {
        fl_alert("Param. number of p-value must be greater than 0 and less 100000!\nThe default value is set: 10000");

        ((My_Dialog*)user)->ii_np->value("10000");

        return;
    }

    if (ns < 50 || ns > 1000)
    {
        fl_alert("Param. number of sample must be greater than 49 and less 1000!\nThe default value is set: 100");

        ((My_Dialog*)user)->ii_ns->value("100");

        return;
    }

    if (ah < 0 || ah > 1)
    {
        fl_alert("Param. significance level must be greater than 0 and less 1!\nThe default value is set: 0.05");

        ((My_Dialog*)user)->if_ah->value("0.05");

        return;
    }

    ((My_Dialog*)user)->get_data()->change_param(NB_distr(d0_p, d0_k), NB_distr(d1_p, d1_k), np, ns, ah);

    if (((My_Dialog*)user)->rb_h0->value())
        ((My_Dialog*)user)->get_data()->set_hyp_d0();
    else
        ((My_Dialog*)user)->get_data()->set_hyp_d1();

    if (((My_Dialog*)user)->rb_mb->value())
        ((My_Dialog*)user)->get_data()->set_bernulli_method();
    else
        ((My_Dialog*)user)->get_data()->set_table_method();

    ((My_Dialog*)user)->hide();                                   
}

My_Button_Dialog_Close::My_Button_Dialog_Close(int x, int y, int w, int h, My_Dialog* md) : My_Button(x, y, w, h, "Close", md) {};

void My_Button_Dialog_Close::on_press(void *user)
{
    close_dialog_callback((My_Dialog*)user, ((My_Dialog*)user)->get_data());
}

void close_dialog_callback(Fl_Widget* w, void* user)
{
    char buff[20];

    sprintf(buff, "%lu", ((Doc_NB*)user)->get_d0()->get_k());
    ((My_Dialog*)w)->ii_d0->value(buff);
    sprintf(buff, "%.2f", ((Doc_NB*)user)->get_d0()->get_p());
    ((My_Dialog*)w)->if_d0->value(buff);
    sprintf(buff, "%lu", ((Doc_NB*)user)->get_d1()->get_k());
    ((My_Dialog*)w)->ii_d1->value(buff);
    sprintf(buff, "%.2f", ((Doc_NB*)user)->get_d1()->get_p());
    ((My_Dialog*)w)->if_d1->value(buff);
    sprintf(buff, "%lu", ((Doc_NB*)user)->get_Sample()->get_n());
    ((My_Dialog*)w)->ii_ns->value(buff);
    sprintf(buff, "%lu", ((Doc_NB*)user)->get_num_p_value());
    ((My_Dialog*)w)->ii_np->value(buff);
    sprintf(buff, "%.2f", ((Doc_NB*)user)->get_sign_lv());
    ((My_Dialog*)w)->if_ah->value(buff);

    ((My_Dialog*)w)->hide();
}

My_Dialog::My_Dialog(int w, int h, char* title, Doc_NB* _data) : Fl_Window(w, h, title), data(_data)
{
    callback(close_dialog_callback, (void*)data);

    Fl_Box* hyp0 = new Fl_Box(margin_w, margin_h, input_w + sign_w, input_h, "Hypothesis 0");
    ii_d0 = new Fl_Int_Input(margin_w + sign_w, 2 * margin_h + input_h, input_w, input_h, "k: ");
    ii_d0->value("10");
    if_d0 = new Fl_Float_Input(margin_w + sign_w, 3 * margin_h + 2 * input_h, input_w, input_h, "p: ");
    if_d0->value("0.80");
    Fl_Box* hyp1 = new Fl_Box(margin_w, 4 * margin_h + 3 * input_h, input_w + sign_w, input_h, "Hypothesis 1");
    ii_d1 = new Fl_Int_Input(margin_w + sign_w, 5 * margin_h + 4 * input_h, input_w, input_h, "k: ");
    ii_d1->value("10");
    if_d1 = new Fl_Float_Input(margin_w + sign_w, 6 * margin_h + 5 * input_h, input_w, input_h, "p: ");
    if_d1->value("0.80");
    Fl_Box* num_s = new Fl_Box(3 * margin_w + sign_w + input_w, margin_h, input_w + sign_w, input_h, "Size of sample:");
    ii_ns = new Fl_Int_Input(3 * margin_w + sign_w + input_w, 2 * margin_h + input_h, input_w + sign_w, input_h, "");
    ii_ns->value("100");
    Fl_Box* num_p_value = new Fl_Box(3 * margin_w + sign_w + input_w, 3 * margin_h + 2 * input_h, input_w + sign_w, input_h, "P-value:");
    ii_np = new Fl_Int_Input(3 * margin_w + sign_w + input_w, 4 * margin_h + 3 * input_h, input_w + sign_w, input_h, "");
    ii_np->value("10000");
    Fl_Box* sign_level = new Fl_Box(3 * margin_w + sign_w + input_w, 5 * margin_h + 4 * input_h, input_w + sign_w, input_h, "Sign. level:");
    if_ah = new Fl_Float_Input(3 * margin_w + sign_w + input_w, 6 * margin_h + 5 * input_h, input_w + sign_w, input_h, "");
    if_ah->value("0.05");

    Fl_Group* rb_method_g = new Fl_Group(4 * margin_w + 2 * sign_w + 2 * input_w, 0, input_w + sign_w + 2 * margin_w, 3 * input_h + 4 * margin_h, "");

    Fl_Box* method = new Fl_Box(5 * margin_w + 2 * sign_w + 2 * input_w, margin_h, input_w + sign_w, input_h, "Method:");
    rb_mb = new Fl_Radio_Round_Button(5 * margin_w + 2 * sign_w + 2 * input_w, 2 * margin_h + input_h, input_w + sign_w, input_h, "Bernulli");
    rb_mt = new Fl_Radio_Round_Button(5 * margin_w + 2 * sign_w + 2 * input_w, 3 * margin_h + 2 * input_h, input_w + sign_w, input_h, "Table");

    rb_mb->setonly();

    rb_method_g->end();

    Fl_Group* rb_hup_g = new Fl_Group(4 * margin_w + 2 * sign_w + 2 * input_w, 4 * margin_h + 3 * input_h, input_w + sign_w + 2 * margin_w, 3 * input_h + 4 * margin_h, "");

    Fl_Box* hyp = new Fl_Box(5 * margin_w + 2 * sign_w + 2 * input_w, 5 * margin_h + 3 * input_h, input_w + sign_w, input_h, "Hypothesis:");
    rb_h0 = new Fl_Radio_Round_Button(5 * margin_w + 2 * sign_w + 2 * input_w, 6 * margin_h + 4 * input_h, input_w + sign_w, input_h, "H0");
    rb_h1 = new Fl_Radio_Round_Button(5 * margin_w + 2 * sign_w + 2 * input_w, 7 * margin_h + 5 * input_h, input_w + sign_w, input_h, "H1");

    rb_h0->setonly();

    rb_hup_g->end();

    My_Button_Dialog_Ok* b_ok = new My_Button_Dialog_Ok(w / 2 - button_w - margin_w, 7 * margin_h + 6 * input_h, button_w, button_h, this); 

    My_Button_Dialog_Close* b_close = new My_Button_Dialog_Close(w / 2 + margin_w, 7 * margin_h + 6 * input_h, button_w, button_h, this);   

    end();
    hide();
}

void My_Window::close_callback(Fl_Widget* w, void* user)
{
    ((My_Dialog*)user)->hide();
    ((My_Window*)w)->hide();
}

My_Window::My_Window(int w, int h, char* title, My_Dialog *_md) : Fl_Window(w, h, title), md(_md)
{
    callback(close_callback, (void*)md);

    Fl_Group *graf_g = new Fl_Group(0, menu_h + 2 * margin_h + fast_button_h, w, h - menu_h - fast_button_h - bottom_text_h);

    int y = menu_h + fast_button_h + 3 * margin_h;

    Fl_Output* out = new Fl_Output(margin_w, y, graf_w, text_h, "");
    out->value("Program Negative Binominal Distribution.");
    out->box(FL_FLAT_BOX);
    out->color(FL_BACKGROUND_COLOR);

    y += text_h + 2 * margin_h;

    My_Graf *graf_p_value = new My_Graf(margin_w, y, graf_w, graf_h, "");
    graf_p_value->hide();

    My_Graf *graf_power = new My_Graf(margin_w, y, graf_w, graf_h, "");
    graf_power->hide();

    My_BarChart *bar = new My_BarChart(margin_w, y, graf_w, graf_h, "");
    bar->hide();

    graf_g->end();
    resizable(graf_g);

    Fl_Group *button_g = new Fl_Group(0, 0, w, menu_h + 2 * margin_h + fast_button_h);

    Fl_Box *resize_box = new Fl_Box(w - fast_button_one_w, 0, fast_button_one_w, menu_h + 2 * margin_h + fast_button_h);

    Chart chart_graf_bar;
    chart_graf_bar.b = bar;
    chart_graf_bar.g_p_level = graf_p_value;
    chart_graf_bar.g_power = graf_power;
    chart_graf_bar.out_param = out;

    My_Icon_P_Value *graf_p_value_b = new My_Icon_P_Value(margin_w, menu_h + margin_h, fast_button_one_w, fast_button_h, chart_graf_bar, md->get_data());

    My_Icon_Power *graf_power_b = new My_Icon_Power(3 * margin_w + 2 * fast_button_one_w, menu_h + margin_h, fast_button_one_w, fast_button_h, chart_graf_bar, md->get_data());
    
    My_Icon_Bar *bar_b = new My_Icon_Bar(2 * margin_w + fast_button_one_w, menu_h + margin_h, fast_button_one_w, fast_button_h, chart_graf_bar, md->get_data());

    button_g->end();
    button_g->resizable(resize_box);

    Fl_Menu_Bar *mb = new Fl_Menu_Bar(0, 0, w, menu_h, "");

    Fl_Menu_Item *mi = new Fl_Menu_Item[8] {{"&distribution", FL_ALT+'d', 0, 0, FL_SUBMENU},
                                                {"&sample", FL_ALT+'s', bar_chart_callback, bar_b},
                                                {"&p-value", FL_ALT+'p', graf_p_value_callback, graf_p_value_b},
                                                {"po&wer", FL_ALT+'w', graf_power_callback, graf_power_b},
                                                {0},
                                            {"s&etting", FL_ALT+'e', setting_callback, (void*)md},
                                            {"e&xit", FL_ALT+'x', exit_callback},
                                            {0}};

    mb->menu(mi);

    end();
    show();
}

int Draw_NB::run()
{
    int win_h = menu_h + fast_button_h + graf_h + text_h + bottom_text_h + 5 * margin_h;
    int win_w = graf_w + 2 * margin_w;
    int win_set_h = 6 * input_h + 9 * margin_h + button_h;
    int win_set_w = 3 * input_w + 6 * margin_w + 3 * sign_w;

    My_Dialog *win_setting = new My_Dialog(win_set_w, win_set_h, "Setting", data);

    My_Window *win = new My_Window(win_w, win_h, "Negative Binomial", win_setting);

    return Fl::run();
}

Draw_NB::~Draw_NB()
{

}