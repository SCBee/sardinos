/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/UniqueIdentifier.h>
#include <QObject>
#include <QPointer>
#include <QtGlobal>

class QLabel;
class QWidget;

namespace LmCdl {
class I_GdtApi;
class I_VehicleLocation;
}

class GdtWidgets : public QObject {
public:
    GdtWidgets(
        LmCdl::UniqueIdentifier vehicleId, LmCdl::I_VehicleLocation& vehicleLocationApi, LmCdl::I_GdtApi& gdtApi);
    virtual ~GdtWidgets();

    QWidget& graphicsWidgetContents();

private:
    void refreshContents();

    Q_DISABLE_COPY(GdtWidgets)

private:
    LmCdl::UniqueIdentifier vehicleId_;
    LmCdl::I_GdtApi& gdtApi_;

    QPointer<QWidget> graphicsWidgetContents_;

    QLabel* gdtLatitude_;
    QLabel* gdtLongitude_;
    QLabel* gdtHorizontalRange_;
    QLabel* gdtSlantRange_;
    QLabel* bearingToGdt_;
};
