#pragma once

#include "MathDefines.hpp"

#include "Containers\String.hpp"
#include <math.h>

#ifdef abs
#	undef abs
#endif
#ifdef min
#	undef min
#endif
#ifdef max
#	undef max
#endif
#ifdef round
#	undef round
#endif
#ifdef far
#	undef far
#endif
#ifdef near
#	undef near
#endif

struct Vector2;
struct Vector3;
struct Vector4;
struct Vector2Int;
struct Vector3Int;
struct Vector4Int;

struct Matrix2;
struct Matrix3;
struct Matrix4;

struct Quaternion2;
struct Quaternion3;

template <class Type> concept VectorType = AnyOf<RemovedQuals<Type>, Vector2, Vector3, Vector4, Vector2Int, Vector3Int, Vector4Int>;
template <class Type> concept MatrixType = AnyOf<RemovedQuals<Type>, Matrix2, Matrix3, Matrix4>;
template <class Type> concept QuaternionType = AnyOf<RemovedQuals<Type>, Quaternion2, Quaternion3>;

class NH_API Math
{
public:
	template <typename Type> static constexpr Type Abs(const Type& n) noexcept { return n < (Type)0 ? -n : n; }
	template <typename Type> static constexpr Type Min(const Type& a, const Type& b) noexcept { return a > b ? b : a; }
	template <typename Type> static constexpr Type Max(const Type& a, const Type& b) noexcept { return a < b ? b : a; }
	template <typename Type> static constexpr Type Clamp(const Type& n, const Type& min, const Type& max) noexcept { return n < min ? min : n > max ? max : n; }
	template <typename Type> static constexpr Type Sign(const Type& n) noexcept { return (Type)((n > (Type)0) - (n < (Type)0)); }
	template <typename Type> static constexpr Type NonZeroSign(const Type& n) noexcept { return (Type)(2 * (n > (Type)0) - (Type)1); }
	template <FloatingPoint Type> static constexpr I64 Floor(const Type& n) noexcept { return n >= (Type)0 ? (I64)n : (I64)n - 1; }
	template <FloatingPoint Type> static constexpr Type FloorF(const Type& n) noexcept { return n > Traits<Type>::MaxPrecision ? n : n >= (Type)0 ? (Type)(I64)n : (Type)(I64)n - (Type)1; }
	template <FloatingPoint Type> static constexpr I64 Ceiling(const Type& n) noexcept { return (n - (I64)n) < (Type)0 ? (I64)n : (I64)n + 1; }
	template <FloatingPoint Type> static constexpr Type CeilingF(const Type& n) noexcept { return n > Traits<Type>::MaxPrecision ? n : (n - (I64)n) < (Type)0 ? (Type)(I64)n : (Type)(I64)n + (Type)1; }
	template <FloatingPoint Type> static constexpr Type Round(const Type& n) noexcept { return (Type)(I64)(n + 0.5); }
	template <FloatingPoint Type> static constexpr Type RoundI(const Type& n) noexcept { return (I64)(n + 0.5); }
	template <FloatingPoint Type> static constexpr Type Mod(const Type& n, const Type& d) noexcept { return n - d * FloorF(n / d); }
	template <FloatingPoint Type> static constexpr Type Closest(Type n, Type a, Type b) noexcept { return n < (b + a) * 0.5f ? a : b; }
	template <Integer Type> static constexpr Type Closest(Type n, Type a, Type b) noexcept { return n < (b + a) >> 1 ? a : b; }

	template <FloatingPoint Type> static constexpr Type DegToRad(Type deg) noexcept { return deg * DEG_TO_RAD_T<Type>; }
	template <FloatingPoint Type> static constexpr Type RadToDeg(Type rad) noexcept { return rad * RAD_TO_DEG_T<Type>; }
	template <FloatingPoint Type> static constexpr Type NormalizeAngle(Type f) noexcept { return (Type)(f - (TWO_PI * FloorF((f + PI) / TWO_PI))); }

	template <FloatingPoint Type> static constexpr bool IsZero(Type f) noexcept { return f < Traits<Type>::Epsilon && f > -Traits<Type>::Epsilon; }
	template <FloatingPoint Type> static constexpr bool IsNaN(Type f) noexcept { return f != f; }
	template <FloatingPoint Type> static constexpr bool IsInf(Type f) noexcept { return f == Traits<Type>::Infinity; }
	template <FloatingPoint Type> static constexpr bool IsNegInf(Type f) noexcept { return f == -Traits<Type>::Infinity; }

private:
	template<FloatingPoint Type> static constexpr I64 FindWhole(const Type f) noexcept
	{
		if (Abs(f - FloorF(f)) >= (Type)0.5) { return (I64)(FloorF(f) + Sign(f)); }
		else { return (I64)FloorF(f); }
	}

	template<FloatingPoint Type> static constexpr I64 FindFraction(const Type f) noexcept
	{
		if (Abs(f - FloorF(f)) >= (Type)0.5) { return (I64)(f - FloorF(f) - Sign(f)); }
		else { return (I64)(f - FloorF(f)); }
	}

	template<FloatingPoint Type> static constexpr I64 FindExponent(const Type f, const I64 exponent) noexcept
	{
		if (f < 1e-03) { return FindExponent(f * (Type)1e+04, exponent - 4); }
		else if (f < 1e-01) { return FindExponent(f * (Type)1e+02, exponent - 2); }
		else if (f < 1.0) { return FindExponent(f * (Type)10.0, exponent - 1); }
		else if (f > 10.0) { return FindExponent(f / (Type)10.0, exponent + 1); }
		else if (f > 1e+02) { return FindExponent(f / (Type)1e+02, exponent + 2); }
		else if (f > 1e+04) { return FindExponent(f / (Type)1e+04, exponent + 4); }
		else { return exponent; }
	}

	template<FloatingPoint Type> static constexpr Type FindMantissa(const Type f) noexcept
	{
		if (f < 1.0) { return FindMantissa(f * (Type)10.0); }
		else if (f > 10.0) { return FindMantissa(f / (Type)10.0); }
		else { return f; }
	}

	template<FloatingPoint Type> static constexpr Type LogMantissa(const I32 n) noexcept
	{
		switch (n)
		{
		case 2: return (Type)0.6931471805599453;
		case 3: return (Type)1.0986122886681097;
		case 4: return (Type)1.3862943611198906;
		case 5: return (Type)1.6094379124341004;
		case 6: return (Type)1.7917594692280550;
		case 7: return (Type)1.9459101490553133;
		case 8: return (Type)2.0794415416798359;
		case 9: return (Type)2.1972245773362194;
		case 10: return (Type)2.3025850929940457;
		default: return (Type)0.0;
		}
	}

	template<FloatingPoint Type> static constexpr Type LogMain(const Type f) noexcept
	{
		Type f1 = (f - (Type)1.0) / (f + (Type)1.0);

		Type f2 = f1 * f1;


		I32 depth = 24;
		Type res = (Type)(2 * (depth + 1) - 1);

		while (depth > 0)
		{
			res = (Type)(2 * depth - 1) - (Type)(depth * depth) * f2 / res;

			--depth;
		}

		return (Type)2.0 * f1 / res;
	}

	template<FloatingPoint Type> static constexpr Type ExpRecur(const Type x, const I32 depth) noexcept
	{
		if (depth < 25) { return (Type)1.0 + x / (Type)(depth - 1) - x / depth / ExpRecur(x, depth + 1); }
		else { return (Type)1.0; }
	}

	template <Number Base, Signed Exp> static constexpr Base PowBySquaring(const Base b, const Base val, const Exp e) noexcept
	{
		if (b < 0) { return PowBySquaring(b, (Base)1 / b, -e); }
		else if (e > 1)
		{
			if (e & 1) { return PowBySquaring(b * b, val * b, e / 2); }
			else { return PowBySquaring(b * b, val, e / 2); }
		}
		else if (e == 1) { return val * b; }
		else { return val; }
	}

public:
	template <FloatingPoint Type> static constexpr Type Sin(Type f) noexcept
	{
		if (ConstantEvaluation())
		{
			f = NormalizeAngle(f);

			const Type f2 = f * f;
			Type pow = f;

			Type result = pow;
			pow *= f2;
			result += (Type)-0.16666667 * pow;
			pow *= f2;
			result += (Type)0.0083333333 * pow;
			pow *= f2;
			result += (Type)-0.0001984127 * pow;
			pow *= f2;
			result += (Type)2.7557319e-6 * pow;
			pow *= f2;
			result += (Type)-2.5052108e-8 * pow;
			pow *= f2;
			result += (Type)1.6059044e-10 * pow;
			pow *= f2;
			result += (Type)-7.6471637e-13 * pow;
			pow *= f2;
			result += (Type)2.8114573e-15 * pow;
			pow *= f2;
			result += (Type)-8.2206352e-18 * pow;
			pow *= f2;

			return result;
		}
		else
		{
			if constexpr (IsSame<Type, F32>) { return sinf(f); }
			else { return sin(f); }
		}
	}
	template <FloatingPoint Type> static constexpr Type Cos(Type f) noexcept
	{
		if (ConstantEvaluation())
		{
			f = NormalizeAngle(f);

			const Type f2 = f * f;
			Type pow = 1;

			Type result = 1;
			pow *= f2;
			result += (Type)-0.5 * pow;
			pow *= f2;
			result += (Type)0.0416666667 * pow;
			pow *= f2;
			result += (Type)-0.0013888889 * pow;
			pow *= f2;
			result += (Type)2.48015873e-5 * pow;
			pow *= f2;
			result += (Type)-2.75573192e-7 * pow;
			pow *= f2;
			result += (Type)2.0876757e-9 * pow;
			pow *= f2;
			result += (Type)-1.14707456e-11 * pow;
			pow *= f2;
			result += (Type)4.77947733e-14 * pow;
			pow *= f2;
			result += (Type)-1.5619207e-16 * pow;
			pow *= f2;

			return result;
		}
		else
		{
			if constexpr (IsSame<Type, F32>) { return cosf(f); }
			else { return cos(f); }
		}
	}
	template <FloatingPoint Type> static constexpr Type Tan(Type f) noexcept
	{
		if (ConstantEvaluation())
		{
			return Sin(f) / Cos(f);
		}
		else
		{
			if constexpr (IsSame<Type, F32>) { return tanf(f); }
			else { return tan(f); }
		}
	}

	template <FloatingPoint Type> static Type SinH(Type f) noexcept { if constexpr (IsSame<Type, F32>) { return sinfh(f); } else { return sinh(f); } }
	template <FloatingPoint Type> static Type CosH(Type f) noexcept { if constexpr (IsSame<Type, F32>) { return cosfh(f); } else { return cosh(f); } }
	template <FloatingPoint Type> static Type TanH(Type f) noexcept { if constexpr (IsSame<Type, F32>) { return tanfh(f); } else { return tanh(f); } }

	template <FloatingPoint Type> static Type Asin(Type f) noexcept { if constexpr (IsSame<Type, F32>) { return asinf(f); } else { return asin(f); } }
	template <FloatingPoint Type> static Type Acos(Type f) noexcept { if constexpr (IsSame<Type, F32>) { return acosf(f); } else { return acos(f); } }
	template <FloatingPoint Type> static Type Atan(Type f) noexcept { if constexpr (IsSame<Type, F32>) { return atanf(f); } else { return atan(f); } }
	template <FloatingPoint Type> static Type AsinH(Type f) noexcept { if constexpr (IsSame<Type, F32>) { return asinfh(f); } else { return asinh(f); } }
	template <FloatingPoint Type> static Type AcosH(Type f) noexcept { if constexpr (IsSame<Type, F32>) { return acosfh(f); } else { return acosh(f); } }
	template <FloatingPoint Type> static Type AtanH(Type f) noexcept { if constexpr (IsSame<Type, F32>) { return atanfh(f); } else { return atanh(f); } }
	template <FloatingPoint Type> static Type Atan2(Type x, const Type y) noexcept { if constexpr (IsSame<Type, F32>) { return atan2f(x, y); } else { return atan2(x, y); } }

	template <FloatingPoint Type> static constexpr Type Sqrt(Type f) noexcept
	{
		if (ConstantEvaluation())
		{
			if (f < 0.0) { return Traits<Type>::NaN; }

			Type result = f;
			result = (Type)0.5 * (result + f / result);
			result = (Type)0.5 * (result + f / result);
			result = (Type)0.5 * (result + f / result);
			result = (Type)0.5 * (result + f / result);
			result = (Type)0.5 * (result + f / result);
			result = (Type)0.5 * (result + f / result);
			result = (Type)0.5 * (result + f / result);
			result = (Type)0.5 * (result + f / result);
			result = (Type)0.5 * (result + f / result);
			result = (Type)0.5 * (result + f / result);

			return result;
		}
		else
		{
			if constexpr (IsSame<Type, F32>) { return sqrtf(f); }
			else { return sqrt(f); }
		}
	}
	template <FloatingPoint Type> static Type Cbrt(Type f) noexcept { if constexpr (IsSame<Type, F32>) { return cbrtf(f); } else { return cbrt(f); } }
	template <FloatingPoint Type> static constexpr Type InvSqrt(Type f) noexcept { return (Type)1.0 / Sqrt(f); }
	template <FloatingPoint Type> static constexpr Type LogE(Type f) noexcept
	{
		if (ConstantEvaluation())
		{
			if (IsNaN(f)) { return Traits<Type>::NaN; }
			else if (f < 0) { return Traits<Type>::NaN; }
			else if (f < Traits<Type>::Epsilon) { return -Traits<Type>::Infinity; }
			else if (Abs(f - (Type)1.0) < Traits<Type>::Epsilon) { return (Type)0.0; }
			else if (f == Traits<Type>::Infinity) { return Traits<Type>::Infinity; }
			else
			{
				if (f < (Type)0.5 || f > (Type)1.5)
				{
					Type mantissa = FindMantissa(f);
					Type logMantissa = LogMain(mantissa / FloorF(mantissa)) + LogMantissa<Type>((I32)mantissa);

					return logMantissa + LOG_TEN_T<Type> * FindExponent(f, 0);
				}
				else
				{
					return LogMain(f);
				}
			}
		}
		else
		{
			if constexpr (IsSame<Type, F32>) { return logf(f); }
			else { return log(f); }
		}
	}
	template <FloatingPoint Type> static Type Log2(Type f) noexcept { if constexpr (IsSame<Type, F32>) { return log2f(f); } else { return log2(f); } }
	template <FloatingPoint Type> static Type Log10(Type f) noexcept { if constexpr (IsSame<Type, F32>) { return log10f(f); } else { return log10(f); } }
	template <FloatingPoint Type> static Type LogN(Type f, const Type b) noexcept { if constexpr (IsSame<Type, F32>) { return log2f(f) / log2f(b); } else { return log2(f) / log2(b); } }
	template <FloatingPoint Type> static constexpr Type Exp(Type f) noexcept
	{
		if (ConstantEvaluation())
		{
			if (IsNaN(f)) { return Traits<Type>::NaN; }
			else if (IsNegInf(f)) { return (Type)0; }
			else if (Abs(f) < Traits<Type>::Epsilon) { return 1; }
			else if (IsInf(f)) { return Traits<Type>::Infinity; }
			else if (Abs(f) < 2.0) { return (Type)1.0 / ((Type)1.0 - f / ExpRecur(f, 2)); }
			else { I64 frac = FindFraction(f); return Pow(E_T<Type>, FindWhole(f)) * ((Type)1.0 / ((Type)1.0 - frac / ExpRecur((Type)frac, 2))); }
		}
		else
		{
			if constexpr (IsSame<Type, F32>) { return expf(f); }
			else { return exp(f); }
		}
	}
	template <Number Base, Integer Exp> static constexpr Base Pow(Base b, Exp e) noexcept
	{
		if (ConstantEvaluation())
		{
			if (e == 3) { return b * b * b; }
			else if (e == 2) { return b * b; }
			else if (e == 1) { return b; }
			else if (e == 0) { return 1; }
			else if (e == Traits<Base>::MinValue) { return 0; }
			else if (e == Traits<Base>::MaxValue) { return Traits<Base>::Infinity; }
			else { return PowBySquaring(b, (Base)1, e); }
		}
		else
		{
			if constexpr (IsInteger<Base>) { return (Base)pow((F64)b, (F64)e); }
			else if constexpr (IsSame<Base, F32>) { return powf(b, (F32)e); }
			else { return pow(b, (F64)e); }
		}
	}
	template <Number Base, FloatingPoint Ex> static constexpr Ex Pow(Base b, Ex e) noexcept
	{
		if (ConstantEvaluation())
		{
			if (b < 0.0) { return Traits<Ex>::NaN; }
			else { return Exp(e * LogE(b)); }
		}
		else
		{
			if constexpr (IsSame<Ex, F32>) { return powf((F32)b, e); }
			else { return pow((F64)b, e); }
		}
	}

	//INTERPOLATION
	template <FloatingPoint Type> static constexpr Type Lerp(Type a, Type b, Type t) noexcept { return a + (b - a) * t; }
	template <FloatingPoint Type> static constexpr Type InvLerp(Type a, Type b, Type t) noexcept { return (t - a) / (b - a); }
	template <FloatingPoint Type> static constexpr Type Remap(Type iMin, Type iMax, Type oMin, Type oMax, Type t) noexcept { return Lerp(oMin, oMax, InvLerp(iMin, iMax, t)); } //TODO: Vector for output
	template <VectorType Type> static constexpr Type Lerp(const Type& a, const Type& b, F32 t) noexcept;
	template <FloatingPoint Type> static constexpr Type MoveTowards(Type a, Type b, Type t) noexcept { return Abs(b - a) <= t ? b : a + Sin(b - a) * t; }

	static Quaternion2 Slerp(const Quaternion2& a, const Quaternion2& b, F32 t) noexcept;
	static Quaternion3 Slerp(const Quaternion3& a, const Quaternion3& b, F32 t) noexcept;

	//NOISE
	static F64 Simplex1(F64 x) noexcept;
	static F64 Simplex2(F64 x, F64 y) noexcept;
	static F64 Simplex3(F64 x, F64 y, F64 z) noexcept;

private:

	STATIC_CLASS(Math);
};

struct NH_API Quaternion2
{
	constexpr Quaternion2() : x{ 0.0f }, y{ 1.0f } {}
	constexpr Quaternion2(F32 x, F32 y) : x{ x }, y{ y } {}
	constexpr Quaternion2(F32 angle) { F32 a = angle * DEG_TO_RAD_F; x = Math::Sin(a); y = Math::Cos(a); }
	constexpr Quaternion2(const Quaternion2& q) : x{ q.x }, y{ q.y } {}
	constexpr Quaternion2(Quaternion2&& q) noexcept : x{ q.x }, y{ q.y } {}

	constexpr Quaternion2& operator=(F32 angle) { F32 a = angle * DEG_TO_RAD_F; x = Math::Sin(a); y = Math::Cos(a); return *this; }
	constexpr Quaternion2& operator=(const Quaternion2& q) { x = q.y; y = q.y; return *this; }
	constexpr Quaternion2& operator=(Quaternion2&& q) noexcept { x = q.y; y = q.y; return *this; }

	constexpr Quaternion2& operator+=(const Quaternion2& q) { x += q.x; y += q.y; return *this; }
	constexpr Quaternion2& operator-=(const Quaternion2& q) { x -= q.x; y -= q.y; return *this; }
	constexpr Quaternion2& operator*=(const Quaternion2& q) { x = y * q.x + x * q.y; y = y * q.y - x * q.x; return *this; }
	constexpr Quaternion2& operator/=(const Quaternion2& q)
	{
		F32 n2 = 1.0f / q.SqrNormal();

		x = (-y * q.x + x * q.y) * n2;
		y = (y * q.y + x * q.x) * n2;

		return *this;
	}

	constexpr Quaternion2 operator+(const Quaternion2& q) const { return { x + q.x, y + q.y }; }
	constexpr Quaternion2 operator-(const Quaternion2& q) const { return { x - q.x, y - q.y }; }
	constexpr Quaternion2 operator*(const Quaternion2& q) const { return { y * q.x + x * q.y, y * q.y - x * q.x, }; }
	constexpr Quaternion2 operator/(const Quaternion2& q) const
	{
		F32 n2 = 1.0f / q.SqrNormal();

		return {
			(x * q.y - y * q.x) * n2,
			(y * q.y + x * q.x) * n2
		};
	}

	constexpr void Set(F32 angle) { F32 a = angle * DEG_TO_RAD_F; x = Math::Sin(a); y = Math::Cos(a); }
	constexpr void Rotate(F32 angle) { F32 a = angle * DEG_TO_RAD_F; x += Math::Sin(a); y += Math::Cos(a); }
	F32 Angle() const { return Math::Asin(x); }

	constexpr Quaternion2 Slerp(const Quaternion2& q, F32 t) const
	{
		constexpr F32 DOT_THRESHOLD = 0.9995f;

		Quaternion2 v0 = Normalize();
		Quaternion2 v1 = q.Normalize();

		F32 dot = v0.Dot(v1);

		if (dot < 0.0f)
		{
			v1.x = -v1.x;
			v1.y = -v1.y;
			dot = -dot;
		}

		if (dot > DOT_THRESHOLD)
		{
			Quaternion2 out{
				v0.x + ((v1.x - v0.x) * t),
				v0.y + ((v1.y - v0.y) * t)
			};

			return out.Normalize();
		}

		F32 theta0 = Math::Acos(dot);
		F32 theta = theta0 * t;
		F32 sinTheta = Math::Sin(theta);
		F32 sinTheta0 = Math::Sin(theta0);

		F32 s0 = Math::Cos(theta) - dot * sinTheta / sinTheta0;
		F32 s1 = sinTheta / sinTheta0;

		return {
			v0.x * s0 + v1.x * s1,
			v0.y * s0 + v1.y * s1
		};
	}
	constexpr Quaternion2& Slerped(const Quaternion2& q, F32 t)
	{
		constexpr F32 DOT_THRESHOLD = 0.9995f;

		Quaternion2 v0 = Normalize();
		Quaternion2 v1 = q.Normalize();

		F32 dot = v0.Dot(v1);

		if (dot < 0.0f)
		{
			v1.x = -v1.x;
			v1.y = -v1.y;
			dot = -dot;
		}

		if (dot > DOT_THRESHOLD)
		{
			x = v0.x + ((v1.x - v0.x) * t);
			y = v0.y + ((v1.y - v0.y) * t);

			return Normalized();
		}

		F32 theta0 = Math::Acos(dot);
		F32 theta = theta0 * t;
		F32 sinTheta = Math::Sin(theta);
		F32 sinTheta0 = Math::Sin(theta0);

		F32 s0 = Math::Cos(theta) - dot * sinTheta / sinTheta0;
		F32 s1 = sinTheta / sinTheta0;

		x = v0.x * s0 + v1.x * s1;
		y = v0.y * s0 + v1.y * s1;

		return *this;
	}

	constexpr F32 Dot(const Quaternion2& q) const { return x * q.x + y * q.y; }
	constexpr F32 SqrNormal() const { return x * x + y * y; }
	constexpr F32 Normal() const { return Math::Sqrt(x * x + y * y); }
	constexpr Quaternion2 Normalize() const { F32 n = 1.0f / Normal(); return { x * n, y * n }; }
	constexpr Quaternion2& Normalized() { F32 n = 1.0f / Normal(); x *= n; y *= n; return *this; }
	constexpr Quaternion2 Conjugate() const { return { -x, y }; }
	constexpr Quaternion2& Conjugated() { x = -x; return *this; }
	constexpr Quaternion2 Inverse() const { F32 n = 1.0f / Math::Sqrt(x * x + y * y); return { -x * n, y * n }; }
	constexpr Quaternion2& Inversed() { return Conjugated().Normalized(); }

	F32& operator[] (U8 i) { return (&x)[i]; }
	const F32& operator[] (U8 i) const { return (&x)[i]; }

	F32* Data() { return &x; }
	const F32* Data() const { return &x; }

public:
	F32 x, y; //sin, cos
};

struct NH_API Vector2
{
	constexpr Vector2() : x{ 0.0f }, y{ 0.0f } {}
	constexpr Vector2(F32 f) : x{ f }, y{ f } {}
	constexpr Vector2(F32 x, F32 y) : x{ x }, y{ y } {}
	constexpr Vector2(const Vector2& v) : x{ v.x }, y{ v.y } {}
	constexpr Vector2(Vector2&& v) noexcept : x{ v.x }, y{ v.y } {}

	constexpr Vector2& operator=(F32 f) { x = f; y = f; return *this; }
	constexpr Vector2& operator=(const Vector2& v) { x = v.x; y = v.y; return *this; }
	constexpr Vector2& operator=(Vector2&& v) noexcept { x = v.x; y = v.y; return *this; }

	constexpr Vector2& operator+=(F32 f) { x += f; y += f; return *this; }
	constexpr Vector2& operator-=(F32 f) { x -= f; y -= f; return *this; }
	constexpr Vector2& operator*=(F32 f) { x *= f; y *= f; return *this; }
	constexpr Vector2& operator/=(F32 f) { x /= f; y /= f; return *this; }
	constexpr Vector2& operator%=(F32 f) { x = Math::Mod(x, f); y = Math::Mod(y, f); return *this; }
	constexpr Vector2& operator+=(const Vector2& v) { x += v.x; y += v.y; return *this; }
	constexpr Vector2& operator-=(const Vector2& v) { x -= v.x; y -= v.y; return *this; }
	constexpr Vector2& operator*=(const Vector2& v) { x *= v.x; y *= v.y; return *this; }
	constexpr Vector2& operator/=(const Vector2& v) { x /= v.x; y /= v.y; return *this; }
	constexpr Vector2& operator%=(const Vector2& v) { x = Math::Mod(x, v.x); y = Math::Mod(y, v.y); return *this; }
	constexpr Vector2& operator*=(const Quaternion2& q) { F32 temp = q.y * x - q.x * y; y = q.x * x + q.y * y; x = temp; return *this; }

	constexpr Vector2 operator+(F32 f) const { return { x + f, y + f }; }
	constexpr Vector2 operator-(F32 f) const { return { x - f, y - f }; }
	constexpr Vector2 operator*(F32 f) const { return { x * f, y * f }; }
	constexpr Vector2 operator/(F32 f) const { return { x / f, y / f }; }
	constexpr Vector2 operator%(F32 f) const { return { Math::Mod(x, f), Math::Mod(y, f) }; }
	constexpr Vector2 operator+(const Vector2& v) const { return { x + v.x, y + v.y }; }
	constexpr Vector2 operator-(const Vector2& v) const { return { x - v.x, y - v.y }; }
	constexpr Vector2 operator*(const Vector2& v) const { return { x * v.x, y * v.y }; }
	constexpr Vector2 operator/(const Vector2& v) const { return { x / v.x, y / v.y }; }
	constexpr Vector2 operator%(const Vector2& v) const { return { Math::Mod(x, v.x), Math::Mod(y, v.y) }; }
	constexpr Vector2 operator*(const Quaternion2& q) const { return Vector2{ q.y * x - q.x * y, q.x * x + q.y * y }; }

	constexpr bool operator==(const Vector2& v) const { return Math::IsZero(x - v.x) && Math::IsZero(y - v.y); }
	constexpr bool operator!=(const Vector2& v) const { return !Math::IsZero(x - v.x) || !Math::IsZero(y - v.y); }
	constexpr bool operator>(const Vector2& v) const { return SqrMagnitude() > v.SqrMagnitude(); }
	constexpr bool operator<(const Vector2& v) const { return SqrMagnitude() < v.SqrMagnitude(); }
	constexpr bool operator>=(const Vector2& v) const { return *this == v || SqrMagnitude() > v.SqrMagnitude(); }
	constexpr bool operator<=(const Vector2& v) const { return *this == v || SqrMagnitude() < v.SqrMagnitude(); }
	constexpr bool IsZero() const { return Math::IsZero(x) && Math::IsZero(y); }

	constexpr Vector2 operator-() const { return { -x, -y }; }
	constexpr Vector2 operator~() const { return { -x, -y }; }
	constexpr Vector2 operator!() const { return { -x, -y }; }

	constexpr F32 SqrMagnitude() const { return x * x + y * y; }
	constexpr F32 Magnitude() const { return Math::Sqrt(x * x + y * y); }
	constexpr F32 Dot(const Vector2& v) const { return x * v.x + y * v.y; }
	constexpr F32 Dot(F32 vx, F32 vy) const { return x * vx + y * vy; }
	constexpr Vector2& Normalize() { Vector2 v = Normalized(); x = v.x; y = v.y; return *this; }
	constexpr Vector2 Normalized() const { return IsZero() ? Vector2{ 0.0f } : (*this) / Magnitude(); }
	F32 AngleBetween(const Vector2& v) const { return Math::Acos(Dot(v) * Math::InvSqrt(Dot(*this) * v.Dot(v))); }
	constexpr Vector2 Projection(const Vector2& v) const { return v * (Dot(v) / v.Dot(v)); }
	constexpr Vector2 OrthoProjection(const Vector2& v) const { return *this - Projection(v); }
	constexpr F32 Cross(const Vector2& v) const { return x * v.y - y * v.x; }
	constexpr Vector2 Cross(const F32 f) const { return { y * f, x * -f }; }
	constexpr Vector2 Normal(const Vector2& v) const { return Vector2(-(v.y - y), v.x - x).Normalized(); }
	constexpr Vector2& Rotate(const Vector2& center, F32 angle)
	{
		F32 cos = Math::Cos(angle * DEG_TO_RAD_F);
		F32 sin = Math::Sin(angle * DEG_TO_RAD_F);
		F32 temp = cos * (x - center.x) - sin * (y - center.y) + center.x;
		y = sin * (x - center.x) + cos * (y - center.y) + center.y;
		x = temp;

		return *this;
	}
	constexpr Vector2 Rotated(const Vector2& center, F32 angle) const
	{
		F32 cos = Math::Cos(angle * DEG_TO_RAD_F);
		F32 sin = Math::Sin(angle * DEG_TO_RAD_F);
		return Vector2{ cos * (x - center.x) - sin * (y - center.y) + center.x,
		sin * (x - center.x) + cos * (y - center.y) + center.y };
	}
	constexpr Vector2& Rotate(const Vector2& center, const Quaternion2& quat)
	{
		F32 temp = quat.y * (x - center.x) - quat.x * (y - center.y) + center.x;
		y = quat.x * (x - center.x) + quat.y * (y - center.y) + center.y;
		x = temp;
		return *this;
	}
	constexpr Vector2 Rotated(const Vector2& center, const Quaternion2& quat) const
	{
		return Vector2{ quat.y * (x - center.x) - quat.x * (y - center.y) + center.x, quat.x * (x - center.x) + quat.y * (y - center.y) + center.y };
	}
	constexpr Vector2& Clamp(const Vector2& min, const Vector2& max) { x = Math::Clamp(x, min.x, max.x); y = Math::Clamp(y, min.y, max.y); return *this; }
	constexpr Vector2 Clamped(const Vector2& min, const Vector2& max) const { return { Math::Clamp(x, min.x, max.x), Math::Clamp(y, min.y, max.y) }; }
	constexpr Vector2& SetClosest(const Vector2& min, const Vector2& max) { x = Math::Closest(x, min.x, max.x); y = Math::Closest(y, min.y, max.y); return *this; }
	constexpr Vector2 Closest(const Vector2& min, const Vector2& max) const { return { Math::Closest(x, min.x, max.x), Math::Closest(y, min.y, max.y) }; }

	F32& operator[] (U64 i) { return (&x)[i]; }
	const F32& operator[] (U64 i) const { return (&x)[i]; }

	F32* Data() { return &x; }
	const F32* Data() const { return &x; }

	operator String() const;
	operator String16() const;
	operator String32() const;

public:
	F32 x, y;
};

struct NH_API Vector3
{
	constexpr Vector3() : x{ 0.0f }, y{ 0.0f }, z{ 0.0f } {}
	constexpr Vector3(F32 f) : x{ f }, y{ f }, z{ f } {}
	constexpr Vector3(F32 x, F32 y, F32 z) : x{ x }, y{ y }, z{ z } {}
	constexpr Vector3(const Vector2& v, F32 z) : x{ v.x }, y{ v.y }, z{ z } {}
	constexpr Vector3(F32 x, const Vector2& v) : x{ x }, y{ v.x }, z{ v.y } {}
	constexpr Vector3(const Vector3& v) : x{ v.x }, y{ v.y }, z{ v.z } {}
	constexpr Vector3(Vector3&& v) noexcept : x{ v.x }, y{ v.y }, z{ v.z } {}

	constexpr Vector3& operator=(F32 f) { x = f; y = f; z = f; return *this; }
	constexpr Vector3& operator=(const Vector3& v) { x = v.x; y = v.y; z = v.z; return *this; }
	constexpr Vector3& operator=(Vector3&& v) noexcept { x = v.x; y = v.y; z = v.z; return *this; }

	constexpr Vector3& operator+=(F32 f) { x += f; y += f; z += f; return *this; }
	constexpr Vector3& operator-=(F32 f) { x -= f; y -= f; z -= f; return *this; }
	constexpr Vector3& operator*=(F32 f) { x *= f; y *= f; z *= f; return *this; }
	constexpr Vector3& operator/=(F32 f) { x /= f; y /= f; z /= f; return *this; }
	constexpr Vector3& operator%=(F32 f) { x = Math::Mod(x, f); y = Math::Mod(y, f); z = Math::Mod(z, f); return *this; }
	constexpr Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	constexpr Vector3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	constexpr Vector3& operator*=(const Vector3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
	constexpr Vector3& operator/=(const Vector3& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }
	constexpr Vector3& operator%=(const Vector3& v) { x = Math::Mod(x, v.x); y = Math::Mod(y, v.y); z = Math::Mod(z, v.z); return *this; }
	Vector3& operator*=(const Quaternion3& q);

	constexpr Vector3 operator+(F32 f) const { return { x + f, y + f, z + f }; }
	constexpr Vector3 operator-(F32 f) const { return { x - f, y - f, z - f }; }
	constexpr Vector3 operator*(F32 f) const { return { x * f, y * f, z * f }; }
	constexpr Vector3 operator/(F32 f) const { return { x / f, y / f, z / f }; }
	constexpr Vector3 operator%(F32 f) const { return { Math::Mod(x, f), Math::Mod(y, f), Math::Mod(z, f) }; }
	constexpr Vector3 operator+(const Vector3& v) const { return { x + v.x, y + v.y, z + v.z }; }
	constexpr Vector3 operator-(const Vector3& v) const { return { x - v.x, y - v.y, z - v.z }; }
	constexpr Vector3 operator*(const Vector3& v) const { return { x * v.x, y * v.y, z * v.z }; }
	constexpr Vector3 operator/(const Vector3& v) const { return { x / v.x, y / v.y, z / v.z }; }
	constexpr Vector3 operator%(const Vector3& v) const { return { Math::Mod(x, v.x), Math::Mod(y, v.y), Math::Mod(z, v.z) }; }
	Vector3 operator*(const Quaternion3& q) const;

	constexpr bool operator==(const Vector3& v) const { return Math::IsZero(x - v.x) && Math::IsZero(y - v.y) && Math::IsZero(z - v.z); }
	constexpr bool operator!=(const Vector3& v) const { return !Math::IsZero(x - v.x) || !Math::IsZero(y - v.y) || !Math::IsZero(z - v.z); }
	constexpr bool operator>(const Vector3& v) const { return SqrMagnitude() > v.SqrMagnitude(); }
	constexpr bool operator<(const Vector3& v) const { return SqrMagnitude() < v.SqrMagnitude(); }
	constexpr bool operator>=(const Vector3& v) const { return *this == v || SqrMagnitude() > v.SqrMagnitude(); }
	constexpr bool operator<=(const Vector3& v) const { return *this == v || SqrMagnitude() < v.SqrMagnitude(); }
	constexpr bool IsZero() const { return Math::IsZero(x) && Math::IsZero(y) && Math::IsZero(z); }

	constexpr Vector3 operator-() const { return { -x, -y, -z }; }
	constexpr Vector3 operator~() const { return { -x, -y, -z }; }
	constexpr Vector3 operator!() const { return { -x, -y, -z }; }

	constexpr F32 SqrMagnitude() const { return x * x + y * y + z * z; }
	constexpr F32 Magnitude() const { return Math::Sqrt(x * x + y * y + z * z); }
	constexpr F32 Dot(const Vector3& v) const { return x * v.x + y * v.y + z * v.z; }
	constexpr F32 Dot(F32 vx, F32 vy, F32 vz) const { return x * vx + y * vy + z * vz; }
	constexpr Vector3& Normalize() { Vector3 v = Normalized(); x = v.x; y = v.y; z = v.z; return *this; }
	constexpr Vector3 Normalized() const { return IsZero() ? Vector3{ 0.0f } : (*this) / Magnitude(); }
	constexpr Vector3 Projection(const Vector3& v) const { return v * (Dot(v) / v.Dot(v)); }
	constexpr Vector3 OrthoProjection(const Vector3& v) const { return *this - Projection(v); }
	constexpr Vector3 Cross(const Vector3& v) const { return { y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x }; }
	constexpr Vector3 Normal(const Vector3& v) const;
	constexpr Vector3& Rotate(const Vector3& center, const Quaternion3& quat);
	constexpr Vector3 Rotated(const Vector3& center, const Quaternion3& quat) const;
	constexpr Vector3& Clamp(const Vector3& min, const Vector3& max)
	{
		x = Math::Clamp(x, min.x, max.x);
		y = Math::Clamp(y, min.y, max.y);
		z = Math::Clamp(z, min.z, max.z);
		return *this;
	}
	constexpr Vector3 Clamped(const Vector3& min, const Vector3& max) const
	{
		return {
			Math::Clamp(x, min.x, max.x),
			Math::Clamp(y, min.y, max.y),
			Math::Clamp(z, min.z, max.z)
		};
	}
	constexpr Vector3& SetClosest(const Vector3& min, const Vector3& max)
	{
		x = Math::Closest(x, min.x, max.x);
		y = Math::Closest(y, min.y, max.y);
		z = Math::Closest(z, min.z, max.z);
		return *this;
	}
	constexpr Vector3 Closest(const Vector3& min, const Vector3& max) const
	{
		return {
			Math::Closest(x, min.x, max.x),
			Math::Closest(y, min.y, max.y),
			Math::Closest(z, min.z, max.z)
		};
	}

	F32& operator[] (U64 i) { return (&x)[i]; }
	const F32& operator[] (U64 i) const { return (&x)[i]; }

	F32* Data() { return &x; }
	const F32* Data() const { return &x; }

	operator String() const;
	operator String16() const;
	operator String32() const;

public:
	F32 x, y, z;
};

struct NH_API Vector4
{
	constexpr Vector4() : x{ 0.0f }, y{ 0.0f }, z{ 0.0f }, w{ 0.0f } {}
	constexpr Vector4(F32 f) : x{ f }, y{ f }, z{ f }, w{ f } {}
	constexpr Vector4(F32 x, F32 y, F32 z, F32 w) : x{ x }, y{ y }, z{ z }, w{ w } {}
	constexpr Vector4(const Vector4& v) : x{ v.x }, y{ v.y }, z{ v.z }, w{ v.w } {}
	constexpr Vector4(Vector4&& v) noexcept : x{ v.x }, y{ v.y }, z{ v.z }, w{ v.w } {}

	constexpr Vector4& operator=(F32 f) { x = f; y = f; z = f; w = f; return *this; }
	constexpr Vector4& operator=(const Vector4& v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }
	constexpr Vector4& operator=(Vector4&& v) noexcept { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }

	constexpr Vector4& operator+=(F32 f) { x += f; y += f; z += f; w += f; return *this; }
	constexpr Vector4& operator-=(F32 f) { x -= f; y -= f; z -= f; w -= f; return *this; }
	constexpr Vector4& operator*=(F32 f) { x *= f; y *= f; z *= f; w *= f; return *this; }
	constexpr Vector4& operator/=(F32 f) { x /= f; y /= f; z /= f; w /= f; return *this; }
	constexpr Vector4& operator%=(F32 f) { x = Math::Mod(x, f); y = Math::Mod(y, f); z = Math::Mod(z, f); w = Math::Mod(w, f); return *this; }
	constexpr Vector4& operator+=(const Vector4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
	constexpr Vector4& operator-=(const Vector4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
	constexpr Vector4& operator*=(const Vector4& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
	constexpr Vector4& operator/=(const Vector4& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }
	constexpr Vector4& operator%=(const Vector4& v) { x = Math::Mod(x, v.x); y = Math::Mod(y, v.y); z = Math::Mod(z, v.z); w = Math::Mod(w, v.w); return *this; }

	constexpr Vector4 operator+(F32 f) const { return { x + f, y + f, z + f, w + f }; }
	constexpr Vector4 operator-(F32 f) const { return { x - f, y - f, z - f, w - f }; }
	constexpr Vector4 operator*(F32 f) const { return { x * f, y * f, z * f, w * f }; }
	constexpr Vector4 operator/(F32 f) const { return { x / f, y / f, z / f, w / f }; }
	constexpr Vector4 operator%(F32 f) const { return { Math::Mod(x, f), Math::Mod(y, f), Math::Mod(z, f), Math::Mod(w, f) }; }
	constexpr Vector4 operator+(const Vector4& v) const { return { x + v.x, y + v.y, z + v.z, w + v.w }; }
	constexpr Vector4 operator-(const Vector4& v) const { return { x - v.x, y - v.y, z - v.z, w - v.w }; }
	constexpr Vector4 operator*(const Vector4& v) const { return { x * v.x, y * v.y, z * v.z, w * v.w }; }
	constexpr Vector4 operator/(const Vector4& v) const { return { x / v.x, y / v.y, z / v.z, w / v.w }; }
	constexpr Vector4 operator%(const Vector4& v) const { return { Math::Mod(x, v.x), Math::Mod(y, v.y), Math::Mod(z, v.z), Math::Mod(w, v.w) }; }

	constexpr bool operator==(const Vector4& v) const { return Math::IsZero(x - v.x) && Math::IsZero(y - v.y) && Math::IsZero(z - v.z) && Math::IsZero(w - v.w); }
	constexpr bool operator!=(const Vector4& v) const { return !Math::IsZero(x - v.x) || !Math::IsZero(y - v.y) || !Math::IsZero(z - v.z) || !Math::IsZero(w - v.w); }
	constexpr bool operator>(const Vector4& v) const { return SqrMagnitude() > v.SqrMagnitude(); }
	constexpr bool operator<(const Vector4& v) const { return SqrMagnitude() < v.SqrMagnitude(); }
	constexpr bool operator>=(const Vector4& v) const { return *this == v || SqrMagnitude() > v.SqrMagnitude(); }
	constexpr bool operator<=(const Vector4& v) const { return *this == v || SqrMagnitude() < v.SqrMagnitude(); }
	constexpr bool IsZero() const { return Math::IsZero(x) && Math::IsZero(y) && Math::IsZero(z) && Math::IsZero(w); }

	constexpr Vector4 operator-() const { return { -x, -y, -z, -w }; }
	constexpr Vector4 operator~() const { return { -x, -y, -z, -w }; }
	constexpr Vector4 operator!() const { return { -x, -y, -z, -w }; }

	constexpr F32 SqrMagnitude() const { return x * x + y * y + z * z + w * w; }
	constexpr F32 Magnitude() const { return Math::Sqrt(x * x + y * y + z * z + w * w); }
	constexpr F32 Dot(const Vector4& v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }
	constexpr F32 Dot(F32 vx, F32 vy, F32 vz, F32 vw) const { return x * vx + y * vy + z * vz + w * vw; }
	constexpr Vector4& Normalize() { Vector4 v = Normalized(); x = v.x; y = v.y; z = v.z; w = v.w; return *this; }
	constexpr Vector4 Normalized() const { return IsZero() ? Vector4{ 0.0f } : (Vector3(this->x, this->y, this->z) / Magnitude(), this->z); }
	constexpr Vector4 Projection(const Vector4& v) const { return v * (Dot(v) / v.Dot(v)); }
	constexpr Vector4 OrthoProjection(const Vector4& v) const { return *this - Projection(v); }
	constexpr Vector4& Clamp(const Vector4& min, const Vector4& max)
	{
		x = Math::Clamp(x, min.x, max.x);
		y = Math::Clamp(y, min.y, max.y);
		z = Math::Clamp(z, min.z, max.z);
		w = Math::Clamp(w, min.w, max.w);
		return *this;
	}
	constexpr Vector4 Clamped(const Vector4& min, const Vector4& max) const
	{
		return {
			Math::Clamp(x, min.x, max.x),
			Math::Clamp(y, min.y, max.y),
			Math::Clamp(z, min.z, max.z),
			Math::Clamp(w, min.w, max.w)
		};
	}
	constexpr Vector4& SetClosest(const Vector4& min, const Vector4& max)
	{
		x = Math::Closest(x, min.x, max.x);
		y = Math::Closest(y, min.y, max.y);
		z = Math::Closest(z, min.z, max.z);
		w = Math::Closest(w, min.w, max.w);
		return *this;
	}
	constexpr Vector4 Closest(const Vector4& min, const Vector4& max) const
	{
		return {
			Math::Closest(x, min.x, max.x),
			Math::Closest(y, min.y, max.y),
			Math::Closest(z, min.z, max.z),
			Math::Closest(w, min.w, max.w)
		};
	}

	F32& operator[] (U64 i) { return (&x)[i]; }
	const F32& operator[] (U64 i) const { return (&x)[i]; }

	F32* Data() { return &x; }
	const F32* Data() const { return &x; }

	operator String() const;
	operator String16() const;
	operator String32() const;

public:
	F32 x, y, z, w;
};

struct NH_API Vector2Int
{
	constexpr Vector2Int() : x{ 0 }, y{ 0 } {}
	constexpr Vector2Int(I32 i) : x{ i }, y{ i } {}
	constexpr Vector2Int(I32 x, I32 y) : x{ x }, y{ y } {}
	constexpr Vector2Int(const Vector2Int& v) : x{ v.x }, y{ v.y } {}
	constexpr Vector2Int(Vector2Int&& v) noexcept : x{ v.x }, y{ v.y } {}

	constexpr Vector2Int& operator=(I32 i) { x = i; y = i; return *this; }
	constexpr Vector2Int& operator=(const Vector2Int& v) { x = v.x; y = v.y; return *this; }
	constexpr Vector2Int& operator=(Vector2Int&& v) noexcept { x = v.x; y = v.y; return *this; }

	constexpr Vector2Int& operator+=(I32 i) { x += i; y += i; return *this; }
	constexpr Vector2Int& operator-=(I32 i) { x -= i; y -= i; return *this; }
	constexpr Vector2Int& operator*=(I32 i) { x *= i; y *= i; return *this; }
	constexpr Vector2Int& operator/=(I32 i) { x /= i; y /= i; return *this; }
	constexpr Vector2Int& operator%=(I32 i) { x %= i; y %= i; return *this; }
	constexpr Vector2Int& operator+=(const Vector2Int& v) { x += v.x; y += v.y; return *this; }
	constexpr Vector2Int& operator-=(const Vector2Int& v) { x -= v.x; y -= v.y; return *this; }
	constexpr Vector2Int& operator*=(const Vector2Int& v) { x *= v.x; y *= v.y; return *this; }
	constexpr Vector2Int& operator/=(const Vector2Int& v) { x /= v.x; y /= v.y; return *this; }
	constexpr Vector2Int& operator%=(const Vector2Int& v) { x &= v.x; y &= v.y; return *this; }

	constexpr Vector2Int operator+(I32 i) const { return { x + i, y + i }; }
	constexpr Vector2Int operator-(I32 i) const { return { x - i, y - i }; }
	constexpr Vector2Int operator*(I32 i) const { return { x * i, y * i }; }
	constexpr Vector2Int operator/(I32 i) const { return { x / i, y / i }; }
	constexpr Vector2Int operator%(I32 i) const { return { x % i, y % i }; }
	constexpr Vector2Int operator+(const Vector2Int& v) const { return { x + v.x, y + v.y }; }
	constexpr Vector2Int operator-(const Vector2Int& v) const { return { x - v.x, y - v.y }; }
	constexpr Vector2Int operator*(const Vector2Int& v) const { return { x * v.x, y * v.y }; }
	constexpr Vector2Int operator/(const Vector2Int& v) const { return { x / v.x, y / v.y }; }
	constexpr Vector2Int operator%(const Vector2Int& v) const { return { x % v.x, y % v.y }; }
	constexpr Vector2Int operator+(const Vector2& v) const { return { (I32)(x + v.x), (I32)(y + v.y) }; }
	constexpr Vector2Int operator-(const Vector2& v) const { return { (I32)(x - v.x), (I32)(y - v.y) }; }
	constexpr Vector2Int operator*(const Vector2& v) const { return { (I32)(x * v.x), (I32)(y * v.y) }; }
	constexpr Vector2Int operator/(const Vector2& v) const { return { (I32)(x / v.x), (I32)(y / v.y) }; }

	constexpr bool operator==(const Vector2Int& v) const { return !(x - v.x) && !(y - v.y); }
	constexpr bool operator!=(const Vector2Int& v) const { return (x - v.x) || (y - v.y); }
	constexpr bool operator>(const Vector2Int& v) const { return SqrMagnitude() > v.SqrMagnitude(); }
	constexpr bool operator<(const Vector2Int& v) const { return SqrMagnitude() < v.SqrMagnitude(); }
	constexpr bool operator>=(const Vector2Int& v) const { return *this == v || SqrMagnitude() > v.SqrMagnitude(); }
	constexpr bool operator<=(const Vector2Int& v) const { return *this == v || SqrMagnitude() < v.SqrMagnitude(); }
	constexpr bool IsZero() const { return !x && !y; }

	constexpr Vector2Int operator-() const { return { -x, -y }; }
	constexpr Vector2Int operator~() const { return { -x, -y }; }
	constexpr Vector2Int operator!() const { return { -x, -y }; }

	constexpr I32 SqrMagnitude() const { return x * x + y * y; }
	constexpr F32 Magnitude() const { return Math::Sqrt((F32)(x * x + y * y)); }
	constexpr I32 Dot(const Vector2Int& v) const { return x * v.x + y * v.y; }
	constexpr I32 Dot(I32 vx, I32 vy) const { return x * vx + y * vy; }

	constexpr Vector2Int& Clamp(const Vector2Int& min, const Vector2Int& max)
	{
		x = Math::Clamp(x, min.x, max.x);
		y = Math::Clamp(y, min.y, max.y);
		return *this;
	}
	constexpr Vector2Int Clamped(const Vector2Int& min, const Vector2Int& max) const
	{
		return {
			Math::Clamp(x, min.x, max.x),
			Math::Clamp(y, min.y, max.y)
		};
	}
	constexpr Vector2Int& SetClosest(const Vector2Int& min, const Vector2Int& max)
	{
		x = Math::Closest(x, min.x, max.x);
		y = Math::Closest(y, min.y, max.y);
		return *this;
	}
	constexpr Vector2Int Closest(const Vector2Int& min, const Vector2Int& max) const
	{
		return {
			Math::Closest(x, min.x, max.x),
			Math::Closest(y, min.y, max.y)
		};
	}

	I32& operator[] (U64 i) { return (&x)[i]; }
	const I32& operator[] (U64 i) const { return (&x)[i]; }

	I32* Data() { return &x; }
	const I32* Data() const { return &x; }

	operator Vector2() const { return { (F32)x, (F32)y }; }

	operator String() const;
	operator String16() const;
	operator String32() const;

public:
	I32 x, y;
};

struct NH_API Vector3Int
{
	constexpr Vector3Int() : x{ 0 }, y{ 0 }, z{ 0 } {}
	constexpr Vector3Int(I32 i) : x{ i }, y{ i }, z{ i } {}
	constexpr Vector3Int(I32 x, I32 y, I32 z) : x{ x }, y{ y }, z{ z } {}
	constexpr Vector3Int(const Vector3Int& v) : x{ v.x }, y{ v.y }, z{ v.z } {}
	constexpr Vector3Int(Vector3Int&& v) noexcept : x{ v.x }, y{ v.y }, z{ v.z } {}

	constexpr Vector3Int& operator=(I32 i) { x = i; y = i; z = i; return *this; }
	constexpr Vector3Int& operator=(const Vector3Int& v) { x = v.x; y = v.y; z = v.z; return *this; }
	constexpr Vector3Int& operator=(Vector3Int&& v) noexcept { x = v.x; y = v.y; z = v.z; return *this; }

	constexpr Vector3Int& operator+=(I32 i) { x += i; y += i; z += i; return *this; }
	constexpr Vector3Int& operator-=(I32 i) { x -= i; y -= i; z -= i; return *this; }
	constexpr Vector3Int& operator*=(I32 i) { x *= i; y *= i; z *= i; return *this; }
	constexpr Vector3Int& operator/=(I32 i) { x /= i; y /= i; z /= i; return *this; }
	constexpr Vector3Int& operator%=(I32 i) { x %= i; y %= i; z %= i; return *this; }
	constexpr Vector3Int& operator+=(const Vector3Int& v) { x += v.x; y += v.y; z += v.z; return *this; }
	constexpr Vector3Int& operator-=(const Vector3Int& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	constexpr Vector3Int& operator*=(const Vector3Int& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
	constexpr Vector3Int& operator/=(const Vector3Int& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }
	constexpr Vector3Int& operator%=(const Vector3Int& v) { x %= v.x; y %= v.y; z %= v.z; return *this; }

	constexpr Vector3Int operator+(I32 i) const { return { x + i, y + i, z + i }; }
	constexpr Vector3Int operator-(I32 i) const { return { x - i, y - i, z - i }; }
	constexpr Vector3Int operator*(I32 i) const { return { x * i, y * i, z * i }; }
	constexpr Vector3Int operator/(I32 i) const { return { x / i, y / i, z / i }; }
	constexpr Vector3Int operator%(I32 i) const { return { x % i, y % i, z % i }; }
	constexpr Vector3Int operator+(const Vector3Int& v) const { return { x + v.x, y + v.y, z + v.z }; }
	constexpr Vector3Int operator-(const Vector3Int& v) const { return { x - v.x, y - v.y, z - v.z }; }
	constexpr Vector3Int operator*(const Vector3Int& v) const { return { x * v.x, y * v.y, z * v.z }; }
	constexpr Vector3Int operator/(const Vector3Int& v) const { return { x / v.x, y / v.y, z / v.z }; }
	constexpr Vector3Int operator%(const Vector3Int& v) const { return { x % v.x, y % v.y, z % v.z }; }

	constexpr bool operator==(const Vector3Int& v) const { return !(x - v.x) && !(y - v.y) && !(z - v.z); }
	constexpr bool operator!=(const Vector3Int& v) const { return (x - v.x) || (y - v.y) || (z - v.z); }
	constexpr bool operator>(const Vector3Int& v) const { return SqrMagnitude() > v.SqrMagnitude(); }
	constexpr bool operator<(const Vector3Int& v) const { return SqrMagnitude() < v.SqrMagnitude(); }
	constexpr bool operator>=(const Vector3Int& v) const { return *this == v || SqrMagnitude() > v.SqrMagnitude(); }
	constexpr bool operator<=(const Vector3Int& v) const { return *this == v || SqrMagnitude() < v.SqrMagnitude(); }
	constexpr bool IsZero() const { return !x && !y && !z; }

	constexpr Vector3Int operator-() const { return { -x, -y, -z }; }
	constexpr Vector3Int operator~() const { return { -x, -y, -z }; }
	constexpr Vector3Int operator!() const { return { -x, -y, -z }; }

	constexpr I32 SqrMagnitude() const { return x * x + y * y + z * z; }
	constexpr F32 Magnitude() const { return Math::Sqrt((F32)(x * x + y * y + z * z)); }
	constexpr I32 Dot(const Vector3Int& v) const { return x * v.x + y * v.y + z * v.z; }
	constexpr I32 Dot(I32 vx, I32 vy, I32 vz) const { return x * vx + y * vy + z * vz; }

	constexpr Vector3Int& Clamp(const Vector3Int& min, const Vector3Int& max)
	{
		x = Math::Clamp(x, min.x, max.x);
		y = Math::Clamp(y, min.y, max.y);
		z = Math::Clamp(z, min.z, max.z);
		return *this;
	}
	constexpr Vector3Int Clamped(const Vector3Int& min, const Vector3Int& max) const
	{
		return {
			Math::Clamp(x, min.x, max.x),
			Math::Clamp(y, min.y, max.y),
			Math::Clamp(z, min.z, max.z)
		};
	}
	constexpr Vector3Int& SetClosest(const Vector3Int& min, const Vector3Int& max)
	{
		x = Math::Closest(x, min.x, max.x);
		y = Math::Closest(y, min.y, max.y);
		z = Math::Closest(z, min.z, max.z);
		return *this;
	}
	constexpr Vector3Int Closest(const Vector3Int& min, const Vector3Int& max) const
	{
		return {
			Math::Closest(x, min.x, max.x),
			Math::Closest(y, min.y, max.y),
			Math::Closest(z, min.z, max.z)
		};
	}

	I32& operator[] (U64 i) { return (&x)[i]; }
	const I32& operator[] (U64 i) const { return (&x)[i]; }

	I32* Data() { return &x; }
	const I32* Data() const { return &x; }

	operator String() const;
	operator String16() const;
	operator String32() const;

public:
	I32 x, y, z;
};

struct NH_API Vector4Int
{
	constexpr Vector4Int() : x{ 0 }, y{ 0 }, z{ 0 }, w{ 0 } {}
	constexpr Vector4Int(I32 i) : x{ i }, y{ i }, z{ i }, w{ i } {}
	constexpr Vector4Int(I32 x, I32 y, I32 z, I32 w) : x{ x }, y{ y }, z{ z }, w{ w } {}
	constexpr Vector4Int(const Vector4Int& v) : x{ v.x }, y{ v.y }, z{ v.z }, w{ v.w } {}
	constexpr Vector4Int(Vector4Int&& v) noexcept : x{ v.x }, y{ v.y }, z{ v.z }, w{ v.w } {}

	constexpr Vector4Int& operator=(I32 i) { x = i; y = i; z = i; w = i; return *this; }
	constexpr Vector4Int& operator=(const Vector4Int& v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }
	constexpr Vector4Int& operator=(Vector4Int&& v) noexcept { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }

	constexpr Vector4Int& operator+=(I32 i) { x += i; y += i; z += i; w += i; return *this; }
	constexpr Vector4Int& operator-=(I32 i) { x -= i; y -= i; z -= i; w -= i; return *this; }
	constexpr Vector4Int& operator*=(I32 i) { x *= i; y *= i; z *= i; w *= i; return *this; }
	constexpr Vector4Int& operator/=(I32 i) { x /= i; y /= i; z /= i; w /= i; return *this; }
	constexpr Vector4Int& operator%=(I32 i) { x &= i; y %= i; z %= i; w %= i; return *this; }
	constexpr Vector4Int& operator+=(F32 f) { x += (I32)f; y += (I32)f; z += (I32)f; w += (I32)f; return *this; }
	constexpr Vector4Int& operator-=(F32 f) { x -= (I32)f; y -= (I32)f; z -= (I32)f; w -= (I32)f; return *this; }
	constexpr Vector4Int& operator*=(F32 f) { x *= (I32)f; y *= (I32)f; z *= (I32)f; w *= (I32)f; return *this; }
	constexpr Vector4Int& operator/=(F32 f) { x /= (I32)f; y /= (I32)f; z /= (I32)f; w /= (I32)f; return *this; }
	constexpr Vector4Int& operator+=(const Vector4Int& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
	constexpr Vector4Int& operator-=(const Vector4Int& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
	constexpr Vector4Int& operator*=(const Vector4Int& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
	constexpr Vector4Int& operator/=(const Vector4Int& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }
	constexpr Vector4Int& operator%=(const Vector4Int& v) { x %= v.x; y %= v.y; z %= v.z; w %= v.w; return *this; }

	constexpr Vector4Int operator+(I32 i) const { return { x + i, y + i, z + i, w + i }; }
	constexpr Vector4Int operator-(I32 i) const { return { x - i, y - i, z - i, w - i }; }
	constexpr Vector4Int operator*(I32 i) const { return { x * i, y * i, z * i, w * i }; }
	constexpr Vector4Int operator/(I32 i) const { return { x / i, y / i, z / i, w / i }; }
	constexpr Vector4Int operator%(I32 i) const { return { x % i, y % i, z % i, w % i }; }
	constexpr Vector4Int operator+(F32 f) const { return { (I32)(x + f), (I32)(y + f), (I32)(z + f), (I32)(w + f) }; }
	constexpr Vector4Int operator-(F32 f) const { return { (I32)(x - f), (I32)(y - f), (I32)(z - f), (I32)(w - f) }; }
	constexpr Vector4Int operator*(F32 f) const { return { (I32)(x * f), (I32)(y * f), (I32)(z * f), (I32)(w * f) }; }
	constexpr Vector4Int operator/(F32 f) const { return { (I32)(x / f), (I32)(y / f), (I32)(z / f), (I32)(w / f) }; }
	constexpr Vector4Int operator+(const Vector4Int& v) const { return { x + v.x, y + v.y, z + v.z, w + v.w }; }
	constexpr Vector4Int operator-(const Vector4Int& v) const { return { x - v.x, y - v.y, z - v.z, w - v.w }; }
	constexpr Vector4Int operator*(const Vector4Int& v) const { return { x * v.x, y * v.y, z * v.z, w * v.w }; }
	constexpr Vector4Int operator/(const Vector4Int& v) const { return { x / v.x, y / v.y, z / v.z, w / v.w }; }
	constexpr Vector4Int operator%(const Vector4Int& v) const { return { x % v.x, y % v.y, z % v.z, w % v.w }; }

	constexpr bool operator==(const Vector4Int& v) const { return !(x - v.x) && !(y - v.y) && (z - v.z) && !(w - v.w); }
	constexpr bool operator!=(const Vector4Int& v) const { return (x - v.x) || (y - v.y) || (z - v.z) || (w - v.w); }
	constexpr bool operator>(const Vector4Int& v) const { return SqrMagnitude() > v.SqrMagnitude(); }
	constexpr bool operator<(const Vector4Int& v) const { return SqrMagnitude() < v.SqrMagnitude(); }
	constexpr bool operator>=(const Vector4Int& v) const { return *this == v || SqrMagnitude() > v.SqrMagnitude(); }
	constexpr bool operator<=(const Vector4Int& v) const { return *this == v || SqrMagnitude() < v.SqrMagnitude(); }
	constexpr bool IsZero() const { return !x && !y && !z && !w; }

	constexpr Vector4Int operator-() const { return { -x, -y, -z, -w }; }
	constexpr Vector4Int operator~() const { return { -x, -y, -z, -w }; }
	constexpr Vector4Int operator!() const { return { -x, -y, -z, -w }; }

	constexpr I32 SqrMagnitude() const { return x * x + y * y + z * z + w * w; }
	constexpr F32 Magnitude() const { return Math::Sqrt((F32)(x * x + y * y + z * z + w * w)); }
	constexpr I32 Dot(const Vector4Int& v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }
	constexpr I32 Dot(I32 vx, I32 vy, I32 vz, I32 vw) const { return x * vx + y * vy + z * vz + w * vw; }
	constexpr Vector4Int& Clamp(const Vector4Int& min, const Vector4Int& max)
	{
		x = Math::Clamp(x, min.x, max.x);
		y = Math::Clamp(y, min.y, max.y);
		z = Math::Clamp(z, min.z, max.z);
		w = Math::Clamp(w, min.w, max.w);
		return *this;
	}
	constexpr Vector4Int Clamped(const Vector4Int& min, const Vector4Int& max) const
	{
		return {
			Math::Clamp(x, min.x, max.x),
			Math::Clamp(y, min.y, max.y),
			Math::Clamp(z, min.z, max.z),
			Math::Clamp(w, min.w, max.w)
		};
	}
	constexpr Vector4Int& SetClosest(const Vector4Int& min, const Vector4Int& max)
	{
		x = Math::Closest(x, min.x, max.x);
		y = Math::Closest(y, min.y, max.y);
		z = Math::Closest(z, min.z, max.z);
		w = Math::Closest(w, min.w, max.w);
		return *this;
	}
	constexpr Vector4Int Closest(const Vector4Int& min, const Vector4Int& max) const
	{
		return {
			Math::Closest(x, min.x, max.x),
			Math::Closest(y, min.y, max.y),
			Math::Closest(z, min.z, max.z),
			Math::Closest(w, min.w, max.w)
		};
	}

	I32& operator[] (U64 i) { return (&x)[i]; }
	const I32& operator[] (U64 i) const { return (&x)[i]; }

	I32* Data() { return &x; }
	const I32* Data() const { return &x; }

	operator String() const;
	operator String16() const;
	operator String32() const;

public:
	I32 x, y, z, w;
};

template <VectorType Type>
inline constexpr Type Math::Lerp(const Type& a, const Type& b, F32 t) noexcept
{
	return a + (b - a) * t;
}

struct NH_API Matrix3
{
	constexpr Matrix3() : a{ 1.0f, 0.0f, 0.0f }, b{ 0.0f, 1.0f, 0.0f }, c{ 0.0f, 0.0f, 1.0f } {}
	constexpr Matrix3(F32 ax, F32 ay, F32 az, F32 bx, F32 by, F32 bz, F32 cx, F32 cy, F32 cz) : a{ ax, ay, az }, b{ bx, by, bz }, c{ cx, cy, cz } {}
	constexpr Matrix3(const Vector3& a, const Vector3& b, const Vector3& c) : a{ a }, b{ b }, c{ c } {}
	constexpr Matrix3(Vector3&& v1, Vector3&& v2, Vector3&& v3) noexcept : a{ v1 }, b{ v2 }, c{ v3 } {}
	constexpr Matrix3(const Matrix3& m) : a{ m.a }, b{ m.b }, c{ m.c } {}
	constexpr Matrix3(Matrix3&& m) noexcept : a{ m.a }, b{ m.b }, c{ m.c } {}
	constexpr Matrix3(const Vector2& position, const F32& rotation, const Vector2& scale)
	{
		F32 cos = Math::Cos(rotation * DEG_TO_RAD_F);
		F32 sin = Math::Sin(rotation * DEG_TO_RAD_F);
		a.x = cos * scale.x;	b.x = -sin;				c.x = position.x;
		a.y = sin;				b.y = cos * scale.y;	c.y = position.y;
		a.z = 0.0f;				b.z = 0.0f;				c.z = 1.0f;
	}
	constexpr Matrix3(const Vector2& position, const Quaternion2& rotation, const Vector2& scale)
	{
		F32 cos = rotation.y;
		F32 sin = rotation.x;
		a.x = cos * scale.x;	b.x = -sin;				c.x = position.x;
		a.y = sin;				b.y = cos * scale.y;	c.y = position.y;
		a.z = 0.0f;				b.z = 0.0f;				c.z = 1.0f;
	}

	constexpr Matrix3& operator= (const Matrix3& m) { a = m.a; b = m.b; c = m.c; return *this; }
	constexpr Matrix3& operator= (Matrix3&& m) noexcept { a = m.a; b = m.b; c = m.c; return *this; }
	constexpr void Set(const Vector2& position, const Quaternion2& rotation, const Vector2& scale)
	{
		F32 cos = rotation.y;
		F32 sin = rotation.x;
		a.x = cos * scale.x;	b.x = -sin;				c.x = position.x;
		a.y = sin;				b.y = cos * scale.y;	c.y = position.y;
		a.z = 0.0f;				b.z = 0.0f;				c.z = 1.0f;
	}
	constexpr void Set(const Vector2& position, const F32& rotation, const Vector2& scale)
	{
		F32 cos = Math::Cos(rotation * DEG_TO_RAD_F);
		F32 sin = Math::Sin(rotation * DEG_TO_RAD_F);
		a.x = cos * scale.x;	b.x = -sin;				c.x = position.x;
		a.y = sin;				b.y = cos * scale.y;	c.y = position.y;
		a.z = 0.0f;				b.z = 0.0f;				c.z = 1.0f;
	}

	constexpr Matrix3& operator+= (const Matrix3& m) { a += m.a; b += m.b; c += m.c; return *this; }
	constexpr Matrix3& operator-= (const Matrix3& m) { a -= m.a; b -= m.b; c -= m.c; return *this; }
	constexpr Matrix3& operator*= (const Matrix3& m)
	{
		a.x = a.x * m.a.x + b.x * m.a.y + c.x * m.a.z;
		a.y = a.y * m.a.x + b.y * m.a.y + c.y * m.a.z;
		a.z = a.z * m.a.x + b.z * m.a.y + c.z * m.a.z;
		b.x = a.x * m.b.x + b.x * m.b.y + c.x * m.b.z;
		b.y = a.y * m.b.x + b.y * m.b.y + c.y * m.b.z;
		b.z = a.z * m.b.x + b.z * m.b.y + c.z * m.b.z;
		c.x = a.x * m.c.x + b.x * m.c.y + c.x * m.c.z;
		c.y = a.y * m.c.x + b.y * m.c.y + c.y * m.c.z;
		c.z = a.z * m.c.x + b.z * m.c.y + c.z * m.c.z;

		return *this;
	}

	constexpr Matrix3 operator+(const Matrix3& m) const { return { a + m.a, b + m.b, c + m.c }; }
	constexpr Matrix3 operator-(const Matrix3& m) const { return { a - m.a, b - m.b, c - m.c }; }
	constexpr Matrix3 operator*(const Matrix3& m) const
	{
		return {
			a.x * m.a.x + b.x * m.a.y + c.x * m.a.z,
			a.y * m.a.x + b.y * m.a.y + c.y * m.a.z,
			a.z * m.a.x + b.z * m.a.y + c.z * m.a.z,
			a.x * m.b.x + b.x * m.b.y + c.x * m.b.z,
			a.y * m.b.x + b.y * m.b.y + c.y * m.b.z,
			a.z * m.b.x + b.z * m.b.y + c.z * m.b.z,
			a.x * m.c.x + b.x * m.c.y + c.x * m.c.z,
			a.y * m.c.x + b.y * m.c.y + c.y * m.c.z,
			a.z * m.c.x + b.z * m.c.y + c.z * m.c.z
		};
	}
	constexpr Vector3 operator*(const Vector3& v) const
	{
		return {
			a.x * v.x + b.x * v.y + c.x * v.z,
			a.y * v.x + b.y * v.y + c.y * v.z,
			a.z * v.x + b.z * v.y + c.z * v.z
		};
	}

	constexpr Matrix3 operator-() const { return { -a, -b, -c }; }
	constexpr Matrix3 operator~() const { return { -a, -b, -c }; }
	constexpr Matrix3 operator!() const { return { -a, -b, -c }; }
	constexpr bool operator==(const Matrix3& m) const { return a == m.a && b == m.b && c == m.c; }
	constexpr bool operator!=(const Matrix3& m) const { return a != m.a || b != m.b || c != m.c; }

	const Vector3& operator[] (U8 i) const { return (&a)[i]; }
	Vector3& operator[] (U8 i) { return (&a)[i]; }

	F32* Data() { return a.Data(); }
	const F32* Data() const { return a.Data(); }

public:
	Vector3 a, b, c; //Columns
};

struct NH_API Matrix4
{
	constexpr Matrix4() : a{ 1.0f, 0.0f, 0.0f, 0.0f }, b{ 0.0f, 1.0f, 0.0f, 0.0f }, c{ 0.0f, 0.0f, 1.0f, 0.0f }, d{ 0.0f, 0.0f, 0.0f, 1.0f } {}
	constexpr Matrix4(F32 ax, F32 ay, F32 az, F32 aw, F32 bx, F32 by, F32 bz, F32 bw, F32 cx, F32 cy, F32 cz, F32 cw, F32 dx, F32 dy, F32 dz, F32 dw) :
		a{ ax, ay, az, aw }, b{ bx, by, bz, bw }, c{ cx, cy, cz, cw }, d{ dx, dy, dz, dw }
	{
	}
	constexpr Matrix4(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d) : a{ a }, b{ b }, c{ c }, d{ d } {}
	constexpr Matrix4(Vector4&& a, Vector4&& b, Vector4&& c, Vector4&& d) noexcept : a{ a }, b{ b }, c{ c }, d{ d } {}
	constexpr Matrix4(const Matrix4& m) : a{ m.a }, b{ m.b }, c{ m.c }, d{ m.d } {}
	constexpr Matrix4(const Matrix3& m) : a{ m.a.x, m.a.y, 0.0f, m.a.z }, b{ m.b.x, m.b.y, 0.0f, m.b.z }, c{ 0.0f, 0.0f, 1.0f, m.b.z }, d{ m.c.x, m.c.y, 0.0f, 1.0f } {}
	constexpr Matrix4(Matrix4&& m) noexcept : a{ m.a }, b{ m.b }, c{ m.c }, d{ m.d } {}
	Matrix4(const Vector3& position, const Vector3& rotation = { 0.0f }, const Vector3& scale = { 1.0f });
	Matrix4(const Vector3& position, const Quaternion3& rotation, const Vector3& scale = { 1.0f });

	constexpr Matrix4& operator= (const Matrix4& m) { a = m.a; b = m.b; c = m.c; d = m.d; return *this; }
	constexpr Matrix4& operator= (Matrix4&& m) noexcept { a = m.a; b = m.b; c = m.c; d = m.d; return *this; }
	constexpr Matrix4& operator+= (const Matrix4& m) { a += m.a; b += m.b; c += m.c; d += m.d; return *this; }
	constexpr Matrix4& operator-= (const Matrix4& m) { a -= m.a; b -= m.b; c -= m.c; d -= m.d; return *this; }
	constexpr Matrix4& operator*= (const Matrix4& m)
	{
		a.x = a.x * m.a.x + b.x * m.a.y + c.x * m.a.z + c.x * m.a.w;
		a.y = a.y * m.a.x + b.y * m.a.y + c.y * m.a.z + c.y * m.a.w;
		a.z = a.z * m.a.x + b.z * m.a.y + c.z * m.a.z + c.z * m.a.w;
		a.w = a.w * m.a.x + b.w * m.a.y + c.w * m.a.z + c.w * m.a.w;
		b.x = a.x * m.b.x + b.x * m.b.y + c.x * m.b.z + c.x * m.b.w;
		b.y = a.y * m.b.x + b.y * m.b.y + c.y * m.b.z + c.y * m.b.w;
		b.z = a.z * m.b.x + b.z * m.b.y + c.z * m.b.z + c.z * m.b.w;
		b.w = a.w * m.b.x + b.w * m.b.y + c.w * m.b.z + c.w * m.b.w;
		c.x = a.x * m.c.x + b.x * m.c.y + c.x * m.c.z + c.x * m.c.w;
		c.y = a.y * m.c.x + b.y * m.c.y + c.y * m.c.z + c.y * m.c.w;
		c.z = a.z * m.c.x + b.z * m.c.y + c.z * m.c.z + c.z * m.c.w;
		c.w = a.w * m.c.x + b.w * m.c.y + c.w * m.c.z + c.w * m.c.w;
		d.x = a.x * m.d.x + b.x * m.d.y + c.x * m.d.z + c.x * m.d.w;
		d.y = a.y * m.d.x + b.y * m.d.y + c.y * m.d.z + c.y * m.d.w;
		d.z = a.z * m.d.x + b.z * m.d.y + c.z * m.d.z + c.z * m.d.w;
		d.w = a.w * m.d.x + b.w * m.d.y + c.w * m.d.z + c.w * m.d.w;

		return *this;
	}

	constexpr Matrix4 operator+(const Matrix4& m) const { return { a + m.a, b + m.b, c + m.c, d + m.d }; }
	constexpr Matrix4 operator-(const Matrix4& m) const { return { a - m.a, b - m.b, c - m.c, d + m.d }; }
	constexpr Matrix4 operator*(const Matrix4& m) const
	{
		//#if defined NH_AVX
		//
		//#elif defined NH_SSE || defined NH_SSE2 //TODO: Failure
		//	M128 l, r0, r1, r2, r3, v0, v1, v2, v3;
		//	Vector4 f0, f1, f2, f3;
		//
		//	l = _mm_load_ps(a.Data());
		//	r0 = _mm_load_ps(m.a.Data());
		//	r1 = _mm_load_ps(m.b.Data());
		//	r2 = _mm_load_ps(m.c.Data());
		//	r3 = _mm_load_ps(m.d.Data());
		//
		//	v0 = _mm_mul_ps(_mm_shuffle_ps(r0, r0, _MM_SHUFFLE(0, 0, 0, 0)), l);
		//	v1 = _mm_mul_ps(_mm_shuffle_ps(r1, r1, _MM_SHUFFLE(0, 0, 0, 0)), l);
		//	v2 = _mm_mul_ps(_mm_shuffle_ps(r2, r2, _MM_SHUFFLE(0, 0, 0, 0)), l);
		//	v3 = _mm_mul_ps(_mm_shuffle_ps(r0, r3, _MM_SHUFFLE(0, 0, 0, 0)), l);
		//
		//	l = _mm_load_ps(m.b.Data());
		//	v0 = _mm_add_ps(v0, _mm_mul_ps(_mm_shuffle_ps(r0, r0, _MM_SHUFFLE(1, 1, 1, 1)), l));
		//	v1 = _mm_add_ps(v1, _mm_mul_ps(_mm_shuffle_ps(r1, r1, _MM_SHUFFLE(1, 1, 1, 1)), l));
		//	v2 = _mm_add_ps(v2, _mm_mul_ps(_mm_shuffle_ps(r2, r2, _MM_SHUFFLE(1, 1, 1, 1)), l));
		//	v3 = _mm_add_ps(v3, _mm_mul_ps(_mm_shuffle_ps(r3, r3, _MM_SHUFFLE(1, 1, 1, 1)), l));
		//
		//	l = _mm_load_ps(m.c.Data());
		//	v0 = _mm_add_ps(v0, _mm_mul_ps(_mm_shuffle_ps(r0, r0, _MM_SHUFFLE(2, 2, 2, 2)), l));
		//	v1 = _mm_add_ps(v1, _mm_mul_ps(_mm_shuffle_ps(r1, r1, _MM_SHUFFLE(2, 2, 2, 2)), l));
		//	v2 = _mm_add_ps(v2, _mm_mul_ps(_mm_shuffle_ps(r2, r2, _MM_SHUFFLE(2, 2, 2, 2)), l));
		//	v3 = _mm_add_ps(v3, _mm_mul_ps(_mm_shuffle_ps(r3, r3, _MM_SHUFFLE(2, 2, 2, 2)), l));
		//
		//	l = _mm_load_ps(m.d.Data());
		//	v0 = _mm_add_ps(v0, _mm_mul_ps(_mm_shuffle_ps(r0, r0, _MM_SHUFFLE(3, 3, 3, 3)), l));
		//	v1 = _mm_add_ps(v1, _mm_mul_ps(_mm_shuffle_ps(r1, r1, _MM_SHUFFLE(3, 3, 3, 3)), l));
		//	v2 = _mm_add_ps(v2, _mm_mul_ps(_mm_shuffle_ps(r2, r2, _MM_SHUFFLE(3, 3, 3, 3)), l));
		//	v3 = _mm_add_ps(v3, _mm_mul_ps(_mm_shuffle_ps(r3, r3, _MM_SHUFFLE(3, 3, 3, 3)), l));
		//
		//	_mm_store_ps(f0.Data(), v0);
		//	_mm_store_ps(f1.Data(), v1);
		//	_mm_store_ps(f2.Data(), v2);
		//	_mm_store_ps(f3.Data(), v3);
		//
		//	return { f0, f1, f2, f3 };
		//#elif defined NH_ARM_NEON
		//
		//#else
		return {
			a.x * m.a.x + b.x * m.a.y + c.x * m.a.z + d.x * m.a.w,
			a.y * m.a.x + b.y * m.a.y + c.y * m.a.z + d.y * m.a.w,
			a.z * m.a.x + b.z * m.a.y + c.z * m.a.z + d.z * m.a.w,
			a.w * m.a.x + b.w * m.a.y + c.w * m.a.z + d.w * m.a.w,
			a.x * m.b.x + b.x * m.b.y + c.x * m.b.z + d.x * m.b.w,
			a.y * m.b.x + b.y * m.b.y + c.y * m.b.z + d.y * m.b.w,
			a.z * m.b.x + b.z * m.b.y + c.z * m.b.z + d.z * m.b.w,
			a.w * m.b.x + b.w * m.b.y + c.w * m.b.z + d.w * m.b.w,
			a.x * m.c.x + b.x * m.c.y + c.x * m.c.z + d.x * m.c.w,
			a.y * m.c.x + b.y * m.c.y + c.y * m.c.z + d.y * m.c.w,
			a.z * m.c.x + b.z * m.c.y + c.z * m.c.z + d.z * m.c.w,
			a.w * m.c.x + b.w * m.c.y + c.w * m.c.z + d.w * m.c.w,
			a.x * m.d.x + b.x * m.d.y + c.x * m.d.z + d.x * m.d.w,
			a.y * m.d.x + b.y * m.d.y + c.y * m.d.z + d.y * m.d.w,
			a.z * m.d.x + b.z * m.d.y + c.z * m.d.z + d.z * m.d.w,
			a.w * m.d.x + b.w * m.d.y + c.w * m.d.z + d.w * m.d.w
		};
		//#endif
	}
	constexpr Vector2 operator*(const Vector2& v) const
	{
		return {
			a.x * v.x + b.x * v.y,
			a.y * v.x + b.y * v.y
		};
	}
	constexpr Vector3 operator*(const Vector3& v) const
	{
		return {
			a.x * v.x + b.x * v.y + c.x * v.z,
			a.y * v.x + b.y * v.y + c.y * v.z,
			a.z * v.x + b.z * v.y + c.z * v.z
		};
	}
	constexpr Vector4 operator*(const Vector4& v) const
	{
		return {
			a.x * v.x + b.x * v.y + c.x * v.z + c.x * v.w,
			a.y * v.x + b.y * v.y + c.y * v.z + c.y * v.w,
			a.z * v.x + b.z * v.y + c.z * v.z + c.z * v.w,
			a.z * v.x + b.z * v.y + c.z * v.z + c.z * v.w
		};
	}

	constexpr Matrix4 Inverse() const
	{
		Matrix4 m;

		F32 t0 = c.z * d.w;
		F32 t1 = d.z * c.w;
		F32 t2 = b.z * d.w;
		F32 t3 = d.z * b.w;
		F32 t4 = b.z * c.w;
		F32 t5 = c.z * b.w;
		F32 t6 = a.z * d.w;
		F32 t7 = d.z * a.w;
		F32 t8 = a.z * c.w;
		F32 t9 = c.z * a.w;
		F32 t10 = a.z * b.w;
		F32 t11 = b.z * a.w;
		F32 t12 = c.x * d.y;
		F32 t13 = d.x * c.y;
		F32 t14 = b.x * d.y;
		F32 t15 = d.x * b.y;
		F32 t16 = b.x * c.y;
		F32 t17 = c.x * b.y;
		F32 t18 = a.x * d.y;
		F32 t19 = d.x * a.y;
		F32 t20 = a.x * c.y;
		F32 t21 = c.x * a.y;
		F32 t22 = a.x * b.y;
		F32 t23 = b.x * a.y;

		m.a.x = (t0 * b.y + t3 * c.y + t4 * d.y) - (t1 * b.y + t2 * c.y + t5 * d.y);
		m.a.y = (t1 * a.y + t6 * c.y + t9 * d.y) - (t0 * a.y + t7 * c.y + t8 * d.y);
		m.a.z = (t2 * a.y + t7 * b.y + t10 * d.y) - (t3 * a.y + t6 * b.y + t11 * d.y);
		m.a.w = (t5 * a.y + t8 * b.y + t11 * c.y) - (t4 * a.y + t9 * b.y + t10 * c.y);

		F32 determinant = (a.x * m.a.x + b.x * m.a.y + c.x * m.a.z + d.x * m.a.w);
		if (Math::IsZero(determinant)) { return Matrix4{ }; }
		F32 f = 1.0f / determinant;

		m.a.x = f * m.a.x;
		m.a.y = f * m.a.y;
		m.a.z = f * m.a.z;
		m.a.w = f * m.a.w;
		m.b.x = f * ((t1 * b.x + t2 * c.x + t5 * d.x) - (t0 * b.x + t3 * c.x + t4 * d.x));
		m.b.y = f * ((t0 * a.x + t7 * c.x + t8 * d.x) - (t1 * a.x + t6 * c.x + t9 * d.x));
		m.b.z = f * ((t3 * a.x + t6 * b.x + t11 * d.x) - (t2 * a.x + t7 * b.x + t10 * d.x));
		m.b.w = f * ((t4 * a.x + t9 * b.x + t10 * c.x) - (t5 * a.x + t8 * b.x + t11 * c.x));
		m.c.x = f * ((t12 * b.w + t15 * c.w + t16 * d.w) - (t13 * b.w + t14 * c.w + t17 * d.w));
		m.c.y = f * ((t13 * a.w + t18 * c.w + t21 * d.w) - (t12 * a.w + t19 * c.w + t20 * d.w));
		m.c.z = f * ((t14 * a.w + t19 * b.w + t22 * d.w) - (t15 * a.w + t18 * b.w + t23 * d.w));
		m.c.w = f * ((t17 * a.w + t20 * b.w + t23 * c.w) - (t16 * a.w + t21 * b.w + t22 * c.w));
		m.d.x = f * ((t14 * c.z + t17 * d.z + t13 * b.z) - (t16 * d.z + t12 * b.z + t15 * c.z));
		m.d.y = f * ((t20 * d.z + t12 * a.z + t19 * c.z) - (t18 * c.z + t21 * d.z + t13 * a.z));
		m.d.z = f * ((t18 * b.z + t23 * d.z + t15 * a.z) - (t22 * d.z + t14 * a.z + t19 * b.z));
		m.d.w = f * ((t22 * c.z + t16 * a.z + t21 * b.z) - (t20 * b.z + t23 * c.z + t17 * a.z));

		return m;
	}
	constexpr Matrix4& Inversed()
	{
		F32 t0 = c.z * d.w;
		F32 t1 = d.z * c.w;
		F32 t2 = b.z * d.w;
		F32 t3 = d.z * b.w;
		F32 t4 = b.z * c.w;
		F32 t5 = c.z * b.w;
		F32 t6 = a.z * d.w;
		F32 t7 = d.z * a.w;
		F32 t8 = a.z * c.w;
		F32 t9 = c.z * a.w;
		F32 t10 = a.z * b.w;
		F32 t11 = b.z * a.w;
		F32 t12 = c.x * d.y;
		F32 t13 = d.x * c.y;
		F32 t14 = b.x * d.y;
		F32 t15 = d.x * b.y;
		F32 t16 = b.x * c.y;
		F32 t17 = c.x * b.y;
		F32 t18 = a.x * d.y;
		F32 t19 = d.x * a.y;
		F32 t20 = a.x * c.y;
		F32 t21 = c.x * a.y;
		F32 t22 = a.x * b.y;
		F32 t23 = b.x * a.y;

		F32 ax = (t0 * b.y + t3 * c.y + t4 * d.y) - (t1 * b.y + t2 * c.y + t5 * d.y);
		F32 ay = (t1 * a.y + t6 * c.y + t9 * d.y) - (t0 * a.y + t7 * c.y + t8 * d.y);
		F32 az = (t2 * a.y + t7 * b.y + t10 * d.y) - (t3 * a.y + t6 * b.y + t11 * d.y);
		F32 aw = (t5 * a.y + t8 * b.y + t11 * c.y) - (t4 * a.y + t9 * b.y + t10 * c.y);

		F32 determinant = (a.x * ax + b.x * ay + c.x * az + d.x * aw);
		if (Math::IsZero(determinant)) { return *this = Matrix4{}; }
		F32 f = 1.0f / determinant;

		a.x = f * ax;
		a.y = f * ay;
		a.z = f * az;
		a.w = f * aw;

		F32 bx = f * ((t1 * b.x + t2 * c.x + t5 * d.x) - (t0 * b.x + t3 * c.x + t4 * d.x));
		F32 by = f * ((t0 * a.x + t7 * c.x + t8 * d.x) - (t1 * a.x + t6 * c.x + t9 * d.x));
		F32 bz = f * ((t3 * a.x + t6 * b.x + t11 * d.x) - (t2 * a.x + t7 * b.x + t10 * d.x));
		F32 bw = f * ((t4 * a.x + t9 * b.x + t10 * c.x) - (t5 * a.x + t8 * b.x + t11 * c.x));
		F32 cx = f * ((t12 * b.w + t15 * c.w + t16 * d.w) - (t13 * b.w + t14 * c.w + t17 * d.w));
		F32 cy = f * ((t13 * a.w + t18 * c.w + t21 * d.w) - (t12 * a.w + t19 * c.w + t20 * d.w));
		F32 cz = f * ((t14 * a.w + t19 * b.w + t22 * d.w) - (t15 * a.w + t18 * b.w + t23 * d.w));
		F32 cw = f * ((t17 * a.w + t20 * b.w + t23 * c.w) - (t16 * a.w + t21 * b.w + t22 * c.w));
		F32 dx = f * ((t14 * c.z + t17 * d.z + t13 * b.z) - (t16 * d.z + t12 * b.z + t15 * c.z));
		F32 dy = f * ((t20 * d.z + t12 * a.z + t19 * c.z) - (t18 * c.z + t21 * d.z + t13 * a.z));
		F32 dz = f * ((t18 * b.z + t23 * d.z + t15 * a.z) - (t22 * d.z + t14 * a.z + t19 * b.z));
		F32 dw = f * ((t22 * c.z + t16 * a.z + t21 * b.z) - (t20 * b.z + t23 * c.z + t17 * a.z));

		b.x = bx;
		b.y = by;
		b.z = bz;
		b.w = bw;
		c.x = cx;
		c.y = cy;
		c.z = cz;
		c.w = cw;
		d.x = dx;
		d.y = dy;
		d.z = dz;
		d.w = dw;

		return *this;
	}
	constexpr Matrix4 Invert() const
	{
		Matrix4 m;

		F32 t0 = c.z * d.w;
		F32 t1 = d.z * c.w;
		F32 t2 = b.z * d.w;
		F32 t3 = d.z * b.w;
		F32 t4 = b.z * c.w;
		F32 t5 = c.z * b.w;
		F32 t6 = a.z * d.w;
		F32 t7 = d.z * a.w;
		F32 t8 = a.z * c.w;
		F32 t9 = c.z * a.w;
		F32 t10 = a.z * b.w;
		F32 t11 = b.z * a.w;
		F32 t12 = c.x * d.y;
		F32 t13 = d.x * c.y;
		F32 t14 = b.x * d.y;
		F32 t15 = d.x * b.y;
		F32 t16 = b.x * c.y;
		F32 t17 = c.x * b.y;
		F32 t18 = a.x * d.y;
		F32 t19 = d.x * a.y;
		F32 t20 = a.x * c.y;
		F32 t21 = c.x * a.y;
		F32 t22 = a.x * b.y;
		F32 t23 = b.x * a.y;

		F32 ax = (t0 * b.y + t3 * c.y + t4 * d.y) - (t1 * b.y + t2 * c.y + t5 * d.y);
		F32 ay = (t1 * a.y + t6 * c.y + t9 * d.y) - (t0 * a.y + t7 * c.y + t8 * d.y);
		F32 az = (t2 * a.y + t7 * b.y + t10 * d.y) - (t3 * a.y + t6 * b.y + t11 * d.y);
		F32 aw = (t5 * a.y + t8 * b.y + t11 * c.y) - (t4 * a.y + t9 * b.y + t10 * c.y);

		F32 f = 1.0f / (a.x * ax + b.x * ay + c.x * az + d.x * aw);

		return {
			f * ax, f * ay, f * az, f * aw,
			f * ((t1 * b.x + t2 * c.x + t5 * d.x) - (t0 * b.x + t3 * c.x + t4 * d.x)),
			f * ((t0 * a.x + t7 * c.x + t8 * d.x) - (t1 * a.x + t6 * c.x + t9 * d.x)),
			f * ((t3 * a.x + t6 * b.x + t11 * d.x) - (t2 * a.x + t7 * b.x + t10 * d.x)),
			f * ((t4 * a.x + t9 * b.x + t10 * c.x) - (t5 * a.x + t8 * b.x + t11 * c.x)),
			f * ((t12 * b.w + t15 * c.w + t16 * d.w) - (t13 * b.w + t14 * c.w + t17 * d.w)),
			f * ((t13 * a.w + t18 * c.w + t21 * d.w) - (t12 * a.w + t19 * c.w + t20 * d.w)),
			f * ((t14 * a.w + t19 * b.w + t22 * d.w) - (t15 * a.w + t18 * b.w + t23 * d.w)),
			f * ((t17 * a.w + t20 * b.w + t23 * c.w) - (t16 * a.w + t21 * b.w + t22 * c.w)),
			f * ((t14 * c.z + t17 * d.z + t13 * b.z) - (t16 * d.z + t12 * b.z + t15 * c.z)),
			f * ((t20 * d.z + t12 * a.z + t19 * c.z) - (t18 * c.z + t21 * d.z + t13 * a.z)),
			f * ((t18 * b.z + t23 * d.z + t15 * a.z) - (t22 * d.z + t14 * a.z + t19 * b.z)),
			f * ((t22 * c.z + t16 * a.z + t21 * b.z) - (t20 * b.z + t23 * c.z + t17 * a.z))
		};
	}
	constexpr Matrix4& Inverted()
	{
		Matrix4 m;

		F32 t0 = c.z * d.w;
		F32 t1 = d.z * c.w;
		F32 t2 = b.z * d.w;
		F32 t3 = d.z * b.w;
		F32 t4 = b.z * c.w;
		F32 t5 = c.z * b.w;
		F32 t6 = a.z * d.w;
		F32 t7 = d.z * a.w;
		F32 t8 = a.z * c.w;
		F32 t9 = c.z * a.w;
		F32 t10 = a.z * b.w;
		F32 t11 = b.z * a.w;
		F32 t12 = c.x * d.y;
		F32 t13 = d.x * c.y;
		F32 t14 = b.x * d.y;
		F32 t15 = d.x * b.y;
		F32 t16 = b.x * c.y;
		F32 t17 = c.x * b.y;
		F32 t18 = a.x * d.y;
		F32 t19 = d.x * a.y;
		F32 t20 = a.x * c.y;
		F32 t21 = c.x * a.y;
		F32 t22 = a.x * b.y;
		F32 t23 = b.x * a.y;

		F32 ax = (t0 * b.y + t3 * c.y + t4 * d.y) - (t1 * b.y + t2 * c.y + t5 * d.y);
		F32 ay = (t1 * a.y + t6 * c.y + t9 * d.y) - (t0 * a.y + t7 * c.y + t8 * d.y);
		F32 az = (t2 * a.y + t7 * b.y + t10 * d.y) - (t3 * a.y + t6 * b.y + t11 * d.y);
		F32 aw = (t5 * a.y + t8 * b.y + t11 * c.y) - (t4 * a.y + t9 * b.y + t10 * c.y);

		F32 f = 1.0f / (a.x * ax + b.x * ay + c.x * az + d.x * aw);

		m.a.x = f * ax;
		m.a.y = f * ay;
		m.a.z = f * az;
		m.a.w = f * aw;
		m.b.x = f * ((t1 * b.x + t2 * c.x + t5 * d.x) - (t0 * b.x + t3 * c.x + t4 * d.x));
		m.b.y = f * ((t0 * a.x + t7 * c.x + t8 * d.x) - (t1 * a.x + t6 * c.x + t9 * d.x));
		m.b.z = f * ((t3 * a.x + t6 * b.x + t11 * d.x) - (t2 * a.x + t7 * b.x + t10 * d.x));
		m.b.w = f * ((t4 * a.x + t9 * b.x + t10 * c.x) - (t5 * a.x + t8 * b.x + t11 * c.x));
		m.c.x = f * ((t12 * b.w + t15 * c.w + t16 * d.w) - (t13 * b.w + t14 * c.w + t17 * d.w));
		m.c.y = f * ((t13 * a.w + t18 * c.w + t21 * d.w) - (t12 * a.w + t19 * c.w + t20 * d.w));
		m.c.z = f * ((t14 * a.w + t19 * b.w + t22 * d.w) - (t15 * a.w + t18 * b.w + t23 * d.w));
		m.c.w = f * ((t17 * a.w + t20 * b.w + t23 * c.w) - (t16 * a.w + t21 * b.w + t22 * c.w));
		m.d.x = f * ((t14 * c.z + t17 * d.z + t13 * b.z) - (t16 * d.z + t12 * b.z + t15 * c.z));
		m.d.y = f * ((t20 * d.z + t12 * a.z + t19 * c.z) - (t18 * c.z + t21 * d.z + t13 * a.z));
		m.d.z = f * ((t18 * b.z + t23 * d.z + t15 * a.z) - (t22 * d.z + t14 * a.z + t19 * b.z));
		m.d.w = f * ((t22 * c.z + t16 * a.z + t21 * b.z) - (t20 * b.z + t23 * c.z + t17 * a.z));

		a = m.a;
		b = m.b;
		c = m.c;
		d = m.d;

		return *this;
	}
	constexpr Matrix4 Transpose() const
	{
		return {
			a.x, b.x, c.x, d.x,
			a.y, b.y, c.y, d.y,
			a.z, b.z, c.z, d.z,
			a.w, b.w, c.w, c.z
		};
	}
	constexpr Matrix4& Transposed()
	{
		F32 bx = a.y;
		F32 cx = a.z;
		F32 cy = b.z;
		F32 dx = a.w;
		F32 dy = b.w;
		F32 dz = c.w;

		a.y = b.x;
		a.z = c.x;
		a.w = d.x;
		b.z = c.y;
		b.w = d.y;
		c.w = d.z;

		b.x = bx;
		c.x = cx;
		c.y = cy;
		d.x = dx;
		d.y = dy;
		d.z = dz;

		return *this;
	}

	void Set(const Vector3& position, const Vector3& rotation, const Vector3& scale);
	void Set(const Vector3& position, const Quaternion3& rotation, const Vector3& scale);
	constexpr void SetPerspective(F32 fov, F32 aspect, F32 near, F32 far)
	{
		F32 yScale = 1.0f / Math::Tan(fov * DEG_TO_RAD_F * 0.5f);
		F32 xScale = yScale / aspect;
		F32 nearFar = 1.0f / (near - far);

		a.x = xScale;
		a.y = 0.0f;
		a.z = 0.0f;
		a.w = 0.0f;

		b.x = 0.0f;
		b.y = -yScale;
		b.z = 0.0f;
		b.w = 0.0f;

		c.x = 0.0f;
		c.y = 0.0f;
		c.z = -far * nearFar;
		c.w = 1.0f;

		d.x = 0.0f;
		d.y = 0.0f;
		d.z = far * near * nearFar;
		d.w = 0.0f;
	}
	constexpr void SetOrthographic(F32 left, F32 right, F32 bottom, F32 top, F32 near, F32 far)
	{
		F32 rightLeft = 1.0f / (right - left);
		F32 bottomTop = 1.0f / (bottom - top);
		F32 farNear = 1.0f / (far - near);

		a.x = 2.0f * rightLeft;
		a.y = 0.0f;
		a.z = 0.0f;
		a.w = 0.0f;

		b.x = 0.0f;
		b.y = 2.0f * bottomTop;
		b.z = 0.0f;
		b.w = 0.0f;

		c.x = 0.0f;
		c.y = 0.0f;
		c.z = 2.0f * farNear;
		c.w = 0.0f;

		d.x = -(right + left) * rightLeft;
		d.y = -(bottom + top) * bottomTop;
		d.z = -(far + near) * farNear;
		d.w = 1.0f;
	}

	constexpr void LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
	{
		Vector3 f = center - eye;
		f.Normalize();

		Vector3 s = f.Cross(up);
		s.Normalize();

		Vector3 u = s.Cross(f);

		a.x = s.x;
		a.y = u.x;
		a.z = f.x;
		a.w = 0.0f;

		b.x = s.y;
		b.y = u.y;
		b.z = f.y;
		b.w = 0.0f;

		c.x = s.z;
		c.y = u.z;
		c.z = f.z;
		c.w = 0.0f;

		d.x = -s.Dot(eye);
		d.y = -u.Dot(eye);
		d.z = -f.Dot(eye);
		d.w = 1.0f;
	}

	constexpr Vector3 Forward() { return Vector3(-a.z, -b.z, -c.z).Normalize(); }
	constexpr Vector3 Back() { return Vector3(a.z, b.z, c.z).Normalize(); }
	constexpr Vector3 Right() { return Vector3(a.x, b.x, c.x).Normalize(); }
	constexpr Vector3 Left() { return Vector3(-a.x, -b.x, -c.x).Normalize(); }
	constexpr Vector3 Up() { return Vector3(a.y, b.y, c.y).Normalize(); }
	constexpr Vector3 Down() { return Vector3(-a.y, -b.y, -c.y).Normalize(); }

	constexpr Matrix4 operator-() { return { -a, -b, -c, -d }; }
	constexpr Matrix4 operator~() { return { -a, -b, -c, -d }; }
	constexpr Matrix4 operator!() { return { -a, -b, -c, -d }; }

	constexpr bool operator==(const Matrix4& m) const { return a == m.a && b == m.b && c == m.c && d == m.d; }
	constexpr bool operator!=(const Matrix4& m) const { return a != m.a || b != m.b || c != m.c || d != m.d; }

	Vector4& operator[] (U8 i) { return (&a)[i]; }
	const Vector4& operator[] (U8 i) const { return (&a)[i]; }

	F32* Data() { return a.Data(); }
	const F32* Data() const { return a.Data(); }

public:
	Vector4 a, b, c, d; //Columns
};

//TODO: Cache euler angles
struct NH_API Quaternion3
{
	constexpr Quaternion3() : x{ 0.0f }, y{ 0.0f }, z{ 0.0f }, w{ 1.0f } {}
	constexpr Quaternion3(F32 x, F32 y, F32 z, F32 w) : x{ x }, y{ y }, z{ z }, w{ w } {}
	constexpr Quaternion3(const Vector3& euler)
	{
		F32 hx = euler.x * DEG_TO_RAD_F * 0.5f;
		F32 hy = euler.y * DEG_TO_RAD_F * 0.5f;
		F32 hz = euler.z * DEG_TO_RAD_F * 0.5f;

		F32 c0 = Math::Cos(hx);
		F32 c1 = Math::Cos(hy);
		F32 c2 = Math::Cos(hz);
		F32 s0 = Math::Sin(hx);
		F32 s1 = Math::Sin(hy);
		F32 s2 = Math::Sin(hz);

		F32 c0c1 = c0 * c1;
		F32 s0s1 = s0 * s1;
		F32 s0c1 = s0 * c1;
		F32 c0s1 = c0 * s1;

		x = s0c1 * c2 - c0c1 * s2;
		y = c0s1 * c2 + s0c1 * s2;
		z = c0c1 * s2 - s0s1 * c2;
		w = c0c1 * c2 + s0s1 * s2;
	}
	constexpr Quaternion3(const Vector3& axis, F32 angle)
	{
		const F32 halfAngle = angle * DEG_TO_RAD_F * 0.5f;
		F32 s = Math::Sin(halfAngle);
		F32 c = Math::Cos(halfAngle);

		//TODO: Normalize axis?

		x = s * axis.x;
		y = s * axis.y;
		z = s * axis.z;
		w = c;
	}
	constexpr Quaternion3(const Quaternion3& q) : x{ q.x }, y{ q.y }, z{ q.z }, w{ q.w } {}
	constexpr Quaternion3(Quaternion3&& q) noexcept : x{ q.x }, y{ q.y }, z{ q.z }, w{ q.w } {}

	constexpr Quaternion3& operator=(const Vector3& euler)
	{
		F32 hx = euler.x * DEG_TO_RAD_F * 0.5f;
		F32 hy = euler.y * DEG_TO_RAD_F * 0.5f;
		F32 hz = euler.z * DEG_TO_RAD_F * 0.5f;

		F32 c0 = Math::Cos(hx);
		F32 c1 = Math::Cos(hy);
		F32 c2 = Math::Cos(hz);
		F32 s0 = Math::Sin(hx);
		F32 s1 = Math::Sin(hy);
		F32 s2 = Math::Sin(hz);

		F32 c0c1 = c0 * c1;
		F32 s0s1 = s0 * s1;
		F32 s0c1 = s0 * c1;
		F32 c0s1 = c0 * s1;

		x = c0c1 * c2 + s0s1 * s2;
		y = s0c1 * c2 - c0c1 * s2;
		z = c0s1 * c2 + s0c1 * s2;
		w = c0c1 * s2 - s0s1 * c2;

		return *this;
	}

	constexpr Quaternion3& operator=(const Quaternion3& q) { x = q.x; y = q.y; z = q.z; w = q.w; return *this; }
	constexpr Quaternion3& operator=(Quaternion3&& q) noexcept { x = q.x; y = q.y; z = q.z; w = q.w; return *this; }

	constexpr Quaternion3& operator+=(const Quaternion3& q)
	{
		x += q.x;
		y += q.y;
		z += q.z;
		w += q.w;

		return *this;
	}
	constexpr Quaternion3& operator-=(const Quaternion3& q)
	{
		x -= q.x;
		y -= q.y;
		z -= q.z;
		w -= q.w;

		return *this;
	}
	constexpr Quaternion3& operator*=(const Quaternion3& q)
	{
		F32 tx = w * q.x + x * q.w + y * q.z - z * q.y;
		F32 ty = w * q.y - x * q.z + y * q.w + z * q.x;
		F32 tz = w * q.z + x * q.y - y * q.x + z * q.w;
		F32 tw = w * q.w - x * q.x - y * q.y - z * q.z;

		x = tx;
		y = ty;
		z = tz;
		w = tw;

		return *this;
	}
	constexpr Quaternion3& operator/=(const Quaternion3& q)
	{
		F32 n2 = 1.0f / q.SqrNormal();

		F32 tx = -w * q.x + x * q.w - y * q.z + z * q.y;
		F32 ty = -w * q.y + x * q.z + y * q.w - z * q.x;
		F32 tz = -w * q.z - x * q.y + y * q.x + z * q.w;
		F32 tw = w * q.w + x * q.x + y * q.y + z * q.z;

		x = tx * n2;
		y = ty * n2;
		x = tz * n2;
		w = tw * n2;

		return *this;
	}

	constexpr Quaternion3 operator+(const Quaternion3& q) const
	{
		return {
			x + q.x,
			y + q.y,
			z + q.z,
			w + q.w
		};
	}
	constexpr Quaternion3 operator-(const Quaternion3& q) const
	{
		return {
			x - q.x,
			y - q.y,
			z - q.z,
			w - q.w
		};
	}
	constexpr Quaternion3 operator*(const Quaternion3& q) const
	{
		F32 tx = w * q.x + x * q.w + y * q.z - z * q.y;
		F32 ty = w * q.y - x * q.z + y * q.w + z * q.x;
		F32 tz = w * q.z + x * q.y - y * q.x + z * q.w;
		F32 tw = w * q.w - x * q.x - y * q.y - z * q.z;

		return {
			tx,
			ty,
			tz,
			tw
		};
	}
	constexpr Quaternion3 operator/(const Quaternion3& q) const
	{
		F32 n2 = 1.0f / q.SqrNormal();

		F32 tx = -w * q.x + x * q.w - y * q.z + z * q.y;
		F32 ty = -w * q.y + x * q.z + y * q.w - z * q.x;
		F32 tz = -w * q.z - x * q.y + y * q.x + z * q.w;
		F32 tw = w * q.w + x * q.x + y * q.y + z * q.z;

		return {
			tx * n2,
			ty * n2,
			tz * n2,
			tw * n2
		};
	}

	constexpr Matrix3 ToMatrix3() const
	{
		Quaternion3 q = Normalize();

		F32 xx = 2.0f * q.x * q.x;
		F32 xy = 2.0f * q.x * q.y;
		F32 xz = 2.0f * q.x * q.z;
		F32 xw = 2.0f * q.x * q.w;
		F32 yy = 2.0f * q.y * q.y;
		F32 yz = 2.0f * q.y * q.z;
		F32 yw = 2.0f * q.y * q.w;
		F32 zz = 2.0f * q.z * q.z;
		F32 zw = 2.0f * q.z * q.w;

		return {
			1.0f - yy - zz, xy + zw, xz - yw,
			xy - zw,  1.0f - xx - zz,  yz + xw,
			xz + yw, yz - xw, 1.0f - xx - yy
		};
	}
	constexpr Matrix4 ToMatrix4() const
	{
		Quaternion3 q = Normalize();

		F32 xx = 2.0f * q.x * q.x;
		F32 xy = 2.0f * q.x * q.y;
		F32 xz = 2.0f * q.x * q.z;
		F32 xw = 2.0f * q.x * q.w;
		F32 yy = 2.0f * q.y * q.y;
		F32 yz = 2.0f * q.y * q.z;
		F32 yw = 2.0f * q.y * q.w;
		F32 zz = 2.0f * q.z * q.z;
		F32 zw = 2.0f * q.z * q.w;

		return {
			1.0f - yy - zz, xy + zw, xz - yw, 0.0f,
			xy - zw,  1.0f - xx - zz,  yz + xw, 0.0f,
			xz + yw, yz - xw, 1.0f - xx - yy, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}

	constexpr Matrix4 RotationMatrix(Vector3 center) const
	{
		Matrix4 matrix{};

		F32 xx = x * x;
		F32 xy = x * y;
		F32 xz = x * z;
		F32 xw = x * w;
		F32 yy = y * y;
		F32 yz = y * z;
		F32 yw = y * w;
		F32 zz = z * z;
		F32 ww = w * w;

		matrix.a.x = xx - yy - zz + ww;
		matrix.b.x = 2.0f * (xy + xw);
		matrix.c.x = 2.0f * (xz - yw);
		matrix.d.x = center.x - center.x * matrix.a.x - center.y * matrix.b.x - center.z * matrix.c.x;

		matrix.a.y = 2.0f * (xy - xw);
		matrix.b.y = -xx + yy - zz + ww;
		matrix.c.y = 2.0f * (yz + xw);
		matrix.d.y = center.y - center.x * matrix.a.y - center.y * matrix.b.y - center.z * matrix.c.y;

		matrix.a.z = 2.0f * (xz + yw);
		matrix.b.z = 2.0f * (yz - xw);
		matrix.c.z = -xx - yy + zz + ww;
		matrix.d.z = center.z - center.x * matrix.a.z - center.y * matrix.b.z - center.z * matrix.c.z;

		return matrix;
	}

	constexpr Vector3 Euler() const
	{
		F32 v = x * y + z * w;

		if (Math::Abs(v - 0.5f) < Traits<F32>::Epsilon) { return { 2.0f * Math::Atan2(x, w) * RAD_TO_DEG_F, HALF_PI_F * RAD_TO_DEG_F, 0.0f }; }
		if (Math::Abs(v + 0.5f) < Traits<F32>::Epsilon) { return { -2.0f * Math::Atan2(x, w) * RAD_TO_DEG_F, -HALF_PI_F * RAD_TO_DEG_F, 0.0f }; }

		return {
			Math::Asin(2.0f * v) * RAD_TO_DEG_F,
			Math::Atan2(2.0f * (w * y - x * z), 1.0f - 2.0f * (y * y + z * z)) * RAD_TO_DEG_F,
			Math::Atan2(2.0f * (w * x - y * z), 1.0f - 2.0f * (x * x + z * z)) * RAD_TO_DEG_F
		};
	}

	Quaternion3 Slerp(const Quaternion3& q, F32 t) const
	{
		constexpr F32 DOT_THRESHOLD = 0.9995f;

		Quaternion3 v0 = Normalize();
		Quaternion3 v1 = q.Normalize();

		F32 dot = v0.Dot(v1);

		if (dot < 0.0f)
		{
			v1.x = -v1.x;
			v1.y = -v1.y;
			v1.z = -v1.z;
			v1.w = -v1.w;
			dot = -dot;
		}

		if (dot > DOT_THRESHOLD)
		{
			Quaternion3 out{
				v0.x + ((v1.x - v0.x) * t),
				v0.y + ((v1.y - v0.y) * t),
				v0.z + ((v1.z - v0.z) * t),
				v0.w + ((v1.w - v0.w) * t)
			};

			return out.Normalize();
		}

		F32 theta0 = Math::Acos(dot);
		F32 theta = theta0 * t;
		F32 sinTheta = Math::Sin(theta);
		F32 sinTheta0 = Math::Sin(theta0);

		F32 s0 = Math::Cos(theta) - dot * sinTheta / sinTheta0;
		F32 s1 = sinTheta / sinTheta0;

		return {
			v0.x * s0 + v1.x * s1,
			v0.y * s0 + v1.y * s1,
			v0.z * s0 + v1.z * s1,
			v0.w * s0 + v1.w * s1
		};
	}
	Quaternion3& Slerped(const Quaternion3& q, F32 t)
	{
		constexpr F32 DOT_THRESHOLD = 0.9995f;

		Quaternion3 v0 = Normalize();
		Quaternion3 v1 = q.Normalize();

		F32 dot = v0.Dot(v1);

		if (dot < 0.0f)
		{
			v1.x = -v1.x;
			v1.y = -v1.y;
			v1.z = -v1.z;
			v1.w = -v1.w;
			dot = -dot;
		}

		if (dot > DOT_THRESHOLD)
		{
			x = v0.x + ((v1.x - v0.x) * t);
			y = v0.y + ((v1.y - v0.y) * t);
			z = v0.z + ((v1.z - v0.z) * t);
			w = v0.w + ((v1.w - v0.w) * t);

			return Normalized();
		}

		F32 theta0 = Math::Acos(dot);
		F32 theta = theta0 * t;
		F32 sinTheta = Math::Sin(theta);
		F32 sinTheta0 = Math::Sin(theta0);

		F32 s0 = Math::Cos(theta) - dot * sinTheta / sinTheta0;
		F32 s1 = sinTheta / sinTheta0;

		x = v0.x * s0 + v1.x * s1;
		y = v0.y * s0 + v1.y * s1;
		z = v0.z * s0 + v1.z * s1;
		w = v0.w * s0 + v1.w * s1;

		return *this;
	}

	constexpr F32 Dot(const Quaternion3& q) const { return x * q.x + y * q.y + z * q.z + w * q.w; }
	constexpr F32 SqrNormal() const { return x * x + y * y + z * z + w * w; }
	constexpr F32 Normal() const { return Math::Sqrt(x * x + y * y + z * z + w * w); }
	constexpr Quaternion3 Normalize() const { F32 n = 1.0f / Normal(); return { x * n, y * n, z * n, w * n }; }
	constexpr Quaternion3& Normalized() { F32 n = 1.0f / Normal(); x *= n; y *= n; z *= n; w *= n; return *this; }
	constexpr Quaternion3 Conjugate() const { return { -x, -y, -z, w }; }
	constexpr Quaternion3& Conjugated() { x = -x; y = -y; z = -z; return *this; }
	constexpr Quaternion3 Inverse() const
	{
		F32 n = 1.0f / Math::Sqrt(x * x + y * y + z * z + w * w);
		return { -x * n, -y * n, -z * n, w * n };
	}
	constexpr Quaternion3& Inversed() { return Conjugated().Normalized(); }

	F32& operator[] (U8 i) { return (&x)[i]; }
	const F32& operator[] (U8 i) const { return (&x)[i]; }

	F32* Data() { return &x; }
	const F32* Data() const { return &x; }

public:
	F32 x, y, z, w;
};

struct NH_API BezierSpline
{
	//template<Vector2... Points>
	//BezierSpline(const Points&... points) {}

	Vector2 operator[](F32 t) const { return Vector2{ 0.0f }; }

private:
	Vector<Vector2> points;
};

inline constexpr Vector2 Vector2Zero{ 0.0f };
inline constexpr Vector2 Vector2One{ 1.0f };
inline constexpr Vector2 Vector2Left{ -1.0f, 0.0f };
inline constexpr Vector2 Vector2Right{ 1.0f, 0.0f };
inline constexpr Vector2 Vector2Up{ 0.0f, 1.0f };
inline constexpr Vector2 Vector2Down{ 0.0f, -1.0f };

inline constexpr Vector3 Vector3Zero{ 0.0f };
inline constexpr Vector3 Vector3One{ 1.0f };
inline constexpr Vector3 Vector3Left{ -1.0f, 0.0f, 0.0f };
inline constexpr Vector3 Vector3Right{ 1.0f, 0.0f, 0.0f };
inline constexpr Vector3 Vector3Up{ 0.0f, 1.0f, 0.0f };
inline constexpr Vector3 Vector3Down{ 0.0f, -1.0f, 0.0f };
inline constexpr Vector3 Vector3Forward{ 0.0f, 0.0f, 1.0f };
inline constexpr Vector3 Vector3Back{ 0.0f, 0.0f, -1.0f };

inline constexpr Vector4 Vector4Zero{ 0.0f };
inline constexpr Vector4 Vector4One{ 1.0f };
inline constexpr Vector4 Vector4Left{ -1.0f, 0.0f, 0.0f, 0.0f };
inline constexpr Vector4 Vector4Right{ 1.0f, 0.0f, 0.0f, 0.0f };
inline constexpr Vector4 Vector4Up{ 0.0f, 1.0f, 0.0f, 0.0f };
inline constexpr Vector4 Vector4Down{ 0.0f, -1.0f, 0.0f, 0.0f };
inline constexpr Vector4 Vector4Forward{ 0.0f, 0.0f, 1.0f, 0.0f };
inline constexpr Vector4 Vector4Back{ 0.0f, 0.0f, -1.0f, 0.0f };
inline constexpr Vector4 Vector4In{ 0.0f, 0.0f, 0.0f, 1.0f };
inline constexpr Vector4 Vector4Out{ 0.0f, 0.0f, 0.0f, -1.0f };

inline constexpr Vector2Int Vector2IntZero{ 0 };
inline constexpr Vector2Int Vector2IntOne{ 1 };
inline constexpr Vector2Int Vector2IntLeft{ -1, 0 };
inline constexpr Vector2Int Vector2IntRight{ 1, 0 };
inline constexpr Vector2Int Vector2IntUp{ 0, 1 };
inline constexpr Vector2Int Vector2IntDown{ 0, -1 };

inline constexpr Vector3Int Vector3IntZero{ 0 };
inline constexpr Vector3Int Vector3IntOne{ 1 };
inline constexpr Vector3Int Vector3IntLeft{ -1, 0, 0 };
inline constexpr Vector3Int Vector3IntRight{ 1, 0, 0 };
inline constexpr Vector3Int Vector3IntUp{ 0, 1, 0 };
inline constexpr Vector3Int Vector3IntDown{ 0, -1, 0 };
inline constexpr Vector3Int Vector3IntForward{ 0, 0, 1 };
inline constexpr Vector3Int Vector3IntBack{ 0, 0, -1 };

inline constexpr Vector4Int Vector4IntZero{ 0 };
inline constexpr Vector4Int Vector4IntOne{ 1 };
inline constexpr Vector4Int Vector4IntLeft{ -1, 0, 0, 0 };
inline constexpr Vector4Int Vector4IntRight{ 1, 0, 0, 0 };
inline constexpr Vector4Int Vector4IntUp{ 0, 1, 0, 0 };
inline constexpr Vector4Int Vector4IntDown{ 0, -1, 0, 0 };
inline constexpr Vector4Int Vector4IntForward{ 0, 0, 1, 0 };
inline constexpr Vector4Int Vector4IntBack{ 0, 0, -1, 0 };
inline constexpr Vector4Int Vector4IntIn{ 0, 0, 0, 1 };
inline constexpr Vector4Int Vector4IntOut{ 0, 0, 0, -1 };

inline constexpr Matrix3 Matrix3Identity{};
inline constexpr Matrix4 Matrix4Identity{};

inline constexpr Quaternion2 Quaternion2Identity{};
inline constexpr Quaternion3 Quaternion3Identity{};