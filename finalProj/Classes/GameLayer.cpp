#include "GameLayer.h"
#include "SimpleAudioEngine.h"
#define database UserDefault::getInstance()
using namespace CocosDenshion;

Scene* GameLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = GameLayer::create();
	scene->addChild(layer);
	return scene;
}

bool GameLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
	waveNum = 0;
    playerHP = 10;
    money = 500;
	gameEnded = false;
	isPlaceTower1or2 = 0;
	visibleSize = Director::getInstance()->getVisibleSize();
    
	//���ñ���
	bgsprite = Sprite::create("bg.png");
	bgsprite->setPosition(visibleSize / 2);
	bgsprite->setScale(visibleSize.width / bgsprite->getContentSize().width,
		visibleSize.height / bgsprite->getContentSize().height);
	this->addChild(bgsprite, 0);

    //��ʾHP
    HPLabel = Label::create("HP: 10", "fonts/MarkerFelt.ttf", 20);
	HPLabel->setPosition(Vec2(visibleSize.width * 0.5 + 50, visibleSize.height - 20));
    this->addChild(HPLabel);

	//��ʾmoney
	MoneyLabel = Label::create("Money: 500", "fonts/MarkerFelt.ttf", 20);
	MoneyLabel->setPosition(Vec2(visibleSize.width * 0.5 + 200, visibleSize.height - 20));
	this->addChild(MoneyLabel);

	//��ʾ���ﲨ��
	WaveLabel = Label::create("Wave: 0", "fonts/MarkerFelt.ttf", 20);
    WaveLabel->setPosition(Vec2(visibleSize.width * 0.5 + 350, visibleSize.height - 20));
	this->addChild(WaveLabel);
    
	//��ӱ��ش洢����¼����
	if (!database->getBoolForKey("isExist")) {
		database->setBoolForKey("isExist", true);
	}
	database->setIntegerForKey("money", money);

	addTowerTag();
	addTouchListener();
    addWaypoints();
    
	preloadmusic();
	playBgm();
	loadAnimation();

	//���ø��ֵ�����
	schedule(schedule_selector(GameLayer::create_enemy), 1.0f, kRepeatForever, 0);
	schedule(schedule_selector(GameLayer::update_enemy1), 1.5f, kRepeatForever, 0);
	schedule(schedule_selector(GameLayer::update_enemy2), 0.5f, kRepeatForever, 0);
	schedule(schedule_selector(GameLayer::update_logic), 1.5f, kRepeatForever, 0);
	schedule(schedule_selector(GameLayer::collide), 0.2f, kRepeatForever, 0);

    return true;
}

//���ȸ����ж��߼�
void GameLayer::update_logic(float f)
{
	//�ж�ʤ��
	if (waveNum > 5 && !gameEnded && enemies1.size() == 0 && enemies2.size() == 0) {
		gameEnded = true;
		auto win = Sprite::create("win.png");
		win->setPosition(visibleSize.width / 2, visibleSize.height / 2);
		this->addChild(win, 3);
		_eventDispatcher->removeAllEventListeners();
		unschedule(schedule_selector(GameLayer::create_enemy));
		unschedule(schedule_selector(GameLayer::update_logic));
	}

	//�ж���Ϸʧ��
	if (playerHP == 0) {
		GameOver();
	}

	//��1�����ӵ�1����ⷶΧ��С��һ��ֻ�ܴ�һ�ֹ�
	for (cocos2d::Vector<Sprite*>::iterator it1 = towerBase1.begin(); it1 != towerBase1.end(); it1++) {
		bool flag = false;
		for (list<Enemy>::iterator it2 = enemies1.begin(); it2 != enemies1.end(); it2++) {
			if ((*it1)->getPosition().getDistance((*it2).enemy->getPosition()) < 120) {
				Sprite* b1 = Sprite::create("sprite_level13-hd.png", CC_RECT_PIXELS_TO_POINTS(Rect(510, 843, 16, 20)));
				b1->setPosition((*it1)->getPosition());
				bullet.pushBack(b1);
				addChild(b1);
				MoveTo* move = MoveTo::create(0.1f, (*it2).enemy->getPosition());
				Sequence* seq = Sequence::create(move, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParentAndCleanup, b1, true)), NULL);
				b1->runAction(seq);

				auto audio = SimpleAudioEngine::getInstance();
				audio->playEffect("music/fire.wav", false);

				flag = true;
				break;
			}
		}

		if (flag)
			continue;

		for (list<Enemy>::iterator it3 = enemies2.begin(); it3 != enemies2.end(); it3++) {
			if ((*it1)->getPosition().getDistance((*it3).enemy->getPosition()) < 120) {
				Sprite* b1 = Sprite::create("sprite_level13-hd.png", CC_RECT_PIXELS_TO_POINTS(Rect(510, 843, 16, 20)));
				b1->setPosition((*it1)->getPosition());
				bullet.pushBack(b1);
				addChild(b1);
				MoveTo* move = MoveTo::create(0.1f, (*it3).enemy->getPosition());
				Sequence* seq = Sequence::create(move, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParentAndCleanup, b1, true)), NULL);
				b1->runAction(seq);

				auto audio = SimpleAudioEngine::getInstance();
				audio->playEffect("music/fire.wav", false);
				break;
			}
		}
	}

	//��2�����ӵ�2����ⷶΧ�ϴ󣬿�ͬʱ�����ֹ�
	for (cocos2d::Vector<Sprite*>::iterator it1 = towerBase2.begin(); it1 != towerBase2.end(); it1++) {
		for (list<Enemy>::iterator it2 = enemies1.begin(); it2 != enemies1.end(); it2++) {
			if ((*it1)->getPosition().getDistance((*it2).enemy->getPosition()) < 200) {
				Sprite* b2 = Sprite::create("sprite_level13-hd.png", CC_RECT_PIXELS_TO_POINTS(Rect(945, 998, 22, 26)));
				b2->setPosition((*it1)->getPosition());
				bullet.pushBack(b2);
				addChild(b2);
				MoveTo* move = MoveTo::create(0.1f, (*it2).enemy->getPosition());
				Sequence* seq = Sequence::create(move, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParentAndCleanup, b2, true)), NULL);
				b2->runAction(seq);

				auto audio = SimpleAudioEngine::getInstance();
				audio->playEffect("music/fire.wav", false);
				break;
			}
		}

		for (list<Enemy>::iterator it3 = enemies2.begin(); it3 != enemies2.end(); it3++) {
			if ((*it1)->getPosition().getDistance((*it3).enemy->getPosition()) < 200) {
				Sprite* b2 = Sprite::create("sprite_level13-hd.png", CC_RECT_PIXELS_TO_POINTS(Rect(945, 998, 22, 26)));
				b2->setPosition((*it1)->getPosition());
				bullet.pushBack(b2);
				addChild(b2);
				MoveTo* move = MoveTo::create(0.1f, (*it3).enemy->getPosition());
				Sequence* seq = Sequence::create(move, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParentAndCleanup, b2, true)), NULL);
				b2->runAction(seq);

				auto audio = SimpleAudioEngine::getInstance();
				audio->playEffect("music/fire.wav", false);
				break;
			}
		}
	}

}

//�����ײ
void GameLayer::collide(float f)
{
	//�ӵ��������ײ����ʧ����û���
	for (cocos2d::Vector<Sprite*>::iterator it1 = bullet.begin(); it1 != bullet.end();) {
		if (*it1 == NULL) {
			continue;
		}
		bool flag = false;

		for (list<Enemy>::iterator it2 = enemies1.begin(); it2 != enemies1.end();) {
			if ((*it1)->getPosition().getDistance((*it2).enemy->getPosition()) < 75) {
				flag = true;
				Animation* anim = Animation::createWithSpriteFrames(explode, 0.1f);
				Animate* ani = Animate::create(anim);
				Sequence* seq = Sequence::create(ani, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParentAndCleanup, (*it2).enemy, true)), NULL);
				(*it2).enemy->runAction(seq);
				enemies1.erase(it2);

				(*it1)->runAction(CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParentAndCleanup, *it1, true)));
				bullet.erase(it1);

				auto audio = SimpleAudioEngine::getInstance();
				audio->playEffect("music/EnemyKilled.wav", false);

				money += 50;
				MoneyLabel->setString(CCString::createWithFormat("Money: %d", money)->getCString());

				break;
			}
			else {
				it2++;
			}
		}

		if (flag) {
			break;
		}
		else {
			it1++;
		}
	}

	for (list<Enemy>::iterator it3 = enemies2.begin(); it3 != enemies2.end();) {
		bool flag = false;
		for (cocos2d::Vector<Sprite*>::iterator it1 = bullet.begin(); it1 != bullet.end();) {
			if (*it1 == NULL) {
				continue;
			}

			if ((*it1)->getPosition().getDistance((*it3).enemy->getPosition()) < 75) {
				flag = true;
				Animation* anim = Animation::createWithSpriteFrames(explode, 0.1f);
				Animate* ani = Animate::create(anim);
				Sequence* seq = Sequence::create(ani, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParentAndCleanup, (*it3).enemy, true)), NULL);
				(*it3).enemy->runAction(seq);
				enemies2.erase(it3);

				(*it1)->runAction(CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParentAndCleanup, *it1, true)));
				bullet.erase(it1);

				auto audio = SimpleAudioEngine::getInstance();
				audio->playEffect("music/EnemyKilled.wav", false);

				money += 100;
				MoneyLabel->setString(CCString::createWithFormat("Money: %d", money)->getCString());

				break;
			}
			else {
				it1++;
			}
		}

		if (flag) {
			break;
		}
		else {
			it3++;
		}
	}
}

//�������ɹ���
void GameLayer::create_enemy(float f)
{
	static int count = 0; //������
	if (count == 0) {
		waveNum++;
		WaveLabel->setString(CCString::createWithFormat("Wave: %d", waveNum)->getCString());
	}
	count++;
	
	if (count < 10) {
		srand((unsigned int)time(0));
		int x = random(0, 1); //��������������
		int y = random(0, 1); //�����������1�����2

		//����5��ʱ����������2
		if (waveNum >= 3) {
			y = 1;
		}
		if (x == 0) {
			if (waveNum <= 1 || y == 0) {
				auto animation1 = Animation::createWithSpriteFrames(enemy1Move, 0.1f);
				auto animate1 = Animate::create(animation1);
				Sprite* enemy1 = Sprite::create();
				enemy1->runAction(RepeatForever::create(animate1));
				enemy1->setPosition(WayPoints.at(0));
				addChild(enemy1, 2);
				Enemy e1(enemy1, 0);
				enemies1.push_back(e1);
			}
			else {
				auto animation2 = Animation::createWithSpriteFrames(enemy2Move, 0.1f);
				auto animate2 = Animate::create(animation2);
				Sprite* enemy2 = Sprite::create();
				enemy2->runAction(RepeatForever::create(animate2));
				enemy2->setPosition(WayPoints.at(0));
				addChild(enemy2, 2);
				Enemy e2(enemy2, 0);
				enemies2.push_back(e2);
			}
		}
		else {
			if (waveNum <= 1 || y == 0) {
				auto animation1 = Animation::createWithSpriteFrames(enemy1Move, 0.1f);
				auto animate1 = Animate::create(animation1);
				Sprite* enemy1 = Sprite::create();
				enemy1->runAction(RepeatForever::create(animate1));
				enemy1->setPosition(WayPoints.at(0));
				addChild(enemy1, 2);
				Enemy e1(enemy1, 9);
				enemies1.push_back(e1);
			}
			else {
				auto animation2 = Animation::createWithSpriteFrames(enemy2Move, 0.1f);
				auto animate2 = Animate::create(animation2);
				Sprite* enemy2 = Sprite::create();
				enemy2->runAction(RepeatForever::create(animate2));
				enemy2->setPosition(WayPoints.at(0));
				addChild(enemy2, 2);
				Enemy e2(enemy2, 9);
				enemies2.push_back(e2);
			}
		}
	}

	//��3�ֺ������
	if (count == 30 && waveNum < 3) {
		count = 0;
	}
	else if (count == 15 && waveNum >= 3) {
		count = 0;
	}
}

//���ȹ���1�ж�
void GameLayer::update_enemy1(float f)
{
	for (list<Enemy>::iterator it = enemies1.begin(); it != enemies1.end();) {
		(*it).pos++;
		if ((*it).pos == 8 || (*it).pos == 17) {
			(*it).enemy->runAction(CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, (*it).enemy)));
			it = enemies1.erase(it);
			playerHP--;
			HPLabel->setString(CCString::createWithFormat("HP: %d", playerHP)->getCString());
		}
		else {
			(*it).enemy->runAction(MoveTo::create(1.5f, WayPoints.at((*it).pos)));
			it++;
		}
	}
}

//���ȹ���2�ж�
void GameLayer::update_enemy2(float f)
{
	for (list<Enemy>::iterator it = enemies2.begin(); it != enemies2.end();) {
		(*it).pos++;
		if ((*it).pos == 8 || (*it).pos == 17) {
			(*it).enemy->runAction(CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, (*it).enemy)));
			it = enemies2.erase(it);
			playerHP--;
			HPLabel->setString(CCString::createWithFormat("HP: %d", playerHP)->getCString());
		}
		else {
			(*it).enemy->runAction(MoveTo::create(0.5f, WayPoints.at((*it).pos)));
			it++;
		}
	}
}

//Ԥ��������
void GameLayer::preloadmusic() 
{
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("music/bgm.mp3");
	audio->preloadEffect("music/fire.wav");
	audio->preloadEffect("music/EnemyKilled.wav");
}

//��������
void GameLayer::playBgm() {
	auto audio = SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("music/bgm.mp3", true);
}

//���ض���
void GameLayer::loadAnimation()
{
	auto texture = Director::getInstance()->getTextureCache()->addImage("enemy1.png");
	enemy1Move.reserve(5);
	for (int i = 0; i < 4; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(49 * i, 0, 49, 50)));
		enemy1Move.pushBack(frame);
	}
	auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 40, 50)));
	enemy1Move.pushBack(frame);

	auto texture1 = Director::getInstance()->getTextureCache()->addImage("enemy2.png");
	enemy2Move.reserve(5);
	for (int i = 0; i < 4; i++) {
		auto frame = SpriteFrame::createWithTexture(texture1, CC_RECT_PIXELS_TO_POINTS(Rect(48 * i, 0, 48, 50)));
		enemy2Move.pushBack(frame);
	}
	auto frame1 = SpriteFrame::createWithTexture(texture1, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 48, 50)));
	enemy2Move.pushBack(frame1);


	auto texture2 = Director::getInstance()->getTextureCache()->addImage("explode.png");
	explode.reserve(8);
	for (int i = 0; i < 5; i++) {
		auto frame = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(190 * i, 0, 190, 175)));
		explode.pushBack(frame);
	}
	for (int i = 0; i < 3; i++) {
		auto frame = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(190 * i, 175, 190, 175)));
		explode.pushBack(frame);
	}
}

//��Ӵ������ı�ǩ
void GameLayer::addTowerTag()
{
	//�����������ı�ǩ
	auto label1 = Label::create("Create Tower1: 200", "fonts/MarkerFelt.ttf", 20);
	auto label2 = Label::create("Create Tower2: 400", "fonts/MarkerFelt.ttf", 20);


	//������������item
	auto item1 = MenuItemLabel::create(label1, CC_CALLBACK_1(GameLayer::addTower, this, '1'));
	auto item2 = MenuItemLabel::create(label2, CC_CALLBACK_1(GameLayer::addTower, this, '2'));

	//��item����ָ����λ��
	item1->setPosition(Vec2(visibleSize.width / 2 + 100, visibleSize.height - 50));
	item2->setPosition(Vec2(visibleSize.width / 2 + 300, visibleSize.height - 50));

	auto menu = Menu::create(item1, item2, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
}

//���������������
void GameLayer::addTower(Ref * pSender, char dit)
{
	if (dit == '1') {
		isPlaceTower1or2 = 1;
	}
	else if (dit == '2') {
		isPlaceTower1or2 = 2;
	}
}

//��Ӵ���������
void GameLayer::addTouchListener() {
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, bgsprite);
}

//��������
bool GameLayer::onTouchBegan(Touch *touch, Event *event) {
	if (isPlaceTower1or2 == 1) {
		if (canBuyTower(1)) {
			Sprite* tower1 = Sprite::create("sprite_level13-hd.png", CC_RECT_PIXELS_TO_POINTS(Rect(618, 680, 104, 91)));
			tower1->setPosition(touch->getLocation());
			addChild(tower1);
			money -= 200;
			MoneyLabel->setString(CCString::createWithFormat("Money: %d", money)->getCString());
			towerBase1.pushBack(tower1);
		}
	}
	else if (isPlaceTower1or2 == 2) {
		if (canBuyTower(2)) {
			Sprite* tower2 = Sprite::create("sprite_level13-hd.png", CC_RECT_PIXELS_TO_POINTS(Rect(888, 529, 106, 90)));
			tower2->setPosition(touch->getLocation());
			addChild(tower2);
			money -= 400;
			MoneyLabel->setString(CCString::createWithFormat("Money: %d", money)->getCString());
			towerBase2.pushBack(tower2);
		}
	}
	return true;
}

void GameLayer::onTouchEnded(Touch *touch, Event *event) {
	isPlaceTower1or2 = 0;
}

//��ӹ�������·���㣬�ӻ��ؿ�ʼ������ĳ��������
void GameLayer::addWaypoints()
{
	//·��1���Ϸ���0-8
	WayPoints.push_back(Vec2(0, visibleSize.height / 2));
	WayPoints.push_back(Vec2(visibleSize.width / 2 - 350, visibleSize.height / 2));
	WayPoints.push_back(Vec2(visibleSize.width / 2 - 200, visibleSize.height / 2));
	WayPoints.push_back(Vec2(visibleSize.width / 2 - 100, visibleSize.height / 2 + 130));
	WayPoints.push_back(Vec2(visibleSize.width / 2 + 100, visibleSize.height / 2 + 130));
	WayPoints.push_back(Vec2(visibleSize.width / 2 + 250, visibleSize.height / 2 + 20));
	WayPoints.push_back(Vec2(visibleSize.width / 2 + 350, visibleSize.height / 2 + 20));
	WayPoints.push_back(Vec2(visibleSize.width, visibleSize.height / 2 + 20));
	WayPoints.push_back(Vec2(0, 0)); //indicate end

	//·��2���·���9-16
	WayPoints.push_back(Vec2(0, visibleSize.height / 2));
	WayPoints.push_back(Vec2(visibleSize.width / 2 - 350, visibleSize.height / 2));
	WayPoints.push_back(Vec2(visibleSize.width / 2 - 200, visibleSize.height / 2));
	WayPoints.push_back(Vec2(visibleSize.width / 2 - 100, visibleSize.height / 2 - 110));
	WayPoints.push_back(Vec2(visibleSize.width / 2 + 100, visibleSize.height / 2 - 110));
	WayPoints.push_back(Vec2(visibleSize.width / 2 + 250, visibleSize.height / 2 + 20));
	WayPoints.push_back(Vec2(visibleSize.width / 2 + 350, visibleSize.height / 2 + 20));
	WayPoints.push_back(Vec2(visibleSize.width, visibleSize.height / 2 + 20));
}

//�жϻ����Ƿ��㹻������
bool GameLayer::canBuyTower(int type)
{
	if (type == 1) {
		return (money >= 200);
	}
	else {
		return (money >= 400);
	}
}

//��Ϸ����
void GameLayer::GameOver()
{
    if (!gameEnded)
    {
        gameEnded = true;
		auto over = Sprite::create("gameOver.png");
		over->setPosition(visibleSize.width / 2, visibleSize.height / 2);
		this->addChild(over, 3);
		_eventDispatcher->removeAllEventListeners();
		unschedule(schedule_selector(GameLayer::create_enemy));
		unschedule(schedule_selector(GameLayer::update_logic));
    }
}


