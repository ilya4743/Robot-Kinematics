#include "kinematics.h"
#include "core/kinematics_solver.h"
#include <QtMath>
#include <QQuaternion>

Kinematics::Kinematics(QObject* parent)
    : QObject(parent), m_models(buildRobotModels()) {
    m_thetasDeg = initialThetas();
    recompute();
}


QStringList Kinematics::modelNames() const {
    QStringList names;
    for (const auto& m : m_models) {
        names << m.name;
    }
    return names;
}


void Kinematics::setCurrentModel(int index) {
    if (index < 0 || index >= m_models.size()) {
        return;
    }
    if (index == m_modelIndex) {
        return;
    }

    m_modelIndex = index;
    m_thetasDeg = initialThetas();

    emit currentModelChanged();
    emit thetasChanged();
    recompute();
}


QStringList Kinematics::jointNames() const {
    QStringList names;
    const auto& model = m_models[m_modelIndex];
    for (int i = 0; i < model.joints.size(); ++i) {
        names << QString("Joint %1").arg(i);
    }
    return names;
}


void Kinematics::setThetas(const QVector<double>& deg) {
    if (deg == m_thetasDeg) {
        return;
    }
    m_thetasDeg = deg;
    emit thetasChanged();
    recompute();
}


void Kinematics::resetThetas() {
    setThetas(initialThetas());
}


QVector<double> Kinematics::initialThetas() const {
    const auto& joints = m_models[m_modelIndex].joints;
    QVector<double> thetas;
    thetas.reserve(joints.size());
    for (const auto& joint : joints) {
        thetas.append(joint.theta0_deg);
    }
    return thetas;
}


void Kinematics::recompute() {
    QVector<double> radians;
    radians.reserve(m_thetasDeg.size());
    for (double degrees : m_thetasDeg) {
        radians.append(qDegreesToRadians(degrees));
    }

    const auto frames = KinematicsSolver::forwardFrames(m_models[m_modelIndex], radians);
    m_jointPositions3D.clear();
    m_jointRotations3D.clear();
    m_jointPositions3D.reserve(frames.size());
    m_jointRotations3D.reserve(frames.size());

    for (const auto& frame : frames) {
        const auto position = frame.column(3).toVector3D();
        const auto rotation = QQuaternion::fromRotationMatrix(frame.toGenericMatrix<3, 3>());
        m_jointPositions3D.append(QVariant::fromValue(position));
        m_jointRotations3D.append(QVariant::fromValue(rotation.toEulerAngles()));
    }

    const QVector3D tcp = frames.constLast().column(3).toVector3D();
    if (tcp != m_tcp) {
        m_tcp = tcp;
        emit tcpChanged();
    }

    emit jointData3DChanged();
}
