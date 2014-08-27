#ifndef MIDIMESSAGE_H
#define MIDIMESSAGE_H

class MIDICommand
{
public:
    enum MIDICommandType {NOTEON, NOTEOFF};
    MIDICommand();
    MIDICommand(MIDICommandType type, unsigned char channel, unsigned char note, unsigned char velocity);
    MIDICommandType getType();
    void setType(const MIDICommandType & type);
    unsigned char getNote();
    void setNote(const unsigned char & note);
    unsigned char getVelocity();
    void setVelocity(const unsigned char & velocity);
    unsigned char getChannel();
    void setChannel(const unsigned char & channel);

private:
    MIDICommandType type_;
    unsigned char channel_;
    unsigned char note_;
    unsigned char velocity_;


};

inline MIDICommand::MIDICommandType MIDICommand::getType()
{
    return type_;
}

inline void MIDICommand::setType(const MIDICommand::MIDICommandType & type)
{
    type_ = type;
}

inline unsigned char MIDICommand::getNote()
{
    return note_;
}
inline void MIDICommand::setNote(const unsigned char & note)
{
    note_ = note;
}
inline unsigned char MIDICommand::getVelocity()
{
    return velocity_;
}

inline void MIDICommand::setVelocity(const unsigned char & velocity)
{
    velocity_ = velocity;
}

inline unsigned char MIDICommand::getChannel()
{
    return channel_;
}

inline void MIDICommand::setChannel(const unsigned char & channel)
{
    channel_ = channel;
}

#endif // MIDIMESSAGE_H
