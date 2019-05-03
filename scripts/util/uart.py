import serial
import google.protobuf
from nanopb.SetDacVoltagePayload_pb2 import SetDacVoltagePayload
from nanopb.SetPaAttenuationPayload_pb2 import SetPaAttenuationPayload
import struct
import nanopb.SetSteeringAnglePayload_pb2 import SetSteeringAnglePayload_pb2

DAC_MAX_VOLTAGE = 5
DAC_RESOLUTION = 1023
SYNC_BYTE = 0xA5

command_ids = {'set_dac_voltage': 0, 'set_pa_attn': 1, 'set_steering_angle':2}
dac_ids = {'dac_1': 0, 'dac_2': 1, 'dac_3': 2, 'dac_4': 3}
dac_channels = {'channel_a': 0, 'channel_b': 1}
pa_ids = {'pa_1': 0, 'pa_2': 1, 'pa_3': 2, 'pa_4': 3}
steering_angles = [0, 10, 20, 30]


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

    def set_pa_attenuation(self, pa_id, attenuation):
        msg = SetPaAttenuationPayload()
        msg.pa_id = pa_ids.get(pa_id, None)
        msg.attenuation = attenuation
        if pa_id == None:
            raise ValueError("Invalid PA id entered.")
        self.write(command_ids['set_pa_attn'], msg.SerializeToString())

    def set_steering_angles(self, steering_angle):
        msg = SetSteeringAnglePayload()
        if steering_angle not in steering_angles:
            raise ValueError("Invalid steering angle entered.")
        msg.steering_angle = steering_angle
        self.write(command_ids['set_steering_angle'], msg.SerializeToString())