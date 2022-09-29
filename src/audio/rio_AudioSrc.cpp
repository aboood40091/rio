#include <audio/rio_AudioMgr.h>
#include <audio/rio_AudioSrc.h>

namespace rio {

#ifdef RIO_AUDIO_USE_SDL_MIXER

AudioBgm* AudioBgm::sCurrent = nullptr;

void AudioBgm::setCurrentVolume(f32 volume)
{
    RIO_ASSERT(volume >= 0.0f);
    Mix_VolumeMusic(volume * (AudioMgr::instance()->getMusicVolume() * AudioMgr::instance()->getMasterVolume() * MIX_MAX_VOLUME));

    if (sCurrent)
        sCurrent->mVolume = volume;
}

f32 AudioBgm::getCurrentVolume()
{
    if (sCurrent)
        return sCurrent->mVolume;

    return Mix_VolumeMusic(-1) / (AudioMgr::instance()->getMusicVolume() * AudioMgr::instance()->getMasterVolume() * MIX_MAX_VOLUME);
}

void AudioBgm::play(bool loop)
{
    if (isPlaying())
        stop();

    Mix_PlayMusic(mInnerHandle, loop ? -1 : 0);
    sCurrent = this;

    setCurrentVolume(mVolume);
}

bool AudioBgm::isCurrent() const
{
    return sCurrent == this && Mix_PlayingMusic();
}

bool AudioBgm::isPlaying() const
{
    return isCurrent() && !Mix_PausedMusic();
}

bool AudioBgm::isPaused() const
{
    return isCurrent() && Mix_PausedMusic();
}

void AudioBgm::pause()
{
    if (isPlaying())
        Mix_PauseMusic();
}

void AudioBgm::resume()
{
    if (isPaused())
        Mix_ResumeMusic();
}

void AudioBgm::stop()
{
    if (isCurrent())
    {
        Mix_HaltMusic();
        sCurrent = nullptr;
    }
}

s32 AudioSfx::sCurrentSlot = 0;
AudioSfx::CurrentList AudioSfx::sCurrent{};

void AudioSfx::setVolume(f32 volume)
{
    RIO_ASSERT(volume >= 0.0f);
    Mix_VolumeChunk(mInnerHandle, volume * (AudioMgr::instance()->getSfxVolume() * AudioMgr::instance()->getMasterVolume() * MIX_MAX_VOLUME));
}

f32 AudioSfx::getVolume() const
{
    return Mix_VolumeChunk(mInnerHandle, -1) / (AudioMgr::instance()->getSfxVolume() * AudioMgr::instance()->getMasterVolume() * MIX_MAX_VOLUME);
}

AudioSfxHandle AudioSfx::play(bool loop)
{
    if (Mix_Playing(sCurrentSlot))
        sCurrentSlot = (sCurrentSlot + 1) % cSlotMaxNum;

    Mix_SetPosition(sCurrentSlot, 0, 0);

    Mix_PlayChannel(sCurrentSlot, mInnerHandle, loop ? -1 : 0);
    sCurrent[sCurrentSlot] = this;

    return AudioSfxHandle(this, sCurrentSlot);
}

void AudioSfx::play(u32 slot, bool loop)
{
    RIO_ASSERT(slot < size_t(cSlotMaxNum));

    if (isPlaying(slot))
        stop(slot);

    Mix_SetPosition(slot, 0, 0);

    Mix_PlayChannel(slot, mInnerHandle, loop ? -1 : 0);
    sCurrent[slot] = this;
}

AudioSfxHandle AudioSfx::play(const Vector3f& pos, bool loop)
{
    if (Mix_Playing(sCurrentSlot))
        sCurrentSlot = (sCurrentSlot + 1) % cSlotMaxNum;

    setPosition(sCurrentSlot, pos);

    Mix_PlayChannel(sCurrentSlot, mInnerHandle, loop ? -1 : 0);
    sCurrent[sCurrentSlot] = this;

    return AudioSfxHandle(this, sCurrentSlot);
}

void AudioSfx::play(u32 slot, const Vector3f& pos, bool loop)
{
    RIO_ASSERT(slot < size_t(cSlotMaxNum));

    if (isPlaying(slot))
        stop(slot);

    setPosition(slot, pos);

    Mix_PlayChannel(slot, mInnerHandle, loop ? -1 : 0);
    sCurrent[slot] = this;

}

void AudioSfx::setPosition(u32 slot, const Vector3f& pos)
{
    if (!isCurrent(slot))
        return;

    RIO_ASSERT(slot < size_t(cSlotMaxNum));

    const rio::Vector3f& front    = rio::AudioMgr::instance()->getListenerFront();
    const rio::Vector3f& right    = rio::AudioMgr::instance()->getListenerRight();
    const rio::Vector3f& cam_pos  = rio::AudioMgr::instance()->getListenerPosition();
    const f32            max_dist = rio::AudioMgr::instance()->getListenerMaxDistance();

    rio::Vector3f dir = pos - cam_pos;
    s32 dist = dir.length() / max_dist * 255;
    if (dist > 255)
        dist = 255;

    f32 angle = 90.f - rio::Mathf::rad2deg(std::atan2(dir.dot(front), dir.dot(right)));

    Mix_SetPosition(slot, angle, dist);
}

bool AudioSfx::isCurrent(u32 slot) const
{
    RIO_ASSERT(slot < size_t(cSlotMaxNum));
    return sCurrent[slot] == this && Mix_Playing(slot);
}

bool AudioSfx::isPlaying(u32 slot) const
{
    RIO_ASSERT(slot < size_t(cSlotMaxNum));
    return isCurrent(slot) && !Mix_Paused(slot);
}

bool AudioSfx::isPaused(u32 slot) const
{
    RIO_ASSERT(slot < size_t(cSlotMaxNum));
    return isCurrent(slot) && Mix_Paused(slot);
}

void AudioSfx::pause(u32 slot)
{
    RIO_ASSERT(slot < size_t(cSlotMaxNum));
    if (isPlaying(slot))
        Mix_Pause(slot);
}

void AudioSfx::resume(u32 slot)
{
    RIO_ASSERT(slot < size_t(cSlotMaxNum));
    if (isPaused(slot))
        Mix_Resume(slot);
}

void AudioSfx::stop(u32 slot)
{
    RIO_ASSERT(slot < size_t(cSlotMaxNum));
    if (isCurrent(slot))
    {
        Mix_HaltChannel(slot);
        sCurrent[slot] = nullptr;
    }
}

#endif // RIO_AUDIO_USE_SDL_MIXER

}
