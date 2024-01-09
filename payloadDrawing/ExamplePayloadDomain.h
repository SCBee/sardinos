/**
\file
Copyright (c) 2020 Lockheed Martin Corp.  All rights reserved.
*/

#pragma once

#include <QList>
#include <QObject>
#include <QScopedPointer>
#include <QTimer>

class ExamplePayloadProjectedFootprintDrawing;
class ExamplePayloadStarepointDrawing;

namespace LmCdl {
class I_GroundElevationApi;
class I_PayloadProjectedFootprint;
class I_PayloadStarepoint;
}

class ExamplePayloadDomain : public QObject {
    Q_OBJECT
public:
    explicit ExamplePayloadDomain(LmCdl::I_GroundElevationApi& groundElevationApi);
    ~ExamplePayloadDomain() override;

    QList<LmCdl::I_PayloadProjectedFootprint*> projectedFootprintDrawings() const;
    QList<LmCdl::I_PayloadStarepoint*> starepointsDrawings() const;

private:
    Q_DISABLE_COPY(ExamplePayloadDomain);

private:
    QScopedPointer<ExamplePayloadProjectedFootprintDrawing> projectedFootprintDrawing_;
    QScopedPointer<ExamplePayloadStarepointDrawing> starepointNearFootprint_;

    QScopedPointer<ExamplePayloadStarepointDrawing> lowerStarepoint_;
    QScopedPointer<ExamplePayloadStarepointDrawing> higherStarepoint_;
    QTimer higherStarepointVisibilityToggleTimer_;

    QList<LmCdl::I_PayloadProjectedFootprint*> projectedFootprintDrawings_;
    QList<LmCdl::I_PayloadStarepoint*> starepointDrawings_;
};
