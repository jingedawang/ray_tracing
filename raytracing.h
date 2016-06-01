#pragma once
//Compile using clang under Windows: cl -o RayTracer.exe RayTracer.cpp

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <vector>
#include <iostream>
#include <cassert>
#include <algorithm>

#define M_PI 3.141592653589
#define INFINITY 1e8

using namespace std;

//define class Vec_3, used in ray direction
template<typename T>
class Vec_3
{
public:
	T x, y, z;
	Vec_3() : x(T(0)), y(T(0)), z(T(0)) {}
	Vec_3(T xx) : x(xx), y(xx), z(xx) {}
	Vec_3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}
	Vec_3<T> operator * (const T &f) const
	{
		return Vec_3<T>(x * f, y * f, z * f);
	}
	Vec_3<T> operator * (const Vec_3<T> &v) const
	{
		return Vec_3<T>(x * v.x, y * v.y, z * v.z);
	}
	T dot(const Vec_3<T> &v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}
	Vec_3<T> operator - (const Vec_3<T> &v) const
	{
		return Vec_3<T>(x - v.x, y - v.y, z - v.z);
	}
	Vec_3<T> operator + (const  Vec_3<T> &v) const
	{
		return Vec_3<T>(x + v.x, y + v.y, z + v.z);
	}
	Vec_3<T>& operator += (const Vec_3<T> &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	Vec_3<T>& operator *= (const Vec_3<T> &v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}
	Vec_3<T> operator - () const
	{
		return Vec_3<T>(-x, -y, -z);
	}
	T length2() const
	{
		return x * x + y * y + z * z;
	}
	T length() const
	{
		return sqrt(length2());
	}
	Vec_3& normal()
	{
		T nor2 = length2();
		if (nor2 > 0)
		{
			T nor2_inv = 1 / sqrt(nor2);
			x *= nor2_inv;
			y *= nor2_inv;
			z *= nor2_inv;
		}
		return *this;
	}
	friend std::ostream & operator << (std::ostream &os, const Vec_3<T> &v)
	{
		os << "[" << v.x << " " << v.y << " " << v.z << "]";
		return os;
	}
};

typedef Vec_3<float> Vec_3f;

class Solid
{
public:
	float transparency, reflection;
	Vec_3f surfaceColor, emissionColor;

	Solid(
		const Vec_3f &sc,
		const float &refl = 0,
		const float &transp = 0,
		const Vec_3f &ec = 0);

	virtual bool intersect(const Vec_3f &rayorigin, const Vec_3f & raydirection, float &t0, float &t1) const = 0;

	virtual Vec_3f nhit(const Vec_3f &phit) const = 0 ;
};

//Define Sphere Class
class Sphere : public Solid
{
public:
	Vec_3f center;
	float radius, radius2;
	
	Sphere(
		const Vec_3f &c,
		const float &r,
		const Vec_3f &sc,
		const float &refl = 0,
		const float &transp = 0,
		const Vec_3f &ec = 0);

	//Use geometric solution to solve a ray-sphere intersection 
	bool intersect(const Vec_3f &rayorigin, const Vec_3f & raydirection, float &t0, float &t1) const;

	Vec_3f nhit(const Vec_3f &phit) const;
};

class Cube : public Solid
{
public:
	Vec_3f vertex;
	float xLength, yLength, zLength;

	Cube(
		const Vec_3f &vertex,
		const float &xLength,
		const float &yLength,
		const float &zLength,
		const Vec_3f &sc,
		const float &refl = 0,
		const float &transp = 0);

	virtual bool intersect(const Vec_3f &rayorigin, const Vec_3f & raydirection, float &t0, float &t1) const;

	virtual Vec_3f nhit(const Vec_3f &phit) const;
};

//Define the maximum recursion depth
#define MAX_DEPTH 5

//Calculate the mix value for reflection and refraction
float mix(const float &a, const float &b, const float &mix);

//Ray Tracing Function: takes a ray (defined by its origin and direction) as argument.
//Through the function, we can know if the ray intersects any of the geometry in the scene.
//If the ray intersects an object, calculate the intersection point and its normal, then shade the point.
//Shading depends on the surface (transparent, reflective, diffuse)
//If the ray intersects an object, then return the color of the object at the intersection point, otherwise return the backgroud color.
Vec_3f trace(
	const Vec_3f &rayorigin,
	const Vec_3f &raydirection,
	const std::vector<Solid*> &solids,
	const int &depth
	);

//Render function, compute each pixel of the image.
void render(char *file, const std::vector<Solid*> &solids, const Vec_3f &offset = 0, const float rotateAngle = 0);

void rotateByZ(Vec_3f &direction, float angle);