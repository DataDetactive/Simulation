#ifndef SIMULATION_H
#define SIMULATION_H

#include <gui/Simulation.h>
#include <Sofa/Vec.h>
#include <Sofa/Mat.h>
#include <dataStructures.h>
#include <core/Node.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <animationloop/AnimationLoop.h>
#include <state/State.h>
#include <core/Visitor.h>
#include <libxml/xmlreader.h>
#include <constraint/Constraint.h>
#include <forcefield/ForceField.h>

class PickingVisitor : public Visitor {
public:
    PickingVisitor(TVec3 orig,TVec3 dir,bool can_map) : Visitor(true) {
        m_state = NULL;
        m_mindist = std::numeric_limits<double>::max();
        m_indice = -1;
        m_picked_origin = orig;
        m_picked_dir = dir;
        m_can_map = can_map;
    }

    void try_picking(State * mstate)
    {
        TReal dotmargin = (TReal)0.9995;
        std::vector<TVec3> pos = mstate->get(VecID::position);
        unsigned int nbp = pos.size();

        for (unsigned int i=0;i<nbp;++i)
        {
            TVec3 p = pos[i];
            p -= m_picked_origin;
            TReal pdist = p.norm();
            TReal pdot = dot(p,m_picked_dir);
            // we add to the distance to the particle 10 times the distance to the ray to prefer the closest particle of the same surface
            TReal dist = pdist + 10 * (m_picked_dir * pdot - p).norm();
            if (pdot / pdist > dotmargin && dist < m_mindist) {
                m_mindist = dist;
                m_state = mstate;
                m_indice = i;
            }
        }
    }

    virtual bool processObject(BaseObject * o) {
        if (State * s = dynamic_cast<State *>(o)) {
            if (m_can_map || s->isMechanical()) try_picking((State *)  o);
        }

        return true;
    }

    State * getPickedState() {
        return m_state;
    }

    int getIndice() {
        return m_indice;
    }

private :
    State * m_state;
    double m_mindist;
    int m_indice;
    bool m_can_map;
    TVec3 m_picked_origin;
    TVec3 m_picked_dir;
};

class PickingConstraint : public Constraint {
public:

    PickingConstraint() {
        d_name.setValue("PickingConstraint");
        m_indice = -1;
    }

    void applyConstraint(TVecId vf) {
        if (m_indice == -1) return;

        std::vector<TVec3> & pos = this->getContext()->getMstate()->get(VecID::position);
        std::vector<TVec3> & f = this->getContext()->getMstate()->get(vf);

        f[m_indice] = TVec3();
        pos[m_indice] = m_position;
    }

    void draw(DisplayFlag /*flag*/) {
        if (m_indice == -1) return;
        if (this->getContext() == NULL) return;

        std::vector<TVec3> & pos = this->getContext()->getMstate()->get(VecID::position);

        TVec3 p1 = pos[m_indice];

        glColor3f(1.0f,0.4f,0.4f);
        glPointSize(10);
        glBegin(GL_POINTS);
            glVertex3fv(p1.ptr());
        glEnd();
        glPointSize(1);
    }

    int m_indice;
    TVec3 m_position;
};

class PickingForceField :  public ForceField {
public:

    PickingForceField() {
        d_name.setValue("PickingForceField");
        m_indice  = -1;
        m_force = TVec3(0,0,0);
        m_stiffness = (double) 20.0;
    }

    void addForce(TVecId f) {
        this->getContext()->getMstate()->get(f)[m_indice] += m_force * m_stiffness;
    }

    void draw(DisplayFlag flag) {
        if (m_indice == -1) return;
        if (this->getContext() == NULL) return;

        std::vector<TVec3> & pos = this->getContext()->getMstate()->get(VecID::position);

        TVec3 p1 = pos[m_indice];
        TVec3 p2 = p1 + m_force;

        glLineWidth(3);
        glBegin(GL_LINES);
        glColor3f(0.8f,0.2f,0.2f); glVertex3fv(p1.ptr());
        glColor3f(1.0f,0.6f,0.6f); glVertex3fv(p2.ptr());
        glEnd(); // GL_LINES
        glLineWidth(1);
    }

    int m_indice;
    TVec3 m_force;
    double m_stiffness;

};

class Simulation {
public:
    Simulation();

    bool read_scene(const char * filename);

    void initgl();

    void step(double ell);

    void render();

    void reset_camera();

    void rotateCamera(int dx,int dy);

    void translateCamera(int dx,int dy);

    void zoomCamera(int dx,int dy);

    void updatePickingForce(int x,int y);

    void updatePickingConstraint(int x,int y);

    void startForcePicking(int x,int y);

    void startConstraintPicking(int x,int y);

    void stopForcePicking();

    void stopConstraintPicking();

    double getFPS();

    double getSize();

    double getPickingStiffness();

    void addPickingStiffness(double s);

    const char * getDeviceName();

    void  setDeviceName(std::string name);

    void switchFlag(DisplayFlag::DisplayMode flag);

    DisplayFlag getDisplayFlag();

private:
    TVec3 camera_lookat;
    double camera_distance;
    TVec3 camera_direction;

    TVec3 camera_position;
    TVec4 light0_position;
    TVec4 light1_position;

    TColor light_ambient;
    TColor light0_color;
    TColor light1_color;


    TVec3 picked_origin;
    TVec3 picked_dir;

    GLUquadric* pGLUquadric = NULL;
    Node * main_Node;

    int anim_iteration;
    int iter_last_time;
    double fps;
    double mean_fps;

    DisplayFlag m_displayFlag;

    enum { FPS_ITERATIONS=100 };
    enum { FPS_SAMPLES=10 };
    double iter_time_buffer[FPS_SAMPLES];

    std::string device_name;

    PickingForceField pickingForceField;
    PickingConstraint pickingConstraint;

    void update_picking_org(int x, int y);
    void updatePickingForce();

    bool processNode(Node * node,xmlTextReaderPtr reader);
};

#endif
