/**
\file
Copyright (c) 2022 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <QObject>
#include <QQuickItem>
#include <QUrl>
#include <QtGlobal>
#include <memory>

namespace LmCdl {
class I_GdtApi;
class I_QmlApi;
class I_GraphicsWidget;
class I_VcsiMapExtensionApi;
}

class QmlGdtView : public QObject {
    Q_OBJECT
public:
    explicit QmlGdtView(LmCdl::I_QmlApi& qmlApi, LmCdl::I_GdtApi& gdtApi, LmCdl::I_VcsiMapExtensionApi& mapApi);
    virtual ~QmlGdtView();

private:
    QQuickItem* createQQuickItem(const QUrl& url);

private slots:
    void updateGdtOverlayLocation();

private:
    LmCdl::I_QmlApi& qmlApi_;
    LmCdl::I_GdtApi& gdtApi_;
    LmCdl::I_VcsiMapExtensionApi& mapApi_;
    QUrl gdtResourceQmlUrl_;
    QQuickItem* gdtOverlayItem_;
    LmCdl::I_GraphicsWidget& gdtOverlayOnMap_;
};