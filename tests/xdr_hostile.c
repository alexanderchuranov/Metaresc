/* -*- C -*- */
/* This file is a part of Metaresc project */
/*
  Deserialization hardening tests for the XDR loader.

  The round-trip tests only ever feed the loader data that the library
  itself produced, so they never exercise the loader against a crafted
  stream. These tests hand-build hostile XDR streams and check that the
  loader rejects them (returns MR_FAILURE): streams whose embedded
  length/count fields exceed the capacity of the destination field, which
  would overflow the fixed-size destination buffer, and streams with
  pointer flags that XDR save never writes.

  Stream layout for a struct with a single leading array field is just
  the field's own encoding (XDR has no per-struct/per-field framing):
    - char[N]  : [uint32 BE str_len][str_len payload bytes]
    - int32[N] : [uint32 BE byte_size][byte_size payload bytes]
  Multi-byte integers are big-endian and opaque data is padded to 4-byte
  XDR units, so all crafted lengths below are multiples of 4.
*/

#include <check.h>
#include <metaresc.h>
#include <mr_save.h>
#include <regression.h>

#ifdef HAVE_RPC_TYPES_H

/* A char-array typedef is detected as MR_TYPE_CHAR_ARRAY (serialized as a
   length-prefixed string, decoded by xdr_char_array_), unlike a plain
   char[] field which is a generic MR_TYPE_ARRAY decoded by xdr_load_array. */
typedef char char_array_t[8];
TYPEDEF_STRUCT (hostile_char_array_t, (char_array_t, str));
TYPEDEF_STRUCT (hostile_int_array_t, (int32_t, arr, [4]));

/* Build a stream with the library's own XDR resizable-array encoder: a
   32-bit header equal to payload_len (read back by the loader as a string
   length or an array's byte size) followed by payload_len filler bytes,
   encoded exactly as the loader reads them. The header is written with
   xdr_uint32_t because the on-wire protocol is defined in terms of
   uint32_t, not a system-dependent u_int.
   Designed for small payloads that fit on the stack. The caller frees
   ra.data.ptr with MR_FREE. */
static mr_rarray_t
encode_hostile_stream (uint32_t payload_len)
{
  XDR xdrs;
  mr_rarray_t ra = { .type = "uint8_t" };
  char payload[512];
  ck_assert_msg (payload_len <= sizeof (payload), "payload does not fit on the stack buffer");
  memset (payload, 'A', payload_len);

  mr_xdrra_create (&xdrs, &ra, XDR_ENCODE);
  ck_assert_msg (xdr_uint32_t (&xdrs, &payload_len), "failed to encode header");
  ck_assert_msg (xdr_opaque (&xdrs, payload, payload_len), "failed to encode payload");

  ck_assert_ptr_nonnull (ra.data.ptr);
  return (ra);
}

/* Sanity: a well-formed stream for each type still loads. This guards
   against the bound checks rejecting legitimate input. */
START_TEST (valid_char_array_roundtrips)
{
  hostile_char_array_t orig = { .str = "hello" };
  mr_rarray_t ra = MR_SAVE_XDR_RA (hostile_char_array_t, &orig);

  hostile_char_array_t restored;
  memset (&restored, 0, sizeof (restored));
  mr_status_t status = MR_LOAD_XDR_RA (hostile_char_array_t, &ra, &restored);

  ck_assert_msg (MR_SUCCESS == status, "valid char array failed to load");
  ck_assert_msg (0 == strcmp (restored.str, orig.str), "restored value mismatched");

  MR_FREE (ra.data.ptr);
} END_TEST

START_TEST (valid_int_array_roundtrips)
{
  hostile_int_array_t orig;
  memset (&orig, 0x55, sizeof (orig));
  mr_rarray_t ra = MR_SAVE_XDR_RA (hostile_int_array_t, &orig);

  hostile_int_array_t restored;
  memset (&restored, 0, sizeof (restored));
  mr_status_t status = MR_LOAD_XDR_RA (hostile_int_array_t, &ra, &restored);

  ck_assert_msg (MR_SUCCESS == status, "valid int array failed to load");
  ck_assert_msg (0 == memcmp (orig.arr, restored.arr, sizeof (orig.arr)), "restored value mismatched");

  MR_FREE (ra.data.ptr);
} END_TEST

/* A string length one byte past the declared char[8]; without the bound
   check xdr_opaque would write that byte past the inline buffer. */
START_TEST (oversized_char_array_rejected)
{
  hostile_char_array_t restored;
  mr_rarray_t ra = encode_hostile_stream (sizeof (restored.str) + sizeof (restored.str[0]));
  memset (&restored, 0, sizeof (restored));
  mr_status_t status = MR_LOAD_XDR_RA (hostile_char_array_t, &ra, &restored);

  ck_assert_msg (MR_SUCCESS != status, "oversized char array length was not rejected");

  MR_FREE (ra.data.ptr);
} END_TEST

/* A byte size one element past the declared int32[4]; without the bound
   check the loader would write a fifth element past the inline buffer. */
START_TEST (oversized_int_array_rejected)
{
  hostile_int_array_t restored;
  mr_rarray_t ra = encode_hostile_stream (sizeof (restored.arr) + sizeof (restored.arr[0]));
  memset (&restored, 0, sizeof (restored));
  mr_status_t status = MR_LOAD_XDR_RA (hostile_int_array_t, &ra, &restored);

  ck_assert_msg (MR_SUCCESS != status, "oversized int array count was not rejected");

  MR_FREE (ra.data.ptr);
} END_TEST

/*
  A string and a pointer start with a byte of flags (an XDR unit of 4 bytes).
  The stream below has flags for both fields, followed by a reference index
  if a flag says that the node is a reference.
*/
TYPEDEF_STRUCT (hostile_flags_t, (char *, str), (int32_t *, ptr));

static mr_status_t
load_with_flags (uint32_t str_flags, uint32_t ptr_flags)
{
  XDR xdrs;
  mr_rarray_t ra = { .type = "uint8_t" };
  uint32_t ref_idx = 1;

  mr_xdrra_create (&xdrs, &ra, XDR_ENCODE);
  ck_assert (xdr_uint32_t (&xdrs, &str_flags)); /* the same encoding as xdr_uint8_t */
  if (str_flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE))
    ck_assert (xdr_uint32_t (&xdrs, &ref_idx));
  ck_assert (xdr_uint32_t (&xdrs, &ptr_flags));
  if (ptr_flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE))
    ck_assert (xdr_uint32_t (&xdrs, &ref_idx));

  hostile_flags_t restored;
  memset (&restored, 0, sizeof (restored));
  mr_status_t status = MR_LOAD_XDR_RA (hostile_flags_t, &ra, &restored);
  if (MR_SUCCESS == status)
    MR_FREE_RECURSIVELY (hostile_flags_t, &restored);
  MR_FREE (ra.data.ptr);
  return (status);
}

START_TEST (unexpected_flags_rejected)
{
  ck_assert_msg (MR_SUCCESS == load_with_flags (MR_IS_NULL, MR_IS_NULL), "NULL fields failed to load");
  ck_assert_msg (MR_SUCCESS == load_with_flags (MR_IS_NULL | MR_IS_UNNAMED, MR_IS_NULL | MR_IS_REFERENCED),
                 "flags that XDR save writes failed to load");
  ck_assert_msg (MR_FAILURE == load_with_flags (MR_IS_NULL | 0x80, MR_IS_NULL), "unknown flag of a string was accepted");
  ck_assert_msg (MR_FAILURE == load_with_flags (MR_IS_NULL, MR_IS_NULL | 0x80), "unknown flag of a pointer was accepted");
  ck_assert_msg (MR_FAILURE == load_with_flags (MR_IS_NULL | MR_IS_TYPED, MR_IS_NULL), "MR_IS_TYPED was accepted");
  ck_assert_msg (MR_FAILURE == load_with_flags (MR_IS_NULL, MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE),
                 "both reference flags were accepted");
} END_TEST

/*
  A pointer on a char array type with a descriptor has elements of a fixed
  size in the block of the pointer. Each element is encoded as a string:
  [uint32 str_len][str_len payload bytes padded to 4]. The stream of
  hostile_ca_ptr_t is the flags of the pointer, the size of the block in
  bytes, the elements and the count field.
*/
typedef char hostile_ca4_t[4];
MR_TYPEDEF_DESC_BI (hostile_ca4_t);
TYPEDEF_STRUCT (hostile_ca_ptr_t,
                (hostile_ca4_t *, p, , "pointer on char arrays", { "n" }, "count_field_name"),
                (int32_t, n));

static mr_status_t
load_ca_ptr (uint32_t block_size, char * first, uint32_t first_len, char * second, uint32_t second_len, int32_t n)
{
  XDR xdrs;
  mr_rarray_t ra = { .type = "uint8_t" };
  uint32_t flags = 0;

  mr_xdrra_create (&xdrs, &ra, XDR_ENCODE);
  ck_assert (xdr_uint32_t (&xdrs, &flags));
  ck_assert (xdr_uint32_t (&xdrs, &block_size));
  ck_assert (xdr_uint32_t (&xdrs, &first_len));
  ck_assert (xdr_opaque (&xdrs, first, first_len));
  ck_assert (xdr_uint32_t (&xdrs, &second_len));
  ck_assert (xdr_opaque (&xdrs, second, second_len));
  ck_assert (xdr_int32_t (&xdrs, &n));

  hostile_ca_ptr_t restored;
  memset (&restored, 0, sizeof (restored));
  mr_status_t status = MR_LOAD_XDR_RA (hostile_ca_ptr_t, &ra, &restored);
  if (MR_SUCCESS == status)
    {
      ck_assert_msg ((0 == strcmp (restored.p[0], first)) && (0 == strcmp (restored.p[1], second)),
                     "wrong elements loaded");
      MR_FREE_RECURSIVELY (hostile_ca_ptr_t, &restored);
    }
  MR_FREE (ra.data.ptr);
  return (status);
}

START_TEST (char_array_pointer_elements)
{
  hostile_ca_ptr_t orig = { .p = (hostile_ca4_t[]){ "ab", "cde" }, .n = 2 };
  ASSERT_SAVE_LOAD (XDR, hostile_ca_ptr_t, &orig);

  ck_assert_msg (MR_SUCCESS == load_ca_ptr (8, "ab", sizeof ("ab"), "cde", sizeof ("cde"), 2),
                 "valid pointer on char arrays failed to load");
  ck_assert_msg (MR_FAILURE == load_ca_ptr (8, "ab", sizeof ("ab"), "abcdefg", sizeof ("abcdefg"), 2),
                 "second element longer than its type was accepted");
  ck_assert_msg (MR_FAILURE == load_ca_ptr (8, "abcdefg", sizeof ("abcdefg"), "ab", sizeof ("ab"), 2),
                 "first element longer than its type was accepted");
  ck_assert_msg (MR_FAILURE == load_ca_ptr (8, "ab", sizeof ("ab"), "cd", sizeof ("cd"), 3),
                 "count field larger than the loaded elements was accepted");
} END_TEST

MAIN_TEST_SUITE ((valid_char_array_roundtrips, "valid char array round-trips"),
                 (char_array_pointer_elements, "elements of pointers on char arrays have a fixed size"),
                 (valid_int_array_roundtrips, "valid int array round-trips"),
                 (oversized_char_array_rejected, "oversized char array length rejected"),
                 (oversized_int_array_rejected, "oversized int array count rejected"),
                 (unexpected_flags_rejected, "pointer flags that XDR save does not write are rejected")
                 );

#else /* HAVE_RPC_TYPES_H */

int main (int argc, char * argv[]) { return (0); }

#endif /* HAVE_RPC_TYPES_H */
