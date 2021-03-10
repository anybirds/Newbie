#include <iostream>

#include <Graphics/Mesh.hpp>
#include <Graphics/Model.hpp>

using namespace std;
using namespace Engine;

shared_ptr<Resource> AMesh::GetResource() {
    shared_ptr<Resource> sp;
    if (!(sp = resource.lock())) {
        try {
            sp = make_shared<Mesh>(this);
        } catch(...) { }
        resource = sp;
    }
    return sp;
}

Mesh::Mesh(AMesh *amesh) : Resource(amesh) {
    Apply();
}

Mesh::~Mesh() {
    if (icnt > 0) {
        // indexed wireframe
        glDeleteBuffers(1, &ebo);
    }
    // delete
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void Mesh::Apply() {
    shared_ptr<Model> model = GetModel();
    if (!model) {
        cerr << '[' << __FUNCTION__ << ']' << " missing Model in Mesh:" << GetName() << '\n';
        throw exception();
    }

    if (icnt > 0) {
        // indexed wireframe
        glDeleteBuffers(1, &ebo);
    }
    // delete
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    const aiScene *scene = GetModel()->importer->GetScene();
    aiMesh *aimesh = scene->mMeshes[GetIndex()];
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

    cerr << '[' << __FUNCTION__ << ']' << " created Mesh: " << GetName() << '\n';
}

shared_ptr<Model> Mesh::GetModel() const { 
    AMesh *amesh = (AMesh *)asset; 
    return dynamic_pointer_cast<Model>(amesh->GetModel()->GetResource()); 
}