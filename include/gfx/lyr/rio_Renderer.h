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
    Layer::iterator addLayer(const char* name, s32 priority = 0);

    // Remove layer
    void removeLayer(Layer::iterator it);
    // Remove all layers
    void clearLayers();

    // Render all layers
    void render() const;

private:
     Layer::List mLayers; // List of all layers
};

} }

#endif // RIO_GFX_LYR_RENDERER_H
