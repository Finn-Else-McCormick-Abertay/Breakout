#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <array>

class GameManager;  // forward declaration

class Ball {
public:
	Ball(sf::RenderWindow* window, float velocity, GameManager* gameManager);

	void update(float dt);
	void render();
	void setVelocity(float coeff, float duration);
	void setFireBall(float duration);

private:
	sf::CircleShape _sprite;
	sf::Vector2f _direction;
	sf::RenderWindow* _window;

	std::unique_ptr<sf::SoundBuffer> _bounceSFXBuffer;
	std::array<sf::Sound, 5> _bounceSounds = {};

	enum class BounceType { WALL, CEILING, BRICKS, PADDLE, DEATHPLANE };
	void onBounced(BounceType);
	int _bouncesSinceHitPaddle = 0;

	float _velocity;
	bool _isAlive;
	bool _isFireBall;
	float _timeWithPowerupEffect;

	GameManager* _gameManager;  // Reference to the GameManager

	static constexpr float RADIUS = 10.0f;      
	static constexpr float VELOCITY = 350.0f;   // for reference.
};

