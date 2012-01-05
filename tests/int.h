#ifndef _INT_H_
#define _INT_H_

#define ASSERT_SAVE_LOAD_INT(METHOD, BITS, VALUE) ({			\
      ASSERT_SAVE_LOAD_TYPE (METHOD, uint ## BITS ## _t, VALUE);	\
      ASSERT_SAVE_LOAD_TYPE (METHOD, int ## BITS ## _t, VALUE);		\
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_uint ## BITS ## _t, { VALUE }); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_int ## BITS ## _t, { VALUE }); \
    })

#endif /* _INT_H_ */
