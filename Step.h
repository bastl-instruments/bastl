#ifndef SEQUENCEMESSAGE_H
#define SEQUENCEMESSAGE_H

class Step {
public:
    enum InstrumentType {DRUM, MONO, POLY};

    Step( bool mute);
    bool isMuted();
    void setMuted(bool muted);
private:
    bool _mute;
};

inline bool Step::isMuted() {
    return _mute;
}

inline void Step::setMuted(bool mute) {
    _mute = mute;
}

#endif // SEQUENCEMESSAGE_H
