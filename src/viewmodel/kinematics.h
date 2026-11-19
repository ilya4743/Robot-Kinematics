#pragma once
#include <QObject>
#include <QStringList>
#include <QVector>
#include <QVector3D>
#include <QVariantList>
#include "core/robot_models.h"

class Kinematics : public QObject {
    Q_OBJECT

    Q_PROPERTY(QStringList modelNames READ modelNames CONSTANT)
    Q_PROPERTY(int currentModel READ currentModel WRITE setCurrentModel NOTIFY currentModelChanged)
    Q_PROPERTY(QStringList jointNames READ jointNames NOTIFY currentModelChanged)
    Q_PROPERTY(QVector<double> thetas READ thetas WRITE setThetas NOTIFY thetasChanged)
    Q_PROPERTY(double tcpX READ tcpX NOTIFY tcpChanged)
    Q_PROPERTY(double tcpY READ tcpY NOTIFY tcpChanged)
    Q_PROPERTY(double tcpZ READ tcpZ NOTIFY tcpChanged)

    Q_PROPERTY(QVariantList jointPositions3D READ jointPositions3D NOTIFY jointData3DChanged)
    Q_PROPERTY(QVariantList jointRotations3D READ jointRotations3D NOTIFY jointData3DChanged)

public:
    explicit Kinematics(QObject* parent = nullptr);

    QStringList modelNames() const;
    int currentModel() const { return m_modelIndex; }
    void setCurrentModel(int index);

    QStringList jointNames() const;
    QVector<double> thetas() const { return m_thetasDeg; }
    void setThetas(const QVector<double>& deg);
    Q_INVOKABLE void resetThetas();

    double tcpX() const { return m_tcp.x(); }
    double tcpY() const { return m_tcp.y(); }
    double tcpZ() const { return m_tcp.z(); }

    QVariantList jointPositions3D() const { return m_jointPositions3D; }
    QVariantList jointRotations3D() const { return m_jointRotations3D; }

signals:
    void currentModelChanged();
    void thetasChanged();
    void tcpChanged();
    void jointData3DChanged();

private:
    QVector<double> initialThetas() const;
    void recompute();

    QVector<RobotModel> m_models;
    int m_modelIndex = 0;
    QVector<double> m_thetasDeg;
    QVector3D m_tcp;

    QVariantList m_jointPositions3D;
    QVariantList m_jointRotations3D;
};
