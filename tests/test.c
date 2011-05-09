/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */
#define __USE_GNU
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef HAVE_CONFIG_H
# include <rlconfig.h>
#endif /* HAVE_CONFIG_H */

#define BUF_SIZE (65536)
typedef char str_t[16];
#define RL_MODE PROTO
#include "test.h"
#define RL_MODE DESC
#include "test.h"

#include <tsearch.h>

static void
free_sample (sample_t * sample_)
{
  if (sample_->union_str_discriminator)
    RL_FREE (sample_->union_str_discriminator);
  if (sample_->string)
    RL_FREE (sample_->string);
  if (sample_->string_zero)
    RL_FREE (sample_->string_zero);
  if (sample_->string_full)
    RL_FREE (sample_->string_full);
  if (sample_->rarray.data)
    RL_FREE (sample_->rarray.data);
  if (sample_->rarray_.data)
    RL_FREE (sample_->rarray_.data);
  if (sample_->ptr_external)
    RL_FREE ((void*)sample_->ptr_external);
  if (sample_->typename_with_spaces)
    RL_FREE (sample_->typename_with_spaces);
  if (sample_->ptr_int8)
    RL_FREE (sample_->ptr_int8);
  if (sample_->ptr_uint8)
    RL_FREE (sample_->ptr_uint8);
  if (sample_->ptr_char)
    RL_FREE (sample_->ptr_char);
}

static void rl_mem_init_custom (void)
{
  void * my_malloc (const char * filename, const char * function, int line, size_t size) { return (malloc (size)); }
  void * my_realloc (const char * filename, const char * function, int line, void * ptr, size_t size) { return (realloc (ptr, size)); }
  char * my_strdup (const char * filename, const char * function, int line, const char * str) { return (strdup (str)); }
  void my_free (const char * filename, const char * function, int line, void * ptr) { free (ptr); }

  //RL_MESSAGE (RL_LL_TRACE, RL_MESSAGE_ENTER_CONSTRUCTOR);
  
  rl_conf.rl_mem.mem_alloc_strategy = 2;
  rl_conf.rl_mem.malloc = my_malloc;
  rl_conf.rl_mem.realloc = my_realloc;
  rl_conf.rl_mem.strdup = my_strdup;
  rl_conf.rl_mem.free = my_free;
}

RL_MEM_INIT (rl_mem_init_custom ())

#define MAX_LEN (5)
#define ALPH "_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"

static uint64_t
hash_str (char * str)
{
  uint64_t hash_value = 0;
  if (NULL == str)
    return (hash_value);
  while (*str)
    hash_value = (hash_value + (unsigned char)*str++) * 0xFEDCBA987654321LL;
  
  return (hash_value);
}

static int cmp_hash (const void * x, const void * y)
{
  uint64_t x_ = (uint64_t)x;
  uint64_t y_ = (uint64_t)y;
  return ((x_ > y_) - (x_ < y_));
}

static int hash_name (char * name, rl_red_black_tree_node_t ** root)
{
  static int cnt = 0;
  uint64_t hash = hash_str (name);
  rl_red_black_tree_node_t * match = tfind ((void*)hash, (void*)root, cmp_hash);
  if ((++cnt & ((1 << 19) - 1)) == 0)
    {
      printf ("%d '%s' %" SCNu64 "\r", cnt, name, hash);
      fflush (stdout);
    }
  if (match)
    printf ("match %d '%s' %" SCNu64 "\n", cnt, name, hash);
  else
    {
      match = tsearch ((void*)hash, (void*)root, cmp_hash);
      if (!match)
	{
	  printf ("Malloc failed\n");
	  return (!0);
	}
    }
  return (0);
}

static int test_hash ()
{
  int idx[MAX_LEN];
  char name[MAX_LEN + 1];
  int i, len;
  rl_red_black_tree_node_t * root = NULL;

  for (len = 1; len <= MAX_LEN; ++len)
    {
      for (i = 0; i < len; ++i)
	name[i] = ALPH[ idx[i] = 0 ];
      name[len] = 0;
      for (;;)
	{
	  if (hash_name (name, &root))
	    break;
	  for (i = len - 1; ((i >= 0) && (idx[i] == sizeof (ALPH) - 2)); --i)
	    name[i] = ALPH[ idx[i] = 0 ];
	  if (i < 0)
	    break;
	  name[i] = ALPH[ ++idx[i] ];
	}
    }
  return (0);
}

int
main (void)
{
  rl_output_format_t old_uint8 = rl_output_format[RL_TYPE_UINT8];
  char * str = NULL;
  XDR xdrs;
  rl_rarray_t ra;
  char xdr_orig[BUF_SIZE];
  int xdr_orig_size = 0;
  char xdr_dump[BUF_SIZE];
  int xdr_dump_size = 0;
  int xml1_size = 0;
  char xdr_xml1[BUF_SIZE];
  int xdr_xml1_size = 0;
#ifdef HAVE_LIBXML2
  int xml2_size = 0;
  char xdr_xml2[BUF_SIZE];
  int xdr_xml2_size = 0;
#endif /* HAVE_LIBXML2 */
  int cinit_size = 0;
  char xdr_cinit[BUF_SIZE];
  int xdr_cinit_size = 0;
  int json_size = 0;
  char xdr_json[BUF_SIZE];
  int xdr_json_size = 0;
  int scm_size = 0;
  char xdr_scm[BUF_SIZE];
  int xdr_scm_size = 0;
  point_t point_;
  point_t point = {-M_PI, M_E};
  sample_t sample_;
  sample_t sample = {
    .ptr_fwd = &sample.point,
    .ptr_fwd_mismatch = (point_t*)&sample.point.y,
    .point = {.x = M_PI, .y = -M_E},
    .ptr_bkw = &sample.point,
    .ptr_bkw_mismatch = (point_t*)&sample.point.y,
    .ptr_external = &point,
    .type_renamed = &point.y,
    .typename_with_spaces = (long double[]){ 1 },
    .pointer_on_pointer = &sample.ptr_bkw,
    ._int8 = -1 ^ ((uint8_t)-1 >> 1),
    ._uint8 = (uint8_t)-1,
    ._int16 = -1 ^ ((uint16_t)-1 >> 1),
    ._uint16 = (uint16_t)-1,
    ._int32 = -1 ^ ((uint32_t)-1 >> 1),
    ._uint32 = (uint32_t)-1,
    ._int = -1,
    ._int64 = -1 ^ ((uint64_t)-1 >> 1),
    ._uint64 = (uint64_t)-1,
    ._float = M_E,
    ._double = M_PI,
    .ld = -M_PI,
    ._ld = {-M_PI, M_PI},
    ._char = '\225',
    .prev = NULL,
    .union_static_str_discriminator = "union_float",
    "", {
      .union_uint1 = {-1},
      .union_float = M_PI,
    },
    .union_str_discriminator = "union_double",
    "", {
      .union_uint2 = {0},
      .union_double = M_E,
    },
    .union_enum_discriminator = UED_FLOAT,
    ._union_enum = {.union_uint32 = 1987654321},
    .union_int_discriminator = 0,
    ._union_int = {.union_float = M_PI},
    ._bool = TRUE,
    .mask = READ | EXEC,
    .string = "\n\\\"HI\62\65\12<&lt;&gt;>&amp;\\\"",
    .string_empty = NULL,
    .string_zero = "",
    .string_full = "\040\041\042\043\044\045\046\047\050\051\052\053\054\055\056\057\060\061\062\063\064\065\066\067\070\071\072\073\074\075\076\077\100\101\102\103\104\105\106\107\110\111\112\113\114\115\116\117\120\121\122\123\124\125\126\127\130\131\132\133\134\135\136\137\140\141\142\143\144\145\146\147\150\151\152\153\154\155\156\157\160\161\162\163\164\165\166\167\170\171\172\173\174\175\176",
    .char_array = "sample\"",
    .char_array_type = "",
    .array = "ab",
    .array_ = {{'c'}, {'d'}},
    .array2d = {{1 ,2 }, {3, 4}, {5, 6}},
    .array_none = {{'e'}, {'f'}},
    .rarray = { .data = (char_t[]){{'g'}, {'h'}}, .size = 2 },
    .rarray_ = { .data = "ijk", .size = 4 },
    .rarray_empty = { .data = NULL, .size = 0 },
    ._void = NULL,
    .next = &sample,
    .prev = &sample,
    .ptr_int8 = (int8_t*)"x",
    .ptr_uint8 = (uint8_t*)"y",
    .ptr_char = "z",
    .ptr_null = NULL,
    .arr = {2, 3},
  };

  //test_hash ();

#if 1
  point.sample = &sample;
  sample.ptr_char_ = &sample.rarray_.data[3];
  sample._void = &sample.string_empty;
  
#ifdef HAVE_LIBXML2
  LIBXML_TEST_VERSION;
  xmlKeepBlanksDefault(0);
#endif /* HAVE_LIBXML2 */

  char * format_float(rl_ptrdes_t * ptrdes) { char str[RL_FLOAT_TO_STRING_BUF_SIZE] = ""; sprintf (str, "%.10f", *(float*)ptrdes->data); return RL_STRDUP (str); }
  char * format_double(rl_ptrdes_t * ptrdes) { char str[RL_FLOAT_TO_STRING_BUF_SIZE] = ""; sprintf (str, "%.20g", *(double*)ptrdes->data); return RL_STRDUP (str); }
  char * format_uint8(rl_ptrdes_t * ptrdes) {
    char str[RL_INT_TO_STRING_BUF_SIZE] = "";
    if ((NULL == ptrdes->fd.ext) || (NULL == ((meta_info_t*)ptrdes->fd.ext)->format))
      return (old_uint8 (ptrdes));
    sprintf (str, ((meta_info_t*)ptrdes->fd.ext)->format, *(int8_t*)ptrdes->data);
    return (RL_STRDUP (str));
  }
  rl_output_format[RL_TYPE_FLOAT] = format_float;
  rl_output_format[RL_TYPE_DOUBLE] = format_double;
  rl_output_format[RL_TYPE_UINT8] = format_uint8;

  xdrmem_create (&xdrs, xdr_orig, sizeof (xdr_orig), XDR_ENCODE);
  if (0 == RL_SAVE_XDR (sample_t, &xdrs, &sample))
    {
      printf ("Failed to save into XDR. Exiting...\n");
      return (EXIT_FAILURE);
    }
  xdr_orig_size = XDR_GETPOS (&xdrs);
  
#define TEST_LOAD_METHOD(METHOD, ACTION) ({				\
  sample_ = sample;							\
  memset (&sample, 0, sizeof (sample));					\
  if (0 == ACTION (sample_t, str, &sample))				\
    printf ("Failed to load from " #METHOD "\n");			\
  memset (xdr_ ## METHOD, 0, sizeof (xdr_ ## METHOD));			\
  xdrmem_create (&xdrs, xdr_ ## METHOD, sizeof (xdr_ ## METHOD), XDR_ENCODE); \
  if (0 == RL_SAVE_XDR (sample_t, &xdrs, &sample))			\
    printf ("Failed to save into XDR.\n");				\
  free_sample (&sample);						\
  sample = sample_;							\
  XDR_GETPOS (&xdrs);							\
    })

#ifdef HAVE_LIBXML2
  str = RL_SAVE_XML2 (sample_t, &sample);
  if (NULL == str)
    printf ("XML save failed.\n");
  else
    {
      xml2_size = strlen (str);
      printf ("%s\n", str);
      xdr_xml2_size = TEST_LOAD_METHOD (xml2, RL_LOAD_XML2);
      RL_FREE (str);
    }
#endif /* HAVE_LIBXML2 */
  
  str = RL_SAVE_XML1 (sample_t, (sample_t*)((&sample ) ) ); // test top level name extraction
  if (NULL == str)
    printf ("XML1 save failed.\n");
  else
    {
      xml1_size = strlen (str);
      printf ("%s\n\n", str);
#ifdef HAVE_BISON_FLEX
      xdr_xml1_size = TEST_LOAD_METHOD (xml1, RL_LOAD_XML1);
#endif /* HAVE_BISON_FLEX */
      RL_FREE (str);
    }

  str = RL_SAVE_CINIT (sample_t, &sample);
  if (NULL == str)
    printf ("CINIT save failed.\n");
  else
    {
      cinit_size = strlen (str);
      printf ("%s\n", str);
#ifdef HAVE_BISON_FLEX
      xdr_cinit_size = TEST_LOAD_METHOD (cinit, RL_LOAD_CINIT);
#endif /* HAVE_BISON_FLEX */
      RL_FREE (str);
    }

  str = RL_SAVE_JSON (sample_t, &sample);
  if (NULL == str)
    printf ("JSON save failed.\n");
  else
    {
      json_size = strlen (str);
      printf ("%s\n", str);
#ifdef HAVE_BISON_FLEX
      xdr_json_size = TEST_LOAD_METHOD (json, RL_LOAD_JSON);
#endif /* HAVE_BISON_FLEX */
      RL_FREE (str);
    }
  
  str = RL_SAVE_SCM (sample_t, &sample);
  if (NULL == str)
    printf ("SCM save failed.\n");
  else
    {
      scm_size = strlen (str);
      printf ("%s\n\n", str);
#ifdef HAVE_BISON_FLEX
      xdr_scm_size = TEST_LOAD_METHOD (scm, RL_LOAD_SCM);
#endif /* HAVE_BISON_FLEX */
      RL_FREE (str);
    }
  
  sample_ = sample;
  memset (&sample, 0, sizeof (sample));
  xdrmem_create (&xdrs, xdr_orig, sizeof (xdr_orig), XDR_DECODE);
  if (0 == RL_LOAD_XDR (sample_t, &xdrs, &sample))
    printf ("Failed to load from XDR\n");
  memset (xdr_dump, 0, sizeof (xdr_dump));
  xdrmem_create (&xdrs, xdr_dump, sizeof (xdr_dump), XDR_ENCODE);
  if (0 == RL_SAVE_XDR (sample_t, &xdrs, &sample))
    printf ("Failed to save into XDR.\n");
  xdr_dump_size = XDR_GETPOS (&xdrs);
  free_sample (&sample);
  sample = sample_;

#ifdef HAVE_LIBXML2
  printf ("XML2 (size %d) save/load matched original dump: %s\n", xml2_size,
	  (xdr_xml2_size == xdr_orig_size) && (0 == memcmp (xdr_orig, xdr_xml2, xdr_orig_size)) ? "PASSED" : "FAILED");
#endif /* HAVE_LIBXML2 */
  printf ("XML1 (size %d) save/load matched original dump: %s\n", xml1_size,
	  (xdr_xml1_size == xdr_orig_size) && (0 == memcmp (xdr_orig, xdr_xml1, xdr_orig_size)) ? "PASSED" : "FAILED");
  printf ("CINIT (size %d) save/load matched original dump: %s\n", cinit_size,
	  (xdr_cinit_size == xdr_orig_size) && (0 == memcmp (xdr_orig, xdr_cinit, xdr_orig_size)) ? "PASSED" : "FAILED");
  printf ("JSON (size %d) save/load matched original dump: %s\n", json_size,
	  (xdr_json_size == xdr_orig_size) && (0 == memcmp (xdr_orig, xdr_json, xdr_orig_size)) ? "PASSED" : "FAILED");
  printf ("SCM (size %d) save/load matched original dump: %s\n", scm_size,
	  (xdr_scm_size == xdr_orig_size) && (0 == memcmp (xdr_orig, xdr_scm, xdr_orig_size)) ? "PASSED" : "FAILED");
  printf ("XDR (size %d) save/load matched original dump: %s\n", xdr_orig_size,
	  (xdr_dump_size == xdr_orig_size) && (0 == memcmp (xdr_orig, xdr_dump, xdr_orig_size)) ? "PASSED" : "FAILED");

  ra = RL_SAVE_XML_RA (sample_t, &sample);
  printf ("XML_RA (size %d)\n", ra.size);
  if (ra.data)
    RL_FREE (ra.data);
  ra = RL_SAVE_CINIT_RA (sample_t, &sample);
  printf ("CINIT_RA (size %d)\n", ra.size);
  if (ra.data)
    RL_FREE (ra.data);
  ra = RL_SAVE_SCM_RA (sample_t, &sample);
  printf ("SCM_RA (size %d)\n", ra.size);
  if (ra.data)
    RL_FREE (ra.data);
  ra = RL_SAVE_XDR_RA (sample_t, &sample);
  printf ("XDR_RA (size %d)\n", ra.size);
  if (ra.data)
    RL_FREE (ra.data);

  point.sample = NULL;

#ifdef HAVE_LIBXML2
  str = RL_SAVE_XML2 (point_t, &point);
  if (str)
    {
      memset (&point_, 0, sizeof (point_));
      point_ = RL_LOAD_XML2 (point_t, str);
      RL_FREE (str);
      printf ("XML2 doubles load/save (dx = %g dy = %g): %s\n",
	      point_.x - point.x, point_.y - point.y, memcmp (&point, &point_, sizeof (point_t)) ? "FAILED" : "PASSED");
    }
#endif /* HAVE_LIBXML2 */
#ifdef HAVE_BISON_FLEX
  str = RL_SAVE_XML1 (point_t, &point);
  if (str)
    {
      memset (&point_, 0, sizeof (point_));
      point_ = RL_LOAD_XML1 (point_t, str);
      RL_FREE (str);
      printf ("XML1 doubles load/save (dx = %g dy = %g): %s\n",
	      point_.x - point.x, point_.y - point.y, memcmp (&point, &point_, sizeof (point_t)) ? "FAILED" : "PASSED");
    }
#endif /* HAVE_BISON_FLEX */
#ifdef HAVE_BISON_FLEX
  str = RL_SAVE_CINIT (point_t, &point);
  if (str)
    {
      memset (&point_, 0, sizeof (point_));
      point_ = RL_LOAD_CINIT (point_t, str);
      RL_FREE (str);
      printf ("CINIT doubles load/save (dx = %g dy = %g): %s\n",
	      point_.x - point.x, point_.y - point.y, memcmp (&point, &point_, sizeof (point_t)) ? "FAILED" : "PASSED");
    }
#endif /* HAVE_BISON_FLEX */
#ifdef HAVE_BISON_FLEX
  str = RL_SAVE_JSON (point_t, &point);
  if (str)
    {
      memset (&point_, 0, sizeof (point_));
      point_ = RL_LOAD_CINIT (point_t, str);
      RL_FREE (str);
      printf ("JSON doubles load/save (dx = %g dy = %g): %s\n",
	      point_.x - point.x, point_.y - point.y, memcmp (&point, &point_, sizeof (point_t)) ? "FAILED" : "PASSED");
    }
#endif /* HAVE_BISON_FLEX */
#ifdef HAVE_BISON_FLEX
  str = RL_SAVE_SCM (point_t, &point);
  if (str)
    {
      memset (&point_, 0, sizeof (point_));
      point_ = RL_LOAD_SCM (point_t, str);
      RL_FREE (str);
      printf ("SCHEME doubles load/save (dx = %g dy = %g): %s\n",
	      point_.x - point.x, point_.y - point.y, memcmp (&point, &point_, sizeof (point_t)) ? "FAILED" : "PASSED");
    }
#endif /* HAVE_BISON_FLEX */

  memset (xdr_dump, 0, sizeof (xdr_dump));
  xdrmem_create (&xdrs, xdr_dump, sizeof (xdr_dump), XDR_ENCODE);
  RL_SAVE_XDR (point_t, &xdrs, &point);
  xdrmem_create (&xdrs, xdr_dump, sizeof (xdr_dump), XDR_DECODE);
  point_ = RL_LOAD_XDR (point_t, &xdrs);
  printf ("XDR doubles load/save (dx = %g dy = %g): %s\n\n",
	  point_.x - point.x, point_.y - point.y, memcmp (&point, &point_, sizeof (point_t)) ? "FAILED" : "PASSED");

  (int)sizeof(struct { int:-!!(0); });
#endif
  
#if 1
  str = RL_SAVE_CINIT (rl_conf_t, &rl_conf);
  if (str)
    {
      printf ("rl_conf (size = %d):\n%s\n", (int)strlen (str), str);
      RL_FREE (str);
    }
  xdrmem_create (&xdrs, xdr_dump, sizeof (xdr_dump), XDR_ENCODE);
  RL_SAVE_XDR (rl_conf_t, &xdrs, &rl_conf);
  printf ("XDR size = %d\n", XDR_GETPOS (&xdrs));
#endif

  rl_type_t x = RL_TYPE_DETECT (typeof (volatile const char*));
  str = RL_SAVE_CINIT (rl_type_t, &x);
  if (str)
    {
      rl_type_t x_ = RL_LOAD_CINIT (rl_type_t, str);
      printf ("x = %d = %s, x_ = %d\n", x, str, x_);
      RL_FREE (str);
    }
  int y[] = {1, 2, 3, 4};
  str = RL_SAVE_CINIT (int, y);
  if (str)
    {
      int i;
      int y_[4] = {[0 ... 3] = -1};
      RL_LOAD_CINIT (int, str, y_);
      printf ("y = %s\n", str);
      for (i = 0; i < sizeof (y_) / sizeof (y_[0]); ++i)
	printf ("y_[%d] = %d ", i, y_[i]);
      printf ("\n");
      RL_FREE (str);
    }
#ifdef HAVE_LIBXML2 
  /* Clean up everything else before quitting. */
  xmlCleanupParser();
#endif /* HAVE_LIBXML2 */
  return (EXIT_SUCCESS);
}


