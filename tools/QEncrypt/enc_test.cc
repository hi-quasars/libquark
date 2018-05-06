#include "cryptlib.h"
#include "secblock.h"
#include "filters.h"
#include "chacha.h"
#include "osrng.h"
#include "files.h"
#include "hex.h"

#include <iostream>
#include <string>

int main()
{
    using CryptoPP::byte;
    using CryptoPP::ChaCha20;
    using CryptoPP::FileSink;
    using CryptoPP::StringSink;
    using CryptoPP::StringSource;
    using CryptoPP::HexEncoder;
    using CryptoPP::SecByteBlock;
    using CryptoPP::AutoSeededRandomPool;
    using CryptoPP::StreamTransformationFilter;

    AutoSeededRandomPool prng;
    HexEncoder encoder(new FileSink(std::cout));
    std::string plain("My Plaintext!! My Dear plaintext!!"), cipher, recover;

    SecByteBlock key(32), iv(8);
    prng.GenerateBlock(key, key.size());
    prng.GenerateBlock(iv, iv.size());

    std::cout << "Key: ";
    encoder.Put(key.data(), key.size());
    encoder.MessageEnd();
    std::cout << std::endl;

    std::cout << "IV: ";
    encoder.Put(iv.data(), iv.size());
    encoder.MessageEnd();
    std::cout << std::endl;

    // Encryption object
    ChaCha20::Encryption enc;    
    enc.SetKeyWithIV(key, key.size(), iv, iv.size());

    // Decryption object
    ChaCha20::Decryption dec;    
    dec.SetKeyWithIV(key, key.size(), iv, iv.size());

    StringSource ss1(plain, true, new StreamTransformationFilter(enc, new StringSink(cipher)));
    StringSource ss2(cipher, true, new StreamTransformationFilter(dec, new StringSink(recover)));

    std::cout << "Plain: " << plain << std::endl;

    std::cout << "Cipher: ";
    encoder.Put((const byte*)cipher.data(), cipher.size());
    encoder.MessageEnd();
    std::cout << std::endl;

    std::cout << "Recovered: " << recover << std::endl;

    return 0;
}
