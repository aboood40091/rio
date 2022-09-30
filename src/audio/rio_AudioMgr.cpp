#include <audio/rio_AudioMgr.h>
#include <filedevice/rio_FileDeviceMgr.h>

#include <vector>

#ifdef RIO_AUDIO_USE_SDL_MIXER

#include <SDL2/SDL.h>

namespace {

inline rio::FileHandle* GetFileHandleFromSDLRWops(SDL_RWops* rwops)
{
    return static_cast<rio::FileHandle*>(rwops->hidden.unknown.data1);
}

Sint64 SDLRWopsGetSize(SDL_RWops* context)
{
    return GetFileHandleFromSDLRWops(context)->getFileSize();
}

Sint64 SDLRWopsSeek(SDL_RWops* context, Sint64 offset, int whence)
{
    rio::FileHandle* p_handle = GetFileHandleFromSDLRWops(context);

    if (!p_handle->trySeek(offset, rio::FileDevice::SeekOrigin(whence)))
        return -1;

    return p_handle->getCurrentSeekPos();
}

size_t SDLRWopsRead(SDL_RWops* context, void* ptr, size_t size, size_t maxnum)
{
    const size_t total_size = size * maxnum;
    if (total_size == 0)
        return 0;

    rio::FileHandle* p_handle = GetFileHandleFromSDLRWops(context);

    if (uintptr_t(ptr) % rio::FileDevice::cBufferMinAlignment == 0)
        return p_handle->read(static_cast<u8*>(ptr), total_size) / size;

    u8* tmp = static_cast<u8*>(rio::MemUtil::alloc(total_size, rio::FileDevice::cBufferMinAlignment));

    u32 read_size = p_handle->read(static_cast<u8*>(tmp), total_size);
    if (read_size > 0)
        rio::MemUtil::copy(ptr, tmp, read_size);

    rio::MemUtil::free(tmp);

    return read_size / size;
}

size_t SDLRWopsWrite(SDL_RWops* context, const void* ptr, size_t size, size_t num)
{
    return 0;
}

int SDLRWopsClose(SDL_RWops* context)
{
    if (!GetFileHandleFromSDLRWops(context)->tryClose())
        return -1;

    return 0;
}

SDL_RWops* CreateSDLRWops(const char* fname)
{
    const std::string& path = std::string("sounds/") + fname;

    rio::FileHandle* p_handle = new rio::FileHandle;
    if (!rio::FileDeviceMgr::instance()->tryOpen(p_handle, path, rio::FileDevice::FILE_OPEN_FLAG_READ))
    {
        delete p_handle;
        return nullptr;
    }

    SDL_RWops* rwops = SDL_AllocRW();
    if (!rwops)
    {
        delete p_handle;
        return nullptr;
    }

    rwops->size = SDLRWopsGetSize;
    rwops->seek = SDLRWopsSeek;
    rwops->read = SDLRWopsRead;
    rwops->write = SDLRWopsWrite;
    rwops->close = SDLRWopsClose;
    rwops->hidden.unknown.data1 = p_handle;

    return rwops;
}

void DestroySDLRWops(SDL_RWops* rwops)
{
    delete GetFileHandleFromSDLRWops(rwops);
    SDL_FreeRW(rwops);
}

}

#endif // RIO_AUDIO_USE_SDL_MIXER

namespace rio {

AudioMgr* AudioMgr::sInstance = nullptr;

f32 AudioMgr::sMasterVolume = 1.0f;
f32 AudioMgr::sMusicVolume = 1.0f;
f32 AudioMgr::sSfxVolume = 1.0f;

bool AudioMgr::createSingleton()
{
    if (sInstance)
        return false;

    sInstance = new AudioMgr();
    if (!sInstance->mIsInitialized)
    {
        delete sInstance;
        sInstance = nullptr;
        return false;
    }

    return true;
}

void AudioMgr::destroySingleton()
{
    if (!sInstance)
        return;

    delete sInstance;
    sInstance = nullptr;
}

AudioMgr::AudioMgr()
    : mIsInitialized(false)
    , mAudioBgmCache()
    , mAudioSfxCache()
    , mListener{
        { 0.0f, 0.0f,  0.0f },
        { 0.0f, 0.0f, -1.0f },
        { 0.0f, 1.0f,  0.0f },
        { 0.0f, 0.0f, -1.0f },
        { 1.0f, 0.0f,  0.0f },
        2.0f
    }
{
#ifdef RIO_AUDIO_USE_SDL_MIXER
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        return;

    s32 mix_init_flags = MIX_INIT_MP3 |
                         MIX_INIT_OGG;

    if (Mix_Init(mix_init_flags) == 0)
    {
        SDL_Quit();
        return;
    }

    if (Mix_OpenAudio(32000, AUDIO_S16SYS, 2, 2048) < 0)
    {
        Mix_Quit();
        SDL_Quit();
        return;
    }

#ifdef RIO_DEBUG
    RIO_LOG("\nSDL2 Mixer supported decoders...\n");
    // Chunk
    {
        s32 total = Mix_GetNumChunkDecoders();
        for (s32 i = 0; i < total; i++)
            RIO_LOG(" - chunk decoder: %s\n", Mix_GetChunkDecoder(i));
    }
    // Music
    {
        s32 total = Mix_GetNumMusicDecoders();
        for (s32 i = 0; i < total; i++)
            RIO_LOG(" - music decoder: %s\n", Mix_GetMusicDecoder(i));
    }
    RIO_LOG("\n");
#endif // RIO_DEBUG

    Mix_AllocateChannels(AudioSfx::cSlotMaxNum);
    Mix_HookMusicFinished(AudioBgm::onFinishCallback_);

    setMasterVolume(sMasterVolume);
#endif // RIO_AUDIO_USE_SDL_MIXER
    mIsInitialized = true;
}

#ifdef RIO_AUDIO_USE_SDL_MIXER

AudioMgr::~AudioMgr()
{
    for (const auto& it : mAudioBgmCache)
    {
        AudioBgm* src = it.second;
        Mix_FreeMusic(src->mInnerHandle);
        DestroySDLRWops(src->mpRWops);
        delete src;
    }

    for (const auto& it : mAudioSfxCache)
    {
        AudioSfx* src = it.second;
        Mix_FreeChunk(src->mInnerHandle);
        DestroySDLRWops(src->mpRWops);
        delete src;
    }

    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}

bool AudioMgr::loadBgm(const char* fname, const char* key, AudioBgm::FileFormat format)
{
    RIO_ASSERT(mIsInitialized);

    if (getBgm(key))
        return true;

    SDL_RWops* const rwops = CreateSDLRWops(fname);
    if (!rwops)
        return false;

    Mix_Music* handle = Mix_LoadMUSType_RW(rwops, Mix_MusicType(format), false);
    if (!handle)
    {
        DestroySDLRWops(rwops);
        return false;
    }

    AudioBgm* src = new AudioBgm(rwops, handle);
    RIO_ASSERT(src != nullptr);

    mAudioBgmCache.try_emplace(key, src);
    return true;
}

AudioBgm* AudioMgr::getBgm(const char* key) const
{
    auto it = mAudioBgmCache.find(key);
    if (it != mAudioBgmCache.end())
        return it->second;

    return nullptr;
}

bool AudioMgr::loadSfx(const char* fname, const char* key)
{
    RIO_ASSERT(mIsInitialized);

    if (getSfx(key))
        return true;

    SDL_RWops* const rwops = CreateSDLRWops(fname);
    if (!rwops)
        return false;

    Mix_Chunk* handle = Mix_LoadWAV_RW(rwops, true);
    if (!handle)
    {
        DestroySDLRWops(rwops);
        return false;
    }

    AudioSfx* src = new AudioSfx(rwops, handle);
    RIO_ASSERT(src != nullptr);

    const auto& it = mAudioSfxCache.try_emplace(key, src);
    RIO_ASSERT(it.second);
    it.first->second->setVolume(1.0f);
    return true;
}

AudioSfx* AudioMgr::getSfx(const char* key) const
{
    auto it = mAudioSfxCache.find(key);
    if (it != mAudioSfxCache.end())
        return it->second;

    return nullptr;
}

void AudioMgr::setListenerPosition(const Vector3f& pos)
{
    mListener.pos = pos;
    calcListenerFront_();
}

void AudioMgr::setListenerLookAt(const Vector3f& look_at)
{
    mListener.look_at = look_at;
    calcListenerFront_();
}

void AudioMgr::setListenerUp(const Vector3f& up)
{
    mListener.up = up;
    calcListenerRight_();
}

void AudioMgr::calcListenerFront_()
{
    mListener.front.setSub(mListener.look_at, mListener.pos);
    mListener.front.normalize();
}

void AudioMgr::calcListenerRight_()
{
    mListener.right.setCross(mListener.front, mListener.up);
    mListener.right.normalize();
}

void AudioMgr::setListenerMaxDistance(f32 max_dist)
{
    RIO_ASSERT(max_dist >= 0.0f);
    mListener.max_dist = max_dist;
}

void AudioMgr::setListener(const Vector3f& pos, const Vector3f& look_at, const Vector3f& up)
{
    mListener.pos       = pos;
    mListener.look_at   = look_at;
    mListener.up        = up;

    calcListenerFront_();
    calcListenerRight_();
}

void AudioMgr::setMasterVolume(f32 volume)
{
    RIO_ASSERT(volume >= 0.0f && volume <= 1.0f);

    if (!mIsInitialized)
        return;

    f32 music_volume = AudioBgm::getCurrentVolume();

    f32* sfx_volume = new f32[mAudioSfxCache.size()];
    {
        s32 i = 0;
        for (const auto& it : mAudioSfxCache)
            sfx_volume[i++] = it.second->getVolume();
    }

    sMasterVolume = volume;

    AudioBgm::setCurrentVolume(music_volume);
    {
        s32 i = 0;
        for (const auto& it : mAudioSfxCache)
            it.second->setVolume(sfx_volume[i++]);
    }

    delete[] sfx_volume;
}

void AudioMgr::setMusicVolume(f32 volume)
{
    RIO_ASSERT(volume >= 0.0f && volume <= 1.0f);

    if (!mIsInitialized)
        return;

    f32 music_volume = AudioBgm::getCurrentVolume();

    sMusicVolume = volume;

    AudioBgm::setCurrentVolume(music_volume);
}

void AudioMgr::setSfxVolume(f32 volume)
{
    RIO_ASSERT(volume >= 0.0f && volume <= 1.0f);

    if (!mIsInitialized)
        return;

    f32* sfx_volume = new f32[mAudioSfxCache.size()];
    {
        s32 i = 0;
        for (const auto& it : mAudioSfxCache)
            sfx_volume[i++] = it.second->getVolume();
    }

    sSfxVolume = volume;

    {
        s32 i = 0;
        for (const auto& it : mAudioSfxCache)
            it.second->setVolume(sfx_volume[i++]);
    }

    delete[] sfx_volume;
}

f32 AudioMgr::getMasterVolume()
{
    if (!sInstance)
        return 0.0f;

    return sMasterVolume;
}

f32 AudioMgr::getMusicVolume()
{
    if (!sInstance)
        return 0.0f;

    return sMusicVolume;
}

f32 AudioMgr::getSfxVolume()
{
    if (!sInstance)
        return 0.0f;

    return sSfxVolume;
}

#endif // RIO_AUDIO_USE_SDL_MIXER

}
