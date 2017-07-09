#include "EnterScene.h"

cocos2d::Scene * EnterScene::createScene()
{
	auto scene = Scene::create();

	auto layer = EnterScene::create();

	scene->addChild(layer);

	return scene;
}

bool EnterScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto bg = Sprite::create("EnterScene.png");
	bg->setPosition(visibleSize / 2);
	bg->setScale(visibleSize.width / bg->getContentSize().width,
		visibleSize.height / bg->getContentSize().height);
	this->addChild(bg, 0);

	auto logo = Sprite::create("logo.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 655, 478, 368)));
	logo->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 120));
	addChild(logo, 1);

	start = Label::create("Start Game", "fonts/Comic_Book.ttf", 100);
	auto item1 = MenuItemLabel::create(start, CC_CALLBACK_1(EnterScene::StartCallback, this));
	item1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 100));

	helper = Label::create("Help", "fonts/Comic_Book.ttf", 100);
	auto item2 = MenuItemLabel::create(helper, CC_CALLBACK_1(EnterScene::HelperCallback, this));
	item2->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 200));

	auto menu = Menu::create(item1, item2, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	return true;
}

//��ʼ��ť�ص�����
void EnterScene::StartCallback(cocos2d::Ref * pSender)
{
	Director::getInstance()->replaceScene(TransitionFade::create(1, GameLayer::createScene()));
}


//������ť�ص�����
void EnterScene::HelperCallback(cocos2d::Ref * pSender)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	help = Sprite::create("help.png");
	help->setPosition(visibleSize / 2);
	addChild(help, 3);

	quit = Label::create("quit", "fonts/Comic_Book.ttf", 50);
	auto item = MenuItemLabel::create(quit, CC_CALLBACK_1(EnterScene::QuitCallback, this));
	item->setPosition(Vec2(visibleSize.width / 2 + 200, visibleSize.height / 2 - 200));
	auto menu = Menu::create(item, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 3);
}

//���ذ�ť�ص�����
void EnterScene::QuitCallback(cocos2d::Ref * pSender)
{
	help->removeFromParent();
	quit->removeFromParent();
}
