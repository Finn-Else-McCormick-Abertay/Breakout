#include "Ball.h"
#include "GameManager.h" // avoid cicular dependencies
#include <iostream>

Ball::Ball(sf::RenderWindow* window, float velocity, GameManager* gameManager)
    : _window(window), _velocity(velocity), _gameManager(gameManager),
    _timeWithPowerupEffect(0.f), _isFireBall(false), _isAlive(true), _direction({1,1})
{
    _sprite.setRadius(RADIUS);
    _sprite.setFillColor(sf::Color::Cyan);
    _sprite.setPosition(0, 300);

    _bounceSFXBuffer = std::make_unique<sf::SoundBuffer>();
    if (!_bounceSFXBuffer->loadFromFile("audio/sfx/bounce.wav")) _bounceSFXBuffer = nullptr;
    else { for (auto& sound : _bounceSounds) { sound.setBuffer(*_bounceSFXBuffer); } }
}

void Ball::update(float dt)
{
    // check for powerup, tick down or correct
    if (_timeWithPowerupEffect > 0.f)
    {
        _timeWithPowerupEffect -= dt;
    }
    else
    {
        if (_velocity != VELOCITY)
            _velocity = VELOCITY;   // reset speed.
        else
        {
            setFireBall(0);    // disable fireball
            _sprite.setFillColor(sf::Color::Cyan);  // back to normal colour.
        }        
    }

    // Fireball effect
    if (_isFireBall)
    {
        // Flickering effect
        int flicker = rand() % 50 + 205; // Random value between 205 and 255
        _sprite.setFillColor(sf::Color(flicker, flicker / 2, 0)); // Orange flickering color
    }

    // Update position with a subtle floating-point error
    _sprite.move(_direction * _velocity * dt);

    // check bounds and bounce
    sf::Vector2f position = _sprite.getPosition();
    sf::Vector2u windowDimensions = _window->getSize();

    // bounce on walls
    if ((position.x >= windowDimensions.x - 2 * RADIUS && _direction.x > 0) || (position.x <= 0 && _direction.x < 0)) {
        _direction.x *= -1;
        onBounced(BounceType::WALL);
    }

    // bounce on ceiling
    if (position.y <= 0 && _direction.y < 0) {
        _direction.y *= -1;
        onBounced(BounceType::CEILING);
    }

    // lose life bounce
    if (position.y > windowDimensions.y) {
        _sprite.setPosition(0, 300);
        _direction = { 1, 1 };
        onBounced(BounceType::DEATHPLANE);
        _gameManager->loseLife();
    }

    // collision with paddle
    if (_sprite.getGlobalBounds().intersects(_gameManager->getPaddle()->getBounds()))
    {
        _direction.y *= -1; // Bounce vertically

        float paddlePositionProportion = (_sprite.getPosition().x - _gameManager->getPaddle()->getBounds().left) / _gameManager->getPaddle()->getBounds().width;
        _direction.x = paddlePositionProportion * 2.0f - 1.0f;

        // Adjust position to avoid getting stuck inside the paddle
        _sprite.setPosition(_sprite.getPosition().x, _gameManager->getPaddle()->getBounds().top - 2 * RADIUS);

        onBounced(BounceType::PADDLE);
    }

    // collision with bricks
    int collisionResponse = _gameManager->getBrickManager()->checkCollision(_sprite, _direction);
    if (!_isFireBall) {// no collisisons when in fireBall mode.
        if (collisionResponse != 0) onBounced(BounceType::BRICKS);
        if (collisionResponse == 1) {
            _direction.x *= -1; // Bounce horizontally
        }
        else if (collisionResponse == 2) {
            _direction.y *= -1; // Bounce vertically
        }
    }
}

void Ball::render()
{
    _window->draw(_sprite);
}

void Ball::setVelocity(float coeff, float duration)
{
    _velocity = coeff * VELOCITY;
    _timeWithPowerupEffect = duration;
}

void Ball::setFireBall(float duration)
{
    if (duration) 
    {
        _isFireBall = true;
        _timeWithPowerupEffect = duration;        
        return;
    }
    _isFireBall = false;
    _timeWithPowerupEffect = 0.f;    
}


void Ball::onBounced(BounceType type) {
    if (!_bounceSFXBuffer) return;

    if (type == BounceType::PADDLE || type == BounceType::DEATHPLANE) _bouncesSinceHitPaddle = 0;
    else _bouncesSinceHitPaddle++;

    // Keep a buffer of multiple sound objects, so sound effects are not noticeably cut short when there are multiple bounces in quick succession
    sf::Sound* soundToUse = nullptr;
    for (auto& sound : _bounceSounds) {
        // If there is a stopped sound, use that
        if (sound.getStatus() != sf::Sound::Status::Playing) { soundToUse = &sound; break; }
        // Else get the sound with the longest elapsed time
        if (!soundToUse || (sound.getPlayingOffset() > soundToUse->getPlayingOffset())) soundToUse = &sound;
    }

    float pitch = 1.f + _bouncesSinceHitPaddle * 0.01f;
    if (soundToUse) { soundToUse->setPitch(pitch); soundToUse->play(); }
}