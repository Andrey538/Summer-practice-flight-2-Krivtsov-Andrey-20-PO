#include "rsa_encrypt_decrypt.h"

ll binpowmod(ll a, ll e, ll m)
{
    ll r = 1;
    while (e != 0) {
        if (e & 1) r = r * a % m;
        a = a * a % m;
        e >>= 1;
    }
    return r;
}

RSA_Encrypt_Decrypt::RSA_Encrypt_Decrypt()
{
}

ll RSA_Encrypt_Decrypt::Encrypt(ll data, RSA_Key publicKey)
{
    return binpowmod(data, publicKey.get_a(), publicKey.get_n());
}

ll RSA_Encrypt_Decrypt::Decrypt(ll data, RSA_Key privateKey)
{
    return binpowmod(data, privateKey.get_a(), privateKey.get_n());
}

const std::wstring& RSA_Encrypt_Decrypt::Encrypt(const wchar_t* data, ll size, RSA_Key publicKey)
{
    std::wstring encryptedText = L"";
    for (size_t i = 0; i < size; i++)
    {
        wchar_t code = data[i];
        ll encryptedCode = Encrypt(static_cast<ll>(code), publicKey);
        encryptedText += static_cast<wchar_t>(encryptedCode);
    }
    return encryptedText;

}

const std::wstring& RSA_Encrypt_Decrypt::Decrypt(const wchar_t* data, ll size, RSA_Key privateKey)
{
    std::wstring decryptedText = L"";
    for (size_t i = 0; i < size; i++)
    {
        wchar_t code = data[i];
        ll decryptedCode = Decrypt(static_cast<ll>(code), privateKey);
        decryptedText += static_cast<wchar_t>(decryptedCode);
    }
    return decryptedText;
}

void RSA_Encrypt_Decrypt::Encrypt(wchar_t* data, ll size, RSA_Key publicKey) {
    bool skip = true;
    for (ll i = size - 1; i >= 0; i--)
    {
        if (skip && data[i] == -51) continue;
        else skip = false;
        wchar_t code = data[i];
        ll encryptedCode = Encrypt(static_cast<ll>(code), publicKey);
        data[i] = static_cast<wchar_t>(encryptedCode);
    }
}


void RSA_Encrypt_Decrypt::Decrypt(wchar_t* data, ll size, RSA_Key privateKey) {
    bool skip = true;
    for (ll i = size - 1; i >= 0; i--)
    {
        if (skip && data[i] == -51) continue;
        else skip = false;
        wchar_t code = data[i];
        ll encryptedCode = Decrypt(static_cast<ll>(code), privateKey);
        data[i] = static_cast<wchar_t>(encryptedCode);
    }
}


const std::wstring& RSA_Encrypt_Decrypt::Encrypt(const std::wstring& data, RSA_Key publicKey)
{
    return Encrypt(data.c_str(), data.size(), publicKey);
}

const std::wstring& RSA_Encrypt_Decrypt::Decrypt(const std::wstring& data, RSA_Key privateKey)
{
    return Decrypt(data.c_str(), data.size(), privateKey);
}

void RSA_Encrypt_Decrypt::Encrypt(Andrew::File& file, RSA_Key publicKey) {
    Encrypt(file.data, file.getSize(), publicKey);
}

void RSA_Encrypt_Decrypt::Decrypt(Andrew::File& file, RSA_Key privateKey) {
    Decrypt(file.data, file.getSize(), privateKey);
}