#pragma once
#include <SDL/SDL_mixer.h>
#include <vector>
class SDL__Audio
{
public:
	SDL__Audio();
	~SDL__Audio();

	void addSoundEffect(const char* path);
	void addAudioTrack(const char* path);
	void playSoundEffect(const int which) const;
	void playMusic();

private:
	Mix_Music* gMusic;
	std::vector<Mix_Chunk*> mSoundEffectBank;
};

