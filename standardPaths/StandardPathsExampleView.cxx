/**
\file
Copyright (c) 2020 Lockheed Martin Corp.  All rights reserved.
*/

#include <StandardPathsExampleView.h>

#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/I_VcsiDockableExtensionApi.h>
#include <LmCdl/I_VcsiMenuBarExtensionApi.h>
#include <LmCdl/I_VcsiStandardPaths.h>
#include <LmCdl/I_VcsiWidgetExtensionApi.h>
#include <QAction>
#include <QDockWidget>
#include <QFileDialog>
#include <QFileInfo>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QStringList>
#include <QTextEdit>
#include <QToolButton>
#include <QWidget>
#include <StandardPathsExampleContent.h>
#include <algorithm>
#include <vector>

namespace {
QString toString(LmCdl::I_VcsiStandardPaths::Location location)
{
    switch (location) {
    case LmCdl::I_VcsiStandardPaths::Location::LmCdlSharedDataRoot:
        return QString::fromLatin1("LmCdlSharedDataRoot");
    case LmCdl::I_VcsiStandardPaths::Location::VcsiApplicationDataRoot:
        return QString::fromLatin1("VcsiApplicationDataRoot");
    case LmCdl::I_VcsiStandardPaths::Location::ApplicationSettings:
        return QString::fromLatin1("ApplicationSettings");
    case LmCdl::I_VcsiStandardPaths::Location::Logs:
        return QString::fromLatin1("Logs");
    case LmCdl::I_VcsiStandardPaths::Location::Translations:
        return QString::fromLatin1("Translations");
    case LmCdl::I_VcsiStandardPaths::Location::Overlays:
        return QString::fromLatin1("Overlays");
    case LmCdl::I_VcsiStandardPaths::Location::Recordings:
        return QString::fromLatin1("Recordings");
    case LmCdl::I_VcsiStandardPaths::Location::Audio:
        return QString::fromLatin1("Audio");
    case LmCdl::I_VcsiStandardPaths::Location::GuiStyles:
        return QString::fromLatin1("GuiStyles");
    case LmCdl::I_VcsiStandardPaths::Location::GdalData:
        return QString::fromLatin1("GdalData");
    case LmCdl::I_VcsiStandardPaths::Location::Sessions:
        return QString::fromLatin1("Sessions");
    }

    return QString::fromLatin1("Unknown");
}

std::vector<LmCdl::I_VcsiStandardPaths::Location> allLocations()
{
    return std::vector<LmCdl::I_VcsiStandardPaths::Location> {
        LmCdl::I_VcsiStandardPaths::Location::LmCdlSharedDataRoot,
        LmCdl::I_VcsiStandardPaths::Location::VcsiApplicationDataRoot,
        LmCdl::I_VcsiStandardPaths::Location::ApplicationSettings,
        LmCdl::I_VcsiStandardPaths::Location::Logs,
        LmCdl::I_VcsiStandardPaths::Location::Translations,
        LmCdl::I_VcsiStandardPaths::Location::Overlays,
        LmCdl::I_VcsiStandardPaths::Location::Recordings,
        LmCdl::I_VcsiStandardPaths::Location::Audio,
        LmCdl::I_VcsiStandardPaths::Location::GuiStyles,
        LmCdl::I_VcsiStandardPaths::Location::GdalData,
        LmCdl::I_VcsiStandardPaths::Location::Sessions,
    };
}
}

StandardPathsExampleView::StandardPathsExampleView(LmCdl::I_VcsiApplicationApi& applicationApi,
    LmCdl::I_VcsiStandardPaths& standardPaths, StandardPathsExampleContent& content)
    : standardPaths_(standardPaths)
    , content_(content)

    , mainWidget_(new QWidget())
    , dockWidget_(new QDockWidget("Standard Paths"))
    , showDockWidgetButton_(new QToolButton())
    , warningCount_(new QLabel("0"))
    , warningUpdate_(new QLabel("no update available"))
    , injectWarning_(new QPushButton("Inject Warning"))
    , status_(new QTextEdit())
    , showWarnings_(new QPushButton("Show Warnings"))
    , diffFile_(new QPushButton("Diff Log"))
    , paths_(new QTextEdit())
{
    status_->setReadOnly(true);
    paths_->setReadOnly(true);

    QLabel* warningsLabel = new QLabel("Warnings:");

    QGridLayout* mainLayout = new QGridLayout(mainWidget_);
    mainLayout->addWidget(warningsLabel, 0, 0);
    mainLayout->addWidget(warningCount_, 0, 1);
    mainLayout->addWidget(injectWarning_, 0, 2, 1, 3);
    mainLayout->addWidget(warningUpdate_, 1, 0, 1, 4);
    mainLayout->addWidget(status_, 2, 0, 1, -1);
    mainLayout->addWidget(showWarnings_, 3, 0, 1, 3);
    mainLayout->addWidget(diffFile_, 3, 3, 1, 2);
    generateStandardPaths();
    mainLayout->addWidget(paths_, 4, 0, -1, -1);

    showDockWidgetButton_->setCheckable(true);
    showDockWidgetButton_->setText(dockWidget_->windowTitle());
    showDockWidgetButton_->setToolTip(dockWidget_->windowTitle());
    applicationApi.widgetExtensionApi().menuBarApi().addItemToRightToolBar(showDockWidgetButton_);

    dockWidget_->setObjectName(dockWidget_->windowTitle());
    dockWidget_->setFeatures(QDockWidget::DockWidgetFeature::DockWidgetMovable
        | QDockWidget::DockWidgetFeature::DockWidgetFloatable | QDockWidget::DockWidgetFeature::DockWidgetClosable);
    dockWidget_->setAllowedAreas(Qt::DockWidgetArea::TopDockWidgetArea | Qt::DockWidgetArea::BottomDockWidgetArea
        | Qt::DockWidgetArea::LeftDockWidgetArea);
    dockWidget_->setWidget(mainWidget_);
    applicationApi.widgetExtensionApi().dockableApi().addDockWidget(
        Qt::DockWidgetArea::LeftDockWidgetArea, dockWidget_);

    connect(
        showDockWidgetButton_, &QAbstractButton::clicked, this, &StandardPathsExampleView::syncDockWidgetVisibility);
    connect(dockWidget_->toggleViewAction(), &QAction::toggled, showDockWidgetButton_, &QAbstractButton::setChecked);

    connect(
        injectWarning_, &QAbstractButton::clicked, this, [this]() { qWarning("Log File Utils: warning injected"); });

    connect(showWarnings_, &QAbstractButton::clicked, this, &StandardPathsExampleView::showWarnings);
    connect(diffFile_, &QAbstractButton::clicked, this, &StandardPathsExampleView::diffCurrentLog);

    connect(
        &content_, &StandardPathsExampleContent::warningUpdated, this, [this](WarningInfo total, WarningInfo update) {
            warningCount_->setText(QString::number(total.count));
            warningUpdate_->setText(QString("Found %1 new errors in %2 lines").arg(update.count).arg(update.lines));
        });
}

StandardPathsExampleView::~StandardPathsExampleView() { }

void StandardPathsExampleView::generateStandardPaths()
{
    auto generateStandardPath = [this](LmCdl::I_VcsiStandardPaths::Location location) {
        QDir directory = standardPaths_.locateDirectory(location);
        QString description = toString(location);
        formatStandardPathLine(directory, description);
    };

    auto locations = allLocations();
    std::for_each(locations.begin(), locations.end(), generateStandardPath);
}

void StandardPathsExampleView::formatStandardPathLine(QDir directory, QString description)
{
    paths_->append(description);
    paths_->append(" ");
    paths_->append(directory.absolutePath());
    paths_->append("\n");
}

void StandardPathsExampleView::showWarnings()
{
    QStringList warnings = content_.warnings();
    status_->clear();
    status_->append(warnings.join("\n"));
}

void StandardPathsExampleView::diffCurrentLog()
{
    QFileDialog dialog;
    dialog.setDirectory(standardPaths_.locateDirectory(LmCdl::I_VcsiStandardPaths::Location::Logs));
    QFileInfo file(dialog.getOpenFileName(nullptr, tr("Diff Log Files"), "", tr("Log Files (*.log*)")));
    if (file.exists()) {
        status_->clear();

        DiffInfo diff = content_.diff(file.filePath());
        QString warnings
            = diff.diff.empty() ? QString("Current log file contains no new errors") : diff.diff.values().join("\n");
        status_->append(warnings);

        status_->append("\n\n");
        QString errorCountString = QString("(current errors) %1 vs %2 (%3)")
                                       .arg(diff.currentWarningCount)
                                       .arg(diff.otherWarningCount)
                                       .arg(file.filePath());
        status_->append(errorCountString);
    }
}

void StandardPathsExampleView::syncDockWidgetVisibility(bool visible)
{
    if (visible) {
        dockWidget_->show();
        dockWidget_->raise();
    } else {
        dockWidget_->hide();
    }
}
