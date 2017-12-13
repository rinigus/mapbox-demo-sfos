# mapbox-demo-sfos

Demo application for Sailfish using Mapbox GL Native QML bindings from
https://github.com/rinigus/mapbox-gl-qml

For QML-only demo, copy contents of `qml-only` to your device, install
mapboxgl-qml from one of the repositories of
https://build.merproject.org/package/show/home:rinigus:maps/mapboxgl-qml,
and run it by

```
qmlscene mapbox-gl-qml.qml
```

While C++ version is given as well, QML approach is recommended. Note that C++ 
version may require some adjustments of qmake pro-file to include all required 
libraries. For example, recently, `libcurl` has been used ni SFOS to avoid issues
originating from Qt network handling.
