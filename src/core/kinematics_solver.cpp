#include "kinematics_solver.h"
#include <QtMath>

QMatrix4x4 KinematicsSolver::dh_transform(double theta, double d, double a, double alpha) {
    const double ct = qCos(theta), st = qSin(theta);
    const double ca = qCos(alpha), sa = qSin(alpha);

    return QMatrix4x4(
        ct,  -st*ca,  st*sa,  a*ct,
        st,   ct*ca, -ct*sa,  a*st,
        0.0,  sa,     ca,     d,
        0.0,  0.0,    0.0,    1.0
        );
}


QMatrix4x4 KinematicsSolver::forward(const RobotModel& model,
                                     const QVector<double>& theta_rad) {
    return forwardFrames(model, theta_rad).constLast();
}


QVector<QMatrix4x4> KinematicsSolver::forwardFrames(const RobotModel& model,
                                                    const QVector<double>& theta_rad) {
    QVector<QMatrix4x4> frames;
    frames.reserve(model.joints.size() + 1);

    QMatrix4x4 frame;
    frames.append(frame);
    for (qsizetype i = 0; i < model.joints.size(); ++i) {
        const auto& joint = model.joints[i];
        const double theta = i < theta_rad.size() ? theta_rad[i]
                                                 : qDegreesToRadians(joint.theta0_deg);
        frame = frame * dh_transform(theta, joint.d, joint.a, joint.alpha);
        frames.append(frame);
    }
    return frames;
}
