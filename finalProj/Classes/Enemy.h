#include "cocos2d.h"

using namespace cocos2d;

class Enemy : public Sprite {
public:
	int pos;
	Sprite* enemy;
	
	Enemy(Sprite* e, int p);
	Enemy(const Enemy & obj);
	
};
