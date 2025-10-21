#include "SoundManager.h"

SoundManager::SoundManager() {
	_activeSounds[SoundCategory::Gameplay] = {};
	_activeSounds[SoundCategory::UI] = {};
}

float SoundManager::getVolume(SoundCategory category) {
	if (_volumes.count(category) != 0) return _volumes.at(category);
	return _fallbackVolume;
}
void SoundManager::setVolume(SoundCategory category, float volume) {
	float oldVolume = getVolume(category);
	_volumes[category] = volume;

	// Account for sounds with set relative volumes
	for (auto& sound : _activeSounds.at(category)) {
		if (sound.getStatus() == sf::Sound::Status::Stopped) continue;
		sound.setVolume((sound.getVolume() / oldVolume) * volume);
	}
}

bool SoundManager::preloadSoundEffect(std::string path) { return getBuffer(path) != nullptr; }

sf::SoundBuffer* SoundManager::getBuffer(std::string path) {
	if (_soundBufferCache.count(path) != 0) return &_soundBufferCache.at(path);

	sf::SoundBuffer buffer;
	if (buffer.loadFromFile(path)) {
		_soundBufferCache.emplace(path, std::move(buffer));
		return &_soundBufferCache.at(path);
	}

	return nullptr;
}

void SoundManager::playSound(std::string path, float pitch, float volumeRelative, SoundCategory category) {
	sf::SoundBuffer* buffer = getBuffer(path);
	if (!buffer) return;

	sf::Sound* soundToUse = nullptr;
	for (auto& sound : _activeSounds.at(category)) {
		// If there is a stopped sound, use that
		if (sound.getStatus() != sf::Sound::Status::Playing) { soundToUse = &sound; break; }
		// Else get the sound with the longest elapsed time
		if (!soundToUse || (sound.getPlayingOffset() > soundToUse->getPlayingOffset())) soundToUse = &sound;
	}

	if (soundToUse) {
		soundToUse->stop();
		soundToUse->setBuffer(*buffer);
		soundToUse->setVolume(volumeRelative * getVolume(category));
		soundToUse->setPitch(pitch);
		soundToUse->play();
	}
}