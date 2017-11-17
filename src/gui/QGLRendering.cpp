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

QGLRendering::QGLRendering(Gui * gui) : QOpenGLWidget() {
    m_gui = gui;
}

void QGLRendering::initializeGL() {
    QOpenGLWidget::initializeGL();

    m_gui->initGL();
}

void QGLRendering::paintGL() {
    m_gui->idle();

    m_gui->display();

//    frameSwapped();
    this->update();
}

void QGLRendering::resizeGL(int w, int h) {
    m_gui->reshape(w,h);
    this->update();
}

void QGLRendering::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) m_gui->mouse(Gui::LEFT_BUTTON,Gui::STATE_DOWN,event->modifiers() & Qt::ShiftModifier,event->x(),event->y());
    else if (event->button() == Qt::RightButton) m_gui->mouse(Gui::RIGHT_BUTTON,Gui::STATE_DOWN,event->modifiers() & Qt::ShiftModifier,event->x(),event->y());
    this->update();
}

void QGLRendering::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) m_gui->mouse(Gui::LEFT_BUTTON,Gui::STATE_UP,event->modifiers() & Qt::ShiftModifier,event->x(),event->y());
    else if (event->button() == Qt::RightButton) m_gui->mouse(Gui::RIGHT_BUTTON,Gui::STATE_UP,event->modifiers() & Qt::ShiftModifier,event->x(),event->y());
    this->update();
}

void QGLRendering::wheelEvent ( QWheelEvent * event ) {
  if(event->delta() > 0) {
      Gui::MOUSE_BUTTON _button = Gui::WHEEL_UP;
      Gui::MOUSE_STATE _state = Gui::STATE_UP;
      bool _shift = event->modifiers() & Qt::ShiftModifier;

      m_gui->mouse(_button,_state,_shift,event->x(),event->y());
  } else if(event->delta() < 0) {
      Gui::MOUSE_BUTTON _button = Gui::WHEEL_DOWN;
      Gui::MOUSE_STATE _state = Gui::STATE_DOWN;
      bool _shift = event->modifiers() & Qt::ShiftModifier;
      m_gui->mouse(_button,_state,_shift,event->x(),event->y());
  }
  this->update();
}


void QGLRendering::QGLRendering::mouseMoveEvent(QMouseEvent *event) {
    m_gui->motion(event->x(),event->y());
    this->update();
}

void QGLRendering::keyPressEvent(QKeyEvent *e) {

}

void QGLRendering::keyReleaseEvent(QKeyEvent *e) {

}


