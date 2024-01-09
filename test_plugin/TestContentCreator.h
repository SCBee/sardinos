//
// Created by dev on 1/9/2024.
//

#ifndef VCSI_SARDINOS_TESTCONTENTCREATOR_H
#define VCSI_SARDINOS_TESTCONTENTCREATOR_H

#include <memory>
#include <unordered_map>

#include <LmCdl/UniqueIdentifier.h>
#include <LmCdl/VcsiIdentifiedPointOfInterest.h>
#include <LmCdl/VcsiPointOfInterestProperties.h>
#include <LmCdl/I_ContextMenu.h>
#include <LmCdl/VcsiMilStdCode.h>
#include <LmCdl/I_ContextMenuItem.h>
#include <LmCdl/I_VcsiMapExtensionApi.h>
#include <LmCdl/I_PointOfInterestApi.h>
#include <LmCdl/I_VcsiUserNotificationApi.h>
#include <LmCdl/I_UserNotification.h>

#include <QObject>
#include <QtGlobal>
#include <qgeocoordinate.h>

namespace LmCdl {
    class I_VcsiMapExtensionApi;

    class I_PointOfInterestApi;

    class I_VcsiApplicationApi;
}

class TestContentCreator : public QObject {
Q_OBJECT
public:
    TestContentCreator(LmCdl::I_VcsiMapExtensionApi &mapApi, LmCdl::I_PointOfInterestApi &poiApi,
                       LmCdl::I_VcsiUserNotificationApi &notApi);

    virtual ~TestContentCreator();

    void publishAndMapPointOfInterest(const LmCdl::VcsiPointOfInterestId &sourceId,
                                      const LmCdl::VcsiPointOfInterestProperties &pointOfInterest);

    void getPoiProperties(const LmCdl::ContextMenuEvent &event);

    void connectToApiSignals();

    void removeNotification();

private:
    Q_DISABLE_COPY(TestContentCreator);

    LmCdl::I_ContextMenuItem &contextMenuItem_;
    QHash<LmCdl::VcsiPointOfInterestId, LmCdl::VcsiPointOfInterestId> sourceIdsToClonedIds_;
    LmCdl::I_PointOfInterestApi &poiApi_;
    LmCdl::I_VcsiUserNotificationApi &notApi_;
    LmCdl::I_UserNotification *notification_;
};

#endif //VCSI_SARDINOS_TESTCONTENTCREATOR_H
