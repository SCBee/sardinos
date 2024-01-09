/**
\file
Copyright (c) 2022 Lockheed Martin Corp.  All rights reserved.
*/

#pragma once

#include <ActionExampleDockWidget.h>
#include <LmCdl/ActionResult.h>
#include <QFuture>
#include <QMap>
#include <QObject>
#include <QQuickWidget>
#include <functional>

class ActionExampleDockWidget;
namespace LmCdl {
class Action;
class I_ActionApi;
class I_ActionCollection;
class I_QmlApi;
class I_VcsiApplicationApi;
}

class ActionExamplePluginContent : public QObject {
    Q_OBJECT

public:
    explicit ActionExamplePluginContent(
        LmCdl::I_ActionApi& actionApi, LmCdl::I_VcsiApplicationApi& applicationApi, LmCdl::I_QmlApi& qmlApi);
    virtual ~ActionExamplePluginContent();

private:
    QQuickWidget* createQQuickWidget(const QUrl& url);
    void initializeExampleActions();

private:
    LmCdl::I_ActionApi& actionApi_;
    LmCdl::I_ActionCollection& actionCollection_;
    LmCdl::I_VcsiApplicationApi& applicationApi_;
    LmCdl::I_QmlApi& qmlApi_;
    ActionExampleDockWidget* dockWidget_;
    QUrl resourceQmlUrl_;
    QQuickWidget* exampleContent_;

    QList<LmCdl::Action*> exampleActions_;
};
