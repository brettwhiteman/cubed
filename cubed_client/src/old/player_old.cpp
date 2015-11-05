#include "player.h"
#include "game.h"
#include "blocks.h"

Player::Player(glm::vec3 pos) :
	m_camera(std::move(pos), PI / 3.0f, 1.333333f, 0.05f, 1000.0f),
	m_blockBreakTimer(0),
	m_blockPlaceTimer(0),
	m_blockSelected(false),
	m_grounded(false),
	m_canPlaceBlock(false)
{
	m_pPos = &m_camera.m_pos;
}

Player::~Player()
{

}

void Player::update()
{
	// Mouse - yaw/pitch
	std::pair<int, int> mouseOffset = InputManager::instance().getMouseOffset();
	m_camera.rotatePitch(static_cast<float>(std::get<1>(mouseOffset)) / 1000.0f);
	m_camera.rotateYaw(static_cast<float>(std::get<0>(mouseOffset)) / -1000.0f);

	// mouse - attack/activate
	if(InputManager::instance().mouseLeft())
		attack();
	else
		m_blockBreakTimer = 0;
	if(InputManager::instance().mouseRight())
		activate();
	else
		m_blockPlaceTimer = 0;
	// keyboard - movement
	glm::vec3 direction(0.0f, 0.0f, 0.0f);
	if(InputManager::instance().keyDown(SDL_SCANCODE_W))
		direction += m_camera.getMoveForwardVector();
	if(InputManager::instance().keyDown(SDL_SCANCODE_A))
		direction -= m_camera.getMoveRightVector();
	if(InputManager::instance().keyDown(SDL_SCANCODE_S))
		direction -= m_camera.getMoveForwardVector();
	if(InputManager::instance().keyDown(SDL_SCANCODE_D))
		direction += m_camera.getMoveRightVector();
	float len = glm::sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
	if(len > 0.0f)
		Globals::game->m_physics.accelerate(*this, glm::normalize(direction) * 30.0f);
	if(InputManager::instance().keyDown(SDL_SCANCODE_SPACE))
		jump();


	// Update the currently selected block
	glm::vec3 add = glm::normalize(m_camera.m_forward) / 40.0f;
	glm::vec3 pos = m_camera.m_pos;

	m_blockSelected = false;
	m_canPlaceBlock = false;

	for(unsigned int i = 0; i < 250; ++i)
	{
		pos += add;

		int x = static_cast<int>(pos.x < 0.0f ? pos.x - 1.0f : pos.x);
		int y = static_cast<int>(pos.y < 0.0f ? pos.y - 1.0f : pos.y);
		int z = static_cast<int>(pos.z < 0.0f ? pos.z - 1.0f : pos.z);

		BlockID selBlock = Globals::game->m_world.getBlockID(x, y, z);
		if(selBlock != BLOCK_AIR)
		{
			m_selX = x;
			m_selY = y;
			m_selZ = z;

			m_blockSelected = true;

			break;
		}
		else
		{
			m_canPlaceBlock = true;
			m_freeX = x;
			m_freeY = y;
			m_freeZ = z;
		}
	}

	if((m_lastSelX != m_selX) || (m_lastSelY != m_selY) || (m_lastSelZ != m_selZ))
	{
		m_blockBreakTimer = 0;

		m_lastSelX = m_selX;
		m_lastSelY = m_selY;
		m_lastSelZ = m_selZ;
	}

	m_pPos->y -= 0.003f;

	if(Globals::game->m_physics.collidingWithWorld(*this))
		m_grounded = true;
	else
		m_grounded = false;

	m_pPos->y += 0.003f;
}

void Player::draw()
{

}

void Player::tick()
{
	++m_blockBreakTimer;
	--m_blockPlaceTimer;
}

const Texture* Player::getTexture()
{
	return nullptr;
}

const Transform* Player::getTransform()
{
	return nullptr;
}

void Player::attack()
{
	if(m_blockSelected)
	{
		auto type = Blocks::instance().get(Globals::game->m_world.getBlockID(m_selX, m_selY, m_selZ));
		if(m_blockBreakTimer >= type->hardness() && (type->hardness() != 0))
		{
			Globals::game->m_world.removeBlock(m_selX, m_selY, m_selZ);
			type->onBreak(m_selX, m_selY, m_selZ);
		}
	}
}

void Player::activate()
{
	if(m_blockSelected)
	{
		auto type = Blocks::instance().get(Globals::game->m_world.getBlockID(m_selX, m_selY, m_selZ));
		if(!type->activate(m_selX, m_selY, m_selZ))
		{
			if(m_canPlaceBlock)
				if(m_blockPlaceTimer <= 0)
				{
					auto newBlockType = Blocks::instance().get(BLOCK_GRASS);
					Globals::game->m_world.addBlock(m_freeX, m_freeY, m_freeZ, BLOCK_GRASS);
					m_blockPlaceTimer += BLOCK_PLACE_TIME;
					newBlockType->onPlace(m_freeX, m_freeY, m_freeZ);
				}
		}
	}
}

void Player::jump()
{
	if(m_grounded)
		Globals::game->m_physics.accelerate(*this, glm::vec3(0.0f, 200.0f, 0.0f));
}