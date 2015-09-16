#ifndef __START_SCENE_H__
#define __START_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

typedef struct {
	float Position[2];
	float Color[4];
	float TexCoord[2];
} Vertex;

class StartScene : public cocos2d::Layer
{
public:

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();
	void StartScene::myDraw();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	void StartScene::addBackground();
	void StartScene::generateUV();
	virtual void visit(Renderer* renderer, const Mat4 &parentTransform, uint32_t parentFlags);

	void onDraw();
	CustomCommand _command;
	GLuint textureId;


	// implement the "static create()" method manually
	CREATE_FUNC(StartScene);
};

#endif // __StartScene_SCENE_H__
