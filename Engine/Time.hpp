#ifndef TIME_H
#define TIME_H

#include <engine_global.hpp>

namespace Engine {
    /*
    static class that manages time by utilizing glfw functions.
	*/
    class ENGINE_EXPORT Time final {
	public:
        static void Init();
		static void Tick();
        static float DeltaTime() { return static_cast<float>(dt); }

	private:
        static double dt;
        static double prev;

	public:
        Time() = delete;
	};
}

#endif
