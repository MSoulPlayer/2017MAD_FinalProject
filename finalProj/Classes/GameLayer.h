#ifndef __TowerDefence__GameLayer__
#define __TowerDefence__GameLayer__

#include "cocos2d.h"
#include "Enemy.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace std;

class GameLayer : public cocos2d::Layer
{
private:
	int waveNum;   //怪物的波数
    int playerHP;  //血量
    int money;    //积分
	int isPlaceTower1or2; //放置塔1或塔2
    bool gameEnded;  //游戏是否结束
	Size visibleSize;

	Label* HPLabel;
	Label* MoneyLabel;
	Label* WaveLabel;

	Sprite* bgsprite;  //背景

	cocos2d::Vector<SpriteFrame*> enemy1Move;  //怪物1动画帧
	cocos2d::Vector<SpriteFrame*> enemy2Move;  //怪物2动画帧
	cocos2d::Vector<SpriteFrame*> explode;  //爆炸动画帧

	std::vector<Vec2> WayPoints;  //怪物的两条行走路径

	cocos2d::Vector<Sprite*> towerBase1;  //保存塔1
	cocos2d::Vector<Sprite*> towerBase2;  //保存塔2
	cocos2d::Vector<Sprite*> bullet;  //保存子弹
	list<Enemy> enemies1;  //保存怪物1
	list<Enemy> enemies2;  //保存怪物2
	
	void preloadmusic();
	void playBgm();
	void loadAnimation();

	void addTouchListener();
	bool onTouchBegan(Touch *touch, Event *event);
	void onTouchEnded(Touch *touch, Event *event);

	void update_logic(float f);
	void create_enemy(float f);
	void update_enemy1(float f);
	void update_enemy2(float f);
	void collide(float f);
    
    void addTowerTag();
	void addTower(Ref* pSender, char dit);
    void addWaypoints();

    bool canBuyTower(int type);
    void GameOver();
    
public:
    virtual bool init();
    static cocos2d::Scene* createScene();
    CREATE_FUNC(GameLayer);
};

#endif /* defined(__TowerDefence__GameLayer__) */
