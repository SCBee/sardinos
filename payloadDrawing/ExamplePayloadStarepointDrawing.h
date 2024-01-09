/**
 \file
 Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
 */

#pragma once

#include <LmCdl/I_PayloadStarepoint.h>
#include <QColor>
#include <QGeoCoordinate>

class ExamplePayloadStarepointDrawing : public LmCdl::I_PayloadStarepoint {
    Q_OBJECT

public:
    ExamplePayloadStarepointDrawing();

    void setVisible(bool visible);
    bool visible() const override;

    void setLocation(const QGeoCoordinate& location);
    QGeoCoordinate location() const override;

    void setColor(const QColor& color);
    QColor color() const override;

    void setRelativeZOrder(int zOrder);
    int relativeZOrder() const override;

private:
    Q_DISABLE_COPY(ExamplePayloadStarepointDrawing)

    bool visible_;
    QGeoCoordinate location_;
    QColor color_;
    int relativeZOrder_;
};
