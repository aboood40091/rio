# Built-in libraries
import os
import struct
import sys

# Local libraries
from ninTexUtils.gx2.gfd import GFDFile
from ninTexUtils.gx2 import GX2Surface
from ninTexUtils.gx2 import GX2SurfaceFormat
from ninTexUtils.gx2 import GX2Texture
from ninTexUtils.gx2 import GX2TexturePrintInfo
from ninTexUtils.gx2 import GX2TileMode


TEXTURE_FORMAT_R8_UNORM             = 0x001
TEXTURE_FORMAT_R8_UINT              = 0x101
TEXTURE_FORMAT_R8_SNORM             = 0x201
TEXTURE_FORMAT_R8_SINT              = 0x301
TEXTURE_FORMAT_R4_G4_UNORM          = 0x002
TEXTURE_FORMAT_R8_G8_UNORM          = 0x007
TEXTURE_FORMAT_R8_G8_UINT           = 0x107
TEXTURE_FORMAT_R8_G8_SNORM          = 0x207
TEXTURE_FORMAT_R8_G8_SINT           = 0x307
TEXTURE_FORMAT_R5_G6_B5_UNORM       = 0x008
TEXTURE_FORMAT_R5_G5_B5_A1_UNORM    = 0x00A
TEXTURE_FORMAT_R4_G4_B4_A4_UNORM    = 0x00B
TEXTURE_FORMAT_R8_G8_B8_A8_UNORM    = 0x01A
TEXTURE_FORMAT_R8_G8_B8_A8_UINT     = 0x11A
TEXTURE_FORMAT_R8_G8_B8_A8_SNORM    = 0x21A
TEXTURE_FORMAT_R8_G8_B8_A8_SINT     = 0x31A
TEXTURE_FORMAT_R8_G8_B8_A8_SRGB     = 0x41A
TEXTURE_FORMAT_R10_G10_B10_A2_UNORM = 0x019
TEXTURE_FORMAT_R10_G10_B10_A2_UINT  = 0x119
TEXTURE_FORMAT_R10_G10_B10_A2_SNORM = 0x219
TEXTURE_FORMAT_R10_G10_B10_A2_SINT  = 0x319
TEXTURE_FORMAT_BC1_UNORM            = 0x031
TEXTURE_FORMAT_BC1_SRGB             = 0x431
TEXTURE_FORMAT_BC2_UNORM            = 0x032
TEXTURE_FORMAT_BC2_SRGB             = 0x432
TEXTURE_FORMAT_BC3_UNORM            = 0x033
TEXTURE_FORMAT_BC3_SRGB             = 0x433
TEXTURE_FORMAT_BC4_UNORM            = 0x034
TEXTURE_FORMAT_BC4_SNORM            = 0x234
TEXTURE_FORMAT_BC5_UNORM            = 0x035
TEXTURE_FORMAT_BC5_SNORM            = 0x235

supported_formats = (
    TEXTURE_FORMAT_R8_UNORM,
    TEXTURE_FORMAT_R8_UINT,
    TEXTURE_FORMAT_R8_SNORM,
    TEXTURE_FORMAT_R8_SINT,
    TEXTURE_FORMAT_R8_G8_UNORM,
    TEXTURE_FORMAT_R8_G8_UINT,
    TEXTURE_FORMAT_R8_G8_SNORM,
    TEXTURE_FORMAT_R8_G8_SINT,
    TEXTURE_FORMAT_R5_G6_B5_UNORM,
    TEXTURE_FORMAT_R5_G5_B5_A1_UNORM,
    TEXTURE_FORMAT_R4_G4_B4_A4_UNORM,
    TEXTURE_FORMAT_R8_G8_B8_A8_UNORM,
    TEXTURE_FORMAT_R8_G8_B8_A8_UINT,
    TEXTURE_FORMAT_R8_G8_B8_A8_SNORM,
    TEXTURE_FORMAT_R8_G8_B8_A8_SINT,
    TEXTURE_FORMAT_R8_G8_B8_A8_SRGB,
    TEXTURE_FORMAT_R10_G10_B10_A2_UNORM,
    TEXTURE_FORMAT_R10_G10_B10_A2_UINT,
    TEXTURE_FORMAT_BC1_UNORM,
    TEXTURE_FORMAT_BC1_SRGB,
    TEXTURE_FORMAT_BC2_UNORM,
    TEXTURE_FORMAT_BC2_SRGB,
    TEXTURE_FORMAT_BC3_UNORM,
    TEXTURE_FORMAT_BC3_SRGB,
    TEXTURE_FORMAT_BC4_UNORM,
    TEXTURE_FORMAT_BC4_SNORM,
    TEXTURE_FORMAT_BC5_UNORM,
    TEXTURE_FORMAT_BC5_SNORM
)


class RioTexture:
    _sFmt = '<24I16x3I4x'
    assert struct.calcsize(_sFmt) == 0x80

    def load(self, data, pos=0):
        self.mipLevelOffset = [0 for _ in range(13)]

        (self.width,
         self.height,
         self.mipLevels,
         self.format,
         self.gl_internalformat,
         self.gl_format,
         self.gl_type,
         self.imageSize,
         self.imageOffset,
         self.mipmapSize,
         self.mipmapsOffset,
         *self.mipLevelOffset,
         self.compMap,

         magic,
         version) = struct.unpack_from(self._sFmt, data, pos)

        assert magic == 0x5101382D
        assert version == 0x01000000


def main():
    # Check input
    file = sys.argv[-1]
    if len(sys.argv) < 2 or not os.path.isfile(file):
        raise RuntimeError("No valid input file was given!")

    # Read the whole file
    with open(file, "rb") as inf:
        inb = inf.read()

    # Create a new RioTexture object
    rio_tex = RioTexture()
    # Parse the input file
    rio_tex.load(inb)

    # Check if format is supported
    global supported_formats
    assert rio_tex.format in supported_formats

    # Create a new GX2Texture to store the untiled texture
    linear_texture = GX2Texture.initTexture(
        1, rio_tex.width, rio_tex.height,
        1, rio_tex.mipLevels, GX2SurfaceFormat(rio_tex.format),
        rio_tex.compMap, GX2TileMode.Linear_Special,
    )

    # Set image data
    imageData = inb[rio_tex.imageOffset:rio_tex.imageOffset + rio_tex.imageSize]
    linear_texture.surface.imageSize = len(imageData)
    linear_texture.surface.imageData = imageData

    # Set mipmap data
    if rio_tex.mipLevels > 1:
        mipData = inb[rio_tex.mipmapsOffset:rio_tex.mipmapsOffset + rio_tex.mipmapSize]
        linear_texture.surface.mipSize = len(mipData)
        linear_texture.surface.mipData = mipData

    else:
        linear_texture.surface.mipSize = 0
        linear_texture.surface.mipData = b''

    # Export texture
    gfd = GFDFile()
    gfd.textures.append(linear_texture)

    # Print debug info
    GX2TexturePrintInfo(linear_texture)

    # Get the filename without the extension
    filename = os.path.splitext(file)[0]

    # Bruh
    out_data = gfd.save()
    with open(filename + '.gtx', 'wb') as outf:
        outf.write(out_data)


if __name__ == '__main__':
    main()
