#pragma once
#include <string>
#include "rsa_key.h"
#include "Base.h"
#include "File.h"

class RSA_Encrypt_Decrypt
{
public:
    RSA_Encrypt_Decrypt();

    ll Encrypt(ll data, RSA_Key publicKey);
    ll Decrypt(ll data, RSA_Key privateKey);
    const std::wstring& Encrypt(const wchar_t* data, ll size, RSA_Key publicKey);
    const std::wstring& Decrypt(const wchar_t* data, ll size, RSA_Key privateKey);
    const std::wstring& Encrypt(const std::wstring& data, RSA_Key publicKey);
    const std::wstring& Decrypt(const std::wstring& data, RSA_Key privateKey);
    void Encrypt(wchar_t* data, ll size, RSA_Key publicKey);
    void Decrypt(wchar_t* data, ll size, RSA_Key privateKey);

    void Encrypt(Andrew::File& data, RSA_Key publicKey);
    void Decrypt(Andrew::File& data, RSA_Key privateKey);
};