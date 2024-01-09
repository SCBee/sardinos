/**
\file
Copyright (c) 2020 Lockheed Martin Corp.  All rights reserved.
*/

#pragma once

#include <QObject>
class StandardPathsExampleContent;
class QAbstractButton;
class QDir;
class QDockWidget;
class QGridLayout;
class QLabel;
class QTextEdit;
class QWidget;
namespace LmCdl {
class I_VcsiApplicationApi;
class I_VcsiStandardPaths;
}

class StandardPathsExampleView : public QObject {
    Q_OBJECT
public:
    explicit StandardPathsExampleView(LmCdl::I_VcsiApplicationApi& applicationApi,
        LmCdl::I_VcsiStandardPaths& standardPaths, StandardPathsExampleContent& content);
    virtual ~StandardPathsExampleView();

private slots:
    void showWarnings();
    void diffCurrentLog();
    void syncDockWidgetVisibility(bool visible);

private:
    void generateStandardPaths();
    void formatStandardPathLine(QDir directory, QString description);

private:
    Q_DISABLE_COPY(StandardPathsExampleView);

private:
    LmCdl::I_VcsiStandardPaths& standardPaths_;
    StandardPathsExampleContent& content_;

    QWidget* mainWidget_;
    QDockWidget* dockWidget_;
    QAbstractButton* showDockWidgetButton_;
    QLabel* warningCount_;
    QLabel* warningUpdate_;
    QAbstractButton* injectWarning_;
    QTextEdit* status_;
    QAbstractButton* showWarnings_;
    QAbstractButton* diffFile_;
    QTextEdit* paths_;
};
