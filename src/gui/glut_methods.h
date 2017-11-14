#ifndef GLUT_METHODS_H
#define GLUT_METHODS_H

#include <gui/Gui.h>
#include <gui/Simulation.h>


class GuiGlut : public Gui {
public :
    void init(int argc, char ** argv);

    int run();

private :
    Simulation m_simulation;
};

#endif
