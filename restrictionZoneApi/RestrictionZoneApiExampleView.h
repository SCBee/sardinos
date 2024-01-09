/**
\file
Copyright (c) 2020 Lockheed Martin Corp.  All rights reserved.
*/

#pragma once

#include <QObject>
class QAbstractButton;
class QDockWidget;
class QTextEdit;
class QWidget;
class RestrictionZoneApiExampleContent;
namespace LmCdl {
class I_VcsiApplicationApi;
}

class RestrictionZoneApiExampleView : public QObject {
    Q_OBJECT
public:
    explicit RestrictionZoneApiExampleView(
        LmCdl::I_VcsiApplicationApi& applicationApi, RestrictionZoneApiExampleContent& vectorDataRestrictionZones);
    virtual ~RestrictionZoneApiExampleView();

private slots:
    void createRestrictionZonesFromFile();

    void syncDockWidgetVisibility(bool visible);

private:
    Q_DISABLE_COPY(RestrictionZoneApiExampleView);

private:
    RestrictionZoneApiExampleContent& vectorDataRestrictionZones_;

    QWidget* mainWidget_;
    QDockWidget* dockWidget_;
    QAbstractButton* showDockWidgetButton_;
    QTextEdit* status_;
    QAbstractButton* selectFile_;
    QAbstractButton* queryZones_;
    QAbstractButton* clearZones_;
};
