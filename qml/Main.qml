import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick3D

ApplicationWindow {
    id: root
    readonly property size fixedSize: Qt.size(900, 600)
    width: fixedSize.width
    height: fixedSize.height
    minimumWidth: fixedSize.width
    minimumHeight: fixedSize.height
    maximumWidth: fixedSize.width
    maximumHeight: fixedSize.height
    flags: Qt.Window | Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowSystemMenuHint
           | Qt.WindowMinimizeButtonHint | Qt.WindowCloseButtonHint
    title: "Robot Kinematics"
    color: "#f5f5f5"

    property var jointAxesVisibility: []

    function jointAxesVisible(jointIndex: int): bool {
        return jointAxesVisibility[jointIndex] ?? (jointIndex === kinematics.jointNames.length - 1)
    }

    header: ToolBar {
        background: Rectangle {
            color: root.color
            Rectangle {
                width: parent.width
                height: 1
                anchors.bottom: parent.bottom
                color: "#d0d0d0"
            }
        }
        RowLayout {
            anchors.fill: parent
            spacing: 5
            ToolButton { text: "О программе"; onClicked: aboutDialog.open() }
            Item { Layout.fillWidth: true }
        }
    }

    Dialog {
        id: aboutDialog
        title: "О программе"
        modal: true
        anchors.centerIn: parent
        standardButtons: Dialog.Ok
        palette.window: root.color

        Label {
            text: "Тестовое задание\nПрямая кинематика робота-манипулятора."
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 16

        ScrollView {
            id: controlsScroll
            Layout.fillHeight: true
            Layout.preferredWidth: 380
            implicitHeight: 0
            padding: 16
            contentWidth: availableWidth
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

            background: Rectangle {
                color: "white"
                radius: 8
                border.color: "#e0e0e0"
                border.width: 1
            }

            Item {
                width: controlsScroll.availableWidth
                implicitHeight: Math.max(controlsScroll.availableHeight,
                                         jointControls.implicitHeight + lowerControls.implicitHeight + 12)

                ColumnLayout {
                    id: jointControls
                    width: parent.width
                    spacing: 12

                    RowLayout {
                        Label { text: "Модель:"; font.bold: true }
                        ComboBox {
                            Layout.fillWidth: true
                            model: kinematics.modelNames
                            currentIndex: kinematics.currentModel
                            onActivated: kinematics.currentModel = currentIndex
                        }
                        Button {
                            text: "Сбросить углы"
                            onClicked: kinematics.resetThetas()
                        }
                    }

                    Repeater {
                        model: kinematics.jointNames
                        delegate: RowLayout {
                            required property int index
                            required property string modelData

                            Label { text: modelData; Layout.preferredWidth: 60 }

                            Slider {
                                id: jointSlider
                                Layout.fillWidth: true
                                from: -180
                                to: 180
                                stepSize: 1

                                value: kinematics.thetas[index] ?? 0

                                onMoved: {
                                    if (kinematics.thetas[index] !== value) {
                                        let t = kinematics.thetas.slice()
                                        t[index] = value
                                        kinematics.thetas = t
                                    }
                                }
                            }

                            Label {
                                text: (kinematics.thetas[index] ?? 0).toFixed(0) + "°"
                                Layout.preferredWidth: 40
                                horizontalAlignment: Text.AlignRight
                            }

                            CheckBox {
                                text: "Оси"
                                Accessible.name: "Показать оси " + modelData
                                checked: root.jointAxesVisible(index)
                                onToggled: {
                                    const visibility = root.jointAxesVisibility.slice()
                                    visibility[index] = checked
                                    root.jointAxesVisibility = visibility
                                }
                            }
                        }
                    }
                }

                ColumnLayout {
                    id: lowerControls
                    anchors.bottom: parent.bottom
                    width: parent.width
                    spacing: 12

                    GroupBox {
                        title: "Камера"
                        Layout.fillWidth: true
                        font.bold: true
                        palette.window: controlsScroll.background.color

                        RowLayout {
                            anchors.fill: parent

                            Label {
                                text: "Масштаб"
                                Layout.preferredWidth: 60
                            }

                            Slider {
                                id: cameraZoom
                                Layout.fillWidth: true
                                from: 100
                                to: 200
                                stepSize: 5
                                value: from
                                Accessible.name: "Масштаб 3D-сцены"
                            }

                            Label {
                                text: Math.round(cameraZoom.value) + "%"
                                Layout.preferredWidth: 40
                                horizontalAlignment: Text.AlignRight
                            }
                        }
                    }

                    GroupBox {
                        title: "TCP Координаты (м)"
                        Layout.fillWidth: true
                        font.bold: true
                        palette.window: controlsScroll.background.color

                        GridLayout {
                            columns: 2
                            rowSpacing: 8
                            columnSpacing: 16

                            Label { text: "X:" }
                            Label { text: kinematics.tcpX.toFixed(4); font.family: "Monospace" }

                            Label { text: "Y:" }
                            Label { text: kinematics.tcpY.toFixed(4); font.family: "Monospace" }

                            Label { text: "Z:" }
                            Label { text: kinematics.tcpZ.toFixed(4); font.family: "Monospace" }
                        }
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#2b2b2b"
            radius: 8
            clip: true

            View3D {
                anchors.fill: parent
                camera: mainCamera

                environment: SceneEnvironment {
                    clearColor: "#2b2b2b"
                    backgroundMode: SceneEnvironment.Color
                    antialiasingMode: SceneEnvironment.MSAA
                }

                DirectionalLight {
                    eulerRotation.x: -45
                    eulerRotation.y: -45
                    brightness: 2.0
                }
                DirectionalLight {
                    eulerRotation.x: 45
                    eulerRotation.y: 135
                    brightness: 1.0
                }

                Node {
                    position: Qt.vector3d(0, -450, 0)
                    eulerRotation.x: -30
                    eulerRotation.y: 45

                    PerspectiveCamera {
                        id: mainCamera
                        z: 4900 / (1.5 * cameraZoom.value / 100)
                        clipNear: 10
                        clipFar: 20000
                    }
                }

                Node {
                    id: robotRoot
                    // Размер стандартных примитивов Qt Quick 3D до масштабирования.
                    readonly property real primitiveSize: 100
                    position: Qt.vector3d(0, -500, 0)
                    scale: Qt.vector3d(1000, 1000, 1000) // Метры в миллиметры.
                    eulerRotation.x: -90 // Ось +Z робота направлена вверх по +Y сцены.

                    Repeater3D {
                        model: kinematics.jointPositions3D.length

                        delegate: Node {
                            required property int index

                            position: kinematics.jointPositions3D[index] ?? Qt.vector3d(0,0,0)

                            Model {
                                source: "#Sphere"
                                readonly property real diameterScale: 0.08 / robotRoot.primitiveSize
                                scale: Qt.vector3d(diameterScale, diameterScale, diameterScale)
                                materials: PrincipledMaterial {
                                    baseColor: "#ffffff"
                                    metalness: 0.8
                                    roughness: 0.2
                                }
                            }
                        }
                    }

                    Repeater3D {
                        model: Math.max(0, kinematics.jointPositions3D.length - 1)

                        delegate: Node {
                            required property int index

                            readonly property vector3d start: kinematics.jointPositions3D[index]
                                ?? Qt.vector3d(0, 0, 0)
                            readonly property vector3d end: kinematics.jointPositions3D[index + 1] ?? start
                            readonly property vector3d direction: end.minus(start)
                            readonly property real linkLength: direction.length()

                            position: start.plus(end).times(0.5)
                            visible: linkLength > 0
                            // Направляем локальную ось +Z куба вдоль звена.
                            eulerRotation.x: -Math.atan2(direction.y,
                                Math.hypot(direction.x, direction.z)) * 180 / Math.PI
                            eulerRotation.y: Math.atan2(direction.x, direction.z) * 180 / Math.PI

                            Model {
                                source: "#Cube"
                                readonly property real widthScale: 0.04 / robotRoot.primitiveSize
                                scale: Qt.vector3d(widthScale, widthScale,
                                                   linkLength / robotRoot.primitiveSize)
                                materials: PrincipledMaterial {
                                    baseColor: index % 2 === 0 ? "#4a90e2" : "#50e3c2"
                                    metalness: 0.5
                                    roughness: 0.3
                                }
                            }
                        }
                    }

                    Repeater3D {
                        model: kinematics.jointNames.length

                        delegate: Node {
                            id: jointFrame
                            required property int index
                            // Элемент 0 в списке кадров соответствует основанию робота.
                            readonly property int frameIndex: index + 1
                            readonly property real shaftLength: frameIndex === kinematics.jointNames.length
                                ? 0.18 : 0.12
                            position: kinematics.jointPositions3D[frameIndex] ?? Qt.vector3d(0, 0, 0)
                            eulerRotation: kinematics.jointRotations3D[frameIndex] ?? Qt.vector3d(0, 0, 0)
                            visible: root.jointAxesVisible(index)

                            Repeater3D {
                                model: [
                                    { color: "#ff5757", rotation: Qt.vector3d(0, 0, -90) }, // X
                                    { color: "#54df80", rotation: Qt.vector3d(0, 0, 0) },   // Y
                                    { color: "#5e9dff", rotation: Qt.vector3d(90, 0, 0) }   // Z
                                ]

                                delegate: Node {
                                    required property var modelData
                                    eulerRotation: modelData.rotation

                                    DefaultMaterial {
                                        id: axisMaterial
                                        lighting: DefaultMaterial.NoLighting
                                        diffuseColor: modelData.color
                                    }

                                    Model {
                                        source: "#Cylinder"
                                        y: jointFrame.shaftLength / 2
                                        scale: Qt.vector3d(0.018, jointFrame.shaftLength, 0.018)
                                            .times(1 / robotRoot.primitiveSize)
                                        materials: axisMaterial
                                    }

                                    Model {
                                        source: "#Cone"
                                        y: jointFrame.shaftLength
                                        scale: Qt.vector3d(0.045, 0.06, 0.045)
                                            .times(1 / robotRoot.primitiveSize)
                                        materials: axisMaterial
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
