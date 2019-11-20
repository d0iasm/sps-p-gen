# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: steps.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='steps.proto',
  package='data',
  syntax='proto3',
  serialized_options=None,
  serialized_pb=_b('\n\x0bsteps.proto\x12\x04\x64\x61ta\"\x8a\x03\n\x04Step\x12\x0c\n\x04step\x18\x01 \x01(\x05\x12&\n\tparticles\x18\x02 \x03(\x0b\x32\x13.data.Step.Particle\x12\x15\n\rstatic_energy\x18\x03 \x01(\x01\x12\x16\n\x0e\x64ynamic_energy\x18\x04 \x01(\x01\x12\x1e\n\x16static_energy_variance\x18\x05 \x01(\x01\x12\x1f\n\x17\x64ynamic_energy_variance\x18\x06 \x01(\x01\x12\x0f\n\x07x_value\x18\x07 \x01(\x01\x12\x0f\n\x07v_value\x18\x08 \x01(\x01\x12\x1d\n\x07k_count\x18\t \x03(\x0b\x32\x0c.data.Step.K\x1a|\n\x08Particle\x12\n\n\x02id\x18\x01 \x01(\x05\x12\t\n\x01x\x18\x02 \x01(\x01\x12\t\n\x01y\x18\x03 \x01(\x01\x12+\n\x07kparams\x18\x04 \x03(\x0b\x32\x1a.data.Step.Particle.Kparam\x1a!\n\x06Kparam\x12\n\n\x02id\x18\x01 \x01(\x05\x12\x0b\n\x03val\x18\x02 \x01(\x02\x1a\x1d\n\x01K\x12\t\n\x01k\x18\x01 \x01(\x01\x12\r\n\x05\x63ount\x18\x02 \x01(\x05\"\"\n\x05Steps\x12\x19\n\x05steps\x18\x01 \x03(\x0b\x32\n.data.Stepb\x06proto3')
)




_STEP_PARTICLE_KPARAM = _descriptor.Descriptor(
  name='Kparam',
  full_name='data.Step.Particle.Kparam',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='id', full_name='data.Step.Particle.Kparam.id', index=0,
      number=1, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='val', full_name='data.Step.Particle.Kparam.val', index=1,
      number=2, type=2, cpp_type=6, label=1,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=352,
  serialized_end=385,
)

_STEP_PARTICLE = _descriptor.Descriptor(
  name='Particle',
  full_name='data.Step.Particle',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='id', full_name='data.Step.Particle.id', index=0,
      number=1, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='x', full_name='data.Step.Particle.x', index=1,
      number=2, type=1, cpp_type=5, label=1,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='y', full_name='data.Step.Particle.y', index=2,
      number=3, type=1, cpp_type=5, label=1,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='kparams', full_name='data.Step.Particle.kparams', index=3,
      number=4, type=11, cpp_type=10, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[_STEP_PARTICLE_KPARAM, ],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=261,
  serialized_end=385,
)

_STEP_K = _descriptor.Descriptor(
  name='K',
  full_name='data.Step.K',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='k', full_name='data.Step.K.k', index=0,
      number=1, type=1, cpp_type=5, label=1,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='count', full_name='data.Step.K.count', index=1,
      number=2, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=387,
  serialized_end=416,
)

_STEP = _descriptor.Descriptor(
  name='Step',
  full_name='data.Step',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='step', full_name='data.Step.step', index=0,
      number=1, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='particles', full_name='data.Step.particles', index=1,
      number=2, type=11, cpp_type=10, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='static_energy', full_name='data.Step.static_energy', index=2,
      number=3, type=1, cpp_type=5, label=1,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='dynamic_energy', full_name='data.Step.dynamic_energy', index=3,
      number=4, type=1, cpp_type=5, label=1,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='static_energy_variance', full_name='data.Step.static_energy_variance', index=4,
      number=5, type=1, cpp_type=5, label=1,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='dynamic_energy_variance', full_name='data.Step.dynamic_energy_variance', index=5,
      number=6, type=1, cpp_type=5, label=1,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='x_value', full_name='data.Step.x_value', index=6,
      number=7, type=1, cpp_type=5, label=1,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='v_value', full_name='data.Step.v_value', index=7,
      number=8, type=1, cpp_type=5, label=1,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='k_count', full_name='data.Step.k_count', index=8,
      number=9, type=11, cpp_type=10, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[_STEP_PARTICLE, _STEP_K, ],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=22,
  serialized_end=416,
)


_STEPS = _descriptor.Descriptor(
  name='Steps',
  full_name='data.Steps',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='steps', full_name='data.Steps.steps', index=0,
      number=1, type=11, cpp_type=10, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=418,
  serialized_end=452,
)

_STEP_PARTICLE_KPARAM.containing_type = _STEP_PARTICLE
_STEP_PARTICLE.fields_by_name['kparams'].message_type = _STEP_PARTICLE_KPARAM
_STEP_PARTICLE.containing_type = _STEP
_STEP_K.containing_type = _STEP
_STEP.fields_by_name['particles'].message_type = _STEP_PARTICLE
_STEP.fields_by_name['k_count'].message_type = _STEP_K
_STEPS.fields_by_name['steps'].message_type = _STEP
DESCRIPTOR.message_types_by_name['Step'] = _STEP
DESCRIPTOR.message_types_by_name['Steps'] = _STEPS
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

Step = _reflection.GeneratedProtocolMessageType('Step', (_message.Message,), dict(

  Particle = _reflection.GeneratedProtocolMessageType('Particle', (_message.Message,), dict(

    Kparam = _reflection.GeneratedProtocolMessageType('Kparam', (_message.Message,), dict(
      DESCRIPTOR = _STEP_PARTICLE_KPARAM,
      __module__ = 'steps_pb2'
      # @@protoc_insertion_point(class_scope:data.Step.Particle.Kparam)
      ))
    ,
    DESCRIPTOR = _STEP_PARTICLE,
    __module__ = 'steps_pb2'
    # @@protoc_insertion_point(class_scope:data.Step.Particle)
    ))
  ,

  K = _reflection.GeneratedProtocolMessageType('K', (_message.Message,), dict(
    DESCRIPTOR = _STEP_K,
    __module__ = 'steps_pb2'
    # @@protoc_insertion_point(class_scope:data.Step.K)
    ))
  ,
  DESCRIPTOR = _STEP,
  __module__ = 'steps_pb2'
  # @@protoc_insertion_point(class_scope:data.Step)
  ))
_sym_db.RegisterMessage(Step)
_sym_db.RegisterMessage(Step.Particle)
_sym_db.RegisterMessage(Step.Particle.Kparam)
_sym_db.RegisterMessage(Step.K)

Steps = _reflection.GeneratedProtocolMessageType('Steps', (_message.Message,), dict(
  DESCRIPTOR = _STEPS,
  __module__ = 'steps_pb2'
  # @@protoc_insertion_point(class_scope:data.Steps)
  ))
_sym_db.RegisterMessage(Steps)


# @@protoc_insertion_point(module_scope)
