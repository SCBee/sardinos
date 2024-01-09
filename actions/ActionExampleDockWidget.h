/**
\file
Copyright (c) 2022 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <QDockWidget>

namespace LmCdl {
class I_VcsiApplicationApi;
}

class ActionExampleDockWidget : public QDockWidget {
    Q_OBJECT

public:
    explicit ActionExampleDockWidget(LmCdl::I_VcsiApplicationApi& applicationApi);
    virtual ~ActionExampleDockWidget();
};
