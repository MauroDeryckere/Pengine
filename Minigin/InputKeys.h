#ifndef INPUTKEYS
#define INPUTKEYS

namespace Pengin
{
    enum class KeyBoardKey : unsigned
    {
        Backspace,
        Tab,
        Return,
        Shift,
        LeftShift,
        RightShift,
        Ctrl,
        LeftCtrl,
        RightCtrl,
        Alt,
        LeftAlt,
        RightAlt,
        Pause,
        Capslock,
        Escape,
        pageUp,
        PageDown,
        Home,
        End,

        Left,
        Right,
        Up,
        Down,

        PrintScreen,
        Insert,
        Delete,

        Key0,
        Key1,
        Key2,
        Key3,
        Key4,
        Key5,
        Key6,
        Key7,
        Key8,
        Key9,

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

        NumPad0,
        NumPad1,
        NumPad2,
        NumPad3,
        NumPad4,
        NumPad5,
        NumPad6,
        NumPad7,
        NumPad8,
        NumPad9,

        Multiply,
        Add,
        Subtract,
        Decimal,
        Divide,

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

        Numlock,
        Scrollock
    };

    //enum class MouseButton : unsigned
    //{
     //   Left
    //};

    enum class ControllerButton : unsigned
    {
        DPadUp,
        DPadDown,
        DPadLeft,
        DPadRight,

        Start,
        Back,

        LeftThumb,
        RightThumb,
        LeftShoulder,
        RightShoulder,

        A,
        B,
        X,
        Y,
    };

    enum class InputState
    {
        DownThisFrame,
        UpThisFrame,
        Pressed,
        //Movement,

        STATE_COUNT
    };

}

#endif