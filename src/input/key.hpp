#pragma once

#include <GLFW/glfw3.h>

enum class key
{
    NONE,

    // Punctuation / symbols
    SPACE,
    APOSTROPHE,
    COMMA,
    MINUS,
    PERIOD,
    SLASH,
    SEMICOLON,
    EQUAL,
    LEFT_BRACKET,
    BACKSLASH,
    RIGHT_BRACKET,
    GRAVE_ACCENT,
    WORLD_1,
    WORLD_2,

    // Numbers
    D0,
    D1,
    D2,
    D3,
    D4,
    D5,
    D6,
    D7,
    D8,
    D9,

    // Letters
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,

    // Navigation / editing
    ESCAPE,
    ENTER,
    TAB,
    BACKSPACE,
    INSERT,
    DEL,
    RIGHT,
    LEFT,
    DOWN,
    UP,
    PAGE_UP,
    PAGE_DOWN,
    HOME,
    END,

    // Locks
    CAPS_LOCK,
    SCROLL_LOCK,
    NUM_LOCK,
    PRINT_SCREEN,
    PAUSE,

    // Function keys
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    F13,
    F14,
    F15,
    F16,
    F17,
    F18,
    F19,
    F20,
    F21,
    F22,
    F23,
    F24,
    F25,

    // Keypad
    KP_0,
    KP_1,
    KP_2,
    KP_3,
    KP_4,
    KP_5,
    KP_6,
    KP_7,
    KP_8,
    KP_9,
    KP_DECIMAL,
    KP_DIVIDE,
    KP_MULTIPLY,
    KP_SUBTRACT,
    KP_ADD,
    KP_ENTER,
    KP_EQUAL,

    // Modifiers
    LEFT_SHIFT,
    LEFT_CONTROL,
    LEFT_ALT,
    LEFT_SUPER,
    RIGHT_SHIFT,
    RIGHT_CONTROL,
    RIGHT_ALT,
    RIGHT_SUPER,
    MENU,

    // Mouse buttons
    MOUSE_LEFT,
    MOUSE_RIGHT,
    MOUSE_MIDDLE,
    MOUSE_X1,
    MOUSE_X2,

    COUNT,
};

constexpr int glfw_key(key k)
{
    switch (k) {
        case key::SPACE: return GLFW_KEY_SPACE;
        case key::APOSTROPHE: return GLFW_KEY_APOSTROPHE;
        case key::COMMA: return GLFW_KEY_COMMA;
        case key::MINUS: return GLFW_KEY_MINUS;
        case key::PERIOD: return GLFW_KEY_PERIOD;
        case key::SLASH: return GLFW_KEY_SLASH;
        case key::SEMICOLON: return GLFW_KEY_SEMICOLON;
        case key::EQUAL: return GLFW_KEY_EQUAL;
        case key::LEFT_BRACKET: return GLFW_KEY_LEFT_BRACKET;
        case key::BACKSLASH: return GLFW_KEY_BACKSLASH;
        case key::RIGHT_BRACKET: return GLFW_KEY_RIGHT_BRACKET;
        case key::GRAVE_ACCENT: return GLFW_KEY_GRAVE_ACCENT;
        case key::WORLD_1: return GLFW_KEY_WORLD_1;
        case key::WORLD_2: return GLFW_KEY_WORLD_2;

        case key::D0: return GLFW_KEY_0;
        case key::D1: return GLFW_KEY_1;
        case key::D2: return GLFW_KEY_2;
        case key::D3: return GLFW_KEY_3;
        case key::D4: return GLFW_KEY_4;
        case key::D5: return GLFW_KEY_5;
        case key::D6: return GLFW_KEY_6;
        case key::D7: return GLFW_KEY_7;
        case key::D8: return GLFW_KEY_8;
        case key::D9: return GLFW_KEY_9;

        case key::A: return GLFW_KEY_A;
        case key::B: return GLFW_KEY_B;
        case key::C: return GLFW_KEY_C;
        case key::D: return GLFW_KEY_D;
        case key::E: return GLFW_KEY_E;
        case key::F: return GLFW_KEY_F;
        case key::G: return GLFW_KEY_G;
        case key::H: return GLFW_KEY_H;
        case key::I: return GLFW_KEY_I;
        case key::J: return GLFW_KEY_J;
        case key::K: return GLFW_KEY_K;
        case key::L: return GLFW_KEY_L;
        case key::M: return GLFW_KEY_M;
        case key::N: return GLFW_KEY_N;
        case key::O: return GLFW_KEY_O;
        case key::P: return GLFW_KEY_P;
        case key::Q: return GLFW_KEY_Q;
        case key::R: return GLFW_KEY_R;
        case key::S: return GLFW_KEY_S;
        case key::T: return GLFW_KEY_T;
        case key::U: return GLFW_KEY_U;
        case key::V: return GLFW_KEY_V;
        case key::W: return GLFW_KEY_W;
        case key::X: return GLFW_KEY_X;
        case key::Y: return GLFW_KEY_Y;
        case key::Z: return GLFW_KEY_Z;

        case key::ESCAPE: return GLFW_KEY_ESCAPE;
        case key::ENTER: return GLFW_KEY_ENTER;
        case key::TAB: return GLFW_KEY_TAB;
        case key::BACKSPACE: return GLFW_KEY_BACKSPACE;
        case key::INSERT: return GLFW_KEY_INSERT;
        case key::DEL: return GLFW_KEY_DELETE;
        case key::RIGHT: return GLFW_KEY_RIGHT;
        case key::LEFT: return GLFW_KEY_LEFT;
        case key::DOWN: return GLFW_KEY_DOWN;
        case key::UP: return GLFW_KEY_UP;
        case key::PAGE_UP: return GLFW_KEY_PAGE_UP;
        case key::PAGE_DOWN: return GLFW_KEY_PAGE_DOWN;
        case key::HOME: return GLFW_KEY_HOME;
        case key::END: return GLFW_KEY_END;

        case key::CAPS_LOCK: return GLFW_KEY_CAPS_LOCK;
        case key::SCROLL_LOCK: return GLFW_KEY_SCROLL_LOCK;
        case key::NUM_LOCK: return GLFW_KEY_NUM_LOCK;
        case key::PRINT_SCREEN: return GLFW_KEY_PRINT_SCREEN;
        case key::PAUSE: return GLFW_KEY_PAUSE;

        case key::F1: return GLFW_KEY_F1;
        case key::F2: return GLFW_KEY_F2;
        case key::F3: return GLFW_KEY_F3;
        case key::F4: return GLFW_KEY_F4;
        case key::F5: return GLFW_KEY_F5;
        case key::F6: return GLFW_KEY_F6;
        case key::F7: return GLFW_KEY_F7;
        case key::F8: return GLFW_KEY_F8;
        case key::F9: return GLFW_KEY_F9;
        case key::F10: return GLFW_KEY_F10;
        case key::F11: return GLFW_KEY_F11;
        case key::F12: return GLFW_KEY_F12;
        case key::F13: return GLFW_KEY_F13;
        case key::F14: return GLFW_KEY_F14;
        case key::F15: return GLFW_KEY_F15;
        case key::F16: return GLFW_KEY_F16;
        case key::F17: return GLFW_KEY_F17;
        case key::F18: return GLFW_KEY_F18;
        case key::F19: return GLFW_KEY_F19;
        case key::F20: return GLFW_KEY_F20;
        case key::F21: return GLFW_KEY_F21;
        case key::F22: return GLFW_KEY_F22;
        case key::F23: return GLFW_KEY_F23;
        case key::F24: return GLFW_KEY_F24;
        case key::F25: return GLFW_KEY_F25;

        case key::KP_0: return GLFW_KEY_KP_0;
        case key::KP_1: return GLFW_KEY_KP_1;
        case key::KP_2: return GLFW_KEY_KP_2;
        case key::KP_3: return GLFW_KEY_KP_3;
        case key::KP_4: return GLFW_KEY_KP_4;
        case key::KP_5: return GLFW_KEY_KP_5;
        case key::KP_6: return GLFW_KEY_KP_6;
        case key::KP_7: return GLFW_KEY_KP_7;
        case key::KP_8: return GLFW_KEY_KP_8;
        case key::KP_9: return GLFW_KEY_KP_9;
        case key::KP_DECIMAL: return GLFW_KEY_KP_DECIMAL;
        case key::KP_DIVIDE: return GLFW_KEY_KP_DIVIDE;
        case key::KP_MULTIPLY: return GLFW_KEY_KP_MULTIPLY;
        case key::KP_SUBTRACT: return GLFW_KEY_KP_SUBTRACT;
        case key::KP_ADD: return GLFW_KEY_KP_ADD;
        case key::KP_ENTER: return GLFW_KEY_KP_ENTER;
        case key::KP_EQUAL: return GLFW_KEY_KP_EQUAL;

        case key::LEFT_SHIFT: return GLFW_KEY_LEFT_SHIFT;
        case key::LEFT_CONTROL: return GLFW_KEY_LEFT_CONTROL;
        case key::LEFT_ALT: return GLFW_KEY_LEFT_ALT;
        case key::LEFT_SUPER: return GLFW_KEY_LEFT_SUPER;
        case key::RIGHT_SHIFT: return GLFW_KEY_RIGHT_SHIFT;
        case key::RIGHT_CONTROL: return GLFW_KEY_RIGHT_CONTROL;
        case key::RIGHT_ALT: return GLFW_KEY_RIGHT_ALT;
        case key::RIGHT_SUPER: return GLFW_KEY_RIGHT_SUPER;
        case key::MENU: return GLFW_KEY_MENU;

        // Mouse buttons are handled through glfwGetMouseButton; this helper returns
        // GLFW_KEY_UNKNOWN so they are treated as released if used with glfwGetKey.
        case key::MOUSE_LEFT: return GLFW_KEY_UNKNOWN;
        case key::MOUSE_RIGHT: return GLFW_KEY_UNKNOWN;
        case key::MOUSE_MIDDLE: return GLFW_KEY_UNKNOWN;
        case key::MOUSE_X1: return GLFW_KEY_UNKNOWN;
        case key::MOUSE_X2: return GLFW_KEY_UNKNOWN;

        default: return GLFW_KEY_UNKNOWN;
    }
}

constexpr bool is_mouse_button(key k)
{
    switch (k) {
        case key::MOUSE_LEFT:
        case key::MOUSE_RIGHT:
        case key::MOUSE_MIDDLE:
        case key::MOUSE_X1:
        case key::MOUSE_X2:
            return true;
        default:
            return false;
    }
}

constexpr int glfw_mouse_button(key k)
{
    switch (k) {
        case key::MOUSE_LEFT: return GLFW_MOUSE_BUTTON_LEFT;
        case key::MOUSE_RIGHT: return GLFW_MOUSE_BUTTON_RIGHT;
        case key::MOUSE_MIDDLE: return GLFW_MOUSE_BUTTON_MIDDLE;
        case key::MOUSE_X1: return GLFW_MOUSE_BUTTON_4;
        case key::MOUSE_X2: return GLFW_MOUSE_BUTTON_5;
        default: return -1;
    }
}
