/* -*- C -*- */
/* This file is a part of Metaresc project */
/*
  Deserialization hardening tests for the XDR loader.

  The round-trip tests only ever feed the loader data that the library
  itself produced, so they never exercise the loader against a crafted
  stream. These tests hand-build hostile XDR streams whose embedded
  length/count fields exceed the capacity of the destination field, and
  check that the loader rejects them (returns MR_FAILURE) instead of
  overflowing the fixed-size destination buffer.

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

MAIN_TEST_SUITE ((valid_char_array_roundtrips, "valid char array round-trips"),
                 (valid_int_array_roundtrips, "valid int array round-trips"),
                 (oversized_char_array_rejected, "oversized char array length rejected"),
                 (oversized_int_array_rejected, "oversized int array count rejected")
                 );

#else /* HAVE_RPC_TYPES_H */

int main (int argc, char * argv[]) { return (0); }

#endif /* HAVE_RPC_TYPES_H */
