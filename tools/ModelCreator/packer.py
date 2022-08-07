from structs import CURRENT_VERSION
from structs import RenderFlags

from struct import pack as f_pack
from struct import pack_into as f_pack_into


def align(x, y):
    return ((x - 1) | (y - 1)) + 1


def pack(model, endianness):
    meshesCount = len(model.meshes)
    materialsCount = len(model.materials)

    meshesPos = 0x20

    vtxBufsPos = meshesPos
    vtxBufsPos += 0x38 * meshesCount
    vtxBufsPos = align(vtxBufsPos, 0x40)

    idxBufsPos = vtxBufsPos
    for mesh in model.meshes:
        idxBufsPos = align(idxBufsPos, 0x40)
        idxBufsPos += 0x20 * len(mesh.vertices)
    idxBufsPos = align(idxBufsPos, 0x20)

    materialsPos = idxBufsPos
    for mesh in model.meshes:
        materialsPos = align(materialsPos, 0x20)
        materialsPos += 4 * len(mesh.indices)

    texturesPos = materialsPos
    texturesPos += 0x80 * materialsCount

    stringsPos = texturesPos
    for material in model.materials:
        stringsPos += 0x48 * len(material.textures)

    fileSize = stringsPos
    for material in model.materials:
        fileSize += len(material.name) + 1 + len(material.shaderName) + 1
        fileSize += sum(len(texture.name) + 1 + len(texture.samplerName) + 1 for texture in material.textures)

    data = bytearray(b'riomodel')
    data += f_pack(endianness + "I", CURRENT_VERSION)
    data += f_pack(endianness + "I", fileSize)

    assert len(data) == 0x10
    data += f_pack(endianness + "I", meshesPos - 0x10)
    data += f_pack(endianness + "I", meshesCount)

    assert len(data) == 0x18
    data += f_pack(endianness + "I", materialsPos - 0x18)
    data += f_pack(endianness + "I", materialsCount)

    assert len(data) == meshesPos

    curPos = meshesPos
    curVtxBufPos = vtxBufsPos
    curIdxBufPos = idxBufsPos

    for mesh in model.meshes:
        vtxCount = len(mesh.vertices)
        idxCount = len(mesh.indices)

        curVtxBufPos = align(curVtxBufPos, 0x40)
        data += f_pack(endianness + "I", curVtxBufPos - curPos); curPos += 4
        data += f_pack(endianness + "I", vtxCount); curPos += 4
        curVtxBufPos += 0x20 * vtxCount

        curIdxBufPos = align(curIdxBufPos, 0x20)
        data += f_pack(endianness + "I", curIdxBufPos - curPos); curPos += 4
        data += f_pack(endianness + "I", idxCount); curPos += 4
        curIdxBufPos += 4 * idxCount

        data += f_pack(endianness + "3f", *mesh.scale);     curPos += 4 * 3
        data += f_pack(endianness + "3f", *mesh.rotate);    curPos += 4 * 3
        data += f_pack(endianness + "3f", *mesh.translate); curPos += 4 * 3

        data += f_pack(endianness + "I", mesh.materialIdx); curPos += 4

    padSize = align(curPos, 0x40) - curPos
    data += b'\0' * padSize; curPos += padSize
    assert curPos == vtxBufsPos

    for mesh in model.meshes:
        padSize = align(curPos, 0x40) - curPos
        data += b'\0' * padSize; curPos += padSize

        data += b''.join(
            f_pack(
                endianness + "3f2f3f",
                *vertex.pos,
                *vertex.texCoord,
                *vertex.normal
            ) for vertex in mesh.vertices
        )
        curPos += 0x20 * len(mesh.vertices)

    assert curPos == curVtxBufPos

    padSize = align(curPos, 0x20) - curPos
    data += b'\0' * padSize; curPos += padSize
    assert curPos == idxBufsPos

    for mesh in model.meshes:
        padSize = align(curPos, 0x20) - curPos
        data += b'\0' * padSize; curPos += padSize

        data += f_pack(endianness + "%dI" % len(mesh.indices), *mesh.indices)
        curPos += 4 * len(mesh.indices)

    assert curPos == curIdxBufPos
    assert curPos == materialsPos
    assert curPos == len(data)

    curPos = materialsPos
    curTexturePos = texturesPos
    curStringPos = stringsPos

    for material in model.materials:
        materialNameLen = len(material.name) + 1
        shaderNameLen = len(material.shaderName) + 1
        textureCount = len(material.textures)

        data += f_pack(endianness + "I", curStringPos - curPos); curPos += 4
        data += f_pack(endianness + "I", materialNameLen); curPos += 4
        curStringPos += materialNameLen

        data += f_pack(endianness + "I", curStringPos - curPos); curPos += 4
        data += f_pack(endianness + "I", shaderNameLen); curPos += 4
        curStringPos += shaderNameLen

        curStringPos += sum(len(texture.name) + 1 + len(texture.samplerName) + 1 for texture in material.textures)

        data += f_pack(endianness + "I", curTexturePos - curPos); curPos += 4
        data += f_pack(endianness + "I", textureCount); curPos += 4
        curTexturePos += 0x48 * textureCount

        data += b'\0\0\0\0' * 4; curPos += 16

        flags = 0

        if material.isVisible:
            flags |= 1

        data += f_pack(endianness + "H", flags); curPos += 2

        renderFlags = 0

        if material.isTranslucent:
            renderFlags |= RenderFlags.TRANSLUCENT

        if material.depthTestEnable:
            renderFlags |= RenderFlags.DEPTH_TEST_ENABLE

        if material.depthWriteEnable:
            renderFlags |= RenderFlags.DEPTH_WRITE_ENABLE

        if material.blendEnable:
            renderFlags |= RenderFlags.BLEND_ENABLE

        if material.alphaTestEnable:
            renderFlags |= RenderFlags.ALPHA_TEST_ENABLE

        if material.colorMaskR:
            renderFlags |= RenderFlags.COLOR_MASK_R

        if material.colorMaskG:
            renderFlags |= RenderFlags.COLOR_MASK_G

        if material.colorMaskB:
            renderFlags |= RenderFlags.COLOR_MASK_B

        if material.colorMaskA:
            renderFlags |= RenderFlags.COLOR_MASK_A

        if material.stencilTestEnable:
            renderFlags |= RenderFlags.STENCIL_TEST_ENABLE

        if material.polygonOffsetEnable:
            renderFlags |= RenderFlags.POLYGON_OFFSET_ENABLE

        if material.polygonOffsetPointLineEnable:
            renderFlags |= RenderFlags.POLYGON_OFFSET_POINT_LINE_ENABLE

        data += f_pack(endianness + "H", renderFlags); curPos += 2

        data += f_pack(endianness + "I", material.depthFunc);         curPos += 4
        data += f_pack(endianness + "I", material.cullingMode);       curPos += 4
        data += f_pack(endianness + "I", material.blendFactorSrcRGB); curPos += 4
        data += f_pack(endianness + "I", material.blendFactorSrcA);   curPos += 4
        data += f_pack(endianness + "I", material.blendFactorDstRGB); curPos += 4
        data += f_pack(endianness + "I", material.blendFactorDstA);   curPos += 4
        data += f_pack(endianness + "I", material.blendEquationRGB);  curPos += 4
        data += f_pack(endianness + "I", material.blendEquationA);    curPos += 4

        data += f_pack(endianness + "4f", *map(lambda x: x / 255, material.blendConstantColor)); curPos += 4 * 4

        data += f_pack(endianness + "I", material.alphaTestFunc);   curPos += 4
        data += f_pack(endianness + "f", material.alphaTestRef);    curPos += 4
        data += f_pack(endianness + "I", material.stencilTestFunc); curPos += 4
        data += f_pack(endianness + "i", material.stencilTestRef);  curPos += 4
        data += f_pack(endianness + "I", material.stencilTestMask); curPos += 4
        data += f_pack(endianness + "I", material.stencilOpFail);   curPos += 4
        data += f_pack(endianness + "I", material.stencilOpZFail);  curPos += 4
        data += f_pack(endianness + "I", material.stencilOpZPass);  curPos += 4
        data += f_pack(endianness + "I", material.polygonMode);     curPos += 4

    assert curPos == texturesPos

    curStringPos = stringsPos
    for material in model.materials:
        curStringPos += len(material.name) + 1 + len(material.shaderName) + 1

        for texture in material.textures:
            textureNameLen = len(texture.name) + 1
            samplerNameLen = len(texture.samplerName) + 1

            data += f_pack(endianness + "I", curStringPos - curPos); curPos += 4
            data += f_pack(endianness + "I", textureNameLen); curPos += 4
            curStringPos += textureNameLen

            data += f_pack(endianness + "I", curStringPos - curPos); curPos += 4
            data += f_pack(endianness + "I", samplerNameLen); curPos += 4
            curStringPos += samplerNameLen

            data += f_pack(endianness + "I", texture.magFilter); curPos += 4
            data += f_pack(endianness + "I", texture.minFilter); curPos += 4
            data += f_pack(endianness + "I", texture.mipFilter); curPos += 4
            data += f_pack(endianness + "I", texture.maxAniso); curPos += 4

            data += f_pack(endianness + "I", texture.wrapX); curPos += 4
            data += f_pack(endianness + "I", texture.wrapY); curPos += 4
            data += f_pack(endianness + "I", texture.wrapZ); curPos += 4

            data += f_pack(endianness + "4f", *map(lambda x: x / 255, texture.borderColor)); curPos += 4 * 4

            data += f_pack(endianness + "f", texture.minLOD); curPos += 4
            data += f_pack(endianness + "f", texture.maxLOD); curPos += 4
            data += f_pack(endianness + "f", texture.LODBias); curPos += 4

    assert curPos == curTexturePos
    assert curPos == stringsPos

    for material in model.materials:
        data += material.name.encode('utf-8') + b'\0' + material.shaderName.encode('utf-8') + b'\0'
        curPos += len(material.name) + 1 + len(material.shaderName) + 1

        data += b''.join(texture.name.encode('utf-8') + b'\0' + texture.samplerName.encode('utf-8') + b'\0' for texture in material.textures)
        curPos += sum(len(texture.name) + 1 + len(texture.samplerName) + 1 for texture in material.textures)

    assert curPos == curStringPos
    assert curPos == fileSize
    assert curPos == len(data)

    return data
