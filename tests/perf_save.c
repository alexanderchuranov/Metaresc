#include <perf.h>

#define MR_SAVE_NONE_RA(...) ({					\
      mr_ptrdes_t * ptrs = MR_SAVE (__VA_ARGS__);		\
      (mr_rarray_t){						\
	.data.ptr = ptrs,					\
	  .MR_SIZE = mr_ptrs_count (ptrs) * sizeof (ptrs[0]),	\
	  .type = "mr_ptrdes_t",				\
	  };							\
    })

#define MR_LOAD_NONE_RA(TYPE, RA, PTR) ({	\
      memset (PTR, 0, sizeof (*PTR));		\
      MR_SUCCESS;				\
    })

TEST_PERFORMANCE (NONE)
