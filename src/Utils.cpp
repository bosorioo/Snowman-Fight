#include "Utils.h"
#include <iostream>

bool Utils::isLittleEndian()
{
	int i = 1;
	return *(char*)&i == '\1';
}

int Utils::getRandomInt(int min, int max)
{
	if (min > max)
		return getRandomInt(max, min);

	if (min == max)
		return min;
		
	return static_cast<int>((float)(rand() % RAND_MAX) / RAND_MAX * (max - min + 1) + min);
}

float Utils::getRandomFloat(float min, float max)
{
	if (min > max)
		return getRandomFloat(max, min);

	if (min == max)
		return min;
	
	float random = static_cast<float>(rand() % RAND_MAX) / static_cast<float>(RAND_MAX);

	return random * (max - min + 1) + min;
}

float Utils::toRad(float degrees)
{
	return degrees / 180.f * PI;
}

float Utils::toDeg(float radians)
{
	return radians * 180.f / PI;
}

float Utils::degreeRange(float degrees)
{
	int rounds = static_cast<int>(degrees) / 360;
	if (degrees < 0.f)
		rounds--;
	degrees = degrees - rounds * 360.f;
	return degrees >= 360.f ? degrees - 360.f : degrees;
}

int Utils::signalFromBool(bool b)
{
	return b ? 1 : -1;
}

void Utils::FileWriteBytes(FILE* f, void* data, unsigned bytes, bool forceBigEndian)
{
	bool littleEndian = isLittleEndian() && !forceBigEndian;
	char* ptr = (char*)data + (littleEndian ? bytes - 1 : 0);

	while (bytes-- > 0)
	{
		fwrite(ptr, sizeof(char), 1, f);
		ptr += (littleEndian ? -1 : 1);
	}
}

void Utils::FileReadBytes(FILE* f, void* out, unsigned bytes)
{
	char* ptr = (char*)out;

	if (isLittleEndian())
	{
		while (bytes-- > 0)
			if (!fread(ptr + bytes, sizeof(char), 1, f))
				throw std::string("End of file reached.");
	}
	else
	{
		while (bytes-- > 0)
			if (!fread(ptr++, sizeof(char), 1, f))
				throw std::string("End of file reached.");
	}
}

void Utils::VectorWriteBytes(std::vector<unsigned char>& vec, void* data, unsigned bytes)
{
	bool littleEndian = isLittleEndian();
	unsigned char* ptr = (unsigned char*)data + (littleEndian ? bytes - 1 : 0);

	while (bytes-- > 0)
	{
		vec.push_back(*ptr);
		ptr += (littleEndian ? -1 : 1);
	}
}