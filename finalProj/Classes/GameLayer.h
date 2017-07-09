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
	int waveNum;   //����Ĳ���
    int playerHP;  //Ѫ��
    int money;    //����
	int isPlaceTower1or2; //������1����2
    bool gameEnded;  //��Ϸ�Ƿ����
	Size visibleSize;

	Label* HPLabel;
	Label* MoneyLabel;
	Label* WaveLabel;

	Sprite* bgsprite;  //����

	cocos2d::Vector<SpriteFrame*> enemy1Move;  //����1����֡
	cocos2d::Vector<SpriteFrame*> enemy2Move;  //����2����֡
	cocos2d::Vector<SpriteFrame*> explode;  //��ը����֡

	std::vector<Vec2> WayPoints;  //�������������·��

	cocos2d::Vector<Sprite*> towerBase1;  //������1
	cocos2d::Vector<Sprite*> towerBase2;  //������2
	cocos2d::Vector<Sprite*> bullet;  //�����ӵ�
	list<Enemy> enemies1;  //�������1
	list<Enemy> enemies2;  //�������2
	
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
