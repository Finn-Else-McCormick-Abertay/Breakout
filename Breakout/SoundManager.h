#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <array>

enum class SoundCategory {
	Gameplay, UI
};

class SoundManager {
public:
	SoundManager();

	void playSound(std::string path, float pitch = 1.f, float volumeRelative = 1.f, SoundCategory = SoundCategory::Gameplay);

	bool preloadSoundEffect(std::string path);

	float getVolume(SoundCategory);
	void setVolume(SoundCategory, float);

	void pauseCategory(SoundCategory);
	void unpauseCategory(SoundCategory);

private:
	sf::SoundBuffer* getBuffer(std::string path);
	std::map<std::string, sf::SoundBuffer> _soundBufferCache;

	float _fallbackVolume = 50.f;
	std::map<SoundCategory, float> _volumes;

	std::map<SoundCategory, std::array<sf::Sound, 10>> _activeSounds;
};