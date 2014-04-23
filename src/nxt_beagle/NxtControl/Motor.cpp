#include "nxt_control/Motor.hpp"
#include "nxt_control/NxtTelegram.hpp"

namespace nxtcon
{
    void Motor::setBrick(Brick *p_brick)
    {
        brick = p_brick;
    }
    
    void Motor::setPort(const uint8_t p_port)
    {
        port = p_port;
    }
    
    void Motor::setPower(const int8_t p_power)
    {
        Telegram telegram;
        //TODO if not working try with pointer cast
        create_setMotor(&telegram, port, (uint8_t) p_power);
        brick->send(telegram);
    }
    
    void Motor::brake()
    {
        Telegram telegram;
        create_brakeMotor(&telegram, port);
        brick->send(telegram);
    }
    
    void Motor::resetTacho()
    {
        Telegram telegram;
        create_resetTacho(&telegram, port);
        brick->send(telegram);
    }
    
    const TachoData& Motor::getTachoData()
    {
        Telegram telegram;
        create_getOutputState(&telegram, port);
        brick->send(telegram);
        telegram = brick->receive();
        decode_tachoOutputState(telegram, &tacho, port);
        return tacho;
    }
    
}