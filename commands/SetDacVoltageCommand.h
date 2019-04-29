#ifndef COMMANDS_SET_DAC_VOLTAGE_COMMAND_H
#define COMMANDS_SET_DAC_VOLTAGE_COMMAND_H_

#include <commands/command.h>
#include <commands/proto/SetDacVoltagePayload.pb.h>

class SetDacVoltageCommand : public Command {
    public:
        explicit SetDacVoltageCommand(uint8_t * payload);
        bool ExecuteCommmand();

    private:
       SetDacVoltagePayload payload; 
};


#endif /* COMMANDS_SET_DAC_VOLTAGE_COMMAND_H_*/