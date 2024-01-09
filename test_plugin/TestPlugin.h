//
// Created by dev on 1/8/2024.
//

#ifndef VCSI_SARDINOS_TESTPLUGIN_H
#define VCSI_SARDINOS_TESTPLUGIN_H

#include <memory>

#include <LmCdl/I_Plugin.h>

#include <QObject>
#include <QScopedPointer>

class TestContentCreator;

namespace LmCdl {
    class I_VcsiMapExtensionApi;

    class I_PointOfInterestApi;

    class I_VcsiApplicationApi;
}

class TestPlugin : public QObject, public LmCdl::I_Plugin {
Q_OBJECT
    Q_INTERFACES(LmCdl::I_Plugin)
    Q_PLUGIN_METADATA(IID "LMCDL.Plugin.I_Plugin/1.0")

public:
    TestPlugin();

    virtual ~TestPlugin();

    QList<LmCdl::PluginRequirement> requiredApis() const override;

    LmCdl::PluginCapabilityIdentifier providedApi() const override;

    bool setRequiredApi(LmCdl::PluginCapabilityIdentifier id, QObject *api) override;

    QObject *getProvidedApi() override;

    bool isFullyInitialized() const override;

private:
    void startPluginIfInitialized();

    Q_DISABLE_COPY(TestPlugin);

    LmCdl::I_PointOfInterestApi *pointOfInterestApi_;
    LmCdl::I_VcsiApplicationApi *applicationApi_;
    std::unique_ptr<TestContentCreator> pluginContentCreator_;
};

#endif //VCSI_SARDINOS_TESTPLUGIN_H
