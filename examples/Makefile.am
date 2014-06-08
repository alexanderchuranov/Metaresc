# -*- makefile -*-

AM_CFLAGS = @AM_CFLAGS@ @LIBXML2_CFLAGS@
AM_LDFLAGS = -rdynamic
AM_CPPFLAGS = -I$(top_srcdir)/src
LIBS += ../src/libmetaresc.la $(LIBXML2_LIBS)

noinst_PROGRAMS = gjobs basic separate emptystruct justabuiltin builtins suffixes_comments_userdata arrays simple_structure specified_width enums named_union anonymous_unions type_prefixes function_pointers function_type double_pointer bitfields marshalling_structures_basic_types marshalling_enumerations marshalling_arrays marshalling_unions marshalling_pointers marshalling_pointers_to_functions deserialization_3_args deserialization_2_args deserializing_xml save_json load_json simplified_grammar multidimensional_arrays decl_rarray save_cinit load_cinit save_scm load_scm type_descriptors field_types get_set_field function_pointer_metadata array_metadata enums_metadata

separate_SOURCES = separate.c separate_employee.c
