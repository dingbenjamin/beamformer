#ifndef COMMANDS_COMMAND_H_
#define COMMANDS_COMMAND_H_

#include <utils/nanopb_utils.h>
#include <commands/proto/SetDacVoltagePayload.pb.h>

typedef enum {
    kSetDacVoltageCommand,
    kSetPaAttenuationCommand
} Commands;

#endif /* COMMANDS_COMMAND_H_*/