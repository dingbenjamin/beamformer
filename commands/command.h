#ifndef COMMANDS_COMMAND_H_
#define COMMANDS_COMMAND_H_

#include <commands/proto/SetDacVoltagePayload.pb.h>
#include <commands/proto/SetPaAttenuationPayload.pb.h>
#include <utils/nanopb_utils.h>

typedef enum { kSetDacVoltageCommand, kSetPaAttenuationCommand } Commands;

#endif /* COMMANDS_COMMAND_H_*/
