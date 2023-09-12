import bpy
from bpy import context
import json

meshes = []
for obj in bpy.data.objects:
    if obj.type != 'MESH':
        continue
    print(obj)
    
    mesh = obj.data
    # Grab the Number of Vertices
    mesh_verts = mesh.vertices[:]
    # Grab the Number of Polygons
    mesh_polygons = mesh.polygons[:]
    # Count the Number of vertices
    vertexCount = 0
    for polygon in mesh_polygons:
        for vertex in polygon.vertices:
            vertexCount = vertexCount + 1

#    print(vertexCount)
#    print(mesh_verts)
#    print(mesh_polygons)
    
    mesh_materials = mesh.materials[:]
    baseColor = [1.0, 1.0, 1.0, 1.0]
    specular = [0.0]
    for material in mesh_materials:
        if material is None:
            continue
        print(material.name)
        if material.use_nodes:
            for node in material.node_tree.nodes:
                print(node.type)
                if node.type != 'BSDF_PRINCIPLED':
                    continue
                print(dir(node))
                for input in node.inputs:
                    print(input.name)
                    if input.name == 'Base Color':
                        baseColor[0] = input.default_value[0]
                        baseColor[1] = input.default_value[1]
                        baseColor[2] = input.default_value[2]
                        baseColor[3] = input.default_value[3]
                    if input.name == 'Specular':
                        specular = input.default_value
    
    data = {}
    data['baseColor'] = baseColor
    data['specular'] = specular

    data['vertices'] = []
    for vert in mesh_verts:
        x = -vert.co[1]
        y = vert.co[0]
        z = vert.co[2]
        data['vertices'].append(x)
        data['vertices'].append(y)
        data['vertices'].append(z)

    data['faceVertices'] = []
    for polygon in mesh_polygons:
        # All polygons must be triangles.
        if len(polygon.vertices) != 3:
            abort()

        for i in range(0, 3):
            v = polygon.vertices[i]
            if v < -1 or v >= len(data['vertices']):
                abort()
            data['faceVertices'].append(v)

    meshes.append(data)

with open('/PSDEV/work/haribote-ng/win/app/newmesh/out.json', 'w') as f:
    json.dump(meshes, f)
    print(meshes)

