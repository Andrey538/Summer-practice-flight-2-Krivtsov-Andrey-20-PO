#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include "rsa_keycreator.h"
#include <numeric>
#include <algorithm>

ll gcd_ex(ll a, ll b, ll& x, ll& y) {           //Расширенный алгоритм Евклида
    if (a == 0) {
        x = 0; y = 1;
        return b;
    }
    ll x1, y1;
    ll d = gcd_ex(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return d;
}


ll gcd(ll a, ll b) {                        //Алгоритм Евклида
    while (b) {
        a %= b;
        std::swap(a, b);
    }
    return a;
}


RSA_KeyCreator::RSA_KeyCreator()
{
    RSA_KeyCreator::SieveOfEratosthenes();
}

RSA_KeyCreator::RSA_KeyCreator(ll MaxNumber)
{
    RSA_KeyCreator::SieveOfEratosthenes(MaxNumber);
}

RSA_Keys RSA_KeyCreator::CreateKeys()
{
    srand(time(NULL));
    ll p = 0, q = 0, euler = 0;
    ll e = 0, n = 0, d = 0;

    p = primes[rand() % (primes.size() - 1)];
    q = primes[rand() % (primes.size() - 1)];
    n = p * q;
    euler = EulerFunction(p, q);
    do {
        e = 1 + rand() % (euler - 1);
    } while (gcd(euler, e) != 1);
    ll k;
    gcd_ex(e, euler, d, k);
    d = (d % euler + euler) % euler;

    std::cout << (e * d % euler) << std::endl;

    return RSA_Keys(n, e, d);
}

void RSA_KeyCreator::SieveOfEratosthenes(ll MaxNumber)
{
    std::vector<bool> sieve(MaxNumber + 1, true);
    sieve[0] = sieve[1] = false;
    for (ll i = 2; i * i <= MaxNumber; i++)
        if (sieve[i]) {
            primes.push_back(i);
            if (i * i <= MaxNumber)
                for (ll j = i * i; j <= MaxNumber; j += i)
                    sieve[j] = false;
        }
}

ll RSA_KeyCreator::EulerFunction(ll p, ll q)
{
    return (p - 1) * (q - 1);
}

std::ostream& operator<<(std::ostream& stream, RSA_Keys& keys)
{
    RSA_Key& publicKey = keys.publicKey;
    RSA_Key& privateKey = keys.privateKey;

    std::cout << "Public Key: n = " << publicKey.get_n() << ", e = " << publicKey.get_a() << std::endl;
    std::cout << "Private Key: n = " << privateKey.get_n() << ", d = " << privateKey.get_a() << std::endl;
    return stream;
}

