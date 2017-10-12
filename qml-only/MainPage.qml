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

        styleJson: '{
            "version": 8,
            "name": "Empty",
            "sources": {
                "mapbox://mapbox.mapbox-streets-v6": {
                    "url": "mapbox://mapbox.mapbox-streets-v6",
                    "type": "vector"
                }
            },
            "sprite": "mapbox://sprites/tmpsantos/35fb3795",
            "glyphs": "mapbox://fonts/tmpsantos/{fontstack}/{range}.pbf",
            "layers": [
                {
                    "id": "background",
                    "type": "background",
                    "metadata": {},
                    "minzoom": 0,
                    "maxzoom": 22,
                    "layout": {
                        "visibility": "visible"
                    },
                    "paint": {
                        "background-color": "rgba(143,39,39,1)"
                    },
                    "interactive": true
                },
                {
                    "id": "admin",
                    "type": "line",
                    "metadata": {},
                    "source": "mapbox://mapbox.mapbox-streets-v6",
                    "source-layer": "admin",
                    "minzoom": 0,
                    "maxzoom": 22,
                    "filter": [
                        "in",
                        "$type",
                        "LineString",
                        "Polygon",
                        "Point"
                    ],
                    "layout": {
                        "visibility": "visible"
                    },
                    "paint": {
                        "line-color": "rgba(235,204,204,1)",
                        "line-width": 1
                    },
                    "interactive": true
                }
            ],
            "draft": false,
            "id": "35fb3795",
            "modified": "2015-09-04T06:15:21.245Z",
            "owner": "tmpsantos",
            "visibility": "private",
            "created": "2015-09-03T07:50:56.403Z"
        }'

        MapboxMapGestureArea {
            map: map
        }
    }
}

