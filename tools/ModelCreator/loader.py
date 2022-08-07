from structs import Vertex
from structs import Mesh
from structs import CURRENT_VERSION
from structs import TexXYFilterMode
from structs import TexMipFilterMode
from structs import TexAnisoRatio
from structs import TexWrapMode
from structs import Texture
from structs import RenderFlags
from structs import CompareFunc
from structs import CullingMode
from structs import BlendFactor
from structs import BlendEquation
from structs import StencilOp
from structs import PolygonMode
from structs import Material
from structs import Model


from struct import unpack_from as f_unpack_from


def load(inb, endianness):
    assert inb[:8] == b'riomodel'
    assert f_unpack_from(endianness + "I", inb,  8)[0] == CURRENT_VERSION
    assert f_unpack_from(endianness + "I", inb, 12)[0] == len(inb)

    curPos = 16

    meshesOfs = f_unpack_from(endianness + "I", inb, curPos)[0] + curPos; curPos += 4
    meshesCnt = f_unpack_from(endianness + "I", inb, curPos)[0];          curPos += 4

    materialsOfs = f_unpack_from(endianness + "I", inb, curPos)[0] + curPos; curPos += 4
    materialsCnt = f_unpack_from(endianness + "I", inb, curPos)[0];          curPos += 4

    assert curPos == 0x20

    curPos = meshesOfs
    meshes = []

    for i in range(meshesCnt):
        basePos = curPos

        vtxBufOfs = f_unpack_from(endianness + "I", inb, curPos)[0] + curPos; curPos += 4
        vtxBufCnt = f_unpack_from(endianness + "I", inb, curPos)[0];          curPos += 4

        idxBufOfs = f_unpack_from(endianness + "I", inb, curPos)[0] + curPos; curPos += 4
        idxBufCnt = f_unpack_from(endianness + "I", inb, curPos)[0];          curPos += 4

        scale     = f_unpack_from(endianness + "3f", inb, curPos); curPos += 4 * 3
        rotate    = f_unpack_from(endianness + "3f", inb, curPos); curPos += 4 * 3
        translate = f_unpack_from(endianness + "3f", inb, curPos); curPos += 4 * 3

        matIdx = f_unpack_from(endianness + "I", inb, curPos)[0]; curPos += 4

        assert curPos - basePos == 0x38

        curPos = vtxBufOfs
        vertices = []

        for i in range(vtxBufCnt):
            vtxBufBasePos = curPos

            pos      = f_unpack_from(endianness + "3f", inb, curPos); curPos += 4 * 3
            texCoord = f_unpack_from(endianness + "2f", inb, curPos); curPos += 4 * 2
            normal   = f_unpack_from(endianness + "3f", inb, curPos); curPos += 4 * 3

            assert curPos - vtxBufBasePos == 0x20

            vertices.append(Vertex(pos, texCoord, normal))

        assert curPos == vtxBufOfs + 0x20 * vtxBufCnt

        indices = list(f_unpack_from(endianness + "%dI" % idxBufCnt, inb, idxBufOfs))

        mesh = Mesh()
        meshes.append(mesh)

        mesh.materialIdx = matIdx
        mesh.vertices = vertices
        mesh.indices = indices
        mesh.scale = list(scale)
        mesh.rotate = list(rotate)
        mesh.translate = list(translate)

        curPos = basePos + 0x38

    assert curPos == meshesOfs + 0x38 * meshesCnt

    curPos = materialsOfs
    materials = []

    for i in range(materialsCnt):
        basePos = curPos

        materialNameOfs = f_unpack_from(endianness + "I", inb, curPos)[0] + curPos; curPos += 4
        materialNameCnt = f_unpack_from(endianness + "I", inb, curPos)[0];          curPos += 4

        shaderNameOfs = f_unpack_from(endianness + "I", inb, curPos)[0] + curPos; curPos += 4
        shaderNameCnt = f_unpack_from(endianness + "I", inb, curPos)[0];          curPos += 4

        texturesOfs = f_unpack_from(endianness + "I", inb, curPos)[0] + curPos; curPos += 4
        texturesCnt = f_unpack_from(endianness + "I", inb, curPos)[0];          curPos += 4

        # uniformVarsOfs = 0;
        curPos += 4
        # uniformVarsCnt = 0;
        curPos += 4

        # uniformBlocksOfs = 0;
        curPos += 4
        # uniformBlocksCnt = 0;
        curPos += 4

        flags       = f_unpack_from(endianness + "H", inb, curPos)[0]; curPos += 2
        renderFlags = f_unpack_from(endianness + "H", inb, curPos)[0]; curPos += 2

        depthFunc         = f_unpack_from(endianness + "I", inb, curPos)[0]; curPos += 4
        cullingMode       = f_unpack_from(endianness + "I", inb, curPos)[0]; curPos += 4
        blendFactorSrcRGB = f_unpack_from(endianness + "I", inb, curPos)[0]; curPos += 4
        blendFactorSrcA   = f_unpack_from(endianness + "I", inb, curPos)[0]; curPos += 4
        blendFactorDstRGB = f_unpack_from(endianness + "I", inb, curPos)[0]; curPos += 4
        blendFactorDstA   = f_unpack_from(endianness + "I", inb, curPos)[0]; curPos += 4
        blendEquationRGB  = f_unpack_from(endianness + "I", inb, curPos)[0]; curPos += 4
        blendEquationA    = f_unpack_from(endianness + "I", inb, curPos)[0]; curPos += 4

        blendConstantColor = f_unpack_from(endianness + "4f", inb, curPos); curPos += 4 * 4

        alphaTestFunc     = f_unpack_from(endianness + "I", inb, curPos)[0]; curPos += 4
        alphaTestRef      = f_unpack_from(endianness + "f", inb, curPos)[0]; curPos += 4
        stencilTestFunc   = f_unpack_from(endianness + "I", inb, curPos)[0]; curPos += 4
        stencilTestRef    = f_unpack_from(endianness + "i", inb, curPos)[0]; curPos += 4
        stencilTestMask   = f_unpack_from(endianness + "I", inb, curPos)[0]; curPos += 4
        stencilOpFail     = f_unpack_from(endianness + "I", inb, curPos)[0]; curPos += 4
        stencilOpZFail    = f_unpack_from(endianness + "I", inb, curPos)[0]; curPos += 4
        stencilOpZPass    = f_unpack_from(endianness + "I", inb, curPos)[0]; curPos += 4
        polygonMode       = f_unpack_from(endianness + "I", inb, curPos)[0]; curPos += 4

        assert curPos - basePos == 0x80

        materialName = inb[materialNameOfs:materialNameOfs + materialNameCnt]
        assert materialName[-1] == 0

        shaderName = inb[shaderNameOfs:shaderNameOfs + shaderNameCnt]
        assert shaderName[-1] == 0

        curPos = texturesOfs
        textures = []

        for i in range(texturesCnt):
            texturesBasePos = curPos

            textureNameOfs = f_unpack_from(endianness + "I", inb, curPos)[0] + curPos; curPos += 4
            textureNameCnt = f_unpack_from(endianness + "I", inb, curPos)[0];          curPos += 4

            samplerNameOfs = f_unpack_from(endianness + "I", inb, curPos)[0] + curPos; curPos += 4
            samplerNameCnt = f_unpack_from(endianness + "I", inb, curPos)[0];          curPos += 4

            magFilter = f_unpack_from(endianness + "I", inb, curPos)[0]; curPos += 4
            minFilter = f_unpack_from(endianness + "I", inb, curPos)[0]; curPos += 4
            mipFilter = f_unpack_from(endianness + "I", inb, curPos)[0]; curPos += 4
            maxAniso  = f_unpack_from(endianness + "I", inb, curPos)[0]; curPos += 4

            wrapX = f_unpack_from(endianness + "I", inb, curPos)[0]; curPos += 4
            wrapY = f_unpack_from(endianness + "I", inb, curPos)[0]; curPos += 4
            wrapZ = f_unpack_from(endianness + "I", inb, curPos)[0]; curPos += 4

            borderColor = f_unpack_from(endianness + "4f", inb, curPos); curPos += 4 * 4

            minLOD  = f_unpack_from(endianness + "f", inb, curPos)[0]; curPos += 4
            maxLOD  = f_unpack_from(endianness + "f", inb, curPos)[0]; curPos += 4
            LODBias = f_unpack_from(endianness + "f", inb, curPos)[0]; curPos += 4

            assert curPos - texturesBasePos == 0x48

            textureName = inb[textureNameOfs:textureNameOfs + textureNameCnt]
            assert textureName[-1] == 0

            samplerName = inb[samplerNameOfs:samplerNameOfs + samplerNameCnt]
            assert samplerName[-1] == 0

            texture = Texture()
            textures.append(texture)

            texture.name = textureName[:-1].decode()
            texture.samplerName = samplerName[:-1].decode()

            texture.magFilter = TexXYFilterMode(magFilter)
            texture.minFilter = TexXYFilterMode(minFilter)
            texture.mipFilter = TexMipFilterMode(mipFilter)
            texture.maxAniso = TexAnisoRatio(maxAniso)

            texture.wrapX = TexWrapMode(wrapX)
            texture.wrapY = TexWrapMode(wrapY)
            texture.wrapZ = TexWrapMode(wrapZ)

            texture.borderColor = list(map(lambda x: int(x * 255), borderColor))

            texture.minLOD = minLOD
            texture.maxLOD = maxLOD
            texture.LODBias = LODBias

            curPos = texturesBasePos + 0x48

        assert curPos == texturesOfs + 0x48 * texturesCnt

        material = Material()
        materials.append(material)

        material.name = materialName[:-1].decode()
        material.shaderName = shaderName[:-1].decode()

        material.isVisible = bool(flags & 1)

        material.textures = textures
        # material.uniformVars = uniformVars
        # material.uniformBlocks = uniformBlocks

        material.isTranslucent = bool(renderFlags & RenderFlags.TRANSLUCENT)

        material.depthTestEnable = bool(renderFlags & RenderFlags.DEPTH_TEST_ENABLE)
        material.depthWriteEnable = bool(renderFlags & RenderFlags.DEPTH_WRITE_ENABLE)
        material.depthFunc = CompareFunc(depthFunc)

        material.cullingMode = CullingMode(cullingMode)

        material.blendEnable = bool(renderFlags & RenderFlags.BLEND_ENABLE)
        material.blendFactorSrcRGB = BlendFactor(blendFactorSrcRGB)
        material.blendFactorSrcA = BlendFactor(blendFactorSrcA)
        material.blendFactorDstRGB = BlendFactor(blendFactorDstRGB)
        material.blendFactorDstA = BlendFactor(blendFactorDstA)
        material.blendEquationRGB = BlendEquation(blendEquationRGB)
        material.blendEquationA = BlendEquation(blendEquationA)
        material.blendConstantColor = list(map(lambda x: int(x * 255), blendConstantColor))

        material.alphaTestEnable = bool(renderFlags & RenderFlags.ALPHA_TEST_ENABLE)
        material.alphaTestFunc = CompareFunc(alphaTestFunc)
        material.alphaTestRef = alphaTestRef

        material.colorMaskR = bool(renderFlags & RenderFlags.COLOR_MASK_R)
        material.colorMaskG = bool(renderFlags & RenderFlags.COLOR_MASK_G)
        material.colorMaskB = bool(renderFlags & RenderFlags.COLOR_MASK_B)
        material.colorMaskA = bool(renderFlags & RenderFlags.COLOR_MASK_A)

        material.stencilTestEnable = bool(renderFlags & RenderFlags.STENCIL_TEST_ENABLE)
        material.stencilTestFunc = CompareFunc(stencilTestFunc)
        material.stencilTestRef = stencilTestRef
        material.stencilTestMask = stencilTestMask
        material.stencilOpFail = StencilOp(stencilOpFail)
        material.stencilOpZFail = StencilOp(stencilOpZFail)
        material.stencilOpZPass = StencilOp(stencilOpZPass)

        material.polygonMode = PolygonMode(polygonMode)
        material.polygonOffsetEnable = bool(renderFlags & RenderFlags.POLYGON_OFFSET_ENABLE)
        material.polygonOffsetPointLineEnable = bool(renderFlags & RenderFlags.POLYGON_OFFSET_POINT_LINE_ENABLE)

        curPos = basePos + 0x80

    assert curPos == materialsOfs + 0x80 * materialsCnt

    model = Model()

    model.meshes = meshes
    model.materials = materials

    return model
