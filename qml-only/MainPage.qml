import QtQuick 2.0
import Sailfish.Silica 1.0
import MapboxMap 1.0
import QtPositioning 5.3

Page 
{
    id: page

    MapboxMap {
        id: map
        anchors.fill: parent

        center: QtPositioning.coordinate(60.170448, 24.942046)
        zoomLevel: 4.0
        minimumZoomLevel: 0
        maximumZoomLevel: 20
        pixelRatio: 3.0

        accessToken: "pk.eyJ1IjoicmluaWd1cyIsImEiOiJjajdkcHM0MWkwYjE4MzBwZml3OTRqOTc4In0.cjKiY1ZtOyS4KPJF0ewwQQ"
        cacheDatabaseMaximalSize: 20*1024*1024
        cacheDatabasePath: "/tmp/mbgl-cache.db"

        styleUrl: "mapbox://styles/mapbox/outdoors-v10"

        MapboxMapGestureArea {
            map: map
        }
    }
}

