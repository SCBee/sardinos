/**
\file
Copyright (c) 2019 Lockheed Martin Corp. All rights reserved.
*/

#include <JoystickDisplay.h>

#include <LmCdl/I_HardwareCollectionApi.h>
#include <LmCdl/I_VcsiJoystick.h>
#include <QDialog>
#include <QListWidgetItem>
#include <QVBoxLayout>

JoystickDisplay::JoystickDisplay(LmCdl::I_HardwareCollectionApi& hardwareCollectionApi)
    : hardwareCollectionApi_(hardwareCollectionApi)
    , dialog_(new QDialog)
    , layout_(new QVBoxLayout(dialog_))
    , listWidget_(new QListWidget(dialog_))
{
    listWidget_->setAutoScroll(true);
    connect(&hardwareCollectionApi_, &LmCdl::I_HardwareCollectionApi::joystickAdded, this,
        &JoystickDisplay::connectToJoystick);
}

JoystickDisplay::~JoystickDisplay() { }

void JoystickDisplay::connectToJoystick(QString joystickName)
{
    LmCdl::I_VcsiJoystick& joystick = hardwareCollectionApi_.joystick(joystickName);
    connect(&joystick, &LmCdl::I_VcsiJoystick::buttonChanged, this, &JoystickDisplay::displayButtonValue);
    connect(&joystick, &LmCdl::I_VcsiJoystick::axisChanged, this, &JoystickDisplay::displayAxisValue);
    layout_->addWidget(listWidget_);
    dialog_->setLayout(layout_);
    dialog_->setWindowTitle(joystickName);
    dialog_->show();
}

void JoystickDisplay::displayButtonValue(int button, bool value)
{
    QString description = QString("button %1, value: %2").arg(button).arg(value);
    addItemWithDescriptionAndCheckSize(description);
}

void JoystickDisplay::displayAxisValue(int button, LmCdl::Percentage value)
{
    QString description = QString("axis %1, value: %2%").arg(button).arg(value.value(LmCdl::PercentageUnit::OutOf100));
    addItemWithDescriptionAndCheckSize(description);
}

void JoystickDisplay::addItemWithDescriptionAndCheckSize(QString description)
{
    QListWidgetItem* newItem = new QListWidgetItem;
    newItem->setText(description);
    listWidget_->insertItem(0, newItem);
    if (listWidget_->count() > 100) {
        delete listWidget_->takeItem(100);
    }
}
