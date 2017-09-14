/*
  Copyright (C) 2017 rinigus
*/

#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>

#include "qquickitemmapboxgl.h"

int main(int argc, char *argv[])
{
  // SailfishApp::main() will display "qml/template.qml", if you need more
  // control over initialization, you can use:
  //
  //   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
  //   - SailfishApp::createView() to get a new QQuickView * instance
  //   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
  //
  // To display the view, call "show()" (will show fullscreen on device).

  qputenv("MAPBOX_ACCESS_TOKEN", "pk.eyJ1IjoicmluaWd1cyIsImEiOiJjajdkcHM0MWkwYjE4MzBwZml3OTRqOTc4In0.cjKiY1ZtOyS4KPJF0ewwQQ");

  qmlRegisterType<QQuickItemMapboxGL>("QQuickItemMapboxGL", 1, 0, "MapboxMap");

  return SailfishApp::main(argc, argv);
}
