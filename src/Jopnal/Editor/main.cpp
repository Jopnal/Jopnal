#include <Jopnal/Editor/Precompiled/Precompiled.hpp>



#include <Jopnal/Jopnal.hpp>
#include <iostream>
#include <nana/gui/wvl.hpp> 
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/menubar.hpp>


int main(int c, char* v[])
{
    JOP_ENGINE_INIT("Jopnal Editor", c, v);

    nana::appearance a;
    a.maximize = true;
    a.taskbar = true;


    //nana::form form(nana::rectangle(nana::point(10, 10), nana::size(1280u, 720u)), a);
    jope::MainWindow form;

    form.bgcolor(nana::color(nana::color_rgb(0x222222)));
    nana::label label(form, nana::rectangle(0, 0, 1000, 220));
    label.caption("Hello Nana");
    form.show();
    nana::exec();
}