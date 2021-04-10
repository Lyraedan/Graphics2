#pragma once
// Credit: Improved Perlin noise by Flafla2 - https://gist.github.com/Flafla2/1a0b9ebef678bbce3215
// Port by Luke Rapkin : 100527733
class PerlinNoise
{
public:
	PerlinNoise();

	static double OctavePerlin(double x, double y, double z, int octaves, double persistence);

	static double perlin(double x, double y, double z);

	static double grad(int hash, double x, double y, double z);

	static double fade(double t);

	static double lerp(double a, double b, double x);

};