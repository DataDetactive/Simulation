/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "QGLRendering.h"
#include <stdio.h>
#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <core/Node.h>

QGLRendering::QGLRendering(MainWindow * mw) {
    main_mindow = mw;
}

void QGLRendering::initializeGL() {
    QGLWidget::initializeGL();
//    this->initializeGL();
//    if (glut_start_step > 0) return;

//    glutInit(&argc, argv);
//    if (strstr((const char *) glGetString(GL_EXTENSIONS),"GL_ARB_multitexture")) {
//        PFNGLACTIVETEXTUREARBPROC tglActiveTextureARB       = (PFNGLACTIVETEXTUREARBPROC)glXGetProcAddress((const GLubyte*) "glActiveTextureARB");
//        PFNGLCLIENTACTIVETEXTUREARBPROC tglClientActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC)glXGetProcAddress((const GLubyte*) "glClientActiveTextureARB");
//        if (!tglActiveTextureARB || !tglClientActiveTextureARB) {
//            std::cerr << "impossible d'initialiser l'extension GL_ARB_multitexture" << std::endl;
//            exit(0);
//        } else {
//            glActiveTextureARB = tglActiveTextureARB;
//            glClientActiveTextureARB = tglClientActiveTextureARB;
//        }
//    } else {
//        std::cerr << "votre carte ne supporte pas l'extension GL_ARB_multitexture" << std::endl;
//        exit(0);
//    }
//    glutInitWindowSize(window_width, window_height);
//    glutInitDisplayString("rgba depth>=16 double samples");
//    glutInit(&argc, argv);
//    winid = glutCreateWindow(title_loading);
//    glutReshapeFunc ( reshape );
//    glutDisplayFunc ( display );
//    glutIdleFunc ( idle );
//    glutMouseFunc ( mouse );
//    glutMotionFunc ( motion );
//    glutKeyboardFunc ( keyboard );
//    glutSpecialFunc ( special );
//    //glutWMCloseFunc ( close );

    glewInit();

    glMatrixMode   ( GL_MODELVIEW );  // Select The Model View Matrix
    glLoadIdentity ( );    // Reset The Model View Matrix

    glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
//    glutSwapBuffers ();
//    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
//    glutSwapBuffers ();

//    glClearColor ( background_color[0], background_color[1], background_color[2], background_color[3] );

//    glut_start_step = 1;
}

void QGLRendering::paintGL() {

//    if (anim_iteration % FPS_ITERATIONS == 0) {
//        if (anim_iteration > 0) {
//            double dt = t - iter_last_time;
//            fps = (FPS_ITERATIONS*1000) / dt;
//            int s = (anim_iteration/FPS_ITERATIONS);
//            iter_time_buffer[(s-1) % FPS_SAMPLES] = dt;
//            int ns = (s >= FPS_SAMPLES) ? FPS_SAMPLES : s;
//            double ttotal = 0;
//            for (int i = s-ns; i < s; ++i)
//                ttotal += iter_time_buffer[i % FPS_SAMPLES];
//            mean_fps = (ns * FPS_ITERATIONS * 1000) / ttotal;

//        }
//        iter_last_time = t;
//    }

    printf("HO\n");
//    main_mindow->simulation.step();

    DisplayFlag flag;
    flag.flag = 1<< DisplayFlag::VISUAL;
    main_mindow->simulation.render(flag);

        //    ++nb_idle;

        //    if (glut_start_step == 1) {
        ////        if (nb_idle < 10) {
        ////            glutPostRedisplay();
        ////        }
        //        setup_glut();
        //    }

        //    if (glut_start_step < 2) return;

        //    if (!pause_animation) simulation.step(glutGet(GLUT_ELAPSED_TIME));

        //    glutPostRedisplay();

//    this->update();
}


void QGLRendering::resizeGL(int w, int h) {
    QGLWidget::resizeGL(w,h);

    TReal simulation_size = 10;//(main_mindow->simulation.main_Node->getBBox().max - main_mindow->simulation.main_Node->getBBox().min).norm();

    glViewport(0, 0, w, h);
    glMatrixMode   ( GL_PROJECTION );  // Select The Projection Matrix
    glLoadIdentity ( );                // Reset The Projection Matrix
    gluPerspective ( 40, (GLdouble)w / (GLdouble)h, 0.001*simulation_size, 100.0*simulation_size );
    glMatrixMode   ( GL_MODELVIEW );  // Select The Model View Matrix
}
