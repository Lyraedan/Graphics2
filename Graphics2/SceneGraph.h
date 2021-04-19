#pragma once
#include "SceneNode.h"
//#include "Graphics2.h"

#include <list>

class SceneGraph : public SceneNode
{
public:
	SceneGraph() : SceneNode(L"Root") {};
	SceneGraph(wstring name) : SceneNode(name) {};
	~SceneGraph(void) {};

	virtual bool Initialise(void);
	virtual void Update(FXMMATRIX& currentWorldTransformation);
	virtual void Render(void);
	virtual void Shutdown(void);

	void Add(SceneNode* node);
	void Remove(SceneNode* node);
	SceneNode* Find(wstring name);
	
	std::list<SceneNode*> _children;

//private:

	// Here you need to add whatever collection you are going to
	// use to store the children of this scene graph

};

//typedef shared_ptr<SceneGraph>			 SceneGraphPointer;
