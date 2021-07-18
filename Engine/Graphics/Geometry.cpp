#include <vector>

#include <Graphics/Geometry.hpp>
#include <Graphics/Mesh.hpp>

using namespace std;

shared_ptr<Mesh> Geometry::GenerateSquareMesh() {
    static vector<float> vert{
        -1.0f, 1.0f, 0.0f,		0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,		0.0f, 0.0f,
        1.0f, -1.0f, 0.0f,		1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,		1.0f, 1.0f
    };
    static vector<unsigned> attrib{
        3, 0, 2
    };
    static vector<unsigned> idx{
        0, 1, 2, 
        0, 2, 3
    };
    return make_shared<Mesh>(vert, attrib, idx);
}

shared_ptr<Mesh> Geometry::GenerateCubeMesh() {
    /*
          4---7
         /|  /|
        0---3 |
        | 5-|-6
        |/  |/
        1---2
    */
    static vector<float> vert{
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
    };
    static vector<unsigned> attrib{
        3
    };
    static vector<unsigned> idx{
        4, 5, 1,
        1, 0, 4,
        3, 2, 6,
        6, 7, 3,
        1, 2, 6,
        6, 5, 1,
        4, 0, 3,
        3, 7, 4,
        0, 1, 2,
        2, 3, 0,
        7, 6, 5,
        5, 4, 7
    };
    return make_shared<Mesh>(vert, attrib, idx);
}

shared_ptr<Mesh> Geometry::GetSquareMesh() {
    static shared_ptr<Mesh> squareMesh = GenerateSquareMesh();
    return squareMesh;
}

shared_ptr<Mesh> Geometry::GetCubeMesh() {
    static shared_ptr<Mesh> cubeMesh = GenerateCubeMesh();
    return cubeMesh;
}

