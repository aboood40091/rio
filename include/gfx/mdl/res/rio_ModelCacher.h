#ifndef RIO_GFX_MDL_RES_MODEL_CACHER_H
#define RIO_GFX_MDL_RES_MODEL_CACHER_H

#include <misc/rio_Types.h>

#include <unordered_map>
#include <string>

namespace rio { namespace mdl { namespace res {

class Model;

class ModelCacher
{
    // Model resource cache manager class
    // TODO: Reference counter + unload

public:
    static bool createSingleton();
    static void destroySingleton();
    static ModelCacher* instance() { return sInstance; }

private:
    static ModelCacher* sInstance;

    ModelCacher();
    ~ModelCacher();

    ModelCacher(const ModelCacher&);
    ModelCacher& operator=(const ModelCacher&);

public:
    Model* loadModel(const char* base_fname, const char* key);
    Model* get(const char* key) const;

private:
    std::unordered_map<std::string, Model*> mModelCache;
};

} } }

#endif // RIO_GFX_MDL_RES_MODEL_CACHER_H
