#ifndef RIO_GFX_LYR_RENDERER_H
#define RIO_GFX_LYR_RENDERER_H

#include <gfx/lyr/rio_Layer.h>

namespace rio { namespace lyr {

class Renderer
{
public:
    // Create renderer singleton instance
    static bool createSingleton();
    // Destroy renderer singleton instance
    static void destroySingleton();
    // Get renderer singleton instance
    static Renderer* instance() { return sInstance; }

private:
    static Renderer* sInstance;

    Renderer();
    ~Renderer();

    Renderer(const Renderer&);
    Renderer& operator=(const Renderer&);

public:
    // Create layer with the given name and render priority (Smaller value = drawn later)
    template <typename T = Layer>
    Layer::iterator addLayer(const char* name, s32 priority = 0)
    {
        Layer* p_layer = new T(name, priority);
        return mLayers.insert(p_layer);
    }

    // Remove layer
    void removeLayer(Layer::iterator it)
    {
        Layer* p_layer = Layer::peelIterator(it);
        mLayers.erase(it);
        delete p_layer;
    }

    // Remove all layers
    void clearLayers()
    {
        for (Layer* p_layer : mLayers)
            delete p_layer;

        mLayers.clear();
    }

    // Render all layers
    void render() const;

private:
     Layer::List mLayers; // List of all layers
};

} }

#endif // RIO_GFX_LYR_RENDERER_H
