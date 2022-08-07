#include <filedevice/rio_FileDeviceMgr.h>
#include <gfx/mdl/res/rio_ModelCacher.h>
#include <gfx/mdl/res/rio_ModelData.h>
#include <gpu/rio_Drawer.h>

namespace rio { namespace mdl { namespace res {

ModelCacher* ModelCacher::sInstance = nullptr;

bool ModelCacher::createSingleton()
{
    if (sInstance)
        return false;

    sInstance = new ModelCacher();
    return true;
}

void ModelCacher::destroySingleton()
{
    if (!sInstance)
        return;

    delete sInstance;
    sInstance = nullptr;
}

ModelCacher::ModelCacher()
{
}

ModelCacher::~ModelCacher()
{
    for (const auto& it : mModelCache)
        MemUtil::free(it.second);

    mModelCache.clear();
}

Model* ModelCacher::loadModel(const char* base_fname, const char* key)
{
    // Check if it exists
    if (Model* model = get(key))
        return model;

    FileDevice::LoadArg arg;
#if RIO_IS_WIN
    arg.path = std::string("models/") + base_fname + "_LE.rmdl";
#elif RIO_IS_CAFE
    arg.path = std::string("models/") + base_fname + "_BE.rmdl";
#else
    arg.path = std::string("models/") + base_fname + ".rmdl";
#endif
    arg.alignment = Drawer::cVtxAlignment;

    u8* const file = FileDeviceMgr::instance()->tryLoad(arg);
    if (!file || arg.read_size < sizeof(Model))
        return nullptr;

    Model* model = (Model*)file;

    RIO_ASSERT(model->mMagic[0] == 'r' &&
               model->mMagic[1] == 'i' &&
               model->mMagic[2] == 'o' &&
               model->mMagic[3] == 'm' &&
               model->mMagic[4] == 'o' &&
               model->mMagic[5] == 'd' &&
               model->mMagic[6] == 'e' &&
               model->mMagic[7] == 'l');

    RIO_ASSERT(Model::cVersionMin <= model->mVersion &&
                                     model->mVersion <= Model::cVersionCurrent);

    RIO_ASSERT(model->mFileSize == arg.read_size);

    auto it = mModelCache.try_emplace(key, model);
    return it.first->second;
}

Model* ModelCacher::get(const char* key) const
{
    auto it = mModelCache.find(key);
    if (it != mModelCache.end())
        return it->second;

    return nullptr;
}

} } }
