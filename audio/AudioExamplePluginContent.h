/**
\file
Copyright (c) 2022 Lockheed Martin Corp.  All rights reserved.
*/

#pragma once

#include <QObject>
#include <QPointer>
#include <memory>

namespace LmCdl {
class Duration;
class I_AudioApi;
class I_VcsiApplicationApi;
}

namespace Ui {
class AudioExampleDockForm;
}

class AudioExampleDockWidget;
class QBoxLayout;
class QLayout;

class AudioExamplePluginContent : public QObject {
    Q_OBJECT

public:
    explicit AudioExamplePluginContent(LmCdl::I_AudioApi& audioApi, LmCdl::I_VcsiApplicationApi& applicationApi);
    virtual ~AudioExamplePluginContent();

private:
    Q_DISABLE_COPY(AudioExamplePluginContent);
    void registerSound(const QString& name, const QString& wavFileName, const LmCdl::Duration& interval);

    LmCdl::I_AudioApi& audioApi_;
    LmCdl::I_VcsiApplicationApi& applicationApi_;
    std::vector<std::unique_ptr<Ui::AudioExampleDockForm>> uis_;
    QPointer<AudioExampleDockWidget> dockWidget_;
    QPointer<QWidget> widget_;
    QPointer<QBoxLayout> layout_;
};
