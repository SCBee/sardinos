/**
 \file
 Copyright (c) 2019 Lockheed Martin Corp. All rights reserved.
 */

#pragma once

#include <LmCdl/VcsiIdentifiedPointOfInterest.h>
#include <QObject>

namespace LmCdl {
class I_ContextMenu;
class I_ContextMenuItem;
class I_PointOfInterestApi;
class VcsiIdentifiedPointOfInterest;
}

class PointOfInterestContextMenuContent : public QObject {
    Q_OBJECT

public:
    explicit PointOfInterestContextMenuContent(LmCdl::I_PointOfInterestApi& pointOfInterestApi);
    virtual ~PointOfInterestContextMenuContent();

private slots:
    void captureClickedPointOfInterestAndSetMapMenuItemVisibilities(
        const LmCdl::VcsiIdentifiedPointOfInterest& clickedPointOfInterest);
    void captureClickedPointOfInterestAndSetVideoMenuItemVisibilities(
        const LmCdl::VcsiIdentifiedPointOfInterest& clickedPointOfInterest);
    void makePointOfInterestHostile();
    void makePointOfInterestFriendly();
    void makePointOfInterestNeutral();

private:
    void connectToApiSignals();
    void initialiseContextMenuItems();
    void initialiseChangeAffiliationContextMenuItem(
        LmCdl::I_ContextMenuItem& contextMenuItem, const QString& description, const QString& iconPath);

    QChar extractAffiliationCode(const LmCdl::VcsiIdentifiedPointOfInterest& pointOfInterest) const;
    bool isPointOfInterestAffiliationMissing(const LmCdl::VcsiIdentifiedPointOfInterest& pointOfInterest) const;
    bool isPointOfInterestHostile(const LmCdl::VcsiIdentifiedPointOfInterest& pointOfInterest) const;
    bool isPointOfInterestFriendly(const LmCdl::VcsiIdentifiedPointOfInterest& pointOfInterest) const;
    bool isPointOfInterestNotFriendlyOrHostile(const LmCdl::VcsiIdentifiedPointOfInterest& pointOfInterest) const;
    void assignAffiliationToPointOfInterest(
        const LmCdl::VcsiIdentifiedPointOfInterest& pointOfInterest, const QChar& affiliationCode);

private:
    LmCdl::I_PointOfInterestApi& pointOfInterestApi_;

    LmCdl::I_ContextMenu& contextMenu_;
    LmCdl::I_ContextMenuItem& makeHostilePointOfInterestMenuItem_;
    LmCdl::I_ContextMenuItem& makeFriendlyPointOfInterestMenuItem_;
    LmCdl::I_ContextMenuItem& makeNeutralPointOfInterestMenuItem_;

    LmCdl::I_ContextMenu& videoContextMenu_;
    LmCdl::I_ContextMenuItem& makeHostilePointOfInterestVideoMenuItem_;
    LmCdl::I_ContextMenuItem& makeFriendlyPointOfInterestVideoMenuItem_;
    LmCdl::I_ContextMenuItem& makeNeutralPointOfInterestVideoMenuItem_;

    LmCdl::VcsiIdentifiedPointOfInterest lastClickedPointOfInterest_;
};
