/*
  Copyright (C) 2017 rinigus
*/

import QtQuick 2.0
import Sailfish.Silica 1.0

import QtPositioning 5.0

import QQuickItemMapboxGL 1.0

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    MapboxMap {
        id: map
        anchors.fill: parent
        center: QtPositioning.coordinate(60.170448, 24.942046) // Helsinki
        zoomLevel: 12.25
        minimumZoomLevel: 0
        maximumZoomLevel: 20
        pixelRatio: 3.0
    }

    //! Panning and pinch implementation on the maps
    PinchArea {
        id: pincharea

        //! Holds previous zoom level value
        property double __oldZoom

        anchors.fill: parent

        //! Calculate zoom level
        function calcZoomDelta(zoom, percent) {
            return zoom + Math.log(percent)/Math.log(2)
        }

        //! Save previous zoom level when pinch gesture started
        onPinchStarted: {
            console.log("Pinch started")
            __oldZoom = map.zoomLevel
        }

        //! Update map's zoom level when pinch is updating
        onPinchUpdated: {
            console.log("Pinch updated")
            map.zoomLevel = calcZoomDelta(__oldZoom, pinch.scale)
        }

        //! Update map's zoom level when pinch is finished
        onPinchFinished: {
            console.log("Pinch finished")
            map.zoomLevel = calcZoomDelta(__oldZoom, pinch.scale)
        }


        //! Map's mouse area for implementation of panning in the map and zoom on double click
        MouseArea {
            id: mousearea

            //! Property used to indicate if panning the map
            property bool __isPanning: false

            //! Last pressed X and Y position
            property int __lastX: -1
            property int __lastY: -1

            anchors.fill : parent

            //! When pressed, indicate that panning has been started and update saved X and Y values
            onPressed: {
                __isPanning = true
                __lastX = mouse.x
                __lastY = mouse.y
            }

            //! When released, indicate that panning has finished
            onReleased: {
                __isPanning = false
            }

            //! Move the map when panning
            onPositionChanged: {
                if (__isPanning) {
                    var dx = mouse.x - __lastX
                    var dy = mouse.y - __lastY
                    map.pan(dx, dy)
                    __lastX = mouse.x
                    __lastY = mouse.y
                }
            }

            //! When canceled, indicate that panning has finished
            onCanceled: {
                __isPanning = false;
            }

            //! Zoom one level when double clicked
            onDoubleClicked: {
                map.zoomLevel += 1
            }
        }
    }
}

