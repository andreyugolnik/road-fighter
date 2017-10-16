#pragma once

struct Mix_Chunk;

bool Sound_initialization(void);
int Sound_initialization(int nc, int nrc);
void Sound_release(void);

Mix_Chunk* Sound_create_sound(const char* file);
void Sound_delete_sound(Mix_Chunk* s);
int Sound_play(Mix_Chunk* s);
int Sound_play(Mix_Chunk* s, int volume);
int Sound_play_continuous(Mix_Chunk* s);
int Sound_play_continuous(Mix_Chunk* s, int volume);
void Sound_play_ch(Mix_Chunk* s, int channel);
void Sound_play_ch(Mix_Chunk* s, int channel, int volume);

void Sound_create_music(const char* f1, int times);
void Sound_release_music(void);
void Sound_pause_music(void);
void Sound_unpause_music(void);

void Sound_music_volume(int volume);

/* These functions are AGRESIVE! (i.e. they actually STOP SDL_mixer and restart it) */
void Stop_playback(void);
void Resume_playback(void);
int Resume_playback(int nc, int nr);
