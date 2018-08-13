![picture](img/logo.png)

What is MaxCoin?
==============

Maxcoin is an alternative cryptocurrency introduced in 2014.

Technical Information

+ ~100,000,000 coins
+ 8 coins rewarded per block, halving every 4 years - last halving 16 July 2017
+ 60 second block times
+ difficulty retargeting using a time-warp resistant implementation of KGW

Notable differences from Bitcoin
-----------------------------

+ sha3 Keccak encryption instead of sha256
+ replacement of ECDSA with Schnorr signing (Schnorr Signatures)
+ use of secp256r1 curve over secp256k1
+ requirement for public key when verifying transactions
+ capable of hosting smart contracts
+ no premine

Modifications to the RPC API
+ verifymessage <publickey> <signature> <message>
+ makekeypair [hex-encoded prefix]
+ dumppubkey <maxcoinaddress>

Additional technical details can be found on Everipedia (https://everipedia.org/wiki/maxcoin-cryptocurrency/) or in the [Wiki](https://github.com/Max-Coin/maxcoin/wiki/_pages).

Forked from Bitcoin reference wallet 0.8.5 and Blakecoin

License
------

MaxCoin is released under the terms of the MIT license. See `COPYING` for more
information or see http://opensource.org/licenses/MIT.
