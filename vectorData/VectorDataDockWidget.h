/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <QDockWidget>

namespace LmCdl {
class I_VcsiApplicationApi;
}

class VectorDataDockWidget : public QDockWidget {
    Q_OBJECT

public:
    explicit VectorDataDockWidget(LmCdl::I_VcsiApplicationApi& applicationApi);
    virtual ~VectorDataDockWidget();

private slots:
    void showOrHideDock(bool visible);
};
