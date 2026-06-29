#include "crypto.h"
#include <cstring>
#include <sodium.h>
#include <stdexcept>

std::vector<unsigned char>
Crypto::encrypt(const std::string &plaintext,
                const std::string &key) { // must match header
  if (key.size() != crypto_aead_xchacha20poly1305_ietf_KEYBYTES)
    throw std::runtime_error("Key must be exactly 32 bytes");

  unsigned char nonce[crypto_aead_xchacha20poly1305_ietf_NPUBBYTES];
  randombytes_buf(nonce, sizeof(nonce));

  unsigned long long cipher_len;
  std::vector<unsigned char> encrypted(
      crypto_aead_xchacha20poly1305_ietf_NPUBBYTES + plaintext.size() +
      crypto_aead_xchacha20poly1305_ietf_ABYTES);

  memcpy(encrypted.data(), nonce, crypto_aead_xchacha20poly1305_ietf_NPUBBYTES);

  crypto_aead_xchacha20poly1305_ietf_encrypt(
      encrypted.data() + crypto_aead_xchacha20poly1305_ietf_NPUBBYTES,
      &cipher_len, reinterpret_cast<const unsigned char *>(plaintext.data()),
      plaintext.size(), nullptr, 0, nullptr, nonce,
      reinterpret_cast<const unsigned char *>(key.data()));

  encrypted.resize(crypto_aead_xchacha20poly1305_ietf_NPUBBYTES + cipher_len);
  return encrypted;
}

std::string Crypto::decrypt(const std::vector<unsigned char> &encrypted,
                            const std::string &key) { // must match header
  if (key.size() != crypto_aead_xchacha20poly1305_ietf_KEYBYTES)
    throw std::runtime_error("Key must be exactly 32 bytes");

  if (encrypted.size() < crypto_aead_xchacha20poly1305_ietf_NPUBBYTES +
                             crypto_aead_xchacha20poly1305_ietf_ABYTES) {
    throw std::runtime_error("Ciphertext too small");
  }

  const unsigned char *nonce = encrypted.data();
  const unsigned char *ciphertext =
      encrypted.data() + crypto_aead_xchacha20poly1305_ietf_NPUBBYTES;

  unsigned long long plaintext_len;
  std::vector<unsigned char> plaintext(
      encrypted.size() - crypto_aead_xchacha20poly1305_ietf_NPUBBYTES -
      crypto_aead_xchacha20poly1305_ietf_ABYTES);

  if (crypto_aead_xchacha20poly1305_ietf_decrypt(
          plaintext.data(), &plaintext_len, nullptr, ciphertext,
          encrypted.size() - crypto_aead_xchacha20poly1305_ietf_NPUBBYTES,
          nullptr, 0, nonce,
          reinterpret_cast<const unsigned char *>(key.data())) != 0) {
    throw std::runtime_error("Decryption failed");
  }

  return std::string(plaintext.begin(), plaintext.begin() + plaintext_len);
}