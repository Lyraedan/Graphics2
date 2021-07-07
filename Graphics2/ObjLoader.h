/*
	Author: Luke Rapkin
*/
#pragma once
#include "Graphics2.h"
#include <iostream>
#include <fstream>

class ObjLoader
{

public:
	struct Face {
		std::vector<XMFLOAT3> faceData;
		std::vector<XMFLOAT3> tris;
	};

	void LoadModel(string file);

	static const vector<string> split(const string& s, const char& c)
	{
		string buff{ "" };
		vector<string> v;

		for (auto n : s)
		{
			if (n != c) buff += n; else
				if (n == c && buff != "") { v.push_back(buff); buff = ""; }
		}
		if (buff != "") v.push_back(buff);

		return v;
	}

	std::vector<XMFLOAT3> verts;
	std::vector<XMFLOAT3> normals;
	std::vector<XMFLOAT2> uvs;
	std::vector<Face> faces;
};

