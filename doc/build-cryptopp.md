Building Crypto++ on Linux
==========================

Install the dependencies

    $ sudo apt-get install unzip

Set up the development folder by downloading the Crypto++ 5.6.2 source
  
    $ mkdir cryptopp
    $ cd cryptopp
    $ wget http://www.cryptopp.com/cryptopp562.zip
    $ unzip -a cryptopp562.zip

Uncomment the line `CXXFLAGS += -fPIC`, by removing the preceeding `#`, inside GNUmakefile and build the library

    $ make static dynamic test

Install it on your system:
  
    $ sudo make install

Troubleshooting:
----------------

If you have issues with an "Illegal instruction" error, as we experienced on one test machine, modify GNUmakefile and set the line `CXXFLAGS += -march=native` to `CXXFLAGS += `
