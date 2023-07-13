#ifndef _vec_h_
#define _vec_h_

#include <cmath>

typedef double dbl;

class vec
{
public:
    dbl X, Y, Z;
    vec() = default;
    vec(dbl x, dbl y, dbl z) : X(x), Y(y), Z(z)
    {
    };
    vec operator+(const vec& A) const
    {
        return {X + A.X, Y + A.Y, Z + A.Z};
    };
    vec operator-(const vec& A) const
    {
        return {X - A.X, Y - A.Y, Z - A.Z};
    };
    vec operator*(dbl N) const
    {
        return {X * N, Y * N, Z * N};
    };
    vec operator/(dbl N) const
    {
        return {X / N, Y / N, Z / N};
    };
    vec& operator+=(const vec& A)
    {
        X += A.X;
        Y += A.Y;
        Z += A.Z;

        return *this;
    };
    vec& operator-=(const vec& A)
    {
        X -= A.X;
        Y -= A.Y;
        Z -= A.Z;

        return *this;
    };
    dbl operator&(const vec& A) const
    {
        return (X * A.X + Y * A.Y + Z * A.Z);
    };
    vec operator%(const vec& A) const
    {
        return {Y * A.Z - Z * A.Y, Z * A.X - X * A.Z, X * A.Y - Y * A.X};
    };

    dbl& operator[](int i)
    {
        return *(&X + i);
    };
    static dbl r1()
    {
        return 2.0 * rand() / RAND_MAX - 1;
    };
    static vec rnd1()
    {
        return {r1(), r1(), r1()};
    };
    [[nodiscard]] vec Normalize() const
    {
        return *this / sqrt(*this & *this);
    };
    vec operator|(const vec& A) const
    {
        return {X * A.X, Y * A.Y, Z * A.Z};
    };
};


#endif //_vec_h_