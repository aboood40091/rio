#ifndef RIO_AUDIO_SOURCE_H
#define RIO_AUDIO_SOURCE_H

#include <math/rio_Vector.h>

#ifdef RIO_AUDIO_USE_SDL_MIXER
#include <SDL2/SDL_mixer.h>
#endif // RIO_AUDIO_USE_SDL_MIXER

#include <array>

namespace rio {

class AudioMgr;

class AudioBgm
{
public:
    enum FileFormat : u32
    {
#ifdef RIO_AUDIO_USE_SDL_MIXER
        FORMAT_AUTO = MUS_NONE,
        FORMAT_WAV  = MUS_WAV,
        FORMAT_OGG  = MUS_OGG,
        FORMAT_MP3  = MUS_MP3
#else
        FORMAT_AUTO = u32(-1),
        FORMAT_WAV  = u32(-1),
        FORMAT_OGG  = u32(-1),
        FORMAT_MP3  = u32(-1)
#endif // RIO_AUDIO_USE_SDL_MIXER
    };

#ifdef RIO_AUDIO_USE_SDL_MIXER
private:
    AudioBgm(SDL_RWops* rwops, Mix_Music* inner_handle)
        : mpRWops(rwops)
        , mInnerHandle(inner_handle)
    {
        RIO_ASSERT(rwops != nullptr);
        RIO_ASSERT(inner_handle != nullptr);
        mVolume = 1.0f;
    }

    static void onFinishCallback_();
#endif // RIO_AUDIO_USE_SDL_MIXER

#ifdef RIO_AUDIO_USE_SDL_MIXER

public:
    static void setCurrentVolume(f32 volume);
    static f32 getCurrentVolume();

    void setVolume(f32 volume)
    {
        if (isCurrent())
            setCurrentVolume(volume);
        else
            mVolume = volume;
    }

    f32 getVolume() const
    {
        return mVolume;
    }

    static void fadeOutCurrent(s32 ms);
    static bool isCurrentFadingOut() { return sIsCurrentFadingOut; }

    void fadeOut(s32 ms)
    {
        if (isCurrent())
            fadeOutCurrent(ms);
    }

    bool isFadingOut()
    {
        if (isCurrent())
            return isCurrentFadingOut();
        else
            return false;
    }

    void play(bool loop = true);

    bool isCurrent() const;
    bool isPlaying() const;
    bool isPaused() const;
    void pause();
    void resume();
    void stop();

    static AudioBgm* getCurrent()
    {
        return sCurrent;
    }

private:
    SDL_RWops* mpRWops;
    Mix_Music* mInnerHandle;
    f32 mVolume;

    static AudioBgm* sCurrent;
    static bool sIsCurrentFadingOut;

    friend class AudioMgr;

#else

    static void setCurrentVolume(f32 volume) { }
    static f32 getCurrentVolume() { return 0.0f; }

    void setVolume(f32 volume) { }
    f32 getVolume() const { return 0.0f; }

    void play(bool loop = true) { }

    bool isCurrent() const { return false; }
    bool isPlaying() const { return false; }
    bool isPaused() const { return false; }
    void pause() { }
    void resume() { }
    void stop() { }

    static AudioBgm* getCurrent() { return nullptr; }

#endif // RIO_AUDIO_USE_SDL_MIXER
};

class AudioSfxHandle;

class AudioSfx
{
public:
#ifdef RIO_AUDIO_USE_SDL_MIXER
    static constexpr s32 cSlotMaxNum = MIX_CHANNELS * 2;
#else
    static constexpr s32 cSlotMaxNum = 0;
#endif // RIO_AUDIO_USE_SDL_MIXER

#ifdef RIO_AUDIO_USE_SDL_MIXER
private:
    AudioSfx(SDL_RWops* rwops, Mix_Chunk* inner_handle)
        : mpRWops(rwops)
        , mInnerHandle(inner_handle)
    {
        RIO_ASSERT(rwops != nullptr);
        RIO_ASSERT(inner_handle != nullptr);
    }
#endif // RIO_AUDIO_USE_SDL_MIXER

#ifdef RIO_AUDIO_USE_SDL_MIXER

public:
    void setVolume(f32 volume);
    f32 getVolume() const;

    AudioSfxHandle play(bool loop = false);
    void play(u32 slot, bool loop = false);

    AudioSfxHandle play(const Vector3f& pos, bool loop = false);
    void play(u32 slot, const Vector3f& pos, bool loop = false);

    void setPosition(u32 slot, const Vector3f& pos);

    bool isCurrent(u32 slot) const;
    bool isPlaying(u32 slot) const;
    bool isPaused(u32 slot) const;
    void pause(u32 slot);
    void resume(u32 slot);
    void stop(u32 slot);

    static AudioSfx* getCurrent(u32 slot)
    {
        RIO_ASSERT(slot < size_t(cSlotMaxNum));
        return sCurrent[slot];
    }

private:
    SDL_RWops* mpRWops;
    Mix_Chunk* mInnerHandle;

    static s32 sCurrentSlot;

    typedef std::array<AudioSfx*, cSlotMaxNum> CurrentList;
    static CurrentList sCurrent;

    friend class AudioMgr;

#else

public:
    void setVolume(f32 volume) { }
    f32 getVolume() const { return 0.0f; }

    AudioSfxHandle play(bool loop = false);
    void play(u32 slot, bool loop = false) { }

    AudioSfxHandle play(const Vector3f& pos, bool loop = false);
    void play(u32 slot, const Vector3f& pos, bool loop = false) { }

    void setPosition(u32 slot, const Vector3f& pos) { }

    bool isCurrent(u32 slot) const { return false; }
    bool isPlaying(u32 slot) const { return false; }
    bool isPaused(u32 slot) const { return false; }
    void pause(u32 slot) { }
    void resume(u32 slot) { }
    void stop(u32 slot) { }

    static AudioSfx* getCurrent(u32 slot) { return nullptr; }

#endif // RIO_AUDIO_USE_SDL_MIXER
};

class AudioSfxHandle
{
#ifdef RIO_AUDIO_USE_SDL_MIXER
private:
    AudioSfxHandle(AudioSfx* sfx, s32 slot)
        : mSfx(sfx)
        , mSlot(slot)
    {
        RIO_ASSERT(sfx != nullptr);
    }
#endif // RIO_AUDIO_USE_SDL_MIXER

#ifdef RIO_AUDIO_USE_SDL_MIXER

public:
    AudioSfx* getSfx() const { return mSfx; }
    s32 getSlot() const { return mSlot; }

    bool isCurrent() const
    {
        return mSfx->isCurrent(mSlot);
    }

    bool isPlaying() const
    {
        return mSfx->isPlaying(mSlot);
    }

    bool isPaused() const
    {
        return mSfx->isPaused(mSlot);
    }

    void pause()
    {
        mSfx->pause(mSlot);
    }

    void resume()
    {
        mSfx->resume(mSlot);
    }

    void stop()
    {
        mSfx->stop(mSlot);
    }

private:
    AudioSfx* mSfx;
    s32 mSlot;

    friend class AudioSfx;

#else

public:
    AudioSfx* getSfx() const { return nullptr; }
    s32 getSlot() const { return 0; }

    bool isCurrent() const { return false; }
    bool isPlaying() const { return false; }
    bool isPaused() const { return false; }
    void pause() { }
    void resume() { }
    void stop() { }

#endif // RIO_AUDIO_USE_SDL_MIXER
};

#ifndef RIO_AUDIO_USE_SDL_MIXER

inline AudioSfxHandle AudioSfx::play(bool loop) { return AudioSfxHandle(); }
inline AudioSfxHandle AudioSfx::play(const Vector3f& pos, bool loop) { return AudioSfxHandle(); }

#endif // RIO_AUDIO_USE_SDL_MIXER

}

#endif // RIO_AUDIO_SOURCE_H
