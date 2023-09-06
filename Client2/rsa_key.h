#pragma once
#include <string>
#include "Base.h"

class RSA_Key
{
public:
    RSA_Key();
    RSA_Key(ll a, ll n);
    ll get_a() const { return a; }
    ll get_n() const { return n; }
private:
    ll a;
    ll n;
};