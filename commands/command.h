#ifndef COMMANDS_COMMAND_H_
#define COMMANDS_COMMAND_H_

#include <stdint.h>
#include <stdbool.h>

class Command {
   public:
    explicit Command(uint8_t command_arg_length);
    virtual ~Command() {};
    virtual bool ExecuteCommand() = 0;
    uint8_t GetCommandArgumentLength() const;

   protected:
    uint8_t command_arg_length;
};

#endif /* COMMANDS_COMMAND_H_*/