# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = mapbox-sfos

CONFIG += sailfishapp c++14
QT += location positioning sql quick

QMAKE_CXX=/opt/gcc6/bin/g++
QMAKE_CC=/opt/gcc6/bin/gcc
QMAKE_LINK=/opt/gcc6/bin/g++

SOURCES += src/mapbox-sfos.cpp

include(mapbox-gl-qml/mapbox-gl-qml.pri)

LIBS += -lqmapboxgl -lz -L/opt/gcc6/lib -static-libstdc++

OTHER_FILES += qml/mapbox-sfos.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FirstPage.qml \
    qml/pages/SecondPage.qml \
    qml/pages/MapMouseArea.qml \
    rpm/mapbox-sfos.changes.in \
    rpm/mapbox-sfos.spec \
    rpm/mapbox-sfos.yaml \
    translations/*.ts \
    mapbox-sfos.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 256x256

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/mapbox-sfos-de.ts

