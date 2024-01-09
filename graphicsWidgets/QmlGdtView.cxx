/**
\file
Copyright (c) 2022 Lockheed Martin Corp. All rights reserved.
*/

#include <QmlGdtView.h>

#include <LmCdl/I_GdtApi.h>
#include <LmCdl/I_GraphicsWidget.h>
#include <LmCdl/I_QmlApi.h>
#include <LmCdl/I_VcsiMapExtensionApi.h>
#include <QGeoCoordinate>
#include <QQmlComponent>

QmlGdtView::QmlGdtView(LmCdl::I_QmlApi& qmlApi, LmCdl::I_GdtApi& gdtApi, LmCdl::I_VcsiMapExtensionApi& mapApi)
    : qmlApi_(qmlApi)
    , gdtApi_(gdtApi)
    , mapApi_(mapApi)
    , gdtResourceQmlUrl_("qrc:///gdtoverlay/GdtOverlayQml")
    , gdtOverlayItem_(createQQuickItem(gdtResourceQmlUrl_))
    , gdtOverlayOnMap_(mapApi_.addGraphicsWidget(gdtOverlayItem_))
{
    connect(&gdtApi_, &LmCdl::I_GdtApi::gdtLocationChanged, this, &QmlGdtView::updateGdtOverlayLocation);

    updateGdtOverlayLocation();
    gdtOverlayOnMap_.setOffsetInPixels(QPoint(-27, -27));
    gdtOverlayOnMap_.setVisible(true);
}
QmlGdtView::~QmlGdtView() { }

QQuickItem* QmlGdtView::createQQuickItem(const QUrl& url)
{
    QQmlComponent component(&qmlApi_.qmlEngine(), url);
    if (!component.isReady()) {
        return nullptr;
    }

    QQuickItem* item = dynamic_cast<QQuickItem*>(component.create());

    return item;
}

void QmlGdtView::updateGdtOverlayLocation() { gdtOverlayOnMap_.setLocation(gdtApi_.gdtLocation()); }