/**
\file
Copyright (c) 2022 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <QDockWidget>

namespace LmCdl {
class I_VcsiApplicationApi;
}

class AudioExampleDockWidget : public QDockWidget {
    Q_OBJECT

public:
    explicit AudioExampleDockWidget(LmCdl::I_VcsiApplicationApi& applicationApi);
    virtual ~AudioExampleDockWidget();
};
