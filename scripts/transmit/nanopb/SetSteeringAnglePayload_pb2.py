# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: SetSteeringAnglePayload.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='SetSteeringAnglePayload.proto',
  package='',
  syntax='proto2',
  serialized_pb=_b('\n\x1dSetSteeringAnglePayload.proto\"4\n\x17SetSteeringAnglePayload\x12\x19\n\x0esteering_angle\x18\x01 \x02(\x07:\x01\x30')
)
_sym_db.RegisterFileDescriptor(DESCRIPTOR)




_SETSTEERINGANGLEPAYLOAD = _descriptor.Descriptor(
  name='SetSteeringAnglePayload',
  full_name='SetSteeringAnglePayload',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='steering_angle', full_name='SetSteeringAnglePayload.steering_angle', index=0,
      number=1, type=7, cpp_type=3, label=2,
      has_default_value=True, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=33,
  serialized_end=85,
)

DESCRIPTOR.message_types_by_name['SetSteeringAnglePayload'] = _SETSTEERINGANGLEPAYLOAD

SetSteeringAnglePayload = _reflection.GeneratedProtocolMessageType('SetSteeringAnglePayload', (_message.Message,), dict(
  DESCRIPTOR = _SETSTEERINGANGLEPAYLOAD,
  __module__ = 'SetSteeringAnglePayload_pb2'
  # @@protoc_insertion_point(class_scope:SetSteeringAnglePayload)
  ))
_sym_db.RegisterMessage(SetSteeringAnglePayload)


# @@protoc_insertion_point(module_scope)