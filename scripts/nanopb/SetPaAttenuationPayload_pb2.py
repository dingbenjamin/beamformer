# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: SetPaAttenuationPayload.proto

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
  name='SetPaAttenuationPayload.proto',
  package='',
  syntax='proto2',
  serialized_pb=_b('\n\x1dSetPaAttenuationPayload.proto\"C\n\x17SetPaAttenuationPayload\x12\x10\n\x05pa_id\x18\x01 \x02(\x07:\x01\x30\x12\x16\n\x0b\x61ttenuation\x18\x02 \x02(\x07:\x01\x30')
)
_sym_db.RegisterFileDescriptor(DESCRIPTOR)




_SETPAATTENUATIONPAYLOAD = _descriptor.Descriptor(
  name='SetPaAttenuationPayload',
  full_name='SetPaAttenuationPayload',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='pa_id', full_name='SetPaAttenuationPayload.pa_id', index=0,
      number=1, type=7, cpp_type=3, label=2,
      has_default_value=True, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='attenuation', full_name='SetPaAttenuationPayload.attenuation', index=1,
      number=2, type=7, cpp_type=3, label=2,
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
  serialized_end=100,
)

DESCRIPTOR.message_types_by_name['SetPaAttenuationPayload'] = _SETPAATTENUATIONPAYLOAD

SetPaAttenuationPayload = _reflection.GeneratedProtocolMessageType('SetPaAttenuationPayload', (_message.Message,), dict(
  DESCRIPTOR = _SETPAATTENUATIONPAYLOAD,
  __module__ = 'SetPaAttenuationPayload_pb2'
  # @@protoc_insertion_point(class_scope:SetPaAttenuationPayload)
  ))
_sym_db.RegisterMessage(SetPaAttenuationPayload)


# @@protoc_insertion_point(module_scope)
