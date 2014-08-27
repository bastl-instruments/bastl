#ifndef SEQUENCEMESSAGE_H
#define SEQUENCEMESSAGE_H

class Step {
public:
    enum StepType {DRUM, MONO, POLY};

    Step( bool active, bool mute);
    bool isActive();
    bool isMuted();
    void setActive(bool active);
    void setMuted(bool muted);
private:
    bool _active;
    bool _mute;
};

inline bool Step::isActive() {
    return _active;
}

inline bool Step::isMuted() {
    return _mute;
}

inline void Step::setActive(bool active) {
    _active = active;
}

inline void Step::setMuted(bool mute) {
    _mute = mute;
}

#endif // SEQUENCEMESSAGE_H
