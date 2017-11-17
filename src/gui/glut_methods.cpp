#include <gui/glut_methods.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <core/Node.h>
#include <stdio.h>


GuiGlut * glut_gui = NULL;

int winid = 0;

#if !defined(GLUT_WHEEL_UP)
#  define GLUT_WHEEL_UP   3
#  define GLUT_WHEEL_DOWN 4
#endif

bool fullscreen = false;
int pause_animation = true;
int prev_width = 800;
int prev_height = 600;
int prev_win_x = 0;
int prev_win_y = 0;

const char* title_loading = " FEM Demo [loading...]";
const char* title_active = " FEM Demo [running]";
const char* title_paused = " FEM Demo [paused]";

float drawText_x = 0;
float drawText_x0 = 0;
float drawText_y = 0;
void* drawText_glutFont = GLUT_BITMAP_HELVETICA_12;
float drawText_lineH = 15;
float drawText_alignH = 0;
float drawText_alignV = 0;

int anim_iteration;
int iter_last_time;
double fps;
double mean_fps;

enum { FPS_ITERATIONS=100 };
enum { FPS_SAMPLES=10 };
double iter_time_buffer[FPS_SAMPLES];

void _reshape(int w, int h);
void _display();
void _idle();
void _mouse(int button, int state, int x, int y);
void _motion(int x, int y);
void _keyboard(unsigned char key, int x, int y);
void _special(int key, int x, int y);
void drawTextFont(float size, bool serif = false);
void drawTextAlign(float h, float v);
void drawTextPos(float x, float y);
float drawText(const char* str);
float drawTextF(const char* fmt, ...);

void GuiGlut::init(int argc, char ** argv) {
    glut_gui = this;

    glutInit(&argc, argv);
    glutInitWindowSize(m_window_width, m_window_height);
    glutInitDisplayString("rgba depth>=16 double samples");

    winid = glutCreateWindow(title_loading);
    glutReshapeFunc ( _reshape );
    glutDisplayFunc ( _display );
    glutIdleFunc ( _idle );
    glutMouseFunc ( _mouse );
    glutMotionFunc ( _motion );
    glutKeyboardFunc ( _keyboard );
    glutSpecialFunc ( _special );
    //glutWMCloseFunc ( close );

    initSimulation(argc,argv);

    initGL();
}

void GuiGlut::run() {
    glutMainLoop();
}

void _reshape(int w, int h) {
    glut_gui->reshape(w,h);
}

void _display() {
    glut_gui->display();

    if (glut_gui->m_displayFlag.isActive(DisplayFlag::STATS)) {
        glDisable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0.5f, glut_gui->m_window_width+0.5f, 0.5f, glut_gui->m_window_height+0.5f);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glEnable(GL_BLEND);

        if (glut_gui->m_background_color[0] > 0.5f)
            glColor4f(0.0f,0.0f,0.0f,1.0f);
        else
            glColor4f(1.0f,1.0f,1.0f,1.0f);

        if (glut_gui->m_background_color[0] > 0.5f)
            glColor4f(0.0f,0.0f,0.0f,1.0f);
        else
            glColor4f(1.0f,1.0f,1.0f,1.0f);
        drawTextFont(15);
        drawTextAlign(0,0);

        if (pause_animation) {
            drawTextPos(glut_gui->m_window_width/2.0-120,glut_gui->m_window_height-20.0);
            drawTextF("PAUSE : press SPACE to start the animation");
        }

        drawTextPos(10,20);
        drawTextF("FPS: %.1f",fps);
        drawTextF("%s",glut_gui->m_deviceName.c_str());
        drawTextF("Picking Stiffness: %.1f",glut_gui->m_simulation.getPickingStiffness());
        drawTextF("");

        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }

    glutSwapBuffers();
}

void _idle() {
    if (!pause_animation) {
        double t = glutGet(GLUT_ELAPSED_TIME);

        if (anim_iteration % FPS_ITERATIONS == 0) {
            if (anim_iteration > 0) {
                double dt = t - iter_last_time;
                fps = (FPS_ITERATIONS*1000) / dt;
                int s = (anim_iteration/FPS_ITERATIONS);
                iter_time_buffer[(s-1) % FPS_SAMPLES] = dt;
                int ns = (s >= FPS_SAMPLES) ? FPS_SAMPLES : s;
                double ttotal = 0;
                for (int i = s-ns; i < s; ++i)
                    ttotal += iter_time_buffer[i % FPS_SAMPLES];
                mean_fps = (ns * FPS_ITERATIONS * 1000) / ttotal;

            }
            iter_last_time = t;
        }

        glut_gui->idle();
        anim_iteration++;
    }

    glutPostRedisplay();
}

void _motion(int x, int y) {
    glut_gui->motion(x,y);
}

void _mouse(int button, int state, int x, int y) {

    int modifiers = glutGetModifiers();

    Gui::MOUSE_BUTTON _button;
    Gui::MOUSE_STATE _state;
    bool _shift = (modifiers == GLUT_ACTIVE_SHIFT);

    if (button == GLUT_LEFT_BUTTON) _button = Gui::LEFT_BUTTON;
    else if (button == GLUT_RIGHT_BUTTON) _button = Gui::RIGHT_BUTTON;
    else if (button == GLUT_WHEEL_UP) _button = Gui::WHEEL_UP;
    else _button = Gui::WHEEL_DOWN;

    if (state == GLUT_DOWN) _state = Gui::STATE_DOWN;
    else _state = Gui::STATE_UP;

    glut_gui->mouse(_button,_state,_shift,x,y);
}

void _keyboard(unsigned char key, int /*x*/, int /*y*/) {
    switch (key)
    {
    case ' ': // SPACE
    {
        pause_animation = !pause_animation;
        glutSetWindowTitle(pause_animation ? title_paused : title_active);
        break;
    }
    case 8: // DEL
    case '0':
        glut_gui->m_simulation.reset_camera();
//        if (pause_animation) glutPostRedisplay();
        break;
    case 27: // ESC
        if (!fullscreen) {
            exit(0);
            break;
        }
    case '\r': // ENTER
        fullscreen = !fullscreen;
        if (fullscreen)
        {
            prev_win_x = glutGet(GLUT_WINDOW_X);
            prev_win_y = glutGet(GLUT_WINDOW_Y);
            prev_width = glutGet(GLUT_WINDOW_WIDTH);
            prev_height = glutGet(GLUT_WINDOW_HEIGHT);
            glutFullScreen();
        }
        else
        {
            glutReshapeWindow(prev_width,  prev_height);
            glutPositionWindow(prev_win_x, prev_win_y);
        }
        break;
    case '+': // ENTER
        glut_gui->m_simulation.addPickingStiffness(10);
        break;
    case '-': // ENTER
        glut_gui->m_simulation.addPickingStiffness(-10);
        break;
    case 's': // STEP
        pause_animation = true;
        glut_gui->m_simulation.step();
//        glutPostRedisplay();
    }
}

void _special(int key, int /*x*/, int /*y*/) {
    if (key >= GLUT_KEY_F1 && key < GLUT_KEY_F1 + DisplayFlag::Nflag)
    {
        DisplayFlag::DisplayMode mode = (DisplayFlag::DisplayMode) (key-GLUT_KEY_F1);
        glut_gui->m_displayFlag.switchMode(mode);
        return;
    }

    switch (key)
    {
    case GLUT_KEY_UP:
        glut_gui->m_simulation.translateCamera(0,10);
        break;
    case GLUT_KEY_DOWN:
        glut_gui->m_simulation.translateCamera(0,-10);
        break;
    case GLUT_KEY_LEFT:
        glut_gui->m_simulation.translateCamera(50,0);
        break;
    case GLUT_KEY_RIGHT:
        glut_gui->m_simulation.translateCamera(-50,0);
        break;
    case GLUT_KEY_PAGE_UP:
        glut_gui->m_simulation.rotateCamera(0,25);
        break;
    case GLUT_KEY_PAGE_DOWN:
        glut_gui->m_simulation.rotateCamera(0,-25);
        break;
    default:
        ;
    }
}

void drawTextPos(float x, float y)
{
    drawText_x0 = drawText_x = x;
    drawText_y = y;
    //glRasterPos2i(x, window_height-y);
}

void drawTextAlign(float h, float v)
{
    drawText_alignH = h;
    drawText_alignV = v;
}

void drawTextFont(float size, bool serif)
{
    if (serif)
    {
        if (size < 17)
        { drawText_glutFont = GLUT_BITMAP_TIMES_ROMAN_10; drawText_lineH = 10; }
        else
        { drawText_glutFont = GLUT_BITMAP_TIMES_ROMAN_24; drawText_lineH = 24; }
    }
    else
    {
        if (size < 12)
        { drawText_glutFont = GLUT_BITMAP_HELVETICA_10; drawText_lineH = 10; }
        else if (size < 16)
        { drawText_glutFont = GLUT_BITMAP_HELVETICA_12; drawText_lineH = 12; }
        else
        { drawText_glutFont = GLUT_BITMAP_HELVETICA_18; drawText_lineH = 18; }
    }
    if (size > drawText_lineH) drawText_lineH = size;
    drawText_lineH += 2; // line spacing
}

float drawText(const char* str)
{
    float x = drawText_x;
    float y = drawText_y;
    float h = 0;
    float lineH = drawText_lineH;
    if (drawText_alignV != 0)
    {
        int nbl = 0;
        const char* s;
        for (s = str; *s; ++s)
            if (*s == '\n') ++nbl;
        if (s > str && s[-1] != '\n') ++nbl;
        y -= lineH*nbl * drawText_alignV;
        if (drawText_alignV <= 0.5f)
            drawText_y += lineH*nbl;
        else
            drawText_y -= lineH*nbl;
        //glRasterPos2i(x, window_height-y);
    }

    glRasterPos2i(x, glut_gui->m_window_height-y);
    bool eol = true;
    while (*str)
    {
        char c = *str;
        if (eol && drawText_alignH != 0)
        {
            float w = 0;
            const char* s;
            for (s = str+1; *s && *s != '\n'; ++s)
                w += glutBitmapWidth(drawText_glutFont, *s);
            x = drawText_x0 - w * drawText_alignH;
        }
        eol = (c == '\n');
        if (eol)
        {
            y += lineH; h += lineH;
            x = drawText_x0;
        }
        else
        {
            glRasterPos2i(x, glut_gui->m_window_height-y);
            glutBitmapCharacter(drawText_glutFont, c);
            x += glutBitmapWidth(drawText_glutFont, c);
        }
        ++str;
    }
    if ((drawText_alignV != 0 || drawText_alignH == 0) && !eol)
    {
        y += lineH; h += lineH; // auto end-of-line
        x = drawText_x0;
    }
    if (drawText_alignV == 0)
        drawText_y = y;
    if (drawText_alignH == 0)
        drawText_x = x;

    return h;
}

float drawTextF(const char* fmt, ...)
{
	va_list args;
	va_start( args, fmt );
    char buffer[2048];
	vsnprintf(buffer, sizeof(buffer), fmt, args );
	va_end( args );
    // make sure the string is zero-terminated
    buffer[sizeof(buffer)-1] = '\0';
	return drawText(buffer);
}
