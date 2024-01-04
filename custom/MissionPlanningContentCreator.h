#pragma once

#include <LmCdl/UniqueIdentifier.h>
#include <QObject>
#include <QtGlobal>
#include <memory>
#include <unordered_map>
#include <LmCdl/VcsiIdentifiedPointOfInterest.h>
#include <LmCdl/VcsiPointOfInterestProperties.h>
#include <LmCdl/I_ContextMenu.h>
#include <qgeocoordinate.h>
#include <LmCdl/VcsiMilStdCode.h>
#include <MissionPlanningContent.h>

class MissionPlanningContent;

namespace LmCdl {
class I_VcsiMapExtensionApi;
class I_PointOfInterestApi;
class I_VcsiApplicationApi;
}

class MissionPlanningContentCreator : public QObject {
    Q_OBJECT
public:
    MissionPlanningContentCreator(LmCdl::I_VcsiMapExtensionApi& mapApi, LmCdl::I_PointOfInterestApi& poiApi);
    virtual ~MissionPlanningContentCreator();

private:
    Q_DISABLE_COPY(MissionPlanningContentCreator);

    QScopedPointer<MissionPlanningContent> content_;
};
