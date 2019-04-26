import serial
import google.protobuf
from nanopb.SetDacVoltage_pb2 import SetDacVoltage

DAC_MAX_VOLTAGE = 5
DAC_RESOLUTION = 1023

class Uart:
    def __init__(self, port, baudrate, timeout=3):
        self.ser = serial.Serial()
        self.ser.baudrate = baudrate
        self.ser.port = port
        self.ser.timeout = timeout
        self.ser.open()

    def write(self, command_id, data):
        self.ser.write(bytes('\xa5','utf-8'))
        self.ser.write(hex(command_id))
        self.ser.write(data)

    def set_dac_voltage(self, dac_id, dac_channel, voltage):
        msg = SetDacVoltage()
        msg.dac_channel = 0 if 'a' else 1
        msg.dac_id = dac_id
        msg.voltage = voltage
        self.write(1, msg.SerializeToString())
    