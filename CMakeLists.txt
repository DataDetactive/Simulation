# Reference: http://qt-project.org/doc/qt-5.0/qtdoc/cmake-manual.html

cmake_minimum_required(VERSION 2.8)
project(microSofa)

#SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCMAKE_BUILD_TYPE=Release")
#SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCMAKE_BUILD_TYPE=Debug")

set(HEADER_FILES
    src/core/Node.h
    src/core/BaseObject.h
    src/core/Context.h
    src/core/Data.h
    src/core/Factory.h
    src/core/Visitor.h
    src/gui/glut_methods.h
    src/gui/Simulation.h
    src/topology/Topology.h
    src/loader/Loader.h
    src/loader/MeshGmshLoader.h
    src/loader/MeshVTKLoader.h
    src/loader/MeshObjLoader.h
    src/forcefield/ForceField.h
    src/forcefield/UniformMass.h
    src/forcefield/TetrahedronFEMForceField.h
    src/forcefield/TetrahedronFEMForceField_assembled.h
    src/forcefield/TetrahedronFEMForceField_optim.h
    src/constraint/Constraint.h
    src/constraint/BoxConstraint.h
    src/state/State.h
    src/state/OglModel.h
    src/integrator/Integrator.h
    src/integrator/EulerExplicit.h
    src/integrator/EulerImplicit.h
    src/mapping/BarycentricMapping.h
    src/mapping/Mapping.h
    src/solver/Solver.h
    src/solver/CGLinearSolver.h
    src/animationloop/AnimationLoop.h
    src/animationloop/DefaultAnimationLoop.h
    src/visual/OglTexture.h
    src/visual/Visual.h
    src/Sofa/Vec.h
    src/Sofa/Mat.h
    src/Sofa/fixed_array.h
    src/dataStructures.h


)

set(SOURCE_FILES
    src/core/Factory.cpp
    src/core/Node.cpp
    src/core/Data.cpp
    src/gui/glut_methods.cpp
    src/gui/Simulation.cpp
    src/loader/Loader.cpp
    src/loader/MeshGmshLoader.cpp
    src/loader/MeshVTKLoader.cpp
    src/loader/MeshObjLoader.cpp
    src/topology/Topology.cpp
    src/forcefield/UniformMass.cpp
    src/forcefield/TetrahedronFEMForceField.cpp
    src/forcefield/TetrahedronFEMForceField_assembled.cpp
    src/forcefield/TetrahedronFEMForceField_optim.cpp
    src/constraint/BoxConstraint.cpp
    src/state/State.cpp
    src/state/OglModel.cpp
    src/integrator/EulerExplicit.cpp
    src/integrator/EulerImplicit.cpp
    src/animationloop/DefaultAnimationLoop.cpp
    src/mapping/BarycentricMapping.cpp
    src/solver/CGLinearSolver.cpp
    src/visual/OglTexture.cpp
    src/visual/Visual.cpp
    src/main.cpp
)


set(OTHER_FILES
    data/config.xml
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")

include_directories(/usr/include/libxml2/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)


add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${OTHER_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DPLUGIN_DATA_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/\"")

target_link_libraries(${PROJECT_NAME} glut GL GLU dl GLEW png xml2 microSofaImpl)

