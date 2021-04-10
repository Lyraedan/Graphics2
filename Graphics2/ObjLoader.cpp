#include "ObjLoader.h"

/// <summary>
/// Author: Luke Rapkin
/// Created: 08/04/2021
/// Last modified: 10/04/2021
/// Ref: https://en.wikipedia.org/wiki/Wavefront_.obj_file
/// </summary>
/// <param name="f">Path to our file</param>
void ObjLoader::LoadModel(string f)
{
	std::ifstream file(f);
	std::string str;
	int f2 = 0;
	while (std::getline(file, str))
	{
		// Geometric vertices
		if (str.rfind("v ", 0) == 0) {
			vector<string> sl = split(str, ' ');
			float x = std::stof(sl[1]);
			float y = std::stof(sl[2]);
			float z = std::stof(sl[3]);
			verts.push_back(XMFLOAT3(x, y, z));
		}
		// Vertex normals
		else if (str.rfind("vn ", 0) == 0) {
			vector<string> sl = split(str, ' ');
			float x = std::stof(sl[1]);
			float y = std::stof(sl[2]);
			float z = std::stof(sl[3]);
			normals.push_back(XMFLOAT3(x, y, z));
		}
		// Texture coordinates
		else if (str.rfind("vt ", 0) == 0) {
			vector<string> sl = split(str, ' ');
			float u = std::stof(sl[1]);
			float v = std::stof(sl[2]);
			uvs.push_back(XMFLOAT2(u, v));
		}
		// Polygonal face elements
		else if (str.rfind("f ", 0) == 0) {
			vector<string> sl = split(str, ' ');

			int j = 1;
			vector<int> intArr;
			Face face;
			// Vertexs, UV, Normals
			while (j < sl.size() && sl[j].size() > 0) {
				XMFLOAT3 temp = XMFLOAT3(0, 0, 0);
				vector<string> sl2 = split(sl[j], '/');
				temp.x = std::stof(sl2[0]);
				if (sl2.size() > 1) {
					if (sl2[1] != "") {
						temp.y = std::stof(sl2[1]);
					}
					temp.z = std::stof(sl2[2]);
				}
				j++;

				face.faceData.push_back(temp);
				intArr.push_back(f2);
				f2++;
			}

			// Indices aka Triangles
			j = 1;
			while (j + 2 < sl.size()) {
				float x = intArr[0];
				float y = intArr[j];
				float z = intArr[j + 1];
				face.tris.push_back(XMFLOAT3(x, y, z));
				j++;
			}
			faces.push_back(face);
		}
	}
	int end = 0;
}

