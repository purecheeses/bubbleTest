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


	//addBackground();
	//this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));
	//create my own program
	//GL::bindTexture2D(textureId);
	auto program = new GLProgram;
	program->initWithFilenames("myVertextShader.vert", "myFragmentShader.frag");
	program->link();
	//set uniform locations
	program->updateUniforms();
	this->setGLProgram(program);

	return true;
}
void StartScene::visit(Renderer *renderer, const Mat4 &transform, uint32_t parentFlags)
{
	Layer::visit(renderer, transform, parentFlags);

	_command.init(_globalZOrder);
	_command.func = CC_CALLBACK_0(StartScene::onDraw, this);
	Director::getInstance()->getRenderer()->addCommand(&_command);
}

void StartScene::myDraw(){
	auto size = Director::getInstance()->getWinSize();
	DrawPrimitives::setDrawColor4B(0, 255, 0, 255);
	glLineWidth(1);
	Vec2 filledVertices[] = { Vec2(50, 120), Vec2(100, 120), Vec2(100, 170), Vec2(75, 200), Vec2(50, 170) };
	DrawPrimitives::drawSolidPoly(filledVertices, 5, Color4F(0.5f, 0.5f, 1, 1));
}

void StartScene::onDraw()
{
	//��õ�ǰHelloWorld��shader
	auto glProgram = getGLProgram();
	//ʹ�ô�shader
	glProgram->use();
	//���ø�shader��һЩ����uniform,��Ҫ��MVP����model-view-project����
	glProgram->setUniformsForBuiltins();



	auto size = Director::getInstance()->getWinSize();
	//ָ����Ҫ���Ƶ������ε��������㣬�ֱ�λ����Ļ���½ǣ����½Ǻ����м�Ķ���
	float vertercies[] = { 0, 0,   //��һ���������
		size.width, 0,   //�ڶ����������
		size.width / 2, size.height };  //�������������
	//ָ��ÿһ���������ɫ����ɫֵ��RGBA��ʽ�ģ�ȡֵ��Χ��0-1
	float color[] = { 0, 1, 0, 1,    //��һ�������ɫ����ɫ
		1, 0, 0, 1,  //�ڶ��������ɫ, ��ɫ
		0, 0, 1, 1 };  //�����������ɫ�� ��ɫ
	//��������Ϊposition��color��vertex attribute
	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
	//�ֱ��position��colorָ������Դ
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertercies);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, color);
	//���������Σ���ν��draw call����ָ�����������
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//֪ͨcocos2d-x ��renderer�������ں��ʵ�ʱ�������ЩOpenGL����
	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 3);
	//��������ˣ�����ʹ�������������ȡ������Ϣ
	CHECK_GL_ERROR_DEBUG();
}

void StartScene::generateUV(){
	Vertex data[] =
	{
		{ { -1, -1 }, { 0, 1, 0, 1 }, { 0, 1 } },
		{ { 1, -1 }, { 0, 1, 0, 1 }, { 1, 1 } },
		{ { -1, 1 }, { 0, 1, 0, 1 }, { 0, 0 } },
		{ { 1, 1 }, { 0, 1, 0, 1 }, { 1, 0 } }
	};

	glGenTextures(1, &textureId);

	Image *image = new Image;
	std::string imagePath = FileUtils::getInstance()->fullPathForFilename("Heart.png");
	image->initWithImageFile(imagePath);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	unsigned char *imageData = image->getData();
	int width = image->getWidth();
	int height = image->getHeight();
	//���ô˷�����imageData��ͼ�����ݴ��ݸ�GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
		GL_UNSIGNED_BYTE,//must be GL_UNSIGNED_BYTE
		imageData);
	//�������ͷ�image�ڴ�
	CC_SAFE_DELETE(image);
}

void StartScene::addBackground(){
	auto sprite = Sprite::create("background3.jpg");
	auto visibleOrigin = Director::getInstance()->getVisibleOrigin();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	sprite->setPosition(Vec2(visibleSize.width / 2 + visibleOrigin.x, visibleSize.height / 2 + visibleOrigin.y));
	addChild(sprite);
}
