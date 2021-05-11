#include "SceneNodeEntity.h"

bool SceneNodeEntity::Initialise()
{
	return SceneNodeMesh::Initialise();
}

void SceneNodeEntity::Update(XMFLOAT4 camPosition)
{
}

void SceneNodeEntity::SetupMesh()
{
}

void SceneNodeEntity::Shutdown()
{
}

void SceneNodeEntity::PlaceAt(XMFLOAT3 pos)
{
	bounds.SetPosition(pos.x, pos.y, pos.z);
	SetWorldTransform(XMMatrixScaling(scale, scale, scale) * XMMatrixTranslation(pos.x, pos.y, pos.z));
}
