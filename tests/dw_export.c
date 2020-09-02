#include <string.h>

#include <metaresc.h>
#include <regression.h>

static mr_td_t ra_td[] = {
#include <dw_export.h>
};

static mr_td_t *
get_td_by_name (char * type)
{
  int i;
  for (i = sizeof (ra_td) / sizeof (ra_td[0]) - 1; i >= 0; --i)
    if (0 == strcmp (ra_td[i].type.str, type))
      break;
  ck_assert_msg (i >= 0, "Failed to find type descriptor for type %s", type);
  return (&ra_td[i]);
}

static void
check_dw_type (char * type)
{
  mr_td_t * dw_td = get_td_by_name (type);
  mr_td_t * mr_td = mr_get_td_by_name (type);
  ck_assert_msg (dw_td->size == mr_td->size, "DWARF descriptor for type '%s' mismatched builtin", type);
}

MR_START_TEST (dw_check, "Check DWARF export")
{
  check_dw_type ("mr_td_t");
} END_TEST

MAIN ();
