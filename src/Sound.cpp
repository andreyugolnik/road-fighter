#include "Sound.h"
#include "Assets.h"
#include "Auxiliar.h"
#include "Debug.h"
#include "FileHandling.h"

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef KITSCHY_DEBUG_MEMORY
#include "debug_memorymanager.h"
#endif

#define AUDIO_BUFFER 2048

bool sound_enabled = false;
Mix_Music* music_sound = nullptr;
int n_channels = -1;

bool Sound_initialization()
{
    return -1 != Sound_initialization(0, 0);
}

int Sound_initialization(int nc, int nrc)
{
    sound_enabled = false;

    const int flags = MIX_INIT_OGG;
    if (Mix_Init(flags) & flags)
    {
        int audio_rate = 44100;
        Uint16 audio_format = MIX_DEFAULT_FORMAT;
        int audio_channels = MIX_DEFAULT_CHANNELS;
        int audio_bufsize = AUDIO_BUFFER;
        n_channels = 8;

        output_debug_message("Initializing SDL_mixer.\n");
        if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_bufsize) == 0)
        {
            char SoundcardName[256];
            SDL_AudioDriverName(SoundcardName, sizeof(SoundcardName));

#if !defined(__EMSCRIPTEN__)
            Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
            output_debug_message("    opened %s at %d Hz %d bit %s, %d bytes audio buffer\n",
                                 SoundcardName, audio_rate, audio_format & 0xFF,
                                 audio_channels > 1 ? "stereo" : "mono", audio_bufsize);

            output_debug_message("    running with SDL_mixer version: %d.%d.%d\n",
                                 Mix_Linked_Version()->major,
                                 Mix_Linked_Version()->minor,
                                 Mix_Linked_Version()->patch);
#endif

            SDL_version compile_version;
            MIX_VERSION(&compile_version);
            output_debug_message("    compiled with SDL_mixer version: %d.%d.%d\n",
                                 compile_version.major,
                                 compile_version.minor,
                                 compile_version.patch);

            if (nc > 0)
                n_channels = Mix_AllocateChannels(nc);
            if (nrc > 0)
                Mix_ReserveChannels(nrc);

            sound_enabled = true;

            return n_channels;
        }
    }

    if (sound_enabled == false)
    {
        output_debug_message("Unable to open audio: %s\n", Mix_GetError());
        output_debug_message("Running the game without audio.\n");
    }

    return -1;
}

void Sound_release()
{
    Sound_release_music();
    if (sound_enabled)
    {
        // Sound_Quit();
        Mix_CloseAudio();
    }
    sound_enabled = false;
}

void Stop_playback()
{
    if (sound_enabled)
    {
        Sound_pause_music();
        // Mix_HookMusic(0, 0);
        Mix_CloseAudio();
        sound_enabled = false;
    }
}

void Resume_playback()
{
    Resume_playback(0, 0);
}

int Resume_playback(int nc, int nrc)
{
    char SoundcardName[256];
    int audio_rate = 44100;
    int audio_channels = 2;
    int audio_bufsize = AUDIO_BUFFER;
    Uint16 audio_format = AUDIO_S16;
    // SDL_version compile_version;
    n_channels = 8;

    sound_enabled = true;
    output_debug_message("Initializing SDL_mixer.\n");
    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_bufsize))
    {
        sound_enabled = false;
        output_debug_message("Unable to open audio: %s\n", Mix_GetError());
        output_debug_message("Running the game without audio.\n");
        return -1;
    }

    SDL_AudioDriverName(SoundcardName, sizeof(SoundcardName));
    // Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
    // output_debug_message("    opened %s at %d Hz %d bit %s, %d bytes audio buffer\n",
    // SoundcardName, audio_rate, audio_format & 0xFF,
    // audio_channels > 1 ? "stereo" : "mono", audio_bufsize);

    // MIX_VERSION(&compile_version);
    // output_debug_message("    compiled with SDL_mixer version: %d.%d.%d\n",
    // compile_version.major,
    // compile_version.minor,
    // compile_version.patch);
    // output_debug_message("    running with SDL_mixer version: %d.%d.%d\n",
    // Mix_Linked_Version()->major,
    // Mix_Linked_Version()->minor,
    // Mix_Linked_Version()->patch);

    if (nc > 0)
        n_channels = Mix_AllocateChannels(nc);
    if (nrc > 0)
        Mix_ReserveChannels(nrc);

    Sound_unpause_music();

    return n_channels;
}

Mix_Chunk* Sound_create_sound(const char* file)
{
    if (sound_enabled)
    {
        const char* ext[] = { "ogg", "wav" };
        for (auto e : ext)
        {
            char name[256];
            ::snprintf(name, sizeof(name), "%s.%s", file, e);

            auto sound = loadSound(name);
            if (sound != nullptr)
            {
                return sound;
            }
        }

        output_debug_message("ERROR in Sound_create_sound(): %s.(wav|ogg)\n", file);
    }

    return nullptr;
}

void Sound_delete_sound(Mix_Chunk* s)
{
    if (sound_enabled)
        Mix_FreeChunk(s);
}

int Sound_play(Mix_Chunk* s)
{
    if (sound_enabled)
        return Mix_PlayChannel(-1, s, 0);
    return -1;
}

int Sound_play(Mix_Chunk* s, int volume)
{
    if (sound_enabled)
    {
        int channel = Mix_PlayChannel(-1, s, 0);
        // output_debug_message("SFX player on channel %i (volume = %i)\n",channel,volume);
        Mix_Volume(channel, volume);
        return channel;
    }
    return -1;
}

int Sound_play_continuous(Mix_Chunk* s)
{
    if (sound_enabled)
        return Mix_PlayChannel(-1, s, -1);
    return -1;
}

int Sound_play_continuous(Mix_Chunk* s, int volume)
{
    if (sound_enabled)
    {
        int channel = Mix_PlayChannel(-1, s, -1);
        Mix_Volume(channel, volume);
        return channel;
    }
    return -1;
}

void Sound_play_ch(Mix_Chunk* s, int ch)
{
    if (sound_enabled && ch < n_channels)
        Mix_PlayChannel(ch, s, 0);
}

void Sound_play_ch(Mix_Chunk* s, int ch, int volume)
{
    //#ifdef __DEBUG_MESSAGES
    //	output_debug_message("Playing %p in %i at %i...",(void *)s,ch,volume);
    //#endif
    if (sound_enabled && ch < n_channels)
    {
        int channel = Mix_PlayChannel(ch, s, 0);
        Mix_Volume(channel, volume);
    }
    //#ifdef __DEBUG_MESSAGES
    //	output_debug_message("ok\n");
    //#endif
}

Mix_Music* Sound_create_stream(const char* file)
{
    if (sound_enabled)
    {
        const char* ext[] = { "ogg", "wav" };
        for (auto e : ext)
        {
            char name[256];
            ::snprintf(name, sizeof(name), "%s.%s", file, e);

            auto music = loadMusic(name);
            if (music != nullptr)
            {
                return music;
            }
        }

        output_debug_message("ERROR in Sound_create_stream(): %s.(wav|ogg)\n", file);
    }

    return nullptr;
}

void Sound_create_music(const char* f1, int loops)
{
    if (sound_enabled)
    {
        if (f1 != nullptr)
        {
            music_sound = Sound_create_stream(f1);
            Mix_PlayMusic(music_sound, loops);
        }
        else
        {
            music_sound = nullptr;
        }

        //		playing_music=true;
    }
}

void Sound_release_music(void)
{
    if (sound_enabled)
    {
        //		playing_music=false;
        Mix_HaltMusic();
        if (music_sound != nullptr)
        {
            Mix_FreeMusic(music_sound);
            music_sound = nullptr;
        }
    }
}

void Sound_pause_music(void)
{
    Mix_PauseMusic();
}

void Sound_unpause_music(void)
{
    Mix_ResumeMusic();
}

void Sound_music_volume(int volume)
{
    if (volume < 0)
        volume = 0;
    if (volume > 127)
        volume = 127;
    Mix_VolumeMusic(volume);
}
