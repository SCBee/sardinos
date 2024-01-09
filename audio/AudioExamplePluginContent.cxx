/**
\file
Copyright (c) 2022 Lockheed Martin Corp.  All rights reserved.
*/

#include <AudioExamplePluginContent.h>

#include <AudioExampleDockWidget.h>
#include <LmCdl/I_AudioApi.h>
#include <LmCdl/I_VcsiApplicationApi.h>
#include <QFile>
#include <ui_audioExampleForm.h>

AudioExamplePluginContent::AudioExamplePluginContent(
    LmCdl::I_AudioApi& audioApi, LmCdl::I_VcsiApplicationApi& applicationApi)
    : audioApi_(audioApi)
    , applicationApi_(applicationApi)
    , dockWidget_(new AudioExampleDockWidget(applicationApi_))
    , widget_(new QWidget)
    , layout_(new QVBoxLayout(widget_))
{
    dockWidget_->setWidget(widget_);
    layout_->addStretch();
    registerSound("Dot", ":/audio/dot.wav", LmCdl::Duration(1.0, LmCdl::DurationUnit::Seconds));
    registerSound("Dash", ":/audio/dash.wav", LmCdl::Duration(1.0, LmCdl::DurationUnit::Seconds));
    registerSound("Gong", ":/audio/gong.wav", LmCdl::Duration(1.0, LmCdl::DurationUnit::Seconds));
    registerSound("Long", ":/audio/long-wav.wav", LmCdl::Duration(9.0, LmCdl::DurationUnit::Seconds));
    registerSound("Sound Effects", ":/audio/SoundEffects.wav", LmCdl::Duration(5.0, LmCdl::DurationUnit::Seconds));
    layout_->addStretch();
}

AudioExamplePluginContent::~AudioExamplePluginContent()
{
    delete layout_;
    delete widget_;
    delete dockWidget_;
}

void AudioExamplePluginContent::registerSound(
    const QString& name, const QString& wavFileName, const LmCdl::Duration& interval)
{
    QWidget* widget = new QWidget;
    Ui::AudioExampleDockForm* ui = new Ui::AudioExampleDockForm;
    uis_.emplace_back(ui);
    ui->setupUi(widget);
    ui->soundGroupBox->setTitle(name);
    layout_->addWidget(widget);
    const LmCdl::AudioSourceId id = audioApi_.registerAudioSource(LmCdl::AudioSource(QFile(wavFileName).fileName()));
    connect(ui->playButton, &QAbstractButton::clicked, this, [this, id]() { this->audioApi_.playOnce(id); });
    connect(ui->playImmediatelyButton, &QAbstractButton::clicked, this,
        [this, id]() { this->audioApi_.playOnceImmediately(id); });
    connect(ui->startRepeatButton, &QAbstractButton::clicked, this,
        [this, id, interval]() { this->audioApi_.startRepeatedlyPlaying(id, LmCdl::RepeatedAudioPlayback(interval)); });
    connect(ui->stopRepeatButton, &QAbstractButton::clicked, this,
        [this, id]() { this->audioApi_.stopRepeatedlyPlaying(id); });
}
