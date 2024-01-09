TEMPLATE = subdirs

# Note the messageDefinitions directory needs to build first, as other plugin examples depend on it...
SUBDIRS += \
    messageDefinitions \
    actions \
    audio \
    dockWidgets \
    flarmDevice \
    graphicsWidgets \
    groundMode \
    joystick \
    lights \
    mapDrawing \
    missionDrawing \
    notification \
    payloadDrawing \
    pointOfInterest \
    preflightChecklist \
    restrictionZoneApi \
    routes \
    shapes \
    stanagE2PrivateMessage \
    standardPaths \
    terrainProfile \
    trackDrawing \
    vectorData \
    vehicleTab \
    videoStream \
    zeroizeGcsData 

routes.depends = messageDefinitions
stanagE2PrivateMessage.depends = messageDefinitions
