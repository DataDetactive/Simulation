#include <animationloop/DefaultAnimationLoop.h>
#include <stdio.h>
#include <loader/Loader.h>
#include <core/Visitor.h>
#include <integrator/Integrator.h>
#include <mapping/Mapping.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

//// TODO : implement this function
//void DefaultAnimationLoop::step() {}

void DefaultAnimationLoop::draw(DisplayFlag flag) {
    if (!flag.isActive(DisplayFlag::STATS)) return;

    char buf[256];
    sprintf(buf,"Simulation Time: %.3f",m_time);

    int window_width = glutGet(GLUT_WINDOW_WIDTH);
    int window_height = glutGet(GLUT_WINDOW_HEIGHT);

    glMatrixMode( GL_PROJECTION ) ;
    glPushMatrix() ; // save
    glLoadIdentity();// and clear
    gluOrtho2D(0.5f, window_width+0.5f, 0.5f, window_height+0.5f);
    glMatrixMode( GL_MODELVIEW ) ;
    glPushMatrix() ;
    glLoadIdentity() ;

    glDisable( GL_DEPTH_TEST ) ; // also disable the depth test so renders on top

    GLfloat pos[4];
    glGetFloatv(GL_CURRENT_RASTER_POSITION, pos);

    glRasterPos2f( 10,10) ; // center of screen. (-1,0) is center left.

    int i=0;
    while (buf[i] != 0) {
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, buf[i] );
        i++;
    }

    glRasterPos4fv(pos) ;

    glEnable( GL_DEPTH_TEST ) ; // Turn depth testing back on

    glMatrixMode( GL_PROJECTION ) ;
    glPopMatrix() ; // revert back to the matrix I had before.
    glMatrixMode( GL_MODELVIEW ) ;
    glPopMatrix() ;
}

DECLARE_CLASS(DefaultAnimationLoop)

