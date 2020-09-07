# -*- makefile -*-

AM_CFLAGS = @AM_CFLAGS@
AM_LDFLAGS = @AM_LDFLAGS@
AM_CPPFLAGS = -I$(top_srcdir)/src
LIBS += ../src/libmetaresc.la $(LIBXML2_LIBS)

noinst_PROGRAMS = gjobs basic separate emptystruct justabuiltin builtins suffixes_comments_userdata arrays simple_structure specified_width enums named_union anonymous_unions type_prefixes function_pointers function_type double_pointer bitfields marshalling_structures_basic_types marshalling_enumerations marshalling_arrays marshalling_unions marshalling_pointers marshalling_pointers_to_functions deserialization_3_args deserialization_2_args deserializing_xml save_json simplified_grammar multidimensional_arrays decl_rarray save_cinit save_scm type_descriptors field_types get_set_field function_pointer_metadata array_metadata enums_metadata dwarf

if HAVE_BISON_FLEX
  noinst_PROGRAMS += load_scm load_json load_cinit
endif

separate_SOURCES = separate.c separate_employee.c
dwarf_SOURCES = dwarf.c dwarf_types.c
dwarf_types.h: dwarf.$(OBJEXT)
	$(top_srcdir)/src/mr_dwarf $< > $@
