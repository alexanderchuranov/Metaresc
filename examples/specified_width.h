
#include <stdint.h>
#include <reslib.h>

TYPEDEF_STRUCT (specified_width_t,
                
                (int16_t, si16),
                (uint64_t, ui64),
                (int_least32_t, sil32),
                (int_fast32_t, sif32),
                (uintptr_t, uip),
                (intmax_t, sim)
                )
