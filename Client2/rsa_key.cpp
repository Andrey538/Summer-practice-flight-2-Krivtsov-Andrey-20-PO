#include "RSA_Key.h"

RSA_Key::RSA_Key()
{
    a = n = 0;
}

RSA_Key::RSA_Key(ll a, ll n)
    : a(a), n(n)
{
}
