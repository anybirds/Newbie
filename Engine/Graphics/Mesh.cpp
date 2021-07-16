#include <iostream>

#include <Graphics/Mesh.hpp>
#include <Graphics/Model.hpp>

using namespace std;

AMesh::AMesh() : amodel(nullptr), index(0U) {}

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

bool Mesh::GenerateBuffers(const vector<float> &vert, const vector<unsigned> &attrib, const vector<unsigned> &idx) {
    // generate
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // bind
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // data
    unsigned stride = 0;
    unsigned acnt = (unsigned)attrib.size();
    for (unsigned i = 0; i < acnt; i++) {
        stride += attrib[i];
    }
    stride *= sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, vcnt * stride, vert.data(), GL_STATIC_DRAW);

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
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, icnt * sizeof(unsigned), idx.data(), GL_STATIC_DRAW);
    }

    glBindVertexArray(0);

    if (glGetError() != GL_NO_ERROR) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot create Mesh: " << GetName() << '\n';
        glDeleteBuffers(1, &ebo);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        return false;
    }

    return true;
}

Mesh::Mesh(AMesh *amesh) : Resource(amesh), vcnt(0), icnt(0), vao(0), vbo(0), ebo(0) {
    Apply();
}

Mesh::Mesh(const vector<float> &vert, const vector<unsigned> &attrib, const vector<unsigned> &idx) : Resource(), index(0U), model(nullptr) {
    unsigned asize = 0; // number of floats for each vertex
    for (size_t i = 0; i < attrib.size(); i++) {
        asize += attrib[i];
    }
    vcnt = (unsigned)vert.size() / asize; 
    icnt = (unsigned)idx.size();
    
    if (!GenerateBuffers(vert, attrib, idx)) {
        throw exception();
    }
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
    Mesh backup(*this);

    Resource::Apply();
    AMesh *amesh = (AMesh *)asset;
    index = amesh->GetIndex();
    if (amesh->GetModel()) {
        model = dynamic_pointer_cast<Model>(amesh->GetModel()->GetResource());    
    }
    if (!model) {
        cerr << '[' << __FUNCTION__ << ']' << " missing Model in Mesh: " << GetName() << '\n';
        *this = backup;
        throw exception();
    }

    const aiScene *scene = GetModel()->importer->GetScene();
    aiMesh *aimesh = scene->mMeshes[GetIndex()];
    unsigned base;

    vector<unsigned> attrib(3);
    if (aimesh->HasPositions()) {
        attrib[POSITION] = 3;
    }
    if (aimesh->HasNormals()) {
        attrib[NORMAL] = 3;
    }
    if (aimesh->HasTextureCoords(0)) {
        attrib[UV] = 2;
    }
    unsigned asize = 0; // number of floats for each vertex
    for (size_t i = 0; i < attrib.size(); i++) {
        asize += attrib[i];
    } 

    vcnt = aimesh->mNumVertices;
    vector<float> vert(vcnt * asize);
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
    vector<unsigned> idx(icnt);
    base = 0;
    for (unsigned i = 0; i < aimesh->mNumFaces; i++) {
        idx[base] = aimesh->mFaces[i].mIndices[0];
        idx[base + 1] = aimesh->mFaces[i].mIndices[1];
        idx[base + 2] = aimesh->mFaces[i].mIndices[2];
        base += 3;
    }

    if (!GenerateBuffers(vert, attrib, idx)) {
        *this = backup;
        throw exception();
    }

    glDeleteBuffers(1, &backup.ebo);
    glDeleteVertexArrays(1, &backup.vao);
    glDeleteBuffers(1, &backup.vbo);
    cerr << '[' << __FUNCTION__ << ']' << " created Mesh: " << GetName() << '\n';
}