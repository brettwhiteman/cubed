#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "physical_object.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"

#define BLOCK_PLACE_TIME 4

class Player : public PhysicalObject
{
public:
	Player(glm::vec3 pos);
	~Player();

	void update(); 
	void draw();
	void tick();
	const Texture* getTexture();
	const Transform* getTransform();

	inline float getWidth() { return 0.2f; }
	inline float getHeightBelow() { return 1.5f; }
	inline float getHeightAbove() { return 0.2f; }
	inline bool gravity() { return true; }

	void jump();
	inline void rotatePitch(float angle) { m_camera.rotatePitch(angle); }
	inline void rotateYaw(float angle) { m_camera.rotateYaw(angle); }

	void attack();
	void activate();

	Camera m_camera;
	int m_blockBreakTimer;
	int m_blockPlaceTimer;

	bool m_blockSelected;
	bool m_canPlaceBlock;
	int m_selX;
	int m_selY;
	int m_selZ;
	int m_lastSelX;
	int m_lastSelY;
	int m_lastSelZ;
	int m_freeX;
	int m_freeY;
	int m_freeZ;

	bool m_grounded;
};

#endif