#include <gui/glut_methods.h>
#include <gui/MainWindows.h>
#include <QApplication>

Gui * gui;

std::string getFullPath(const char * fn) {
    return gui->getFullPath(fn);
}

int main(int argc, char **argv)
{
    if (argc<2) {
        std::cerr << "Error you need to specify a config file" << std::endl;
        return 1;
    }

//    gui = new GuiGlut();
//    gui->init(argc,argv);
//    gui->run();


    QApplication app(argc, argv);
    gui = new MainWindow();
    gui->init(argc,argv);
    gui->run();
    return app.exec();

    return 0;
}



