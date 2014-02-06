Copyright (c) 2009-2013 Bitcoin developers

Copyright (c) 2014 MaxCoin developers

Distributed under the MIT/X11 software license, see the accompanying file COPYING or http://www.opensource.org/licenses/mit-license.php.

This product includes software developed by the OpenSSL Project for use in the [OpenSSL Toolkit](http://www.openssl.org/). 
This product includes cryptographic software written by Eric Young ([eay@cryptsoft.com](mailto:eay@cryptsoft.com)), and UPnP software written by Thomas Bernard.

This product also includes software developed by [Crypto++](http://www.cryptopp.com/) which is released under the Boost Software License 1.0.

UNIX BUILD NOTES
====================

To Build
---------------------

	cd src/
	make -f makefile.unix		# Headless MaxCoin

See readme-qt.rst for instructions on building MaxCoin-Qt, the graphical user interface.

Dependencies
---------------------

 Library     Purpose           Description
 -------     -------           -----------
 libssl      SSL Support       Secure communications
 libdb4.8    Berkeley DB       Blockchain & wallet storage
 libboost    Boost             C++ Library
 miniupnpc   UPnP Support      Optional firewall-jumping support
 libcrypto++ Cryptography      Provides implementations for cryptography in MaxCoin

[miniupnpc](http://miniupnp.free.fr/) may be used for UPnP port mapping.  It can be downloaded from [here](
http://miniupnp.tuxfamily.org/files/).  UPnP support is compiled in and
turned off by default.  Set `USE_UPNP` to a different value to control this:

	USE_UPNP=     No UPnP support miniupnp not required
	USE_UPNP=0    (the default) UPnP support turned off by default at runtime
	USE_UPNP=1    UPnP support turned on by default at runtime

IPv6 support may be disabled by setting:

	USE_IPV6=0    Disable IPv6 support

Licenses of statically linked libraries:
 Berkeley DB   New BSD license with additional requirement that linked
               software must be free open source
 Boost         MIT-like license
 miniupnpc     New (3-clause) BSD license

Versions used in this release:
-  GCC           4.3.3
-  OpenSSL       1.0.1c
-  Berkeley DB   4.8.30.NC
-  Boost         1.37
-  miniupnpc     1.6

Dependency Build Instructions: Ubuntu & Debian
----------------------------------------------

A complete list of dependencies, that can usually be installed via apt-get, is:

	git-core build-essential libssl-dev libboost-all-dev libdb5.1-dev libdb5.1++-dev libgtk2.0-dev libminiupnpc-dev qt4-qmake mingw32 synaptic qt-sdk qt4-dev-tools libqt4-dev libqt4-core libqt4-gui libdb++-dev


Notes
-----
The release is built with GCC and then `strip maxcoind` to strip the debug
symbols, which reduces the executable size by about 90%.


miniupnpc
---------
	tar -xzvf miniupnpc-1.6.tar.gz
	cd miniupnpc-1.6
	make
	sudo su
	make install


Berkeley DB
-----------
You need Berkeley DB 4.8.  If you have to build Berkeley DB yourself:

	../dist/configure --enable-cxx
	make


Boost
-----
If you need to build Boost yourself:

	sudo su
	./bootstrap.sh
	./bjam install


Crypto++
--------
If you need to build Crypto++ yourself, please see http://www.cryptopp.com/wiki/Linux


Security
--------
To help make your MaxCoin installation more secure by making certain attacks impossible to
exploit even if a vulnerability is found, you can take the following measures:

* Position Independent Executable
    Build position independent code to take advantage of Address Space Layout Randomization
    offered by some kernels. An attacker who is able to cause execution of code at an arbitrary
    memory location is thwarted if he doesn't know where anything useful is located.
    The stack and heap are randomly located by default but this allows the code section to be
    randomly located as well.

    On an Amd64 processor where a library was not compiled with `-fPIC`, this will cause an error
    such as: `relocation R_X86_64_32 against ......' can not be used when making a shared object;`

    To build with PIE, use:

    	make -f makefile.unix ... -e PIE=1

    To test that you have built PIE executable, install scanelf, part of paxutils, and use:

    	scanelf -e ./maxcoin

    The output should contain:
     TYPE
    ET_DYN

* Non-executable Stack
    If the stack is executable then trivial stack based buffer overflow exploits are possible if
    vulnerable buffers are found. By default, MaxCoin should be built with a non-executable stack
    but if one of the libraries it uses asks for an executable stack or someone makes a mistake
    and uses a compiler extension which requires an executable stack, it will silently build an
    executable without the non-executable stack protection.

    To verify that the stack is non-executable after compiling use:
    `scanelf -e ./maxcoin`

    the output should contain:
	`STK/REL/PTL
	RW- R-- RW-`

    The STK RW- means that the stack is readable and writeable but not executable.
