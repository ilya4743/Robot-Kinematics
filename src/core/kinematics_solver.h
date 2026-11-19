#pragma once
#include <QVector>
#include <QMatrix4x4>
#include "dhparams.h"

class KinematicsSolver {
public:
    static QMatrix4x4 dh_transform(double theta, double d, double a, double alpha);
    static QMatrix4x4 forward(const RobotModel& model, const QVector<double>& theta_rad);

    static QVector<QMatrix4x4> forwardFrames(const RobotModel& model, const QVector<double>& theta_rad);
};
