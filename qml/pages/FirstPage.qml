/*
  Copyright (C) 2017 rinigus
*/

import QtQuick 2.0
import Sailfish.Silica 1.0

import QtPositioning 5.2

import QQuickItemMapboxGL 1.0

import "."

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    MapboxMap {
        id: map
        anchors.fill: parent
        center: QtPositioning.coordinate(60.170448, 24.942046) // Helsinki
        zoomLevel: 4.0
        minimumZoomLevel: 0
        maximumZoomLevel: 20
        pixelRatio: 3.0

        bearing: bearingSlider.value
        pitch: pitchSlider.value

        //accessToken: "INSERT_THE_TOKEN_OR_DEFINE_IN_ENVIRONMENT"
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

        MapMouseArea { map: map }
    }

    Rectangle {
        anchors.fill: menu
        anchors.margins: -20
        radius: 30
        clip: true
        color: "black"
    }

    Column {
        id: menu

        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 30

        Label {
            text: "Bearing"
        }

        Slider {
            id: bearingSlider

            width: page.width-100
            maximumValue: 180
            minimumValue: 0
            value: 0
        }

        Label {
            text: "Pitch"
        }

        Slider {
            id: pitchSlider

            width: page.width-100
            maximumValue: 60
            minimumValue: 0
            value: 0
        }
    }

    Label {
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 10
        color: "black"
        text: "Scale: %0 m/pixel".arg(map.metersPerPixel.toFixed(2))
    }

    Component.onCompleted: {

        var routeSource = {
            "type": "geojson",
            "data": '{
                "type": "Feature",
                "properties": {},
                "geometry": {
                    "type": "LineString",
                    "coordinates": [
                        [24.942046, 60.170448],
                        [
                            24.934420000000003,
                            60.163500000000006
                        ],
                        [
                            24.923490008921945,
                            60.16159500239787
                        ],
                        [
                            24.916150000000002,
                            60.171530000000004
                        ],
                        [
                            24.931620000000002,
                            60.18218
                        ],
                        [
                            24.961660000000002,
                            60.17557000000001
                        ],
                        [
                            24.954860000000004,
                            60.158930000000005
                        ],
                        [
                            24.943690000000004,
                            60.155280000000005
                        ]
                    ]
                }
            }'
        }

        map.addSource("route", routeSource)

        map.addLayer("routeCase", { "type": "line", "source": "route" }, "waterway-label")
        map.setLayoutProperty("routeCase", "line-join", "round");
        map.setLayoutProperty("routeCase", "line-cap", "round");
        map.setPaintProperty("routeCase", "line-color", "white");
        map.setPaintProperty("routeCase", "line-width", 15.0);

        map.addLayer("route", { "type": "line", "source": "route" }, "waterway-label")
        map.setLayoutProperty("route", "line-join", "round");
        map.setLayoutProperty("route", "line-cap", "round");
        map.setPaintProperty("route", "line-color", "blue");
        map.setPaintProperty("route", "line-width", 10.0);
        map.setPaintPropertyList("route", "line-dasharray", [1,2]);

        /// Location support
        map.addSource("location",
                      {"type": "geojson",
                          "data": {
                              "type": "Feature",
                              "properties": { "name": "location" },
                              "geometry": {
                                  "type": "Point",
                                  "coordinates": [
                                      (24.94),
                                      (60.16)
                                  ]
                              }
                          }
                      })

        map.addLayer("location-uncertainty", {"type": "circle", "source": "location"}, "waterway-label")
        map.setPaintProperty("location-uncertainty", "circle-radius", 20)
        map.setPaintProperty("location-uncertainty", "circle-color", "#87cefa")
        map.setPaintProperty("location-uncertainty", "circle-opacity", 0.25)

        map.addLayer("location-case", {"type": "circle", "source": "location"}, "waterway-label")
        map.setPaintProperty("location-case", "circle-radius", 10)
        map.setPaintProperty("location-case", "circle-color", "white")

        map.addLayer("location", {"type": "circle", "source": "location"}, "waterway-label")
        map.setPaintProperty("location", "circle-radius", 5)
        map.setPaintProperty("location", "circle-color", "blue")

        map.addLayer("location-label", {"type": "symbol", "source": "location"})
        map.setLayoutProperty("location-label", "text-field", "{name}")
        map.setLayoutProperty("location-label", "text-justify", "left")
        map.setLayoutProperty("location-label", "text-anchor", "top-left")
        map.setLayoutPropertyList("location-label", "text-offset", [0.2, 0.2])
        map.setPaintProperty("location-label", "text-halo-color", "white")
        map.setPaintProperty("location-label", "text-halo-width", 2)
    }

    Connections {
        target: map
        onReplySourceExists: {
            console.log("Source: " + sourceID + " " + exists)
        }
    }

    PositionSource {
        id: gps

        active: true

        function update_map() {
            if (gps.position.longitudeValid && gps.position.latitudeValid)
            {
                map.updateSourcePoint("location",
                                      gps.position.coordinate.latitude,
                                      gps.position.coordinate.longitude,
                                      "accuracy: " + gps.position.horizontalAccuracy.toFixed(0) + " meters")
                //map.center = gps.position.coordinate
            }

            if (gps.position.horizontalAccuracyValid)
            {
                map.setPaintProperty("location-uncertainty", "circle-radius", gps.position.horizontalAccuracy / map.metersPerPixel)
            }

            // if (gps.position.directionValid)
            //     map.bearing = gps.position.direction
        }


        onPositionChanged: {
            update_map()
        }

        Component.onCompleted: {
            update_map()
        }
    }

    Connections {
        target: map
        onMetersPerPixelChanged: gps.update_map()
    }

}
