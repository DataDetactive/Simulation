#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <stdio.h>
#include <stdlib.h>

class Event {};


class MouseButtonEvent : public Event {
public :

    MouseButtonEvent(int _b, int _s, int _x,int _y) {
        m_button = _b;
        m_state = _s;
        m_x = _x;
        m_y = _y;
    }

    int getButton() {
        return m_button;
    }

    int getState() {
        return m_button;
    }

    int x() {
        return m_x;
    }

    int y() {
        return m_y;
    }

private :
    int m_button;
    int m_state;
    int m_x;
    int m_y;

};


class MouseMoveEvent : public Event {
public :

    MouseMoveEvent(int _x,int _y) {
        m_x = _x;
        m_y = _y;
    }

    int x() {
        return m_x;
    }

    int y() {
        return m_y;
    }

private :
    int m_x;
    int m_y;

};

#endif
