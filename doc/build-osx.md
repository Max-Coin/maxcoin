Mac OS X maxcoind build instructions
====================================

Authors
-------

*Jonathan Rankin (jonnyrankin@googlemail.com)

License
-------

Copyright (c) 2009-2014 Bitcoin Developers
Copyright (c)      2014 MaxCoin Developers

Distributed under the MIT/X11 software license, see the accompanying
file COPYING or http://www.opensource.org/licenses/mit-license.php.
This product includes software developed by the OpenSSL Project for use in
the OpenSSL Toolkit (http://www.openssl.org/).
This product includes cryptographic software written by
Eric Young (eay@cryptsoft.com) and UPnP software written by Thomas Bernard.
This product also includes software developed by [Crypto++](http://www.cryptopp.com/) which is released under the Boost Software License 1.0

Notes
-----

See `doc/readme-qt.rst` for instructions on building Maxcoin-Qt, the
graphical user interface.

Tested on OS X 10.6 through 10.9 on Intel processors only. PPC is not
supported because it is big-endian.

All of the commands should be executed in a Terminal application. The
built-in one is located in `/Applications/Utilities`.

Preparation
-----------

You need to install XCode with all the options checked so that the compiler
and everything is available in /usr not just /Developer. XCode should be
available on your OS X installation media, but if not, you can get the
current version from https://developer.apple.com/xcode/. If you install
Xcode 4.3 or later, you'll need to install its command line tools. This can
be done in `Xcode > Preferences > Downloads > Components` and generally must
be re-done or updated every time Xcode is updated.

There's an assumption that you already have `git` installed, as well. If
not, it's the path of least resistance to install [Github for Mac](https://mac.github.com/)
(OS X 10.7+) or
[Git for OS X](https://code.google.com/p/git-osx-installer/). It is also
available via Homebrew or MacPorts.

You will also need to install [MacPorts](https://www.macports.org/) in order to install library
dependencies. 


The installation of the actual dependencies is covered in the Instructions
sections below.

Instructions: MacPorts
----------------------

### Install dependencies

Installing the dependencies using MacPorts is very straightforward.

    sudo port install boost db48@+no_java openssl miniupnpc

### Building `maxcoind`

1. Clone the github tree to get the source code and go into the directory.

        git clone https://github.com/Max-Coin/MaxCoin.git MaxCoin
        cd MaxCoin

2. set up leveldb folder

        cd src/leveldb
        chmod +x build_detect_platform
        make clean
        make
        make libmemenv.a

2. Complile the cryptopp library

        cd ../cryptopp
        make dynamic static
        make

2.  Build maxcoind:

        cd ..
        make -f makefile.osx

3.  It is a good idea to build and run the unit tests, too:

        make -f makefile.osx test


Creating a release build
------------------------

A maxcoind binary is not included in the Maxcoin-Qt.app bundle. You can ignore
this section if you are building `maxcoind` for your own use.

If you are building `maxcoind` for others, your build machine should be set up
as follows for maximum compatibility:

All dependencies should be compiled with these flags:

    -mmacosx-version-min=10.5 -arch i386 -isysroot /Developer/SDKs/MacOSX10.6.sdk

For MacPorts, that means editing your macports.conf and setting
`macosx_deployment_target` and `build_arch`:

    macosx_deployment_target=10.5
    build_arch=i386

... and then uninstalling and re-installing, or simply rebuilding, all ports.


Once dependencies are compiled, creating `MaxCoin-Qt.app` is easy:

    navigate to maxcoin-master
    make -f Makefile RELEASE=1

To create a dmg file

     contrib/macdeploy/macdeployqtplus MaxCoin-QT.app -add-qt-tr de,es,ru -dmg -fancy contrib/macdeploy/fancy.plist

Note: 
As of December 2012, the `boost` port does not obey `macosx_deployment_target`.
Download `http://gavinandresen-bitcoin.s3.amazonaws.com/boost_macports_fix.zip`
for a fix. Some ports also seem to obey either `build_arch` or
`macosx_deployment_target`, but not both at the same time. For example, building
on an OS X 10.6 64-bit machine fails.

Running
-------

It's now available at `./maxcoind`, provided that you are still in the `src`
directory. We have to first create the RPC configuration file, though.

Run `./maxcoind` to get the filename where it should be put, or just try these
commands:

    echo -e "rpcuser=maxcoinrpc\nrpcpassword=$(xxd -l 16 -p /dev/urandom)" > "/Users/${USER}/Library/Application Support/maxcoin/maxcoin.conf"
    chmod 600 "/Users/${USER}/Library/Application Support/maxcoin/maxcoin.conf"

When next you run it, it will start downloading the blockchain, but it won't
output anything while it's doing this. This process may take several hours.

Other commands:

    ./maxcoind --help  # for a list of command-line options.
    ./maxcoind -daemon # to start the MaxCoin daemon.
    ./maxcoind help    # when the daemon is running, to get a list of RPC commands
