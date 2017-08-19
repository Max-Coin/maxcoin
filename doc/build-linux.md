Building on Linux
===============

NOTE: These instructions have been tested and verified on Ubuntu 14.04.5 (32 and 64 bit).  Instructions for building on other Ubuntu versions and other Linux platforms still need further testing.

To install the required dependencies, run the following command from Ubuntu:

```bash
sudo apt-get update
sudo apt-get install -y git-core build-essential libssl-dev libboost-all-dev libdb-dev libdb++-dev libminiupnpc-dev libqrencode-dev libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools qt-sdk
```
or the following command from Debian (NOTE: the Debian packages below have not been reviewed recently and may be outdated):

````bash
sudo apt-get install -y git-core build-essential libssl-dev libboost-all-dev libdb-dev libdb4.8++-dev libgtk2.0-dev qt4-qmake mingw32 synaptic qt-sdk qt4-dev-tools libqt4-dev libqt4-core libqt4-gui libdb4.8++-dev
````

Then grab the latest version of the MaxCoin source code from Github:

```bash
cd ~
git clone https://github.com/Max-Coin/MaxCoin.git
cd MaxCoin/src
```

To build the daemon, run the following command:

```bash
make -f makefile.unix
```

Optionally, debugging symbols can be removed from the binary to reduce its size. This can be done using strip:

```bash
strip maxcoind
```

Then, to build the GUI, run the following commands:

```bash
cd ..
qmake "USE_QRCODE=1" "USE_UPNP=1" "USE_IPV6=1" maxcoin-qt.pro
make
```

