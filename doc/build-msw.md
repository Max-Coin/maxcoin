Copyright (c) 2009-2017 Maxcoin Developers

Distributed under the MIT/X11 software license, see the accompanying
file COPYING or http://www.opensource.org/licenses/mit-license.php.
This product includes software developed by the OpenSSL Project for use in the [OpenSSL Toolkit](http://www.openssl.org/). This product includes
cryptographic software written by Eric Young ([eay@cryptsoft.com](mailto:eay@cryptsoft.com)), and UPnP software written by Thomas Bernard.  This product also includes software developed by [Crypto++](http://www.cryptopp.com/) which is released under the Boost Software License 1.0.


WINDOWS BUILD NOTES
===================

Dependencies
------------
Libraries that will be used:

	Name            Default Path              Source
	---------------------------------------------------------------------------------------------------------------------------
	OpenSSL         \deps\openssl-1.0.1f      http://www.openssl.org/source
	Berkeley DB     \deps\db-5.1.29.NC        http://www.oracle.com/technetwork/products/berkeleydb/downloads/index-082944.html
	Boost           \deps\boost_1_54_0        http://www.boost.org/users/history/
	miniupnpc       \deps\miniupnpc-1.6       http://miniupnp.tuxfamily.org/files/
	libpng		\deps\libpng-1.2.50       http://sourceforge.net/projects/libpng/files/
	qrencode	\deps\qrencode-3.4.2      http://fukuchi.org/works/qrencode/
	
	
Their licenses:

	OpenSSL        http://www.openssl.org/source/license.html
	Berkeley DB    http://www.oracle.com/technetwork/database/database-technologies/berkeleydb/downloads/oslicense-093458.html
	Boost          http://www.boost.org/users/license.html
	miniupnpc      http://github.com/miniupnp/miniupnp/blob/master/miniupnpc/LICENSE
	libpng         http://libpng.org/pub/png/src/libpng-LICENSE.txt
	qrencode       https://fukuchi.org/works/qrencode/

Versions used in this release:

	OpenSSL      1.0.1f
	Berkeley DB  5.1.29.NC
	Boost        1.54.0
	miniupnpc    1.6
	png          1.2.50
	qrencode     3.4.2


NOTE: These instructions were developed and tested on Windows 7 Professional (64-bit).

1 - Install Build Environment
=============================
1.1 - Install MinGW MSYS Shell
------------------------------
1. Download MinGW MSYS shell from http://sourceforge.net/projects/mingw/files/Installer/mingw-get-setup.exe/download.
2. Run (as Administrator) "mingw-get-setup.exe".
3. Click "Install".
4. Accept all defaults and click "Continue".
5. When MinGW Installation Manager opens, select "All Packages" on the left-hand pane.
6. Select the following packages as "Mark for Installation":

	| Package       | Class |
	| ------------- | ----- |
	| msys-base     | bin   |
	| msys-autoconf | bin   |
	| msys-automake | bin   |
	| msys-libtool  | bin   |
	| msys-wget     | bin   |

7. Make sure no mingw packages are checked for installation or present from a previous install. Only the above msys packages should be installed. Also make sure that "msys-gcc" and "msys-w32api" packages are not installed.
8. On the application menu, select "Installation" -> "Apply Changes", then click "Apply".
9. After installation has completed, click "Close" and then exit MinGW Installation Manager.
10. "C:\MinGW\bin" should contain nothing but "mingw-get.exe".
11. Add "C:\MinGW\msys\1.0" to the PATH environment variable ("Control Panel" -> "System and Security" -> "System" -> "Advanced system settings" -> "Environment Variables..." -> "System variables").  On Windows 7, the path should end up looking something like:

```
C:\MinGW\msys\1.0;%SystemRoot%\system32;%SystemRoot%;%SystemRoot%\System32\Wbem;%SYSTEMROOT%\System32\WindowsPowerShell\v1.0\
```

1.2 - Install MinGW Toolchain
-----------------------------
1. Download http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/4.9.2/threads-posix/dwarf/i686-4.9.2-release-posix-dwarf-rt_v3-rev1.7z/download
2. Using 7Zip, unpack the "i686-4.9.2-release-posix-dwarf-rt_v3-rev1.7z" source archive to "C:\".
3. Add "C:\mingw32\bin" to the PATH environment variable.  On Windows 7, the path should end up looking something like:

```
C:\mingw32\bin;C:\MinGW\msys\1.0;%SystemRoot%\system32;%SystemRoot%;%SystemRoot%\System32\Wbem;%SYSTEMROOT%\System32\WindowsPowerShell\v1.0\
```

4. From a DOS shell (cmd.exe run as Administrator), run "msys" to launch the MINGW32 shell:

```
msys
```

5. From the MINGW32 shell, run "gcc -v":

```
gcc -v
```

6. The output should be:

```
Using built-in specs.
COLLECT_GCC=c:\mingw32\bin\gcc.exe
COLLECT_LTO_WRAPPER=c:/mingw32/bin/../libexec/gcc/i686-w64-mingw32/4.9.2/lto-wrapper.exe
Target: i686-w64-mingw32
Configured with: ../../../src/gcc-4.9.2/configure --host=i686-w64-mingw32 --build=i686-w64-mingw32 --target=i686-w64-mingw32 --prefix=/mingw32 --with-sysroot=/c/mingw492/i686-492-posix-dwarf-rt_v3-rev1/mingw32 --with-gxx-include-dir=/mingw32/i686-w64-mingw32/include/c++ --enable-shared --enable-static --disable-multilib --enable-languages=ada,c,c++,fortran,objc,obj-c++,lto --enable-libstdcxx-time=yes --enable-threads=posix --enable-libgomp --enable-libatomic --enable-lto --enable-graphite --enable-checking=release --enable-fully-dynamic-string --enable-version-specific-runtime-libs --disable-sjlj-exceptions --with-dwarf2 --disable-isl-version-check --disable-cloog-version-check --disable-libstdcxx-pch --disable-libstdcxx-debug --enable-bootstrap --disable-rpath --disable-win32-registry --disable-nls --disable-werror --disable-symvers --with-gnu-as --with-gnu-ld --with-arch=i686 --with-tune=generic --with-libiconv --with-system-zlib --with-gmp=/c/mingw492/prerequisites/i686-w64-mingw32-static --with-mpfr=/c/mingw492/prerequisites/i686-w64-mingw32-static --with-mpc=/c/mingw492/prerequisites/i686-w64-mingw32-static --with-isl=/c/mingw492/prerequisites/i686-w64-mingw32-static --with-cloog=/c/mingw492/prerequisites/i686-w64-mingw32-static --enable-cloog-backend=isl --with-pkgversion='i686-posix-dwarf-rev1, Built by MinGW-W64 project' --with-bugurl=http://sourceforge.net/projects/mingw-w64 CFLAGS='-O2 -pipe -I/c/mingw492/i686-492-posix-dwarf-rt_v3-rev1/mingw32/opt/include -I/c/mingw492/prerequisites/i686-zlib-static/include -I/c/mingw492/prerequisites/i686-w64-mingw32-static/include' CXXFLAGS='-O2 -pipe -I/c/mingw492/i686-492-posix-dwarf-rt_v3-rev1/mingw32/opt/include -I/c/mingw492/prerequisites/i686-zlib-static/include -I/c/mingw492/prerequisites/i686-w64-mingw32-static/include' CPPFLAGS= LDFLAGS='-pipe -L/c/mingw492/i686-492-posix-dwarf-rt_v3-rev1/mingw32/opt/lib -L/c/mingw492/prerequisites/i686-zlib-static/lib -L/c/mingw492/prerequisites/i686-w64-mingw32-static/lib -Wl,--large-address-aware'
Thread model: posix
gcc version 4.9.2 (i686-posix-dwarf-rev1, Built by MinGW-W64 project)
```

1.3 - Install Git for Windows
-----------------------------
1. Download https://git-scm.com/download/win
2. Run (as Administrator) the downloaded Git installer (which, at the time of this writing is "Git-2.13.3-64-bit.exe").
3. Accept default values at every step of the installation and unselect "View Release Notes" at the end.

1.4 - Install NullSoft Scriptable Install System (required only if you need to build Maxcoin installer for Windows)
-------------------------------------------------------------------------------------------------------------------
1. Download http://prdownloads.sourceforge.net/nsis/nsis-3.02.1-setup.exe?download
2. Run (as Administrator) "nsis-3.02.1-setup.exe".
3. Accept default values at every step of the installation and unselect "Run NSIS" and "Show release notes" at the end.
4. Add "C:\Program Files (x86)\NSIS" to the PATH environment variable.  On Windows 7, the path should end up looking something like:

```
C:\mingw32\bin;C:\MinGW\msys\1.0;%SystemRoot%\system32;%SystemRoot%;%SystemRoot%\System32\Wbem;%SYSTEMROOT%\System32\WindowsPowerShell\v1.0\;C:\Program Files\Git\cmd;C:\Program Files (x86)\NSIS
```

2 - Install Maxcoin Dependencies
================================
We'll use "C:\deps" as the folder for all of the required dependencies.

2.1 - Install OpenSSL Library
-----------------------------
1. From a DOS shell, run "msys" to launch the MINGW32 shell:

```
msys
```

2. From the MINGW32 shell, create a "C:\deps" folder:

```
cd /C
mkdir deps
```

3. From the MINGW32 shell, download http://www.openssl.org/source/openssl-1.0.1f.tar.gz to the "C:\deps" folder:

```
cd deps
wget http://www.openssl.org/source/openssl-1.0.1f.tar.gz --no-check-certificate
```

4. From the MINGW32 shell, unpack the "openssl-1.0.1f.tar.gz" source archive with tar (this will avoid symlink issues that you'd otherwise run into with 7Zip or WinRAR):

```
tar xvfz openssl-1.0.1f.tar.gz
```

5. From the MINGW32 shell, configure OpenSSL:

```
cd openssl-1.0.1f
./config
```

6. From the MINGW32 shell, build OpenSSL:

```
make
```

2.2 - Install Berkeley DB Library
---------------------------------
1. From a MINGW32 shell, download http://download.oracle.com/berkeley-db/db-5.1.29.NC.tar.gz to the "C:\deps" folder:

```
cd /C/deps
wget http://download.oracle.com/berkeley-db/db-5.1.29.NC.tar.gz --no-check-certificate
```

2. From the MINGW32 shell, unpack the "db-5.1.29.NC.tar.gz" source archive:

```
tar xvfz db-5.1.29.NC.tar.gz
```

3. From the MINGW32 shell, configure Berkeley DB:

```
cd db-5.1.29.NC/build_unix
../dist/configure --enable-mingw --enable-cxx --disable-shared --disable-replication
```

4. From the MINGW32 shell, build Berkeley DB:

```
make
```

2.3 - Install Boost Library
---------------------------
1. From a MINGW32 shell, download https://sourceforge.net/projects/boost/files/boost/1.54.0/boost_1_54_0.tar.gz/download to the "C:\deps" folder:

```
cd /C/deps
wget https://sourceforge.net/projects/boost/files/boost/1.54.0/boost_1_54_0.tar.gz/download --no-check-certificate
```

2. From the MINGW32 shell, unpack the "boost_1_54_0.tar.gz" source archive:

```
tar xvfz boost_1_54_0.tar.gz
```

3. From a DOS shell (run as Administrator), configure Boost:

```
cd \deps\boost_1_54_0
bootstrap.bat mingw
```

4. From the DOS shell, build Boost:

```
b2 --build-type=complete --with-chrono --with-filesystem --with-program_options --with-system --with-thread toolset=gcc variant=release link=static threading=multi runtime-link=static stage
```

2.4 - Install Miniupnpc Library
-------------------------------
1. From a MINGW32 shell, download http://miniupnp.free.fr/files/download.php?file=miniupnpc-1.6.tar.gz to the "C:\deps" folder:

```
cd /C/deps
wget http://miniupnp.free.fr/files/download.php?file=miniupnpc-1.6.tar.gz --no-check-certificate
```

2. From the MINGW32 shell, unpack the "miniupnpc-1.6.tar.gz" source archive:

```
tar xvfz miniupnpc-1.6.tar.gz
```

3. From the MINGW32 shell, build Miniupnpc:

```
cd miniupnpc-1.6
mingw32-make -f Makefile.mingw init upnpc-static
mkdir miniupnpc
cp *.h miniupnpc/
```

2.5 - Install Libpng and Libqrencode Libraries
----------------------------------------------
1. From a MINGW32 shell, download http://prdownloads.sourceforge.net/libpng/libpng-1.2.50.tar.gz to the "C:\deps" folder:

```
cd /C/deps
wget http://prdownloads.sourceforge.net/libpng/libpng-1.2.50.tar.gz --no-check-certificate
```

2. From the MINGW32 shell, unpack the "libpng-1.2.50.tar.gz" source archive:

```
tar xzvf libpng-1.2.50.tar.gz
```

3. From the MINGW32 shell, configure libpng:

```
cd libpng-1.2.50
configure
```

4. From the MINGW32 shell, build libpng:

```
make
```

5. From the MINGW32 shell, download https://fukuchi.org/works/qrencode/qrencode-3.4.2.tar.gz to the "C:\deps" folder:

```
cd /C/deps
wget https://fukuchi.org/works/qrencode/qrencode-3.4.2.tar.gz --no-check-certificate
```

6. From the MINGW32 shell, unpack the "qrencode-3.4.2.tar.gz" source archive:

```
tar xzvf qrencode-3.4.2.tar.gz
```

7. From the MINGW32 shell, configure libqrencode:

```
cd qrencode-3.4.2
LIBS="../libpng-1.2.50/.libs/libpng12.a ../../mingw32/i686-w64-mingw32/lib/libz.a" png_CFLAGS="-I../libpng-1.2.50" png_LIBS="-L../libpng-1.2.50/.libs" configure --enable-static --disable-shared
```

8. From the MINGW32 shell, build libqrencode:

```
make
```

2.6 - Install Qt (required only if you need to build maxcoin-qt)
----------------------------------------------------------------
1. From a MINGW32 shell, download http://download.qt-project.org/official_releases/qt/4.8/4.8.6/qt-everywhere-opensource-src-4.8.6.tar.gz to the "C:\deps" folder:

```
cd /C/deps
wget http://download.qt-project.org/official_releases/qt/4.8/4.8.6/qt-everywhere-opensource-src-4.8.6.tar.gz --no-check-certificate
```

2. From the MINGW32 shell, unpack the "qt-everywhere-opensource-src-4.8.6.tar.gz" source archive:

```
tar xzvf qt-everywhere-opensource-src-4.8.6.tar.gz
```

3. From the MINGW32 shell, for convenience, rename "C:\deps\qt-everywhere-opensource-src-4.8.6" to "C:\deps\qt-4.8.6":

```
mv qt-everywhere-opensource-src-4.8.6 qt-4.8.6
```

4. From a DOS shell (run as Aministrator), configure Qt:

```
cd \deps\qt-4.8.6
configure.exe -release -opensource -confirm-license -static -no-sql-sqlite -no-qt3support -no-opengl -qt-zlib -no-gif -qt-libpng -qt-libmng -no-libtiff -qt-libjpeg -no-dsp -no-vcproj -no-openssl -no-dbus -no-phonon -no-phonon-backend -no-multimedia -no-audio-backend -no-webkit -no-script -no-scripttools -no-declarative -no-declarative-debug -qt-style-windows -qt-style-windowsxp -qt-style-windowsvista -no-style-plastique -no-style-cleanlooks -no-style-motif -no-style-cde -nomake demos -nomake examples -fast -platform win32-g++
```

5. From the DOS shell, build Qt:

```
mingw32-make
```

6. Add the path to qmake.exe (C:\deps\qt-4.8.6\bin) to the end of the PATH environment variable.  On Windows 7, the path should end up looking something like:

```	
C:\mingw32\bin;C:\MinGW\msys\1.0;%SystemRoot%\system32;%SystemRoot%;%SystemRoot%\System32\Wbem;%SYSTEMROOT%\System32\WindowsPowerShell\v1.0\;C:\Program Files\Git\cmd;C:\Program Files (x86)\NSIS;C:\deps\qt-4.8.6\bin
```

7. Close and reopen any DOS and MINGW32 shells in order to pick up the change to the PATH environment variable.


3 - Build Maxcoind
==================
1. From a MINGW32 shell, download Maxcoin source code:

```
cd /C
git clone https://github.com/Max-Coin/maxcoin.git
```

2. From the MINGW32 shell, build leveldb:

```
cd /C/maxcoin/src
make -f makefile.mingw leveldb/libleveldb.a
```

3. From the MINGW32 shell, build cryptopp:

```
make -f makefile.mingw cryptopp/libcryptopp.a
```

4. From the MINGW32 shell, create a "obj" directory:

```
mkdir obj
```

5.  From the MINGW32 shell, build maxcoind:

```
cd /C/maxcoin/src
make -f makefile.mingw
strip maxcoind.exe
```

4 - Build Maxcoin-qt
====================
1. From a MINGW32 shell, configure Qt make file:

```
cd /C/maxcoin
../deps/qt-4.8.6/bin/qmake "BOOST_LIB_SUFFIX=-mgw49-mt-s-1_54" "BOOST_INCLUDE_PATH=C:/deps/boost_1_54_0" "BOOST_LIB_PATH=C:/deps/boost_1_54_0/stage/lib" "BDB_INCLUDE_PATH=C:/deps/db-5.1.29.NC/build_unix" "BDB_LIB_PATH=C:/deps/db-5.1.29.NC/build_unix" "OPENSSL_INCLUDE_PATH=C:/deps/openssl-1.0.1f/include" "OPENSSL_LIB_PATH=C:/deps/openssl-1.0.1f" "MINIUPNPC_LIB_SUFFIX=-miniupnpc" "MINIUPNPC_INCLUDE_PATH=C:/deps/miniupnpc-1.6" "MINIUPNPC_LIB_PATH=C:/deps/miniupnpc-1.6" "QRENCODE_INCLUDE_PATH=C:/deps/qrencode-3.4.2" "QRENCODE_LIB_PATH=C:/deps/qrencode-3.4.2/.libs" maxcoin-qt.pro
```
2. From the MINGW32 shell, build maxcoin-qt:

```
mingw32-make -f makefile.Release
```

3. "maxcoin-qt.exe" should be created in the "C:\maxcoin\release" folder.


5 - Build Maxcoin Windows Installer
===================================
1. From a DOS shell (run as Administrator), run NSIS against maxcoin.msi:

```
makensis C:\maxcoin\contrib\installer\maxcoin.nsi
```

2. "maxcoin-installer.exe" should be created in the "C:\maxcoin\contrib\installer" folder.


6 - Celebrate!
==============
After completing steps 1 - 5, C:\maxcoin\src\maxcoind.exe, C:\maxcoin\release\maxcoin-qt.exe, and C:\maxcoin\contrib\installer\maxcoin-installer.exe should be built and ready for use.  That's it!
