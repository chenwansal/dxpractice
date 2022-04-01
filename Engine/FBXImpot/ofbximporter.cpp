#include "ofbximporter.h"

ofbx::IScene *load_fbx(const char *filepath) {

    FILE *fp;
    auto err = fopen_s(&fp, filepath, "rb");

    if (err != 0)
        return NULL;

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    auto *content = new ofbx::u8[file_size];
    fread(content, 1, file_size, fp);
    ofbx::IScene *g_scene = ofbx::load((ofbx::u8 *)content, file_size,
                                       (ofbx::u64)ofbx::LoadFlags::TRIANGULATE);
    delete[] content;
    fclose(fp);

    return g_scene;
}

bool saveAsOBJ(ofbx::IScene &scene, const char *path) {
    FILE *fp;
    auto err = fopen_s(&fp, path, "wb");
    if (err != 0)
        return false;
    int obj_idx = 0;
    int indices_offset = 0;
    int normals_offset = 0;
    int mesh_count = scene.getMeshCount();
    for (int i = 0; i < mesh_count; ++i) {
        fprintf(fp, "o obj%d\ng grp%d\n", i, obj_idx);

        const ofbx::Mesh &mesh = *scene.getMesh(i);
        const ofbx::Geometry &geom = *mesh.getGeometry();
        int vertex_count = geom.getVertexCount();
        const ofbx::Vec3 *vertices = geom.getVertices();
        for (int i = 0; i < vertex_count; ++i) {
            ofbx::Vec3 v = vertices[i];
            fprintf(fp, "v %f %f %f\n", v.x, v.y, v.z);
        }

        bool has_normals = geom.getNormals() != nullptr;
        if (has_normals) {
            const ofbx::Vec3 *normals = geom.getNormals();
            int count = geom.getIndexCount();

            for (int i = 0; i < count; ++i) {
                ofbx::Vec3 n = normals[i];
                fprintf(fp, "vn %f %f %f\n", n.x, n.y, n.z);
            }
        }

        bool has_uvs = geom.getUVs() != nullptr;
        if (has_uvs) {
            const ofbx::Vec2 *uvs = geom.getUVs();
            int count = geom.getIndexCount();

            for (int i = 0; i < count; ++i) {
                ofbx::Vec2 uv = uvs[i];
                fprintf(fp, "vt %f %f\n", uv.x, uv.y);
            }
        }

        const int *faceIndices = geom.getFaceIndices();
        int index_count = geom.getIndexCount();
        bool new_face = true;
        for (int i = 0; i < index_count; ++i) {
            if (new_face) {
                fputs("f ", fp);
                new_face = false;
            }
            int idx =
                (faceIndices[i] < 0) ? -faceIndices[i] : (faceIndices[i] + 1);
            int vertex_idx = indices_offset + idx;
            fprintf(fp, "%d", vertex_idx);

            if (has_uvs) {
                int uv_idx = normals_offset + i + 1;
                fprintf(fp, "/%d", uv_idx);
            } else {
                fprintf(fp, "/");
            }

            if (has_normals) {
                int normal_idx = normals_offset + i + 1;
                fprintf(fp, "/%d", normal_idx);
            } else {
                fprintf(fp, "/");
            }

            new_face = faceIndices[i] < 0;
            fputc(new_face ? '\n' : ' ', fp);
        }

        indices_offset += vertex_count;
        normals_offset += index_count;
        ++obj_idx;
    }
    fclose(fp);
    return true;
}