#pragma once

#include <Fl/Fl.H>
#include <Fl/Fl_Window.H>
#include <Fl/Fl_Box.H>
#include <Fl/Fl_Button.H>
#include <Fl/Fl_Menu_Bar.H>
#include <Fl/Fl_Menu_Item.H>
#include <Fl/Fl_Float_Input.H>
#include <Fl/Fl_Int_Input.H>
#include <Fl/Fl_Output.H>
#include <Fl/Fl_Radio_Round_Button.H>
#include <Fl/fl_ask.H>
#include <FL/fl_draw.H>

#include "Doc_NB.h"

enum
{
    menu_h = 30,
    menu_one_w = 60,
    fast_button_h = 30,
    fast_button_one_w = 30,
    margin_h = 6,
    margin_w = 6,
    graf_h = 500,
    graf_w = 1000,
    text_graf_w = 35,
    text_graf_h = 10,
    text_h = 20,
    bottom_text_h = 30,

    sign_w = 40,
    input_w = 80,
    input_h = 30,
    button_h = 40,
    button_w = 80
};

void setting_callback(Fl_Widget *w, void* user);

class My_Graf;
class My_BarChart;

struct Chart
{
    My_Graf* g_p_level;
    My_Graf* g_power;
    My_BarChart* b;
    Fl_Output* out_param;
};

class My_Chart : public Fl_Box
{
protected:
    void draw();
public:
    My_Chart(int x, int y, int w, int h, const char* s);
};

class My_Graf : public My_Chart
{
    const size_t max_part = 20;
    size_t num;
    double min_x, min_y, max_x, max_y;
    double *x_point;
    double *y_point;
protected:
    void draw();
public:
    My_Graf(int x, int y, int w, int h, const char* s);

    void set_data(size_t _num, double* _x_point, double* _y_point);

    void auto_minmax();

    void set_minmax(double _min_x, double _min_y, double _max_x, double _max_y);

    ~My_Graf();
};

class My_BarChart : public My_Chart
{
    const size_t max_part = 20;
    size_t num;
    double min_y, max_y;
    double *y1_point;
    double *y2_point;
protected:
    void draw();
public:
    My_BarChart(int x, int y, int w, int h, const char* s);

    void set_data(size_t _num, double* _y1_point, double* _y2_point);

    void set_minmax(double _min_y, double _max_y);

    ~My_BarChart();
};

class My_Button : public Fl_Button
{
    static void callback_function(Fl_Widget *w, void *user);
public:
    My_Button(int x, int y, int w, int h, const char* label, void* user);

    virtual void on_press(void *user) {}
};

class My_Icon : public My_Button
{
protected:
    Chart c;
    Doc_NB* data;
public:
    My_Icon(int x, int y, int w, int h, Chart _c, Doc_NB* _data);

    virtual void change_output() = 0;

    virtual void on_press(void *user) {}
};

class My_Icon_P_Value : public My_Icon
{
protected:
    void draw();
public:
    My_Icon_P_Value(int x, int y, int w, int h, Chart _c, Doc_NB* _data);

    virtual void change_output() override;

    virtual void on_press(void *user) override;
};

class My_Icon_Power : public My_Icon
{
protected:
    void draw();
public:
    My_Icon_Power(int x, int y, int w, int h, Chart _c, Doc_NB* _data);

    virtual void change_output() override;

    virtual void on_press(void *user) override;
};

class My_Icon_Bar : public My_Icon
{
protected:
    void draw();
public:
    My_Icon_Bar(int x, int y, int w, int h, Chart _c, Doc_NB* _data);

    virtual void on_press(void *user) override;

    virtual void change_output() override;

    friend void bar_chart_callback(Fl_Widget *w, void* user);
};

void exit_callback(Fl_Widget* w, void* user);

void close_dialog_callback(Fl_Widget* w, void* user);

class My_Dialog : public Fl_Window
{
private:
    Doc_NB* data;
public:
    My_Dialog(int w, int h, char* title, Doc_NB* _data);

    inline Doc_NB* get_data() { return data; }
    
    Fl_Int_Input* ii_d0;
    Fl_Float_Input* if_d0;
    Fl_Int_Input* ii_d1;
    Fl_Float_Input* if_d1;
    Fl_Int_Input* ii_ns;
    Fl_Int_Input* ii_np;
    Fl_Float_Input* if_ah;
    Fl_Radio_Round_Button* rb_mb;
    Fl_Radio_Round_Button* rb_mt;
    Fl_Radio_Round_Button* rb_h0;
    Fl_Radio_Round_Button* rb_h1;
};

class My_Button_Dialog_Ok : public My_Button
{
public:
    My_Button_Dialog_Ok(int x, int y, int w, int h, My_Dialog* _dialog);

    virtual void on_press(void *user) override;
};

class My_Button_Dialog_Close : public My_Button
{
public:
    My_Button_Dialog_Close(int x, int y, int w, int h, My_Dialog* md);

    virtual void on_press(void *user) override;
};

class My_Window : public Fl_Window
{
private:
    My_Dialog *md;

    static void close_callback(Fl_Widget* w, void* user);
public:
    My_Window(int w, int h, char* title, My_Dialog *_md);
};


class Draw_NB
{
private:
    Doc_NB* data;
public:
    Draw_NB(Doc_NB *_data);

    int run();

    ~Draw_NB();
};