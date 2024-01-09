Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.

Overview
---------------

This example shows how to draw on the map, create a widget in the Settings
window, and how to use some of the unit conversion classes.

This example loads data from an FAA Digital Obstacle File and draws spheres
at those locations on the world.  The size and color of the spheres are
based on data in the file.

A widget is added in the Settings window that allows the user to load
individual files and to clear the spheres.

Note that locations in the file are stored in degrees and altitudes are stored
in feet.  The example converts them to radians and metres when needed.

Pre-requisities
---------------

1. Ensure the following packages are installed on your machine:

  qt-devel-4.8.5
  gcc-c++-4.8.5
  make
  
2. Ensure your user has sudo privileges

   Add the following line to the end of /etc/sudoers:

   <username>   ALL=(ALL)       NOPASSWD: ALL

   Where <username> is your username (type: whoami)

Build instructions
------------------

To build and install the plugin:

  ./build.bash

If successful, the plugin will be installed into:

  /opt/vcs/vcsi/lib/plugins


Run-time instructions
---------------------

Start VCSi and click the gear icon in the top-right to open the Settings window.

An entry called "Obstacles" should appear on the left side.  Select it and follow 
the instructions presented.

Note that this example ships with an obstacle file for Hawaii (15-HI.Dat)
