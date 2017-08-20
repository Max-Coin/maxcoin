Building on Linux
=================

NOTE: These instructions have been tested and verified on Ubuntu 14.04.5 (32 and 64 bit) and 16.04.3 (32 and 64 bit).  Instructions for building on other Ubuntu versions and other Linux platforms still need further testing.

To install the required dependencies, run the following command from Ubuntu:

```
sudo apt-get update
sudo apt-get install -y git-core build-essential libssl-dev libboost-all-dev libdb-dev libdb++-dev libminiupnpc-dev libqrencode-dev libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools qt-sdk
```

Then grab the latest version of the MaxCoin source code from Github:

```
cd ~
git clone https://github.com/Max-Coin/MaxCoin.git
cd MaxCoin/src
```

**UBUNTU 16.04.3 ONLY (32 & 64 BIT)**

If building on Ubuntu 16.04.3, a revision is required to rpcrawtransaction.cpp to make it compatible with the version of the default Boost library version used by 16.04 (further details are available at https://bitcointalk.org/index.php?topic=1312757.0 and https://github.com/bitcoin/bitcoin/pull/6114/files).  THIS IS NOT NEEDED ON UBUNTU 14.04.5:

```
sed -i 's/<const\ CScriptID\&/<CScriptID/' rpcrawtransaction.cpp
```

**UBUNTU 16.04.3 32-BIT ONLY**

If building on Ubuntu 16.04.3 32-bit, modify MaxCoin/src/makefile.unix to set path for libminiupnpc.a from "/usr/lib/libminiupnpc.a" to "/usr/lib/i386-linux-gnu/libminiupnpc.a".  THIS IS NOT NEEDED ON UBUNTU 14.04.5:

```
sed -i 's/\/usr\/lib\/libminiupnpc.a/\/usr\/lib\/i386-linux-gnu\/libminiupnpc.a/' makefile.unix
```

**UBUNTU 16.04.3 64-BIT ONLY**

If building on Ubuntu 16.04.3 64-bit, modify MaxCoin/src/makefile.unix to set path for libminiupnpc.a from "/usr/lib/libminiupnpc.a" to "/usr/lib/x86_64-linux-gnu/libminiupnpc.a".  THIS IS NOT NEEDED ON UBUNTU 14.04.5:

```
sed -i 's/\/usr\/lib\/libminiupnpc.a/\/usr\/lib\/x86_64-linux-gnu\/libminiupnpc.a/' makefile.unix
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
