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

			int index = 1;
			vector<int> indiceArray;
			Face face;
			// Vertexs, UV, Normals
			while (index < sl.size() && sl[index].size() > 0) {
				XMFLOAT3 parsed = XMFLOAT3(0, 0, 0);
				vector<string> sl2 = split(sl[index], '/');
				parsed.x = std::stof(sl2[0]);
				if (sl2.size() > 1) {
					if (sl2[1] != "") {
						parsed.y = std::stof(sl2[1]);
					}
					parsed.z = std::stof(sl2[2]);
				}
				index++;

				face.faceData.push_back(parsed);
				indiceArray.push_back(f2);
				f2++;
			}

			// Indices aka Triangles
			index = 1;
			while (index + 2 < sl.size()) {
				float x = indiceArray[0];
				float y = indiceArray[index];
				float z = indiceArray[index + 1];
				face.tris.push_back(XMFLOAT3(x, y, z));
				index++;
			}
			faces.push_back(face);
		}
	}
	int end = 0;
}

