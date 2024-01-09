/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#pragma once

#include <QLineEdit>
#include <QObject>
#include <QPointer>

#include <LmCdl/I_PanterraPlugin.h>
#include <LmCdl/I_PanterraShape.h>
#include <LmCdl/I_VcsiApplicationApiPlugin.h>

#include <FaaObstacleInfo.h>

namespace LmCdl {
class I_PanterraPublicApi;
class I_PanterraShape;
class I_VcsiMapLayer;
}

class FaaObstacleContent : public QObject {
    Q_OBJECT

public:
    explicit FaaObstacleContent(LmCdl::I_PanterraPublicApi& panterraApi, LmCdl::I_VcsiApplicationApi& guiApi);
    virtual ~FaaObstacleContent();

private:
    void browseForFile();
    void loadObstacleFile();
    void clearObstacles();

    void createSpheresAtObstacles(const QList<FaaObstacleInfo>& obstacles);

    void updateObstacleVisibility(bool visibility);

private:
    LmCdl::I_PanterraPublicApi& panterraApi_;
    LmCdl::I_VcsiApplicationApi& guiApi_;

    LmCdl::I_VcsiMapLayer& obstacleMapLayer_;
    bool obstacleVisibility_;

    QPointer<QLineEdit> filePathInput_;

    QList<LmCdl::I_PanterraShape*> shapeList_;
};
