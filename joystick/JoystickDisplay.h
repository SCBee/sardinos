/**
\file
Copyright (c) 2019 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/Percentage.h>
#include <QObject>
#include <QtGlobal>

namespace LmCdl {
class I_HardwareCollectionApi;
}
class QDialog;
class QVBoxLayout;
class QListWidget;

class JoystickDisplay : public QObject {
    Q_OBJECT
public:
    JoystickDisplay(LmCdl::I_HardwareCollectionApi& hardwareCollectionApi);
    ~JoystickDisplay();

private slots:
    void connectToJoystick(QString joystickName);
    void displayButtonValue(int button, bool value);
    void displayAxisValue(int button, LmCdl::Percentage value);

private:
    Q_DISABLE_COPY(JoystickDisplay)

    void addItemWithDescriptionAndCheckSize(QString description);

    LmCdl::I_HardwareCollectionApi& hardwareCollectionApi_;
    QDialog* dialog_;
    QVBoxLayout* layout_;
    QListWidget* listWidget_;
};
