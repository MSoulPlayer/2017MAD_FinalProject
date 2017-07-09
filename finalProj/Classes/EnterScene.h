#pragma once
#include "cocos2d.h"
#include "GameLayer.h"

USING_NS_CC;
using namespace std;

class EnterScene : public cocos2d::Layer {
public:
	Label* start;  //开始按钮
	Label* helper;  //帮助按钮
	Label* quit;  //返回按钮
	Sprite* help;  //帮助图片

	static cocos2d::Scene* createScene();

	virtual bool init();

	void StartCallback(cocos2d::Ref* pSender);

	void HelperCallback(cocos2d::Ref* pSender);

	void QuitCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(EnterScene);
};