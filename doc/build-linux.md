Building on Linux
=================

NOTE: These instructions have been tested and verified on Ubuntu 18.04

To install the required dependencies, run the following command from Ubuntu:

```
sudo apt update
sudo apt install -y git build-essential libssl1.0-dev libboost-all-dev libdb-dev libdb++-dev libminiupnpc-dev libqrencode-dev libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools qt5-default
```

Then grab the latest version of the MaxCoin source code from Github:

```
cd ~
git clone https://github.com/Max-Coin/MaxCoin.git
cd MaxCoin/src
```

To build the daemon, run the following command:

```
make -f makefile.unix
```

Optionally, debugging symbols can be removed from the binary to reduce its size. This can be done using strip:

```
strip maxcoind
```

Then, to build the GUI, run the following commands:

```
cd ..
qmake maxcoin-qt.pro
make
```
