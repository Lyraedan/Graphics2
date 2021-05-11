#include "SceneGraph.h"

#include <iostream>

bool SceneGraph::Initialise() {
	for (SceneNode*& ptr : _children) {
		bool init = ptr->Initialise();
		if (!init) {
			return false;
		}
	}
	return true;
}
void SceneGraph::Update(FXMMATRIX& currentWorldTransformation) {
	SceneNode::Update(currentWorldTransformation);
	XMMATRIX combinedWorldTransform = XMLoadFloat4x4(&_combinedWorldTransformation);

	for (SceneNode*& ptr : _children) {
		ptr->Update(combinedWorldTransform);
	}
}

void SceneGraph::Render() {
	for (SceneNode*& ptr : _children) {
		ptr->Render();
	}
}

void SceneGraph::Shutdown() {
	for (SceneNode*& ptr : _children) {
		ptr->Shutdown();
	}
}

void SceneGraph::Add(SceneNode* node) {
	_children.push_back(node);
}

void SceneGraph::AddFront(SceneNode* node) {
	_children.push_front(node);
}

void SceneGraph::Insert(int index, SceneNode* node)
{
	// seems laggy
	_children.insert(_children.begin(), index, node);
}

int SceneGraph::Size()
{
	return _children.size();
}

void SceneGraph::Remove(SceneNode* node) {
	_children.remove(node);
}

SceneNode* SceneGraph::Find(wstring name) {
	if (_name == name) {
		return this;
	}

	SceneNode* searchFor = nullptr;

	for (SceneNode*& ptr : _children) {
		searchFor = ptr->Find(name);
		if (searchFor != nullptr)
			break;
	}
	return searchFor;
}