#ifndef SIMULATION_H
#define SIMULATION_H

#include <gui/Simulation.h>
#include <dataStructures.h>
#include <core/Node.h>
#include <animationloop/AnimationLoop.h>
#include <state/State.h>
#include <core/Visitor.h>
#include <libxml/xmlreader.h>
#include <constraint/Constraint.h>
#include <constraint/PositionConstraint.h>
#include <forcefield/ForceField.h>
#include <forcefield/SpringForceField.h>

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
            TReal pdot = p.dot(m_picked_dir);
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

class ComputeBBoxVisitor : public Visitor {
public:
    bool processObject(BaseObject * o) {
        o->computeBBox(m_bbox);
        return true;
    }

    BoundingBox get(Context * c) {
        this->execute(c);
        return m_bbox;
    }

private :
    BoundingBox m_bbox;

};

class Simulation {
public:
    Simulation();

    bool read_scene(const char * filename);

    void init();

    void step();

    void render(DisplayFlag displayFlag);

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

    double getSize();

    double getPickingStiffness();

    void addPickingStiffness(double s);

    std::string getFileName();

private:
    TVec3 camera_lookat;
    double camera_distance;
    TVec3 camera_direction;

    TVec3 camera_position;
    TVec3 light0_position;
    TVec3 light1_position;

    TColor light_ambient;
    TColor light0_color;
    TColor light1_color;

    TVec3 picked_origin;
    TVec3 picked_dir;

    Node * main_Node;

    double m_time;
    BoundingBox m_bbox;

    std::string m_filename;

    SpringForceField pickingForceField;
    PositionConstraint pickingConstraint;

    void update_picking_org(int x, int y);
    void updatePickingForce();

    bool processNode(Node * node,xmlTextReaderPtr reader);
};

#endif
