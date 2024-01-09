/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <QObject>
#include <QStringList>

namespace GdWild {
class I_AnimalTrackerApi : public QObject {
    Q_OBJECT
public:
    virtual ~I_AnimalTrackerApi();

    virtual QStringList animalList() const = 0;

signals:
    void animalListChanged();
};
}
