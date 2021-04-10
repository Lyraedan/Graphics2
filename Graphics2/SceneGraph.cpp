#include "SceneGraph.h"

#include <iostream>

bool SceneGraph::Initialise() {
	for (SceneNodePointer& ptr : _children) {
		bool init = ptr->Initialise();
		if (!init) {
			return false;
		}
	}
	return true; //!failed;
}
void SceneGraph::Update(FXMMATRIX& currentWorldTransformation) {
	SceneNode::Update(currentWorldTransformation);
	// No access to DirectXFramework including #graphics2.h causes circular headers
	//XMMATRIX view = DirectXFramework::GetDXFramework()->GetViewTransformation();
	//XMMATRIX proj = DirectXFramework::GetDXFramework()->GetProjectionTransformation();
	XMMATRIX combinedWorldTransform = XMLoadFloat4x4(&_combinedWorldTransformation);

	for (SceneNodePointer& ptr : _children) {
		ptr->Update(combinedWorldTransform);
	}
}

void SceneGraph::Render() {
	for (SceneNodePointer& ptr : _children) {
		ptr->Render();
	}
}

void SceneGraph::Shutdown() {
	for (SceneNodePointer& ptr : _children) {
		ptr->Shutdown();
	}
}

void SceneGraph::Add(SceneNodePointer node) {
	_children.push_back(node);
}

void SceneGraph::Remove(SceneNodePointer node) {
	// Look at linked list example online
	/*
	Find the node then remove it and erase it
	*/
	//node->Remove(node);
	_children.remove(node);
}

SceneNodePointer SceneGraph::Find(wstring name) {
	if (_name == name) {
		return shared_from_this();
	}

	SceneNodePointer searchFor = nullptr;

	for (SceneNodePointer& ptr : _children) {
		searchFor = ptr->Find(name);
		if (searchFor != nullptr)
			break;
	}
	return searchFor;
}