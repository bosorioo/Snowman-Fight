#ifndef _UTILS_H
#define _UTILS_H

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <vector>

#define PI 3.14159265f

namespace Utils
{
	bool isLittleEndian();
	int getRandomInt(int min = 0, int max = 1);
	float getRandomFloat(float min = 0.f, float max = 1.f);
	float toRad(float degrees);
	float toDeg(float radians);
	float degreeRange(float degrees);
	int signalFromBool(bool b);
	void FileWriteBytes(FILE* f, void* data, unsigned bytes, bool forceBigEndian = false);
	void FileReadBytes(FILE* f, void* out, unsigned bytes);
	void VectorWriteBytes(std::vector<unsigned char>& vec, void* data, unsigned bytes);
	template <class T> T vectorLength(T x, T y);
	template <class T> T vectorLength(T x, T y, T z);
	template <class T> T vectorLength(T* vec, unsigned elements);
	template <class T> bool isPositive(T num);
	template <class T> T limit(T min, T num, T max);
	template <class T> T pointDistance(T x1, T y1, T z1, T x2, T y2, T z2);
	template <class T> T pointDistance(T x1, T y1, T x2, T y2);
	template <class T> T pointDistance(T x1, T x2);
	template <class T> void pointAngle(T x1, T y1, T z1, T x2, T y2, T z2, float& omega, float& phi);
	template <class T> T pointAngle(T x1, T y1, T x2, T y2);
	template <class T> void normalize(T points[], int length);
	template <class T, class C> T bitCopy(C bits);
};


template <class T> 
T Utils::vectorLength(T x, T y)
{
	return std::sqrt(x * x + y * y);
}

template <class T> 
T vectorLength(T x, T y, T z)
{
	return std::sqrt(x * x + y * y + z * z);
}

template <class T>
T Utils::vectorLength(T* vec, unsigned elements)
{
	T sum = 0;
	for (; elements >= 1; elements--)
		sum += vec[elements - 1] * vec[elements - 1];
	return std::sqrt(sum);
}

template <class T>
bool Utils::isPositive(T num)
{
	return num >= 0 ? true : false;
}

template <class T>
T Utils::limit(T min, T num, T max)
{
	if (num < min)
		return min;
	else if (num > max)
		return max;

	return num;
}

template <class T>
T Utils::pointDistance(T x1, T y1, T z1, T x2, T y2, T z2)
{
	T difX = x1 - x2, difY = y1 - y2, difZ = z1 - z2;
	return std::sqrt(difX * difX + difY * difY + difZ * difZ);
}

template <class T>
T Utils::pointDistance(T x1, T y1, T x2, T y2)
{
	T difX = x1 - x2, difY = y1 - y2;
	return std::sqrt(difX * difX + difY * difY);
}

template <class T>
T Utils::pointDistance(T x1, T x2)
{
	return std::abs(x1 - x2);
}

template <class T>
void Utils::pointAngle(T x1, T y1, T z1, T x2, T y2, T z2, float& omega, float& phi)
{
	T difX = x2 - x1, difY = y2 - y1, difZ = z2 - z1;
	omega = std::abs(std::atan(difZ / difX));
	phi = std::atan(-difY / std::sqrt(difX * difX + difZ * difZ));

	if (difX < 0 && difZ < 0)
		omega = PI - omega;
	else if (difX < 0 && difZ > 0)
		omega = PI + omega;
	else if (difX > 0 && difZ > 0)
		omega = 2 * PI - omega;

	if (difZ == 0.f && difX == 0.f)
		omega = 0.f;

	if (difX < 0 || difY > 0)
		phi = 2 * PI + phi;
}

template <class T>
T Utils::pointAngle(T x1, T y1, T x2, T y2)
{
	T difX = x2 - x1, difY = y2 - y1;
	T angle = std::atan2(difY, difX);
	return angle > 0 ? angle : angle + 2 * PI;
}

template <class T>
void Utils::normalize(T points[], int length)
{
	int index;
	T sum = 0;

	for (index = 0; index < length; index++)
		sum += points[index] * points[index];

	sum = std::sqrt(sum);

	for (index = 0; index < length; index++)
		points[index] /= sum;
}

template <class T, class C>
T Utils::bitCopy(C bits)
{
	return *(T*)&bits;
}

#endif
