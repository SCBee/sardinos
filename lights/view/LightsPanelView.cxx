/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#include <LightsPanelView.h>

#include <LightsPresenter.h>
#include <LightsQmlComponentRegistration.h>
#include <LmCdl/I_QmlApi.h>
#include <LmCdl/I_VcsiVehicleExtensionApi.h>
#include <LmCdl/I_VehicleType.h>
#include <LmCdl/StanagVehicleType.h>
#include <LmCdl/TabWidgetMetadata.h>

#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickView>
#include <QWidget>

LightsPanelView::LightsPanelView(LmCdl::I_VcsiVehicleExtensionApi& vehiclePanelWidgetApi, LmCdl::I_QmlApi& qmlApi,
    LmCdl::UniqueIdentifier vehicleId, LightsPresenter& presenter)
    : viewRootObject_(nullptr)
{
    LightsQmlComponentRegistration::registerQmlComponents();

    QQuickView* view = new QQuickView(&qmlApi.qmlEngine(), nullptr);

    // Install the \a presenter into the Qt Quick view so that the QML file can
    // access it as "panelPresenter"

    // TODO: It seems to cause a crash after fixing VCSI-11487.
    // If we defer constructing the altitude and speed tape gauges to be
    // after this view then the crash does not happen.
    // view->engine()->rootContext()->setContextProperty("panelPresenter", &presenter);

    // You can display different views for different vehicle types:
    QUrl sourceFile("modules/qml/LightsExampleQuick/MainPanel.qml");
    view->setSource(sourceFile);

    // This creates a bridge between the QML and the Qt Widget systems
    QWidget* container = QWidget::createWindowContainer(view);
    container->setMinimumHeight(300);

    // Inject the widget onto the main Vehicle Panel tabs
    const LmCdl::TabWidgetMetadata metadata(tr("Lights"));
    vehiclePanelWidgetApi.addAvPanelTab(vehicleId, metadata, container);

    // Remember the view's root object since we'll need to delete it later
    viewRootObject_ = view->rootObject();
}

LightsPanelView::~LightsPanelView()
{
    if (viewRootObject_ != nullptr)
        delete viewRootObject_;
}
