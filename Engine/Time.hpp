#pragma once

#include <EngineExport.hpp>

class ENGINE_EXPORT Time final {
public:
    static Time &GetInstance() { static Time time; return time; }

private:
    Time() {}
    
    double dt;
    double prev;

public:
    Time(const Time &) = delete;
    Time &operator=(const Time &) = delete;
    
    void Init();
    void Update();
    double GetTime() const { return prev; }
    float GetDeltaTime() const { return static_cast<float>(dt); }
};