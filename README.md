# Demo for Sailfish Mapbox GL QML

Demo application for Sailfish using Mapbox GL Native QML bindings from
https://github.com/rinigus/mapbox-gl-qml

It is recommended to use QML-only approach. To use Mapbox GL tiles and
styles provided by Mapbox, please register on their site and obtain
access key. The key can be either used to specify `accessToken`
property of MapboxMap (see 
[API description](https://github.com/rinigus/mapbox-gl-qml/blob/master/api.md))
or using environment variable `MAPBOX_ACCESS_TOKEN`.

For QML-only demo, 

* edit `qml-only/MainPage.qml` and specify your access token (skip if using env variable)
* copy contents of `qml-only` to your device
* install mapboxgl-qml from https://openrepos.net/content/rinigus/mapbox-gl-native-bindings-qt-qml or 
https://build.merproject.org/package/show/home:rinigus:maps/mapboxgl-qml,
* specify environment variable by `export MAPBOX_ACCESS_TOKEN=insert_your_token_here` (skip if added token to QML)
* run the demo by `qmlscene mapbox-gl-qml.qml`

While C++ version is given as well, QML approach is recommended. Note that C++ 
version may require some adjustments of qmake pro-file to include all required 
libraries. For example, recently, `libcurl` has been used in SFOS to avoid issues
originating from Qt network handling.
