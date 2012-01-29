# -*- makefile -*-

AM_CFLAGS = --fast-math -Wall -Werror
INCLUDES = -I$(top_srcdir)/src
LIBS += ../src/libreslib.la

noinst_PROGRAMS = gjobs basic separate emptystruct justabuiltin builtins suffixes_comments_userdata arrays simple_structure specified_width enums named_union anonymous_unions type_prefixes function_pointers bitfields marshalling_structures_basic_types marshalling_enumerations marshalling_arrays marshalling_unions marshalling_pointers marshalling_pointers_to_functions marshalling_bit_fields deserialization_3_args deserializing_xml save_json load_json simplified_grammar multidimensional_arrays decl_rarray save_cinit load_cinit save_scm load_scm type_descriptors field_types get_set_field function_pointer_metadata array_metadata enums_metadata

separate_SOURCES = separate.c separate_employee.c
