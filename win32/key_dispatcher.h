#ifndef KEYPRESS_DISPATCHER_H_
#define KEYPRESS_DISPATCHER_H_

#include <emu/emu_joystick.h>
#include <emu/emu_list.h>
#include <emu/emu_machine.h>

struct KeyPressDispatcherItem;

class KeyPressDispatcher
{
public:
    KeyPressDispatcher();

    ~KeyPressDispatcher();

    void add( unsigned key, unsigned id, unsigned data = 0 );

    TJoystick * addJoystick( unsigned id, unsigned kleft, unsigned kright, unsigned kup, unsigned kdown );

    void clear();

    bool handle( unsigned key, unsigned param, TMachine * machine );

    void notifyJoysticks( TMachine * machine );

    TJoystick * joystick( int index ) {
        return (TJoystick *) joysticks_.item(index);
    }

private:
    void expand();

    int count_;
    int capacity_;
    KeyPressDispatcherItem * items_;
    TList joysticks_;
};

#endif // KEYPRESS_DISPATCHER_H_
