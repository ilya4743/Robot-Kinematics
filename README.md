# Robot Kinematics

Тестовое задание — приложение для расчёта и визуализации прямой кинематики робота-манипулятора. Проект написан на C++17 с использованием Qt 6, QML и Qt Quick 3D.

[Демонстрация работы на YouTube](https://youtu.be/Tv4mwPgmCn8)

## Сборка на Ubuntu 26.04

Установите зависимости:

```bash
sudo apt update
sudo apt install build-essential cmake ninja-build \
  qt6-base-dev qt6-declarative-dev qt6-quick3d-dev qt6-wayland
```

В каталоге проекта выполните:

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

Запуск приложения:

```bash
./build/robot_kinematics
```
