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

QGLRendering::QGLRendering(MainWindow * mw) : QOpenGLWidget() {
    main_mindow = mw;
}

void QGLRendering::initializeGL() {
    QOpenGLWidget::initializeGL();

    main_mindow->m_displayFlag.flag = 1<<DisplayFlag::STATS | 1<<DisplayFlag::VISUAL;

//    glutInit(argc, argv);
//    glutInitWindowSize(window_width, window_height);
//    glutInitDisplayString("rgba depth>=16 double samples");

//    winid = glutCreateWindow(title_loading);
//    glutReshapeFunc ( reshape );
//    glutDisplayFunc ( display );
//	glutIdleFunc ( idle );
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

    main_mindow->m_simulation.init();
//    reshape(window_width, window_height);
//    glutSetWindowTitle(pause_animation ? title_paused : title_active);
}

void QGLRendering::paintGL() {
    main_mindow->m_simulation.step();

    main_mindow->m_simulation.render(main_mindow->m_displayFlag);


    this->update();
}


void QGLRendering::resizeGL(int w, int h) {
    QOpenGLWidget::resizeGL(w,h);

    TReal simulation_size = 10;//(main_mindow->simulation.main_Node->getBBox().max - main_mindow->simulation.main_Node->getBBox().min).norm();

    glViewport(0, 0, w, h);
    glMatrixMode   ( GL_PROJECTION );  // Select The Projection Matrix
    glLoadIdentity ( );                // Reset The Projection Matrix
    gluPerspective ( 40, (GLdouble)w / (GLdouble)h, 0.001*simulation_size, 100.0*simulation_size );
    glMatrixMode   ( GL_MODELVIEW );  // Select The Model View Matrix
}
