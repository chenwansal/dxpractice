#pragma once

struct MousePoint {
    /* data */
    int x;
    int y;
};

struct MousePointF {
    float x;
    float y;
};

class MouseEvent {
  public:
    enum class EventType {
        LPress,
        LRelease,
        RPress,
        RRelease,
        MPress,
        MRelease,
        WheelScrollUp,
        WheelScrollDown,
        Move,
        RAW_MOVE,
        Invalid
    };

  public:
    MouseEvent();
    MouseEvent(const EventType type, const int x, const int y);
    bool IsValid() const;
    EventType GetType() const;
    MousePoint GetPos() const;
    int GetPosX() const;
    int GetPosY() const;

  private:
    EventType type;
    int x;
    int y;
};
