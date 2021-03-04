#include <iostream>

#include <Graphics/Mesh.hpp>
#include <Graphics/Model.hpp>

using namespace std;
using namespace Engine;

namespace Engine {
    TYPE_DEF(Mesh)
    SER_DEF(Mesh, Resource,
    MEMBER_SER | MEMBER_SHOW, Model *, model,
    MEMBER_SER | MEMBER_SHOW, int, index
    )
}

Mesh::Mesh(const string &name, Type *type) : Resource(name, type) {

}

void Mesh::OnInit() {
    if (!model->loaded) {
        model->OnInit();
    }

    const aiScene *scene = model->importer->GetScene();
    aiMesh *aimesh = scene->mMeshes[index];
    unsigned base;

    unsigned acnt = 3;
    unsigned *attrib = new unsigned[acnt];
    if (aimesh->HasPositions()) {
        attrib[0] = 3;
    }
    if (aimesh->HasNormals()) {
        attrib[1] = 3;
    }
    if (aimesh->HasTextureCoords(0)) {
        attrib[2] = 2;
    }
    unsigned asize = 0; // number of floats for each vertex
    for (unsigned i = 0; i < acnt; i++) {
        asize += attrib[i];
    }

    vcnt = aimesh->mNumVertices;
    float *vert = new float[vcnt * asize];
    base = 0;
    for (unsigned i = 0; i < aimesh->mNumVertices; i++) {
        if (aimesh->HasPositions()) {
            vert[base] = aimesh->mVertices[i].x;
            vert[base + 1] = aimesh->mVertices[i].y;
            vert[base + 2] = aimesh->mVertices[i].z;
            base += 3;
        }
        if (aimesh->HasNormals()) {
            vert[base] = aimesh->mNormals[i].x;
            vert[base + 1] = aimesh->mNormals[i].y;
            vert[base + 2] = aimesh->mNormals[i].z;
            base += 3;
        }
        if (aimesh->HasTextureCoords(0)) { // texture coordinate set 0
            vert[base] = aimesh->mTextureCoords[0][i].x;
            vert[base + 1] = aimesh->mTextureCoords[0][i].y;
            base += 2;
        }
    }

    icnt = aimesh->mNumFaces * 3;
    unsigned *idx = new unsigned[icnt];
    base = 0;
    for (unsigned i = 0; i < aimesh->mNumFaces; i++) {
        idx[base] = aimesh->mFaces[i].mIndices[0];
        idx[base + 1] = aimesh->mFaces[i].mIndices[1];
        idx[base + 2] = aimesh->mFaces[i].mIndices[2];
        base += 3;
    }

    // generate
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // bind
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // data
    unsigned stride = 0;
    for (unsigned i = 0; i < acnt; i++) {
        stride += attrib[i];
    }
    stride *= sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, vcnt * stride, vert, GL_STATIC_DRAW);

    // attribute
    float *offset = nullptr;
    for (unsigned i = 0; i < acnt; i++) {
        if (attrib[i]) {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, attrib[i], GL_FLOAT, GL_FALSE, stride, (void *)offset);
            offset += attrib[i];
        }
    }

    if (icnt > 0) {
        // indexed wireframe
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, icnt * sizeof(unsigned), idx, GL_STATIC_DRAW);
    }

    // restore
    glBindVertexArray(0);
    delete[] vert;
    delete[] attrib;
    delete[] idx;

#ifdef DEBUG
    cout << '[' << __FUNCTION__ << ']'
        << "{vertex: " << vcnt << " attribute: " << acnt << " index: " << icnt << '}'
        <<" Mesh created." << endl;
#endif

    Resource::OnInit();
}

void Mesh::OnDestroy() {
    if (icnt > 0) {
        // indexed wireframe
        glDeleteBuffers(1, &ebo);
    }
    // delete
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    Resource::OnDestroy();
}
