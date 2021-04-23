#pragma once

#include <EngineExport.hpp>

namespace Engine {
    class ENGINE_EXPORT Input final {
	public:
        static Input &GetInstance() { static Input input; return input; }

	private:
        Input() {}
        
        double mouseCursorX;
        double mouseCursorY;
        double deltaMouseCursorX;
        double deltaMouseCursorY;

	public:
        enum { 
            MOUSE_CURSOR_NORMAL,
            MOUSE_CURSOR_HIDDEN, 
            MOUSE_CURSOR_DISABLED
        };

        Input(const Input &) = delete;
        Input &operator=(const Input &) = delete;
        
        void Init();
        void Update();

        float GetMouseCursorX() { return static_cast<float>(mouseCursorX); }
        float GetMouseCursorY() { return static_cast<float>(mouseCursorY); }
        float GetDeltaMouseCursorX() { return static_cast<float>(deltaMouseCursorX); }
        float GetDeltaMouseCursorY() { return static_cast<float>(deltaMouseCursorY); }
    
        void SetMouseCursorMode(int mode);
	};
}