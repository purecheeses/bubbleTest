#include "StartScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* StartScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = StartScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool StartScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}


	addBackground();
	this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));

	return true;
}
void StartScene::visit(Renderer *renderer, const Mat4 &transform, uint32_t parentFlags)
{
	Layer::visit(renderer, transform, parentFlags);

	_command.init(_globalZOrder);
	_command.func = CC_CALLBACK_0(StartScene::onDraw, this);
	Director::getInstance()->getRenderer()->addCommand(&_command);
}

void StartScene::onDraw()
{
	//获得当前HelloWorld的shader
	auto glProgram = getGLProgram();
	//使用此shader
	glProgram->use();
	//设置该shader的一些内置uniform,主要是MVP，即model-view-project矩阵
	glProgram->setUniformsForBuiltins();

	auto size = Director::getInstance()->getWinSize();
	//指定将要绘制的三角形的三个顶点，分别位到屏幕左下角，右下角和正中间的顶端
	float vertercies[] = { 0, 0,   //第一个点的坐标
		size.width, 0,   //第二个点的坐标
		size.width / 2, size.height };  //第三个点的坐标
	//指定每一个顶点的颜色，颜色值是RGBA格式的，取值范围是0-1
	float color[] = { 0, 1, 0, 1,    //第一个点的颜色，绿色
		1, 0, 0, 1,  //第二个点的颜色, 红色
		0, 0, 1, 1 };  //第三个点的颜色， 蓝色
	//激活名字为position和color的vertex attribute
	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
	//分别给position和color指定数据源
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertercies);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, color);
	//绘制三角形，所谓的draw call就是指这个函数调用
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//通知cocos2d-x 的renderer，让它在合适的时候调用这些OpenGL命令
	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 3);
	//如果出错了，可以使用这个函数来获取出错信息
	CHECK_GL_ERROR_DEBUG();
}

//void StartScene::generateUV(){
//	glEnable(GL_TEXTURE_2D);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_ONE, GL_SRC_COLOR);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glGenTextures(1, &textureId);
//
//	Image *image = new Image;
//	std::string imagePath = FileUtils::getInstance()->fullPathForFilename("Heart.png");
//	image->initWithImageFile(imagePath);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//	unsigned char *imageData = image->getData();
//	int width = image->getWidth();
//	int height = image->getHeight();
//	//调用此方法把imageData的图像数据传递给GPU
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
//		GL_UNSIGNED_BYTE,//must be GL_UNSIGNED_BYTE
//		imageData);
//	//别忘了释放image内存
//	CC_SAFE_DELETE(image);
//}

void StartScene::addBackground(){
	auto sprite = Sprite::create("background3.jpg");
	auto visibleOrigin = Director::getInstance()->getVisibleOrigin();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	sprite->setPosition(Vec2(visibleSize.width / 2 + visibleOrigin.x, visibleSize.height / 2 + visibleOrigin.y));
	addChild(sprite);
}
