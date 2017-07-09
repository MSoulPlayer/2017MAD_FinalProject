#include "Enemy.h"

Enemy::Enemy(Sprite * e, int p)
{
	enemy = e;
	pos = p;
}

Enemy::Enemy(const Enemy & obj)
{
	enemy = obj.enemy;
	pos = obj.pos;
}

/*
void Enemy::activeEnemy() {
	active = true;
	//store explode animation
	auto texture = Director::getInstance()->getTextureCache()->addImage("explode.png");
	explode.reserve(9);
	for (int i = 0; i < 5; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(188.8 * i, 0, 188.8, 160)));
		explode.pushBack(frame);
	}
	for (int i = 5; i < 9; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(188.8 * (i - 5), 160, 188.8, 160)));
		explode.pushBack(frame);
	}
}
*/
