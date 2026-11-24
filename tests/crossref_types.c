#include <check.h>
#include <metaresc.h>
#include <regression.h>

/*
  A reference is made between nodes of the same type. The loaders must not
  set a pointer to a node of another type. These tests load documents where
  `link` is a reference on the node `value`, which is an integer, and on the
  root node, which has the type of the pointer. Indexes in JSON are the
  numbers of the nodes in the document (the root is 1), the other formats
  label the nodes. A document with such a reference must fail to load and
  must leave the pointer unset. Binary stream is covered by fuzz/corpus.
*/

TYPEDEF_STRUCT (xref_t,
                (int, value),
                (xref_t *, link),
                );

#define XREF_BAD_JSON "{ \"value\": 1, \"link\": 2 }"
#define XREF_OK_JSON "{ \"value\": 1, \"link\": 1 }"
#define XREF_BAD_YAML "---\nvalue: &ref_idx_5 1\nlink: *ref_idx_5\n...\n"
#define XREF_OK_YAML "--- &ref_idx_1\nvalue: 1\nlink: *ref_idx_1\n...\n"
#define XREF_BAD_XML "<?xml version=\"1.0\"?>\n<xref_t>\n  <value ref_idx=\"5\">1</value>\n  <link ref=\"5\"/>\n</xref_t>\n"
#define XREF_OK_XML "<?xml version=\"1.0\"?>\n<xref_t ref_idx=\"1\">\n  <value>1</value>\n  <link ref=\"1\"/>\n</xref_t>\n"
#define XREF_BAD_SCM "(\n  ;(ref_idx . 5)\n  (value . 1)\n  ;(ref . 5)\n  (link . nil)\n)\n"
#define XREF_BAD_CINIT "{ .value = /* ref_idx = 5 */ 1, .link = /* ref = 5 */ NULL }"

#define CHECK_LINK(METHOD, TEXT, EXPECTED_STATUS, EXPECTED, MESSAGE) ({	\
      char text[] = TEXT;						\
      xref_t dst;							\
      memset (&dst, 0, sizeof (dst));					\
      mr_status_t status = MR_LOAD_ ## METHOD (xref_t, text, &dst);	\
      ck_assert_msg ((EXPECTED_STATUS) == status, "method " #METHOD ": unexpected status of the load"); \
      ck_assert_msg (dst.link == (EXPECTED), "method " #METHOD ": " MESSAGE); \
    })

#define CHECK_BAD_LINK(METHOD, TEXT) \
  CHECK_LINK (METHOD, TEXT, MR_FAILURE, NULL, "a reference on a node of another type was set")
#define CHECK_OK_LINK(METHOD, TEXT) \
  CHECK_LINK (METHOD, TEXT, MR_SUCCESS, &dst, "a reference on a node of the same type was not set")

#define CHECK_BAD_JSON() CHECK_BAD_LINK (JSON, XREF_BAD_JSON)
#define CHECK_OK_JSON() CHECK_OK_LINK (JSON, XREF_OK_JSON)
#define CHECK_BAD_YAML() CHECK_BAD_LINK (YAML, XREF_BAD_YAML)
#define CHECK_OK_YAML() CHECK_OK_LINK (YAML, XREF_OK_YAML)
#define CHECK_BAD_XML2() CHECK_BAD_LINK (XML2, XREF_BAD_XML)
#define CHECK_OK_XML2() CHECK_OK_LINK (XML2, XREF_OK_XML)
#define CHECK_BAD_XML1() CHECK_BAD_LINK (XML1, XREF_BAD_XML)
#define CHECK_OK_XML1() CHECK_OK_LINK (XML1, XREF_OK_XML)
#define CHECK_BAD_SCM() CHECK_BAD_LINK (SCM, XREF_BAD_SCM)
#define CHECK_OK_SCM()
#define CHECK_BAD_CINIT() CHECK_BAD_LINK (CINIT, XREF_BAD_CINIT)
#define CHECK_OK_CINIT()
#define CHECK_BAD_XDR()
#define CHECK_OK_XDR()

#define CHECK_BAD(METHOD, ...) MR_PASTE2 (CHECK_BAD_, METHOD) ()
#define CHECK_OK(METHOD, ...) MR_PASTE2 (CHECK_OK_, METHOD) ()

START_TEST (reference_on_a_node_of_another_type) { ALL_METHODS (CHECK_BAD); } END_TEST
START_TEST (reference_on_a_node_of_the_same_type) { ALL_METHODS (CHECK_OK); } END_TEST

MAIN_TEST_SUITE ((reference_on_a_node_of_another_type, "load of a reference on a node of another type fails"),
                 (reference_on_a_node_of_the_same_type, "reference on a node of the same type is set")
                 );
