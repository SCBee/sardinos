/**
 \file
 Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
 */

#pragma once

#include <LmCdl/MapPickRequestId.h>
#include <LmCdl/MapPickResult.h>
#include <QBrush>
#include <QGeoCoordinate>
#include <QObject>
#include <QPen>
#include <QScopedPointer>
#include <vector>
class QEvent;
class QGraphicsView;
class QGraphicsScene;
class QAbstractButton;
class QDockWidget;
class QLabel;
class QDoubleSpinBox;
class QSpinBox;
class QWidget;
class QGridLayout;

namespace LmCdl {
class I_VcsiDockableExtensionApi;
class I_VcsiMenuBarExtensionApi;
class I_GroundElevationApi;
class GroundElevationLookupResult;
class GroundElevation;
class I_MapPickApi;
}

class CoordinateEntry : public QObject {
public:
    CoordinateEntry(LmCdl::I_MapPickApi& mapPickApi, const QString& label);

    LmCdl::I_MapPickApi& mapPickApi;
    QAbstractButton* pickFromMap;
    QDoubleSpinBox* latitude;
    QDoubleSpinBox* longitude;
    LmCdl::MapPickRequestId pickId;

    void updatePickState();
    void updateCoordinate(const LmCdl::MapPickResult& result);
    void cancelPick(const LmCdl::MapPickRequestId& id);
    void addToLayout(QGridLayout& layout, int row);
};

class TerrainProfileContent : public QObject {
public:
    explicit TerrainProfileContent(LmCdl::I_VcsiDockableExtensionApi& dockableApi,
        LmCdl::I_VcsiMenuBarExtensionApi& menuBarApi, LmCdl::I_GroundElevationApi& groundElevationApi,
        LmCdl::I_MapPickApi& mapPickApi);
    virtual ~TerrainProfileContent();

protected:
    bool eventFilter(QObject* object, QEvent* event);

private:
    class ProfileLocation {
    public:
        double height;
        double distance;
        bool valid;

        ProfileLocation()
            : height(0.0)
            , distance(0.0)
            , valid(false)
        {
        }

        ProfileLocation(double h, double d, bool v)
            : height(h)
            , distance(d)
            , valid(v)
        {
        }
    };

    class Renderer {
    public:
        Renderer(QGraphicsScene& scene);

        void updateRenderParameters(const QSize& sceneSize);

        void renderProfileLocation(const ProfileLocation& location, double profileLength, double maxHeight);
        void renderProfile(const std::vector<ProfileLocation>& profile, double profileLength, double maxHeight);

    private:
        QGraphicsScene& scene_;

        int xmax_;
        int ymax_;
        bool flipAxes_;

        QPen pen_;
        QBrush brush_;
    };

    void layoutContents();
    void createAndRegisterDockWidget(LmCdl::I_VcsiDockableExtensionApi& dockableApi);
    void createMenuBarButton(LmCdl::I_VcsiMenuBarExtensionApi& menuBarApi);
    void showOrHideDockWidget(bool visible);
    void synchronizeButtonToggleState(bool dialogVisible);

    void synchronousUpdate();

    QGeoCoordinate startLocation();
    QGeoCoordinate endLocation();
    void initializeTerrainProfile(const QGeoCoordinate& start, const QGeoCoordinate& end);

    std::vector<QGeoCoordinate> interpolate(QGeoCoordinate start, QGeoCoordinate end, int numSamples);
    void retrieveElevationSynchronously(QGeoCoordinate start, const std::vector<QGeoCoordinate>& samplePoints);
    void addElevationToProfile(
        const QGeoCoordinate& start, const QGeoCoordinate& location, const LmCdl::GroundElevation& elevation);

    void clearTerrainProfile();
    void enableProfileUpdates(bool enable);

private:
    LmCdl::I_GroundElevationApi& groundElevationApi_;

    QWidget* mainWidget_;
    QDockWidget* dockWidget_;

    CoordinateEntry start_;
    CoordinateEntry end_;

    QSpinBox* sampleCount_;
    QAbstractButton* synchronousUpdate_;

    QScopedPointer<QGraphicsScene> profileScene_;
    QGraphicsView* profileView_;

    QAbstractButton* openDialogButton_;

    double profileLength_;
    double maxHeight_;
    size_t asyncRequestsOutstanding_;

    std::vector<ProfileLocation> profile_;

    Renderer renderer_;
};
