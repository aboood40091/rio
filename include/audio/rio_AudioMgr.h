#ifndef RIO_AUDIO_MANAGER_H
#define RIO_AUDIO_MANAGER_H

#include <audio/rio_AudioSrc.h>

#include <map>
#include <string>

namespace rio {

class AudioMgr
{
public:
    static bool createSingleton();
    static void destroySingleton();
    static AudioMgr* instance() { return sInstance; }

private:
    static AudioMgr* sInstance;

    AudioMgr();
    ~AudioMgr();

    AudioMgr(const AudioMgr&);
    AudioMgr& operator=(const AudioMgr&);

public:
    bool loadBgm(const char* fname, const char* key, AudioBgm::FileFormat format = AudioBgm::FORMAT_AUTO);
    AudioBgm* getBgm(const char* key) const;

    bool loadSfx(const char* fname, const char* key);
    AudioSfx* getSfx(const char* key) const;

    void setListenerPosition(const Vector3f& pos);
    void setListenerLookAt(const Vector3f& look_at);
    void setListenerUp(const Vector3f& up);

    void setListenerMaxDistance(f32 max_dist);

    void setListener(const Vector3f& pos, const Vector3f& look_at, const Vector3f& up);

    void setListener(const Vector3f& pos, const Vector3f& look_at, const Vector3f& up, f32 max_dist)
    {
        setListener(pos, look_at, up);
        setListenerMaxDistance(max_dist);
    }

    const Vector3f& getListenerPosition() const
    {
        return mListener.pos;
    }

    const Vector3f& getListenerLookAt() const
    {
        return mListener.look_at;
    }

    const Vector3f& getListenerUp() const
    {
        return mListener.up;
    }

    const Vector3f& getListenerFront() const
    {
        return mListener.front;
    }

    const Vector3f& getListenerRight() const
    {
        return mListener.right;
    }

    f32 getListenerMaxDistance() const
    {
        return mListener.max_dist;
    }

    void setMasterVolume(f32 volume);
    void setMusicVolume(f32 volume);
    void setSfxVolume(f32 volume);

    static f32 getMasterVolume();
    static f32 getMusicVolume();
    static f32 getSfxVolume();

private:
    void calcListenerFront_();
    void calcListenerRight_();

private:
    bool mIsInitialized;
    std::map<std::string, AudioBgm*> mAudioBgmCache;
    std::map<std::string, AudioSfx*> mAudioSfxCache;

    struct
    {
        Vector3f pos;
        Vector3f look_at;
        Vector3f up;
        Vector3f front;
        Vector3f right;
        f32 max_dist;
    } mListener;

    static f32 sMasterVolume;
    static f32 sMusicVolume;
    static f32 sSfxVolume;
};

#ifndef RIO_AUDIO_USE_SDL_MIXER

inline AudioMgr::~AudioMgr() { }

inline bool AudioMgr::loadBgm(const char* fname, const char* key, AudioBgm::FileFormat format) { return false; }
inline AudioBgm* AudioMgr::getBgm(const char* key) const { return nullptr; }

inline bool AudioMgr::loadSfx(const char* fname, const char* key) { return false; }
inline AudioSfx* AudioMgr::getSfx(const char* key) const { return nullptr; }

inline void AudioMgr::setListenerPosition(const Vector3f& pos) { }
inline void AudioMgr::setListenerLookAt(const Vector3f& look_at) { }
inline void AudioMgr::setListenerUp(const Vector3f& up) { }

inline void AudioMgr::setListenerMaxDistance(f32 max_dist) { }

inline void AudioMgr::setListener(const Vector3f& pos, const Vector3f& look_at, const Vector3f& up) { }

inline void AudioMgr::setMasterVolume(f32 volume) { }
inline void AudioMgr::setMusicVolume(f32 volume) { }
inline void AudioMgr::setSfxVolume(f32 volume) { }

inline f32 AudioMgr::getMasterVolume() { return 0.0f; }
inline f32 AudioMgr::getMusicVolume() { return 0.0f; }
inline f32 AudioMgr::getSfxVolume() { return 0.0f; }

#endif // RIO_AUDIO_USE_SDL_MIXER

}

#endif // RIO_AUDIO_MANAGER_H
