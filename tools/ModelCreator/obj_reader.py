from structs import Vertex


def read_obj(path):
    pos = []
    texCoord = []
    normal = []
    faces = []

    with open(path) as inf:
        for line in inf:
            elem = line.split()[1:]

            if line.startswith("v "):
                assert len(elem) == 3
                pos.append(tuple(map(float, elem)))

            elif line.startswith("vt "):
                assert len(elem) in (2, 3)
                texCoord.append((float(elem[0]), 1.0 - float(elem[1])))

            elif line.startswith("vn "):
                assert len(elem) == 3
                normal.append(tuple(map(float, elem)))

            elif line.startswith("f "):
                assert len(elem) == 3
                faces.append(tuple(tuple(map(lambda x: int(x) - 1, face.split('/'))) for face in elem))

    vertices = []
    indices = []

    for face in faces:
        for posIdx, texCoordIdx, normalIdx in face:
            vertex = Vertex(pos[posIdx], texCoord[texCoordIdx], normal[normalIdx])
            if vertex not in vertices:
                vertices.append(vertex)

            indices.append(vertices.index(vertex))

    return vertices, indices
