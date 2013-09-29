ztorrent
========

A desktop client to quickly do a search for torrents and get links to .torrent files or add to Transmission for download. Backed by [Torrentz](http://torrentz.eu) and [Torcache](http://torcache.net).

Install
---------

### Qt 5.x Dependency

This application is built using Qt 5.x, so you would need to have this before you can compile the source code.

Check the version with:

    qmake -v

Look at the version of Qt libraries, not the version of `qmake` itself. Install relevant package if you don't have the `qmake` command.

### Build

Following applies to Linux only; for MacOS build, check the [releases](https://github.com/viranch/ztorrent/releases) section.

Download and compile:

    git clone git://github.com/viranch/ztorrent.git
    cd ztorrent
    mkdir build && cd build
    qmake ..
    make

Run:

    ./ztorrent

Optionally, install to `/usr/bin`:

    sudo cp build/ztorrent /usr/bin/ztorrent

Use
---------

1. Search for any torrent in this app and you will get search results from [Torrentz](http://torrentz.com/) with size/seeds/peers info.
2. Copy the link for .torrent for any of the search results to clipboard with a single click and use it in your torrent client.
3. Add the download directly to a [Transmission](http://transmissionbt.com/) daemon running locally (enable remote access) or remotely with a single click, you need to add the backend in settings. Multiple backends supported.
