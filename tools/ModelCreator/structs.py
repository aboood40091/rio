from enum import IntEnum
from enum import IntFlag


CURRENT_VERSION = 0x01000000


class Vertex:
    def __init__(self, pos, texCoord, normal):
        self.pos = pos
        self.texCoord = texCoord
        self.normal = normal

    def __hash__(self):
        return hash((self.pos, self.texCoord, self.normal))

    def __eq__(self, other):
        if not isinstance(other, type(self)):
            return NotImplemented

        return (self.pos[0] == other.pos[0] and \
                self.pos[1] == other.pos[1] and \
                self.pos[2] == other.pos[2] and \
                self.texCoord[0] == other.texCoord[0] and \
                self.texCoord[1] == other.texCoord[1] and \
                self.normal[0] == other.normal[0] and \
                self.normal[1] == other.normal[1] and \
                self.normal[2] == other.normal[2])


class Mesh:
    def __init__(self):
        self.materialIdx = -1

        self.vertices = []
        self.indices = []

        self.scale = [1.0, 1.0, 1.0]
        self.rotate = [0.0, 0.0, 0.0]
        self.translate = [0.0, 0.0, 0.0]


class TexXYFilterMode(IntEnum):
    TEX_XY_FILTER_MODE_POINT  = 0
    TEX_XY_FILTER_MODE_LINEAR = 1


class TexMipFilterMode(IntEnum):
    TEX_MIP_FILTER_MODE_NONE   = 0
    TEX_MIP_FILTER_MODE_POINT  = 1
    TEX_MIP_FILTER_MODE_LINEAR = 2


class TexAnisoRatio(IntEnum):
    TEX_ANISO_1_TO_1  = 0
    TEX_ANISO_2_TO_1  = 1
    TEX_ANISO_4_TO_1  = 2
    TEX_ANISO_8_TO_1  = 3
    TEX_ANISO_16_TO_1 = 4


class TexWrapMode(IntEnum):
    TEX_WRAP_MODE_REPEAT                  = 0
    TEX_WRAP_MODE_MIRROR                  = 1
    TEX_WRAP_MODE_CLAMP                   = 2
    TEX_WRAP_MODE_MIRROR_ONCE             = 3
    TEX_WRAP_MODE_CLAMP_HALF_BORDER       = 4
    TEX_WRAP_MODE_MIRROR_ONCE_HALF_BORDER = 5
    TEX_WRAP_MODE_CLAMP_BORDER            = 6
    TEX_WRAP_MODE_MIRROR_ONCE_BORDER      = 7


class Texture:
    def __init__(self):
        self.name = ""
        self.samplerName = ""

        self.magFilter = TexXYFilterMode.TEX_XY_FILTER_MODE_LINEAR
        self.minFilter = TexXYFilterMode.TEX_XY_FILTER_MODE_LINEAR
        self.mipFilter = TexMipFilterMode.TEX_MIP_FILTER_MODE_LINEAR
        self.maxAniso = TexAnisoRatio.TEX_ANISO_1_TO_1

        self.wrapX = TexWrapMode.TEX_WRAP_MODE_CLAMP
        self.wrapY = TexWrapMode.TEX_WRAP_MODE_CLAMP
        self.wrapZ = TexWrapMode.TEX_WRAP_MODE_CLAMP

        self.borderColor = [0, 0, 0, 255]

        self.minLOD = 0.0
        self.maxLOD = 14.0
        self.LODBias = 0.0


class RenderFlags(IntFlag):
    DEPTH_TEST_ENABLE                = 0x0001
    DEPTH_WRITE_ENABLE               = 0x0002
    BLEND_ENABLE                     = 0x0004
    ALPHA_TEST_ENABLE                = 0x0008
    COLOR_MASK_R                     = 0x0010
    COLOR_MASK_G                     = 0x0020
    COLOR_MASK_B                     = 0x0040
    COLOR_MASK_A                     = 0x0080
    STENCIL_TEST_ENABLE              = 0x0100
    POLYGON_OFFSET_ENABLE            = 0x0200
    POLYGON_OFFSET_POINT_LINE_ENABLE = 0x0400
    TRANSLUCENT                      = 0x0800


class CompareFunc(IntEnum):
    COMPARE_FUNC_NEVER    = 0
    COMPARE_FUNC_LESS     = 1
    COMPARE_FUNC_EQUAL    = 2
    COMPARE_FUNC_LEQUAL   = 3
    COMPARE_FUNC_GREATER  = 4
    COMPARE_FUNC_NOTEQUAL = 5
    COMPARE_FUNC_GEQUAL   = 6
    COMPARE_FUNC_ALWAYS   = 7


class CullingMode(IntEnum):
    CULLING_MODE_FRONT = 0
    CULLING_MODE_BACK  = 1
    CULLING_MODE_NONE  = 2
    CULLING_MODE_ALL   = 3


class BlendFactor(IntEnum):
    BLEND_MODE_ZERO                     = 0
    BLEND_MODE_ONE                      = 1
    BLEND_MODE_SRC_COLOR                = 2
    BLEND_MODE_ONE_MINUS_SRC_COLOR      = 3
    BLEND_MODE_SRC_ALPHA                = 4
    BLEND_MODE_ONE_MINUS_SRC_ALPHA      = 5
    BLEND_MODE_DST_ALPHA                = 6
    BLEND_MODE_ONE_MINUS_DST_ALPHA      = 7
    BLEND_MODE_DST_COLOR                = 8
    BLEND_MODE_ONE_MINUS_DST_COLOR      = 9
    BLEND_MODE_SRC_ALPHA_SATURATE       = 10
    BLEND_MODE_CONSTANT_COLOR           = 11
    BLEND_MODE_ONE_MINUS_CONSTANT_COLOR = 12
    BLEND_MODE_CONSTANT_ALPHA           = 13
    BLEND_MODE_ONE_MINUS_CONSTANT_ALPHA = 14


class BlendEquation(IntEnum):
    BLEND_FUNC_ADD         = 0
    BLEND_FUNC_SUB         = 1
    BLEND_FUNC_MIN         = 2
    BLEND_FUNC_MAX         = 3
    BLEND_FUNC_REVERSE_SUB = 4


class StencilOp(IntEnum):
    STENCIL_KEEP      = 0
    STENCIL_ZERO      = 1
    STENCIL_REPLACE   = 2
    STENCIL_INCR      = 3
    STENCIL_DECR      = 4
    STENCIL_INVERT    = 5
    STENCIL_INCR_WRAP = 6
    STENCIL_DECR_WRAP = 7


class PolygonMode(IntEnum):
    POLYGON_MODE_POINT = 0
    POLYGON_MODE_LINE  = 1
    POLYGON_MODE_FILL  = 2


class Material:
    def __init__(self):
        self.name = ""
        self.shaderName = ""

        self.isVisible = True

        self.textures = []
        # self.uniformVars = []
        # self.uniformBlocks = []

        self.isTranslucent = False

        self.depthTestEnable = True
        self.depthWriteEnable = True
        self.depthFunc = CompareFunc.COMPARE_FUNC_LEQUAL

        self.cullingMode = CullingMode.CULLING_MODE_BACK

        self.blendEnable = True
        self.blendFactorSrcRGB = BlendFactor.BLEND_MODE_SRC_ALPHA
        self.blendFactorSrcA = BlendFactor.BLEND_MODE_SRC_ALPHA
        self.blendFactorDstRGB = BlendFactor.BLEND_MODE_ONE_MINUS_SRC_ALPHA
        self.blendFactorDstA = BlendFactor.BLEND_MODE_ONE_MINUS_SRC_ALPHA
        self.blendEquationRGB = BlendEquation.BLEND_FUNC_ADD
        self.blendEquationA = BlendEquation.BLEND_FUNC_ADD
        self.blendConstantColor = [255, 255, 255, 255]

        self.alphaTestEnable = False
        self.alphaTestFunc = CompareFunc.COMPARE_FUNC_GREATER
        self.alphaTestRef = 0.0

        self.colorMaskR = True
        self.colorMaskG = True
        self.colorMaskB = True
        self.colorMaskA = True

        self.stencilTestEnable = False
        self.stencilTestFunc = CompareFunc.COMPARE_FUNC_NEVER
        self.stencilTestRef = 0
        self.stencilTestMask = 0xFFFFFFFF
        self.stencilOpFail = StencilOp.STENCIL_KEEP
        self.stencilOpZFail = StencilOp.STENCIL_KEEP
        self.stencilOpZPass = StencilOp.STENCIL_KEEP

        self.polygonMode = PolygonMode.POLYGON_MODE_FILL
        self.polygonOffsetEnable = False
        self.polygonOffsetPointLineEnable = False


class Model:
    def __init__(self):
        self.meshes = []
        self.materials = []
