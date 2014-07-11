#ifndef SEQUENCEMESSAGE_H
#define SEQUENCEMESSAGE_H

class Step {
public:
    enum StepType {DRUM, MONO, POLY};

    Step( bool active, bool mute);
    bool isActive();
    bool isMuted();
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

#endif // SEQUENCEMESSAGE_H
