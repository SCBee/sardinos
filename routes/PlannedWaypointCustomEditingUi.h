/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <QScopedPointer>
#include <QWidget>
#include <QtGlobal>

class Ui_PlannedWaypointCustomProperties;
class QCheckBox;

class PlannedWaypointCustomEditingUi : public QWidget {
public:
    explicit PlannedWaypointCustomEditingUi();
    virtual ~PlannedWaypointCustomEditingUi();

    QCheckBox& cloakingDeviceEdit();

private:
    Q_DISABLE_COPY(PlannedWaypointCustomEditingUi);

    QScopedPointer<Ui_PlannedWaypointCustomProperties> ui_;
};
