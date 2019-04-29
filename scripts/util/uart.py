import serial
import google.protobuf
from nanopb.SetDacVoltagePayload_pb2 import SetDacVoltagePayload
import struct

DAC_MAX_VOLTAGE = 5
DAC_RESOLUTION = 1023
SYNC_BYTE = 0xA5

command_ids = {'set_dac_voltage': 0, 'set_pa_attn': 2}
dac_ids = {'dac_1': 0, 'dac_2': 1, 'dac_3': 2, 'dac_4': 3}
dac_channels = {'channel_a': 0, 'channel_b': 1}


class Uart:
    def __init__(self, port, baudrate, timeout=3):
        self.ser = serial.Serial()
        self.ser.baudrate = baudrate
        self.ser.port = port
        self.ser.timeout = timeout
        self.ser.open()

    def write(self, command_id, data):
        payload = b''
        payload += struct.pack('<B', SYNC_BYTE)
        payload += struct.pack('<B', command_id)
        payload += data
        self.ser.write(payload)

    def set_dac_voltage(self, dac_id, dac_channel, voltage):
        msg = SetDacVoltagePayload()
        msg.dac_channel = dac_channels.get(dac_channel, None)
        msg.dac_id = dac_ids.get(dac_id, None)
        if msg.dac_channel == None or msg.dac_id == None:
            raise ValueError("DAC channel or DAC id is invalid.")
        msg.voltage = voltage
        self.write(command_ids['set_dac_voltage'], msg.SerializeToString())
