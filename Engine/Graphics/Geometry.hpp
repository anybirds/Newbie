#pragma once

#include <memory>

#include <EngineExport.hpp>

class Mesh;

class ENGINE_EXPORT Geometry {
private:
    static std::shared_ptr<Mesh> GenerateSquareMesh();
    static std::shared_ptr<Mesh> GenerateCubeMesh();

public:
    static std::shared_ptr<Mesh> GetSquareMesh();
    static std::shared_ptr<Mesh> GetCubeMesh();
};
