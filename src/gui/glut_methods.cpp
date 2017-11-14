#include <gui/glut_methods.h>
#include <gui/Simulation.h>

//// GLUT Methods ////

extern Simulation simulation;
TColor background_color = TColor(1.0f, 1.0f, 1.0f, 0.0f);

int glut_start_step = 0;
int pause_animation = true;
int camera_mode = -1;
int window_width = 800;
int window_height = 600;
int winid = 0;
//int menuid = 0;
int nb_idle = 0;

#if !defined(GLUT_WHEEL_UP)
#  define GLUT_WHEEL_UP   3
#  define GLUT_WHEEL_DOWN 4
#endif

int mouse_x = 0;
int mouse_y = 0;

int fullscreen = 0;
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

DisplayFlag displayFlag;
std::string device_name;

int anim_iteration;
int iter_last_time;
double fps;
double mean_fps;

enum { FPS_ITERATIONS=100 };
enum { FPS_SAMPLES=10 };
double iter_time_buffer[FPS_SAMPLES];


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
void init_glut(int* argc, char** argv)
{
    displayFlag.flag = 1<<DisplayFlag::STATS | 1<<DisplayFlag::VISUAL;

    std::ostringstream o;
    o << "CPU: " << cpu_name();
    device_name = o.str();

    if (glut_start_step > 0) return;
    glutInit(argc, argv);
    glutInitWindowSize(window_width, window_height);
    glutInitDisplayString("rgba depth>=16 double samples");

    winid = glutCreateWindow(title_loading);
    glutReshapeFunc ( reshape );
    glutDisplayFunc ( display );
	glutIdleFunc ( idle );
    glutMouseFunc ( mouse );
    glutMotionFunc ( motion );
    glutKeyboardFunc ( keyboard );
    glutSpecialFunc ( special );
    //glutWMCloseFunc ( close );

    glewInit();

    glMatrixMode   ( GL_MODELVIEW );  // Select The Model View Matrix
    glLoadIdentity ( );    // Reset The Model View Matrix

    glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glutSwapBuffers ();
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glutSwapBuffers ();

    glClearColor ( background_color[0], background_color[1], background_color[2], background_color[3] );

    glut_start_step = 1;


}

void setup_glut()
{
    if (glut_start_step != 1) return;
    glut_start_step = 2;
    simulation.init();
    reshape(window_width, window_height);
    glutSetWindowTitle(pause_animation ? title_paused : title_active);
}

void run_glut()
{
    glutMainLoop();
}

void reshape(int w, int h)
{
    TReal simulation_size = simulation.getSize();

    window_width = w;
    window_height = h;
    glViewport(0, 0, w, h);
    glMatrixMode   ( GL_PROJECTION );  // Select The Projection Matrix
    glLoadIdentity ( );                // Reset The Projection Matrix
    gluPerspective ( 40, (GLdouble)w / (GLdouble)h, 0.001*simulation_size, 100.0*simulation_size );
    glMatrixMode   ( GL_MODELVIEW );  // Select The Model View Matrix
}

void display()
{
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    if (glut_start_step >= 2)
    {
        simulation.render(displayFlag);

        if (displayFlag.isActive(DisplayFlag::STATS)) {
            glDisable(GL_DEPTH_TEST);

            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            gluOrtho2D(0.5f, window_width+0.5f, 0.5f, window_height+0.5f);
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();
            glEnable(GL_BLEND);

            if (background_color[0] > 0.5f)
                glColor4f(0.0f,0.0f,0.0f,1.0f);
            else
                glColor4f(1.0f,1.0f,1.0f,1.0f);

            if (background_color[0] > 0.5f)
                glColor4f(0.0f,0.0f,0.0f,1.0f);
            else
                glColor4f(1.0f,1.0f,1.0f,1.0f);
            drawTextFont(15);
            drawTextAlign(0,0);

            if (pause_animation) {
                drawTextPos(window_width/2.0-120,window_height-20.0);
                drawTextF("PAUSE : press SPACE to start the animation");
            }

            drawTextPos(10,20);
            drawTextF("FPS: %.1f",fps);
            drawTextF("%s",device_name.c_str());
            drawTextF("Picking Stiffness: %.1f",simulation.getPickingStiffness());
            drawTextF("");

            glDisable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
            glPopMatrix();
        }
    }
    glutSwapBuffers();
}

void idle()
{
    ++nb_idle;

    if (glut_start_step == 1) {
//        if (nb_idle < 10) {
//            glutPostRedisplay();
//        }
        setup_glut();
    }

    if (glut_start_step < 2) return;

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

        simulation.step();

        ++anim_iteration;
    }

    glutPostRedisplay();
}

void motion(int x, int y)
{
    if (glut_start_step < 2) return;

    int dx = x - mouse_x;
    int dy = y - mouse_y;

    mouse_x = x;
    mouse_y = y;

    if (camera_mode == 0) simulation.rotateCamera(dx,dy);
    else if (camera_mode == 1) simulation.translateCamera(dx,dy);
    else if (camera_mode == 2) simulation.zoomCamera(dx,dy);
    else if (camera_mode == 3) simulation.updatePickingForce(x,y);
    else if (camera_mode == 4) simulation.updatePickingConstraint(x,y);
}

void mouse(int button, int state, int x, int y)
{
    if (glut_start_step < 2) return;

    camera_mode = -1;
    motion(x,y);
    int modifiers = glutGetModifiers();

    switch(button)
    {
        case GLUT_LEFT_BUTTON :
            if (state == GLUT_DOWN) {
                if (modifiers == GLUT_ACTIVE_SHIFT) {
                    simulation.startForcePicking(x,y);
                    camera_mode = 3;
                } else {
                    camera_mode = 0;
                }
            } else simulation.stopForcePicking();

            break;
        case GLUT_RIGHT_BUTTON :
            if (state == GLUT_DOWN) {
                if (modifiers == GLUT_ACTIVE_SHIFT) {
                    simulation.startConstraintPicking(x,y);
                    camera_mode = 4;
                } else {
                    camera_mode = 1;
                }
            } else simulation.stopConstraintPicking();

            break;
        case GLUT_WHEEL_UP:
            simulation.zoomCamera(0,10);
            break;
        case GLUT_WHEEL_DOWN:
            simulation.zoomCamera(0,-10);
//            glutPostRedisplay();
            break;
    }
}

void keyboard(unsigned char key, int /*x*/, int /*y*/)
{
    if (glut_start_step < 2 && key != 27) return;

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
        simulation.reset_camera();
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
        simulation.addPickingStiffness(10);
        break;
    case '-': // ENTER
        simulation.addPickingStiffness(-10);
        break;
    case 's': // STEP
        pause_animation = true;
        simulation.step();
//        glutPostRedisplay();
    }
}

void switchFlag(DisplayFlag::DisplayMode mode) {
    if (displayFlag.isActive(mode)) displayFlag.flag &= ~(1<<mode);
    else displayFlag.flag |= (1<<mode);
}

void special(int key, int /*x*/, int /*y*/)
{
    if (glut_start_step < 2) return;

    if (key >= GLUT_KEY_F1 && key < GLUT_KEY_F1 + DisplayFlag::Nflag)
    {
        DisplayFlag::DisplayMode mode = (DisplayFlag::DisplayMode) (key-GLUT_KEY_F1);
        switchFlag(mode);
//        glutPostRedisplay();
        return;
    }

    switch (key)
    {
    case GLUT_KEY_UP:
        simulation.translateCamera(0,10);
//        glutPostRedisplay();
        break;
    case GLUT_KEY_DOWN:
        simulation.translateCamera(0,-10);
//        glutPostRedisplay();
        break;
    case GLUT_KEY_LEFT:
        simulation.translateCamera(50,0);
//        glutPostRedisplay();
        break;
    case GLUT_KEY_RIGHT:
        simulation.translateCamera(-50,0);
//        glutPostRedisplay();
        break;
    case GLUT_KEY_PAGE_UP:
        simulation.rotateCamera(0,25);
//        glutPostRedisplay();
        break;
    case GLUT_KEY_PAGE_DOWN:
        simulation.rotateCamera(0,-25);
//        glutPostRedisplay();
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

    glRasterPos2i(x, window_height-y);
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
            glRasterPos2i(x, window_height-y);
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
