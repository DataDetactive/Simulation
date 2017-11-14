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


//#include "DiagramScene.h"
#include "MainWindows.h"
#include <QtWidgets>
#include "QGLRendering.h"



//    std::ostringstream o;
//    o << "CPU: " << cpu_name();
//    simulation.setDeviceName(o.str());


static void cpuid(unsigned int a, unsigned int b[4])
{
    asm volatile("xchgl %%ebx, %1\n"
                 "cpuid\n"
                 "xchgl %%ebx, %1\n"
                 :"=a"(*b),"=r"(*(b+1)),
                 "=c"(*(b+2)),"=d"(*(b+3)):"0"(a));
}


std::string MainWindow::cpu_name()
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


////! [0]
MainWindow::MainWindow(int argc, char **argv)
{

//    scene = new DiagramScene(itemMenu, this);
//    scene->setSceneRect(QRectF(0, 0, 5000, 5000));
//    connect(scene, SIGNAL(itemInserted(DiagramItem*)),
//            this, SLOT(itemInserted(DiagramItem*)));
//    connect(scene, SIGNAL(textInserted(QGraphicsTextItem*)),
//            this, SLOT(textInserted(QGraphicsTextItem*)));
//    connect(scene, SIGNAL(itemSelected(QGraphicsItem*)),
//            this, SLOT(itemSelected(QGraphicsItem*)));
//    createToolbars();



    if (! simulation.read_scene(argv[1])) return ;



    rendering = new QGLRendering(this);

    simulation.init();

//    QHBoxLayout *layout = new QHBoxLayout;
////    layout->addWidget(toolBox);
//    view = new QGraphicsView(scene);
//    layout->addWidget(view);

//    QWidget *widget = new QWidget;
//    widget->setLayout(layout);

    setCentralWidget(rendering);
    setWindowTitle(cpu_name().c_str());
//    setUnifiedTitleAndToolBarOnMac(true);


//    this->setGeometry(100, 100, 800, 500);


//    std::cout << PLUGIN_DATA_DIR_ << std::endl;
}

//MainWindow::load(const char * fn)
//{


//}

