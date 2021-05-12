
class AABB
{
public:	
	float x, y, z;
	float width, height, length;

	void SetPosition(float x, float y, float z);

	void SetSize(float width, float height, float length);

	static bool Intersects(AABB a, AABB b) {
		return a.x < b.x + b.width &&
			   a.x + a.width > b.x &&
			   a.y < b.y + b.height &&
			   a.y + a.height > b.y &&
			   a.z < b.z + b.length &&
			   a.z + a.length > b.z;
	}
};

