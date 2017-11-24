#ifndef GUI_H
#define GUI_H

#include <string.h>
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#include <iostream>
#include <iostream>
#include <fstream>
#include <gui/Simulation.h>

class Gui {
public :

    typedef enum {
        LEFT_BUTTON,
        RIGHT_BUTTON,
        WHEEL_UP,
        WHEEL_DOWN
    } MOUSE_BUTTON;

    typedef enum {
        STATE_DOWN,
        STATE_UP
    } MOUSE_STATE;

    virtual void init(int argc, char ** argv) = 0;

    virtual void run() = 0;

    Gui();

    void initSimulation(int argc,char ** argv);

    void initGL();

    void reshape(int w, int h);

    void display();

    void idle();

    void motion(int x, int y);

    void mouse(MOUSE_BUTTON button, MOUSE_STATE state, bool shift, int x, int y);

    void keyboard(unsigned char key, int /*x*/, int /*y*/);

//    void special(int key, int /*x*/, int /*y*/);

    std::string getFullPath(const char * fn);

    std::string m_parentProcessDir;
    std::string m_deviceName;
    Simulation m_simulation;
    DisplayFlag m_displayFlag;
    TColor m_background_color;
    int m_window_width,m_window_height;
    int m_mouse_x;
    int m_mouse_y;
    int m_camera_mode;

};

#endif
