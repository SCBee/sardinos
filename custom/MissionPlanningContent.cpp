#include <MissionPlanningContent.h>

#include <LmCdl/UniqueIdentifier.h>
#include <QObject>
#include <QtGlobal>
#include <memory>
#include <unordered_map>
#include <LmCdl/VcsiIdentifiedPointOfInterest.h>
#include <LmCdl/I_PointOfInterestApi.h>
#include <MissionPlanningContent.h>
#include <LmCdl/I_ContextMenu.h>
#include <LmCdl/I_ContextMenuItem.h>
#include <qobject.h>
#include <QColor>

MissionPlanningContent::MissionPlanningContent(LmCdl::I_VcsiMapExtensionApi& mapApi, LmCdl::I_PointOfInterestApi& poiApi)
    : poiApi_(poiApi)
    , contextMenuItem_(mapApi.terrainContextMenu().registerMenuItem())
{
    Initialize();
    connectToApiSignals();
}

void MissionPlanningContent::Initialize()
{
    contextMenuItem_.setBackgroundColor(*new QColor(235, 12, 12, 180));
    contextMenuItem_.setDescription("Add Mission Bound");
    contextMenuItem_.setGrouping(LmCdl::ContextMenuItemGrouping::Bottom);
    contextMenuItem_.setIcon(":/MissionPlanning/missionPlanningDinoIcon");
    contextMenuItem_.setVisible(true);
}

MissionPlanningContent::~MissionPlanningContent(){ }

void MissionPlanningContent::connectToApiSignals()
{
    connect(&poiApi_,
        SIGNAL(pointOfInterestAdded(LmCdl::VcsiPointOfInterestId, const LmCdl::VcsiPointOfInterestProperties&)), this,
        SLOT(publishAndMapPointOfInterest(LmCdl::VcsiPointOfInterestId, const LmCdl::VcsiPointOfInterestProperties&)));
}

void MissionPlanningContent::publishAndMapPointOfInterest(LmCdl::VcsiPointOfInterestId sourceId, const LmCdl::VcsiPointOfInterestProperties& pointOfInterest)
{
    auto mapIds = [this, sourceId](const LmCdl::VcsiPointOfInterestId& cloneId) {
        if (cloneId.isValid()) {
            sourceIdsToClonedIds_[sourceId] = cloneId;
        }
    };

    poiApi_.addPointOfInterest(pointOfInterest, mapIds);
}