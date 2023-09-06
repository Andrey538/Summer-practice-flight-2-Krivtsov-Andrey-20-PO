#pragma once
#include <vector>
#include "Base.h"
#include "rsa_key.h"

struct RSA_Keys
{
    RSA_Keys(ll n = 0, ll e = 0, ll d = 0)
    {
        publicKey = RSA_Key(e, n);
        privateKey = RSA_Key(d, n);
    }
    RSA_Key publicKey;
    RSA_Key privateKey;
};

std::ostream& operator<<(std::ostream& stream, RSA_Keys& keys);

class RSA_KeyCreator
{
public:
    RSA_KeyCreator();
    RSA_KeyCreator(ll MaxNumber);
    RSA_Keys CreateKeys();
private:
    std::vector<ll> primes;
    void SieveOfEratosthenes(ll Length = 100); //Решето эратосфена
    ll EulerFunction(ll p, ll q);//Функция Эйлера
};
