# Built-in libraries
import os
import struct
import sys

# Local libraries
from ninTexUtils.gx2.gfd import GFDFile
from ninTexUtils.gx2 import GX2Surface
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

    def save(self):
        return struct.pack(
            self._sFmt,
            self.width,
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

            0x5101382D,  # Magic: "RIOTEX2D" + Innovation :)))
            0x01000000   # Version 1.0.0.0
        )


def main():
    # Check input
    file = sys.argv[-1]
    if len(sys.argv) < 2 or not os.path.isfile(file):
        raise RuntimeError("No valid input file was given!")

    # Read the whole file
    with open(file, "rb") as inf:
        inb = inf.read()

    # Create a new GFDFile object
    gfd = GFDFile()

    # Parse the input file
    gfd.load(inb)
    assert len(gfd.textures) == 1
    texture = gfd.textures[0]

    # Print debug info
    GX2TexturePrintInfo(texture)

    # Check if format is supported
    global supported_formats
    assert texture.surface.format in supported_formats

    assert texture.surface.dim == 1
    assert texture.surface.depth <= 1

    # Create a new GX2Texture to store the untiled texture
    linear_texture = GX2Texture.initTexture(
        1, texture.surface.width, texture.surface.height,
        1, texture.surface.numMips, texture.surface.format,
        texture.compSel, GX2TileMode.Linear_Special,
    )

    # Untile our texture
    GX2Surface.copySurface(texture.surface, linear_texture.surface)

    # Export texture
    rio_tex = RioTexture()
    rio_tex.width = linear_texture.surface.width
    rio_tex.height = linear_texture.surface.height
    rio_tex.mipLevels = max(linear_texture.surface.numMips, 1)
    rio_tex.format = linear_texture.surface.format
    rio_tex.imageSize = linear_texture.surface.imageSize
    rio_tex.imageData = linear_texture.surface.imageData
    rio_tex.imageOffset = 0x80
    rio_tex.mipmapSize = linear_texture.surface.mipSize
    rio_tex.compMap = linear_texture.compSel

    if rio_tex.mipLevels > 1:
        assert linear_texture.surface.mipOffset[0] == rio_tex.imageSize
        rio_tex.mipLevelOffset = linear_texture.surface.mipOffset
        rio_tex.mipLevelOffset[0] = 0
        rio_tex.mipmapsData = linear_texture.surface.mipData
        rio_tex.mipmapsOffset = rio_tex.imageOffset + rio_tex.imageSize

    else:
        rio_tex.mipLevelOffset = [0 for _ in range(13)]
        rio_tex.mipmapsData = b''
        rio_tex.mipmapsOffset = 0

    if rio_tex.format == TEXTURE_FORMAT_R8_UNORM:
        rio_tex.gl_internalformat = 0x8229
        rio_tex.gl_format = 0x1903
        rio_tex.gl_type = 0x1401

    elif rio_tex.format == TEXTURE_FORMAT_R8_UINT:
        rio_tex.gl_internalformat = 0x8232
        rio_tex.gl_format = 0x8D94
        rio_tex.gl_type = 0x1401

    elif rio_tex.format == TEXTURE_FORMAT_R8_SNORM:
        rio_tex.gl_internalformat = 0x8F94
        rio_tex.gl_format = 0x1903
        rio_tex.gl_type = 0x1400

    elif rio_tex.format == TEXTURE_FORMAT_R8_SINT:
        rio_tex.gl_internalformat = 0x8231
        rio_tex.gl_format = 0x8D94
        rio_tex.gl_type = 0x1400

    elif rio_tex.format == TEXTURE_FORMAT_R8_G8_UNORM:
        rio_tex.gl_internalformat = 0x822B
        rio_tex.gl_format = 0x8227
        rio_tex.gl_type = 0x1401

    elif rio_tex.format == TEXTURE_FORMAT_R8_G8_UINT:
        rio_tex.gl_internalformat = 0x8238
        rio_tex.gl_format = 0x8228
        rio_tex.gl_type = 0x1401

    elif rio_tex.format == TEXTURE_FORMAT_R8_G8_SNORM:
        rio_tex.gl_internalformat = 0x8F95
        rio_tex.gl_format = 0x8227
        rio_tex.gl_type = 0x1400

    elif rio_tex.format == TEXTURE_FORMAT_R8_G8_SINT:
        rio_tex.gl_internalformat = 0x8237
        rio_tex.gl_format = 0x8228
        rio_tex.gl_type = 0x1400

    elif rio_tex.format == TEXTURE_FORMAT_R5_G6_B5_UNORM:
        rio_tex.gl_internalformat = 0x8D62
        rio_tex.gl_format = 0x1907
        rio_tex.gl_type = 0x8363

    elif rio_tex.format == TEXTURE_FORMAT_R5_G5_B5_A1_UNORM:
        rio_tex.gl_internalformat = 0x8057
        rio_tex.gl_format = 0x1908
        rio_tex.gl_type = 0x8034

    elif rio_tex.format == TEXTURE_FORMAT_R4_G4_B4_A4_UNORM:
        rio_tex.gl_internalformat = 0x8056
        rio_tex.gl_format = 0x1908
        rio_tex.gl_type = 0x8033

    elif rio_tex.format == TEXTURE_FORMAT_R8_G8_B8_A8_UNORM:
        rio_tex.gl_internalformat = 0x8058
        rio_tex.gl_format = 0x1908
        rio_tex.gl_type = 0x1401

    elif rio_tex.format == TEXTURE_FORMAT_R8_G8_B8_A8_UINT:
        rio_tex.gl_internalformat = 0x8D7C
        rio_tex.gl_format = 0x8D99
        rio_tex.gl_type = 0x1401

    elif rio_tex.format == TEXTURE_FORMAT_R8_G8_B8_A8_SNORM:
        rio_tex.gl_internalformat = 0x8F97
        rio_tex.gl_format = 0x1908
        rio_tex.gl_type = 0x1400

    elif rio_tex.format == TEXTURE_FORMAT_R8_G8_B8_A8_SINT:
        rio_tex.gl_internalformat = 0x8D8E
        rio_tex.gl_format = 0x8D99
        rio_tex.gl_type = 0x1400

    elif rio_tex.format == TEXTURE_FORMAT_R8_G8_B8_A8_SRGB:
        rio_tex.gl_internalformat = 0x8C43
        rio_tex.gl_format = 0x1908
        rio_tex.gl_type = 0x1401

    elif rio_tex.format == TEXTURE_FORMAT_R10_G10_B10_A2_UNORM:
        rio_tex.gl_internalformat = 0x8059
        rio_tex.gl_format = 0x1908
        rio_tex.gl_type = 0x8368

    elif rio_tex.format == TEXTURE_FORMAT_R10_G10_B10_A2_UINT:
        rio_tex.gl_internalformat = 0x906F
        rio_tex.gl_format = 0x8D99
        rio_tex.gl_type = 0x8368

    elif rio_tex.format == TEXTURE_FORMAT_BC1_UNORM:
        rio_tex.gl_internalformat = 0x83F1
        rio_tex.gl_format = 0
        rio_tex.gl_type = 0

    elif rio_tex.format == TEXTURE_FORMAT_BC1_SRGB:
        rio_tex.gl_internalformat = 0x8C4D
        rio_tex.gl_format = 0
        rio_tex.gl_type = 0

    elif rio_tex.format == TEXTURE_FORMAT_BC2_UNORM:
        rio_tex.gl_internalformat = 0x83F2
        rio_tex.gl_format = 0
        rio_tex.gl_type = 0

    elif rio_tex.format == TEXTURE_FORMAT_BC2_SRGB:
        rio_tex.gl_internalformat = 0x8C4E
        rio_tex.gl_format = 0
        rio_tex.gl_type = 0

    elif rio_tex.format == TEXTURE_FORMAT_BC3_UNORM:
        rio_tex.gl_internalformat = 0x83F3
        rio_tex.gl_format = 0
        rio_tex.gl_type = 0

    elif rio_tex.format == TEXTURE_FORMAT_BC3_SRGB:
        rio_tex.gl_internalformat = 0x8C4F
        rio_tex.gl_format = 0
        rio_tex.gl_type = 0

    elif rio_tex.format == TEXTURE_FORMAT_BC4_UNORM:
        rio_tex.gl_internalformat = 0x8DBB
        rio_tex.gl_format = 0
        rio_tex.gl_type = 0

    elif rio_tex.format == TEXTURE_FORMAT_BC4_SNORM:
        rio_tex.gl_internalformat = 0x8DBC
        rio_tex.gl_format = 0
        rio_tex.gl_type = 0

    elif rio_tex.format == TEXTURE_FORMAT_BC5_UNORM:
        rio_tex.gl_internalformat = 0x8DBD
        rio_tex.gl_format = 0
        rio_tex.gl_type = 0

    else:  # if rio_tex.format == TEXTURE_FORMAT_BC5_SNORM:
        rio_tex.gl_internalformat = 0x8DBE
        rio_tex.gl_format = 0
        rio_tex.gl_type = 0

    # Get the filename without the extension
    filename = os.path.splitext(file)[0]

    # Bruh
    out_data = rio_tex.save() + rio_tex.imageData + rio_tex.mipmapsData
    with open(filename + '.rtx', 'wb') as outf:
        outf.write(out_data)


if __name__ == '__main__':
    main()
