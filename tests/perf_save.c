#include <perf.h>

#define MR_SAVE_NONE_RA(...) ({				\
      mr_ra_ptrdes_t ptrs = MR_SAVE (__VA_ARGS__);	\
      mr_rarray_t _ra_;					\
      _ra_.data.ptr = ptrs.ra;				\
      _ra_.MR_SIZE = ptrs.size;				\
      _ra_.type = "mr_ptrdes_t";			\
      _ra_;						\
    })
#define MR_LOAD_NONE_RA(TYPE, RA, PTR) ({	\
      memset (PTR, 0, sizeof (*PTR));		\
      MR_SUCCESS;				\
    })

TEST_PERFORMANCE (NONE)
