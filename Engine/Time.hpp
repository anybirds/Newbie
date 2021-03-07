#pragma once

#include <EngineExport.hpp>

namespace Engine {
    /*
    static class that manages time by utilizing glfw functions.
	*/
    class ENGINE_EXPORT Time final {
	public:
        static Time &GetInstance() { static Time time; return time; }
        static void Init();
		static void Tick();
        static float DeltaTime() { return static_cast<float>(GetInstance().dt); }

	private:
        Time() {}
        
        double dt;
        double prev;

	public:
        Time(const Time &) = delete;
        void operator=(const Time &) = delete;
	};
}