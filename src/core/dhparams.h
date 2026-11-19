#pragma once
#include <QString>
#include <QVector>

struct DHParams {
    double theta0_deg = 0.0;
    double a = 0.0;
    double d = 0.0;
    double alpha = 0.0;
};


struct RobotModel {
    QString name;
    QVector<DHParams> joints;
};
