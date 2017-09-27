# IVI-Launcher

This is the Qt module for developing IVI launchers and launcher related plugins.

## Launcher

A launcher is primary component of the IVI stack. Its primary responability is to manager which application will be launched and where they will be shown on the display(s). The IVI Launcher module provides a C++ and QML API that helps the launcher obtain a list available applications, can start of stop the application in a separate process and is able to draw applications in specified region on the display.

The [Application Manager](src/IVI-Launcher/iviapplicationmanager.h) in the IVI launcher module allow the launcher to obtain a list of available applications. In C++ a single instance of this type may be obtained using the IVIApplicationManager::getInstance() method. In QML multiple instances of [ApplicationManager](src/imports/iviapplicationmanagerqml.h) may be created.

The [Application](src/IVI-Launcher/iviapplication.h) in the IVI Launcher module allows the launcher to obtain extra information about a particular applilcation. Instance of this type cannot be created, they can only be obtained using the application manager.

The [Application Container](src/IVI-Launcher/iviabstractappcontainer.h) in the IVI Launcher module allows the launcher to display an application on the display. Currently only a QML implementaion exsists. The application will automaticaly be requested to resize to the size of the AppContainer object and rendered as if the application was contained in the AppContainer object.

## Vehicle Intergration Plugin

This plugin type allows the vehicle vendor to expose vehicle data and vehicle settings to applications. By default all available vehicle integration plugins will be loaded when a launcher is started. To control what plugin get loaded an environement variable IVI_VEHICLE_INTERACTION_BACKENDS may be defined with a colon (:) separeted list of plugin keys that shoudl be loaded.

## TODO
* (Re)create support for softkeys.
* Handle resize of applications contained in multiple app pcontainers.
* Expose vehicle data/vehicle settings to launcher.
