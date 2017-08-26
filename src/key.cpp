// Copyright (c) 2009-2012 The Bitcoin developers
// Copyright (c)      2017 The MaxCoin developers and Nigel Smart
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include <map>

#include "key.h"
#include "schnorr.h"

using namespace CryptoPP;

Integer CKey::HashPointMessage(const ECPPoint& R,
    const byte* message, int mlen)
{
    const int digestsize = 256/8;
    SHA3 sha(digestsize);

    int len = ec.EncodedPointSize();
    byte *buffer = new byte[len];
    ec.EncodePoint(buffer, R, fCompressedPubKey);
    sha.Update(buffer, len);
    delete[] buffer;

    sha.Update(message, mlen);

    byte digest[digestsize];
    sha.Final(digest);
    
    Integer ans;
    ans.Decode(digest, digestsize);
    return ans;
}

void CKey::SetCompressedPubKey(bool fCompressed)
{
    //fCompressedPubKey = fCompressed;
    fCompressedPubKey = false;
}

void CKey::LoadCurve()
{
    secretKeySet = false;
    publicKeySet = false;

    // Load in curve secp256r1
    Integer p, a, b, Gx, Gy;

    // Create the group
    p = Integer("0xFFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF");
    a = Integer("-3");
    b = Integer("0x5AC635D8AA3A93E7B3EBBD55769886BC651D06B0CC53B0F63BCE3C3E27D2604B");
    q = Integer("0xFFFFFFFF00000000FFFFFFFFFFFFFFFFBCE6FAADA7179E84F3B9CAC2FC632551");
    Gx = Integer("0x6B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0F4A13945D898C296");
    Gy = Integer("0x4FE342E2FE1A7F9B8EE7EB4A7C0F9E162BCE33576B315ECECBB6406837BF51F5");

    // Store the curve and the generator
    ec = ECP(p, a, b);
    G = ECPPoint(Gx, Gy);
}

void CKey::Reset()
{
    fCompressedPubKey = false;
    MakeNewKey(fCompressedPubKey);
    fSet = false;
}

CKey::CKey()
{
    LoadCurve();
    Reset();
}

CKey::CKey(const CKey& b)
{
    Q = b.Q;
    secretKey = b.secretKey;
    fSet = b.fSet;
}

CKey& CKey::operator=(const CKey& b)
{
    // copy keys
    Q = b.Q;
    secretKey = b.secretKey;

    fSet = b.fSet;
    return (*this);
}

CKey::~CKey()
{
    // Do nothing
}

bool CKey::IsNull() const
{
    return !fSet;
}

bool CKey::IsCompressed() const
{
    return fCompressedPubKey;
}

bool CKey::GenerateSecretKey()
{
    secretKey = Integer(rng, 256) % q;
    secretKeySet = true;
    return true;
}

bool CKey::GeneratePublicKey()
{
    if (!secretKeySet)
        return false;
    Q = ec.ScalarMultiply(G, secretKey);
    publicKeySet = true;
    return true;
}

// generate new public and secret keys
void CKey::MakeNewKey(bool fCompressed)
{
    
    if (!GenerateSecretKey())
        throw new key_error("CKey::MakeNewKey : could not generate secret key");
    
    if (!GeneratePublicKey())
        throw new key_error("CKey::MakeNewKey : could not generate public key");

    if (fCompressed)
        SetCompressedPubKey();
    
    fSet = true;
}

bool CKey::SetSecret(const CSecret& vchSecret, bool fCompressed)
{
    if (vchSecret.size() != SCHNORR_SECRET_KEY_SIZE)
        return false;
    secretKey.Decode(&vchSecret[0], SCHNORR_SECRET_KEY_SIZE);

    // generate a new public key
    GeneratePublicKey();

    fSet = true;
    if (fCompressed || fCompressedPubKey)
        SetCompressedPubKey();
    return true;
}

CSecret CKey::GetSecret(bool &fCompressed) const
{
    if (!secretKeySet)
        throw new key_error("CKey::GetSecret : secret key not set");

    CSecret vchSecret(SCHNORR_SECRET_KEY_SIZE);
    vchSecret.resize(SCHNORR_SECRET_KEY_SIZE);
    secretKey.Encode(&vchSecret[0], SCHNORR_SECRET_KEY_SIZE);

    fCompressed = fCompressedPubKey;
    return vchSecret;
}

#include "util.h"

bool CKey::SetPubKey(const CPubKey& pubKey)
{
    ECPPoint pubKeyPoint;
    if (!ec.DecodePoint(pubKeyPoint, &pubKey.vchPubKey[0], pubKey.vchPubKey.size()))
        return false;

    SetCompressedPubKey(pubKey.vchPubKey.size() == 33);
    Q = pubKeyPoint;
    fSet = true;
    return true;
}

CPubKey CKey::GetPubKey() const
{
    std::vector<unsigned char> vchPubKey;
    vchPubKey.resize(ec.EncodedPointSize(fCompressedPubKey));
    ec.EncodePoint(&vchPubKey[0], Q, fCompressedPubKey);
    return CPubKey(vchPubKey);
}

bool CKey::Sign(uint256 hash, std::vector<unsigned char>& vchSig)
{
    // sign the hash
    Integer k;
    ECPPoint R;
    Integer sigE, sigS;

    k = Integer(rng, 256) % q;
    R = ec.ScalarMultiply(G, k);

    // encode hash as byte[]
    std::vector<unsigned char> vchHash = hash.toVch();

    sigE = HashPointMessage(R, &vchHash[0], sizeof(hash)) % q;
    sigS = (k - secretKey*sigE) % q;

    // encode the vch format
    vchSig.resize(SCHNORR_SIG_SIZE * 2);
    if (sigE.MinEncodedSize() > SCHNORR_SIG_SIZE || sigS.MinEncodedSize() > SCHNORR_SIG_SIZE)
        return false;

    sigE.Encode(&vchSig[0], SCHNORR_SIG_SIZE);
    sigS.Encode(&vchSig[SCHNORR_SIG_SIZE], SCHNORR_SIG_SIZE);
    return true;
}

bool CKey::Verify(uint256 hash, const std::vector<unsigned char>& vchSig)
{
    // decode the vchSig
    Integer sigE, sigS;
    if (vchSig.size() != (SCHNORR_SIG_SIZE * 2))
        return false;

    // extract bytes
    std::vector<unsigned char> sigEVec(&vchSig[0], &vchSig[SCHNORR_SIG_SIZE]);
    std::vector<unsigned char> sigSVec(&vchSig[SCHNORR_SIG_SIZE], &vchSig[1 + SCHNORR_SIG_SIZE * 2]);

    // vectors -> Integers
    sigE.Decode(&sigEVec[0], SCHNORR_SIG_SIZE);
    sigS.Decode(&sigSVec[0], SCHNORR_SIG_SIZE);

    // verify the hash
    ECPPoint R;
    R = ec.CascadeScalarMultiply(G, sigS, Q, sigE);

    // encode hash as byte[]
    std::vector<unsigned char> vchHash = hash.toVch();
    Integer sigEd = HashPointMessage(R, &vchHash[0], sizeof(hash)) % q;

    return (sigE == sigEd);
}

bool CKey::IsValid()
{
    if (!fSet)
        return false;

    bool fCompr;
    CSecret secret = GetSecret(fCompr);
    CKey key2;
    key2.SetSecret(secret, fCompr);
    return GetPubKey() == key2.GetPubKey();
}
