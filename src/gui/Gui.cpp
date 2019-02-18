#include <gui/glut_methods.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <core/Node.h>
#include <stdio.h>


#define Q(x) #x
#define QUOTE(x) Q(x)

#ifndef PLUGIN_DATA_DIR
#define PLUGIN_DATA_DIR_ ""
#else
#define PLUGIN_DATA_DIR_ QUOTE(PLUGIN_DATA_DIR)
#endif

/// Get the full path of the current process. The given filename should be the value of argv[0].
std::string getProcessFullPath(const char* filename)
{
    if (!filename || filename[0]!='/')
    {
        char path[1024];
        memset(path,0,sizeof(path));
        if (readlink("/proc/self/exe",path,sizeof(path)-1) == -1)
          std::cerr <<"Error: can't read the contents of the link." << std::endl;
        if (path[0])
            return path;
        else
            std::cout << "ERROR: can't get current process path..." << std::endl;
    }

    return filename;
}

std::string getParentDir(std::string path)
{
    std::string::size_type pos = path.find_last_of("/\\");
    if (pos == std::string::npos)
        return ""; // no directory
    else {
        std::string res = path.substr(0,pos);
        if (!res.empty()) res += '/';
        return res;
    }
}

std::string Gui::getFullPath(const char * fn) {
    {
        std::string filename = fn;
        std::ifstream f(filename.c_str());
        if (f.good()) return filename;
    }

    {
        std::string m_parentSceneDir = getParentDir(m_simulation.getFileName());

        std::string filename = m_parentSceneDir + fn;
        std::ifstream f(filename.c_str());
        if (f.good()) return filename;
    }

    {
        std::string filename = m_parentProcessDir + fn;
        std::ifstream f(filename.c_str());
        if (f.good()) return filename;
    }

    {
        std::string filename = std::string(PLUGIN_DATA_DIR_) + fn;
        std::ifstream f(filename.c_str());
        if (f.good()) return filename;
    }

    {
        std::string filename = std::string(PLUGIN_DATA_DIR_) + "data/" + fn;
        std::ifstream f(filename.c_str());
        if (f.good()) return filename;
    }

    return std::string(fn);
}


static void cpuid(unsigned int a, unsigned int b[4])
{
    asm volatile("xchgl %%ebx, %1\n"
                 "cpuid\n"
                 "xchgl %%ebx, %1\n"
                 :"=a"(*b),"=r"(*(b+1)),
                 "=c"(*(b+2)),"=d"(*(b+3)):"0"(a));
}

std::string cpu_name()
{
    unsigned int b[13] = {0};
    cpuid(0x80000000,b);
    unsigned int max = b[0];
    if (max < 0x80000004) return std::string();
    cpuid(0x80000002,b);
    cpuid(0x80000003,b+4);
    cpuid(0x80000004,b+8);
    std::string s;
    b[12] = 0;
    const char* p = (const char*)b;
    char last = '\0';
    while (*p)
    {
        char c = *p; ++p;
        if (c == ' ' && last == ' ') continue;
        if (c == '(')
        {
            while (*p && c != ')') c = *p++;
            continue;
        }
        s += c; last = c;
    }
    return s;
}

Gui::Gui() {
    m_background_color = TColor(1.0f, 1.0f, 1.0f, 0.0f);
    m_window_width = 800;
    m_window_height = 600;

    m_mouse_x = 0;
    m_mouse_y = 0;
    m_camera_mode = -1;
    m_deviceName = cpu_name();

    m_displayFlag.flag = 1<<DisplayFlag::STATS | 1<<DisplayFlag::VISUAL;
}

void Gui::initSimulation(int argc,char ** argv) {
    m_parentProcessDir = getProcessFullPath(argv[0]);
    m_parentProcessDir = getParentDir(m_parentProcessDir);


    if (! m_simulation.read_scene(argv[1])) return;
}

void Gui::initGL() {
    glewInit();

    glMatrixMode   ( GL_MODELVIEW );  // Select The Model View Matrix
    glLoadIdentity ( );    // Reset The Model View Matrix

    glClearColor ( m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3] );

    m_simulation.init();

//    reshape(m_window_width, m_window_height);
}

void Gui::reshape(int w, int h) {
    TReal simulation_size = m_simulation.getSize();

    m_window_width = w;
    m_window_height = h;

    glViewport(0, 0, w, h);
    glMatrixMode   ( GL_PROJECTION );  // Select The Projection Matrix
    glLoadIdentity ( );                // Reset The Projection Matrix
    gluPerspective ( 40, (GLdouble)w / (GLdouble)h, 0.001*simulation_size, 100.0*simulation_size );
    glMatrixMode   ( GL_MODELVIEW );  // Select The Model View Matrix
}

void Gui::display() {
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    m_simulation.render(m_displayFlag);
}

void Gui::idle() {
    m_simulation.step();
}

void Gui::motion(int x, int y) {
    int dx = x - m_mouse_x;
    int dy = y - m_mouse_y;

    m_mouse_x = x;
    m_mouse_y = y;

    if (m_camera_mode == 0) m_simulation.rotateCamera(dx,dy);
    else if (m_camera_mode == 1) m_simulation.translateCamera(dx,dy);
    else if (m_camera_mode == 2) m_simulation.zoomCamera(dx,dy);
    else if (m_camera_mode == 3) m_simulation.updatePickingForce(x,y);
    else if (m_camera_mode == 4) m_simulation.updatePickingConstraint(x,y);
}

void Gui::mouse(MOUSE_BUTTON button, MOUSE_STATE state, bool shift, int x, int y) {
    m_camera_mode = -1;
    motion(x,y);

    switch(button)
    {
        case LEFT_BUTTON :
            if (state == STATE_DOWN) {
                if (shift) {
                    m_simulation.startForcePicking(x,y);
                    m_camera_mode = 3;
                } else {
                    m_camera_mode = 0;
                }
            } else m_simulation.stopForcePicking();

            break;
        case RIGHT_BUTTON :
            if (state == STATE_DOWN) {
                if (shift) {
                    m_simulation.startConstraintPicking(x,y);
                    m_camera_mode = 4;
                } else {
                    m_camera_mode = 1;
                }
            } else m_simulation.stopConstraintPicking();

            break;
        case WHEEL_UP:
            m_simulation.zoomCamera(0,10);
            break;
        case WHEEL_DOWN:
            m_simulation.zoomCamera(0,-10);
            break;
    }
}

void Gui::keyboard(unsigned char key, int /*x*/, int /*y*/) {
    switch (key)
    {
//    case ' ': // SPACE
//    {
//        m_pause_animation = !m_pause_animation;
//        break;
//    }
    case 8: // DEL
    case '0':
        m_simulation.reset_camera();
        break;
//    case 27: // ESC
//        if (!m_fullscreen) {
//            exit(0);
//            break;
//        }
//    case '\r': // ENTER
//        m_fullscreen = !m_fullscreen;
//        if (m_fullscreen)
//        {
//            prev_win_x = glutGet(GLUT_WINDOW_X);
//            prev_win_y = glutGet(GLUT_WINDOW_Y);
//            prev_width = glutGet(GLUT_WINDOW_WIDTH);
//            prev_height = glutGet(GLUT_WINDOW_HEIGHT);
//            glutFullScreen();
//        }
//        else
//        {
//            glutReshapeWindow(prev_width,  prev_height);
//            glutPositionWindow(prev_win_x, prev_win_y);
//        }
//        break;
    case '+': // ENTER
        m_simulation.addPickingStiffness(10);
        break;
    case '-': // ENTER
        m_simulation.addPickingStiffness(-10);
        break;
//    case 's': // STEP
//        m_pause_animation = true;
//        m_simulation.step();
//        glutPostRedisplay();
    }
}

//void Gui::special(int key, int /*x*/, int /*y*/)
//{
//    if (key >= GLUT_KEY_F1 && key < GLUT_KEY_F1 + DisplayFlag::Nflag)
//    {
//        DisplayFlag::DisplayMode mode = (DisplayFlag::DisplayMode) (key-GLUT_KEY_F1);
//        if (displayFlag.isActive(mode)) displayFlag.flag &= ~(1<<mode);
//        else displayFlag.flag |= (1<<mode);
//        return;
//    }

//    switch (key)
//    {
//    case GLUT_KEY_UP:
//        simulation->translateCamera(0,10);
////        glutPostRedisplay();
//        break;
//    case GLUT_KEY_DOWN:
//        simulation->translateCamera(0,-10);
////        glutPostRedisplay();
//        break;
//    case GLUT_KEY_LEFT:
//        simulation->translateCamera(50,0);
////        glutPostRedisplay();
//        break;
//    case GLUT_KEY_RIGHT:
//        simulation->translateCamera(-50,0);
////        glutPostRedisplay();
//        break;
//    case GLUT_KEY_PAGE_UP:
//        simulation->rotateCamera(0,25);
////        glutPostRedisplay();
//        break;
//    case GLUT_KEY_PAGE_DOWN:
//        simulation->rotateCamera(0,-25);
////        glutPostRedisplay();
//        break;
//    default:
//        ;
//    }
//}


