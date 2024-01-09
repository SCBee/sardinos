/**
\file
Copyright (c) 2022 Lockheed Martin Corp.  All rights reserved.
*/

#include <ActionExamplePluginContent.h>

#include <DelayedActionPromiseFulfillment.h>
#include <LmCdl/Action.h>
#include <LmCdl/ActionIdentifier.h>
#include <LmCdl/I_ActionApi.h>
#include <LmCdl/I_ActionCollection.h>
#include <LmCdl/I_ActionRegistrationApi.h>
#include <LmCdl/I_QmlApi.h>
#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/Qt5QPromiseAdapter.h>
#include <QColor>
#include <QQmlComponent>
#include <QThreadPool>
#include <QWidget>

namespace {
const QColor MATERIAL_DARK_BACKGROUND_COLOR = QColor("#303030");

std::function<QFuture<LmCdl::ActionResult>()> createActionFunctions(LmCdl::ActionResult result)
{
    std::function<QFuture<LmCdl::ActionResult>()> exampleAction([=]() {
        LmCdl::Qt5QPromiseAdapter<LmCdl::ActionResult> promise;
        QFuture<LmCdl::ActionResult> future = promise.future();
        DelayedActionPromiseFulfillment* delayedFulfillment
            = new DelayedActionPromiseFulfillment(std::move(promise), 2000, result);
        QThreadPool::globalInstance()->start(delayedFulfillment);
        return future;
    });
    return exampleAction;
}
}

ActionExamplePluginContent::ActionExamplePluginContent(
    LmCdl::I_ActionApi& actionApi, LmCdl::I_VcsiApplicationApi& applicationApi, LmCdl::I_QmlApi& qmlApi)
    : actionApi_(actionApi)
    , actionCollection_(actionApi_.actionCollection())
    , applicationApi_(applicationApi)
    , qmlApi_(qmlApi)
    , dockWidget_(new ActionExampleDockWidget(applicationApi_))
    , resourceQmlUrl_("modules/qml/ActionExampleQuick/ActionExample.qml")
    , exampleContent_(createQQuickWidget(resourceQmlUrl_))
    , exampleActions_()
{
    exampleContent_->setResizeMode(QQuickWidget::SizeViewToRootObject);
    exampleContent_->setMinimumWidth(180);
    exampleContent_->setMinimumHeight(230);
    exampleContent_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    exampleContent_->setAttribute(Qt::WA_AcceptTouchEvents, true);
    exampleContent_->setClearColor(MATERIAL_DARK_BACKGROUND_COLOR);
    dockWidget_->setWidget(exampleContent_);

    initializeExampleActions();
}

ActionExamplePluginContent::~ActionExamplePluginContent() { qDeleteAll(exampleActions_); }

QQuickWidget* ActionExamplePluginContent::createQQuickWidget(const QUrl& url)
{
    QQuickWidget* widget(new QQuickWidget(&qmlApi_.qmlEngine(), dockWidget_));
    if (widget) {
        widget->setSource(url);
    }

    return widget;
}

void ActionExamplePluginContent::initializeExampleActions()
{
    QString source("ActionExamplePlugin");
    QString name("exampleAction%1");
    QString desc("I am the %1 example action, I always %2!.");

    exampleActions_.append(actionApi_.actionRegistrationApi()
                               .addAction(source, name.arg(1), desc.arg("first").arg("succeed"),
                                   createActionFunctions(LmCdl::ActionResult::Success))
                               .release());
    exampleActions_.append(actionApi_.actionRegistrationApi()
                               .addAction(source, name.arg(2), desc.arg("second").arg("fail"),
                                   createActionFunctions(LmCdl::ActionResult::Fail))
                               .release());

    qDebug("Example list size: %d", exampleActions_.size());
    qDebug("Registered actions size: %d", actionCollection_.allActions().size());
}