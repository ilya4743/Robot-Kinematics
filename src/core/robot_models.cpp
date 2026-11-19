#include "robot_models.h"
#include <QtMath>

QVector<RobotModel> buildRobotModels() {
    const double PI = M_PI;

    RobotModel a;
    a.name = "RB-6A";
    a.joints = {
                 { 12, 0.0,    0.200,  PI/2 },
                 { 30, -0.5,   0.130,  0.0  },
                 { -45, -0.45, -0.15,  0.0  },
                 { 20, 0.0,    0.20,   PI/2 },
                 { 60, 0.0,    0.23,  -PI/2 },
                 { 10, 0.0,    0.20,   0.0  },
                 };

    RobotModel b;
    b.name = "RB-6B";
    b.joints = {
                 { 15,  0.0,    0.185,  PI/2 },
                 { -40, -0.6,   0.120,  0.0  },
                 { 55,  -0.42, -0.14,   0.0  },
                 { -20, 0.0,    0.19,   PI/2 },
                 { 70,  0.0,    0.22,  -PI/2 },
                 { 35,  0.0,    0.18,   0.0  },
                 };

    RobotModel c;
    c.name = "RB-6C";
    c.joints = {
                 { -10, 0.0,    0.250,  PI/2 },
                 { 40,  -0.7,   0.150,  0.0  },
                 { -25, -0.5,  -0.16,   0.0  },
                 { 45,  0.0,    0.22,   PI/2 },
                 { -30, 0.0,    0.25,  -PI/2 },
                 { 0,   0.0,    0.22,   0.0  },
                 };

    return { a, b, c };
}