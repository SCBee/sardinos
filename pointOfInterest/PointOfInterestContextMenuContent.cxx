/**
 \file
 Copyright (c) 2019 Lockheed Martin Corp. All rights reserved.
 */

#include <PointOfInterestContextMenuContent.h>

#include <LmCdl/I_ContextMenu.h>
#include <LmCdl/I_ContextMenuItem.h>
#include <LmCdl/I_PointOfInterestApi.h>
#include <LmCdl/VcsiIdentifiedPointOfInterest.h>
#include <LmCdl/VcsiMilStdCode.h>
#include <LmCdl/VcsiPointOfInterestProperties.h>

namespace {
const int MIL_STD_AFFILIATION_POSITION = 1;
const QString HOSTILE_CATEGORY_MIL_STD_CODES = "SHJK";
const QString FRIENDLY_CATEGORY_MIL_STD_CODES = "AFDM";

const QString BASIC_MIL_STD_CODE = "S-GPU----------";
const QChar HOSTILE_MIL_STD_CODE = 'H';
const QChar FRIENDLY_MIL_STD_CODE = 'F';
const QChar NEUTRAL_MIL_STD_CODE = 'N';
const QChar MISSING_AFFILIATION_MIL_STD_CODE = '-';
}

PointOfInterestContextMenuContent::PointOfInterestContextMenuContent(LmCdl::I_PointOfInterestApi& pointOfInterestApi)
    : pointOfInterestApi_(pointOfInterestApi)
    , contextMenu_(pointOfInterestApi.mapContextMenu())
    , makeHostilePointOfInterestMenuItem_(contextMenu_.registerMenuItem())
    , makeFriendlyPointOfInterestMenuItem_(contextMenu_.registerMenuItem())
    , makeNeutralPointOfInterestMenuItem_(contextMenu_.registerMenuItem())
    , videoContextMenu_(pointOfInterestApi.videoContextMenu())
    , makeHostilePointOfInterestVideoMenuItem_(videoContextMenu_.registerMenuItem())
    , makeFriendlyPointOfInterestVideoMenuItem_(videoContextMenu_.registerMenuItem())
    , makeNeutralPointOfInterestVideoMenuItem_(videoContextMenu_.registerMenuItem())
    , lastClickedPointOfInterest_()
{
    connectToApiSignals();
    initialiseContextMenuItems();
}

PointOfInterestContextMenuContent::~PointOfInterestContextMenuContent()
{
    contextMenu_.unregisterMenuItem(makeHostilePointOfInterestMenuItem_);
    contextMenu_.unregisterMenuItem(makeFriendlyPointOfInterestMenuItem_);
    contextMenu_.unregisterMenuItem(makeNeutralPointOfInterestMenuItem_);

    videoContextMenu_.unregisterMenuItem(makeHostilePointOfInterestVideoMenuItem_);
    videoContextMenu_.unregisterMenuItem(makeFriendlyPointOfInterestVideoMenuItem_);
    videoContextMenu_.unregisterMenuItem(makeNeutralPointOfInterestVideoMenuItem_);
}

void PointOfInterestContextMenuContent::connectToApiSignals()
{
    connect(&pointOfInterestApi_,
        SIGNAL(aboutToShowMapContextMenu(const LmCdl::VcsiIdentifiedPointOfInterest&, const LmCdl::ContextMenuEvent&)),
        this,
        SLOT(captureClickedPointOfInterestAndSetMapMenuItemVisibilities(const LmCdl::VcsiIdentifiedPointOfInterest&)));

    connect(&pointOfInterestApi_,
        SIGNAL(
            aboutToShowVideoContextMenu(const LmCdl::VcsiIdentifiedPointOfInterest&, const LmCdl::ContextMenuEvent&)),
        this,
        SLOT(
            captureClickedPointOfInterestAndSetVideoMenuItemVisibilities(const LmCdl::VcsiIdentifiedPointOfInterest&)));
}

void PointOfInterestContextMenuContent::initialiseChangeAffiliationContextMenuItem(
    LmCdl::I_ContextMenuItem& contextMenuItem, const QString& description, const QString& iconPath)
{
    contextMenuItem.setDescription(description);
    contextMenuItem.setVisible(false);
    contextMenuItem.setGrouping(LmCdl::ContextMenuItemGrouping::Top);
    contextMenuItem.setIcon(iconPath);
}

void PointOfInterestContextMenuContent::initialiseContextMenuItems()
{
    initialiseChangeAffiliationContextMenuItem(
        makeHostilePointOfInterestMenuItem_, "Make Hostile", ":/PointOfInterestPlugin/makeHostileMenuItemIcon.svg");
    initialiseChangeAffiliationContextMenuItem(
        makeFriendlyPointOfInterestMenuItem_, "Make Friendly", ":/PointOfInterestPlugin/makeFriendlyMenuItemIcon.svg");
    initialiseChangeAffiliationContextMenuItem(
        makeNeutralPointOfInterestMenuItem_, "Make Neutral", ":/PointOfInterestPlugin/makeNeutralMenuItemIcon.svg");

    initialiseChangeAffiliationContextMenuItem(makeHostilePointOfInterestVideoMenuItem_, "Make Hostile",
        ":/PointOfInterestPlugin/makeHostileMenuItemIcon.svg");
    initialiseChangeAffiliationContextMenuItem(makeFriendlyPointOfInterestVideoMenuItem_, "Make Friendly",
        ":/PointOfInterestPlugin/makeFriendlyMenuItemIcon.svg");
    initialiseChangeAffiliationContextMenuItem(makeNeutralPointOfInterestVideoMenuItem_, "Make Neutral",
        ":/PointOfInterestPlugin/makeNeutralMenuItemIcon.svg");

    connect(&makeHostilePointOfInterestMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this,
        &PointOfInterestContextMenuContent::makePointOfInterestHostile);
    connect(&makeFriendlyPointOfInterestMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this,
        &PointOfInterestContextMenuContent::makePointOfInterestFriendly);
    connect(&makeNeutralPointOfInterestMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this,
        &PointOfInterestContextMenuContent::makePointOfInterestNeutral);
    connect(&makeHostilePointOfInterestVideoMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this,
        &PointOfInterestContextMenuContent::makePointOfInterestHostile);
    connect(&makeFriendlyPointOfInterestVideoMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this,
        &PointOfInterestContextMenuContent::makePointOfInterestFriendly);
    connect(&makeNeutralPointOfInterestVideoMenuItem_, &LmCdl::I_ContextMenuItem::clicked, this,
        &PointOfInterestContextMenuContent::makePointOfInterestNeutral);
}

void PointOfInterestContextMenuContent::captureClickedPointOfInterestAndSetMapMenuItemVisibilities(
    const LmCdl::VcsiIdentifiedPointOfInterest& clickedPointOfInterest)
{
    lastClickedPointOfInterest_ = clickedPointOfInterest;

    makeHostilePointOfInterestMenuItem_.setVisible(!isPointOfInterestHostile(lastClickedPointOfInterest_));
    makeFriendlyPointOfInterestMenuItem_.setVisible(!isPointOfInterestFriendly(lastClickedPointOfInterest_));
    makeNeutralPointOfInterestMenuItem_.setVisible(!isPointOfInterestNotFriendlyOrHostile(lastClickedPointOfInterest_));
}

void PointOfInterestContextMenuContent::captureClickedPointOfInterestAndSetVideoMenuItemVisibilities(
    const LmCdl::VcsiIdentifiedPointOfInterest& clickedPointOfInterest)
{
    lastClickedPointOfInterest_ = clickedPointOfInterest;

    makeHostilePointOfInterestVideoMenuItem_.setVisible(!isPointOfInterestHostile(lastClickedPointOfInterest_));
    makeFriendlyPointOfInterestVideoMenuItem_.setVisible(!isPointOfInterestFriendly(lastClickedPointOfInterest_));
    makeNeutralPointOfInterestVideoMenuItem_.setVisible(
        !isPointOfInterestNotFriendlyOrHostile(lastClickedPointOfInterest_));
}

QChar PointOfInterestContextMenuContent::extractAffiliationCode(
    const LmCdl::VcsiIdentifiedPointOfInterest& pointOfInterest) const
{
    LmCdl::VcsiMilStdCode milStdCode = pointOfInterest.pointOfInterest().milStdCode();
    if (milStdCode.isNull()) {
        return MISSING_AFFILIATION_MIL_STD_CODE;
    }

    if (milStdCode.toString().length() <= MIL_STD_AFFILIATION_POSITION) {
        return MISSING_AFFILIATION_MIL_STD_CODE;
    }

    return milStdCode.toString().at(MIL_STD_AFFILIATION_POSITION);
}

bool PointOfInterestContextMenuContent::isPointOfInterestAffiliationMissing(
    const LmCdl::VcsiIdentifiedPointOfInterest& pointOfInterest) const
{
    QChar affiliation = extractAffiliationCode(pointOfInterest);
    return (QString::compare(affiliation, MISSING_AFFILIATION_MIL_STD_CODE, Qt::CaseInsensitive) == 0);
}

bool PointOfInterestContextMenuContent::isPointOfInterestHostile(
    const LmCdl::VcsiIdentifiedPointOfInterest& pointOfInterest) const
{
    QChar affiliation = extractAffiliationCode(pointOfInterest);
    return HOSTILE_CATEGORY_MIL_STD_CODES.contains(affiliation, Qt::CaseInsensitive);
}

bool PointOfInterestContextMenuContent::isPointOfInterestFriendly(
    const LmCdl::VcsiIdentifiedPointOfInterest& pointOfInterest) const
{
    QChar affiliation = extractAffiliationCode(pointOfInterest);
    return FRIENDLY_CATEGORY_MIL_STD_CODES.contains(affiliation, Qt::CaseInsensitive);
}

bool PointOfInterestContextMenuContent::isPointOfInterestNotFriendlyOrHostile(
    const LmCdl::VcsiIdentifiedPointOfInterest& pointOfInterest) const
{

    return (!isPointOfInterestHostile(lastClickedPointOfInterest_)
        && !isPointOfInterestFriendly(lastClickedPointOfInterest_)
        && !isPointOfInterestAffiliationMissing(lastClickedPointOfInterest_));
}

void PointOfInterestContextMenuContent::makePointOfInterestHostile()
{
    assignAffiliationToPointOfInterest(lastClickedPointOfInterest_, HOSTILE_MIL_STD_CODE);
}

void PointOfInterestContextMenuContent::makePointOfInterestFriendly()
{
    assignAffiliationToPointOfInterest(lastClickedPointOfInterest_, FRIENDLY_MIL_STD_CODE);
}

void PointOfInterestContextMenuContent::makePointOfInterestNeutral()
{
    assignAffiliationToPointOfInterest(lastClickedPointOfInterest_, NEUTRAL_MIL_STD_CODE);
}

void PointOfInterestContextMenuContent::assignAffiliationToPointOfInterest(
    const LmCdl::VcsiIdentifiedPointOfInterest& pointOfInterest, const QChar& affiliationCode)
{
    LmCdl::VcsiMilStdCode originalCode = pointOfInterest.pointOfInterest().milStdCode();
    QString codeString = !originalCode.isNull() ? originalCode.toString() : BASIC_MIL_STD_CODE;

    codeString = codeString.replace(MIL_STD_AFFILIATION_POSITION, 1, affiliationCode);
    LmCdl::VcsiPointOfInterestProperties updatedPointOfInterest
        = pointOfInterest.pointOfInterest().withMilStdCode(LmCdl::VcsiMilStdCode(codeString));
    pointOfInterestApi_.updatePointOfInterest(pointOfInterest.id(), updatedPointOfInterest);
}
