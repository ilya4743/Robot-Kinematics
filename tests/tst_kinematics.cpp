#include <QtTest>
#include "core/kinematics_solver.h"
#include "core/robot_models.h"

class TestKinematics : public QObject {
    Q_OBJECT

private slots:
    void zeroAngles_identity() {
        RobotModel m;
        m.name = "single";
        m.joints = { {0, 0, 0, 0} };
        auto T = KinematicsSolver::forward(m, {0.0});
        QCOMPARE(T(0,3), 0.0f);
        QCOMPARE(T(1,3), 0.0f);
        QCOMPARE(T(2,3), 0.0f);
    }

    void pureTranslationZ() {
        RobotModel m;
        m.name = "single";
        m.joints = { {0, 0, 0.5, 0} };
        auto T = KinematicsSolver::forward(m, {0.0});
        QCOMPARE(T(2,3), 0.5f);
    }

    void rotation90_aroundZ_movesX() {
        RobotModel m;
        m.name = "single";
        m.joints = { {0, 0.5, 0, 0} };
        auto T = KinematicsSolver::forward(m, {M_PI/2});
        QVERIFY(qAbs(T(0,3)) < 1e-6f);
        QVERIFY(qAbs(T(1,3) - 0.5f) < 1e-6f);
    }

    void rb6c_hasSixJoints() {
        auto models = buildRobotModels();
        bool found = false;

        for (const auto& model : models) {
            if (model.name == "RB-6C") {
                QCOMPARE(model.joints.size(), 6);
                found = true;
                break;
            }
        }

        QVERIFY2(found, "Model RB-6C was not found in the list");
    }
};

QTEST_APPLESS_MAIN(TestKinematics)
#include "tst_kinematics.moc"
