/**
\file
Copyright (c) 2019 Lockheed Martin Corp. All rights reserved.
*/

#include <LightsQmlComponentRegistration.h>

#include <I_SingleLightModel.h>
#include <QtQml>

namespace {
static const char* const LIBRARY_IDENTIFIER = "LmCdl.LightsApi";
static const int LIBRARY_MAJOR_VERSION_NUMBER = 1;
static const int LIBRARY_MINOR_VERSION_NUMBER = 0;
static const QString REASON = QString("Not creatable");

#define REGISTER_COMPONENT(T)                                                                                          \
    qmlRegisterUncreatableType<T>(                                                                                     \
        LIBRARY_IDENTIFIER, LIBRARY_MAJOR_VERSION_NUMBER, LIBRARY_MINOR_VERSION_NUMBER, #T, REASON);
}

namespace LightsQmlComponentRegistration {

void registerQmlComponents() { REGISTER_COMPONENT(I_SingleLightModel) }
}
