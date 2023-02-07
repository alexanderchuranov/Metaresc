/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */
#ifndef _MR_HSORT_H_
#define _MR_HSORT_H_

#include <stddef.h> /* size_t */
#include <metaresc.h>

extern void mr_hsort (void * array, size_t count, size_t size, mr_compar_fn_t compar_fn, void * context);

#endif /* _MR_HSORT_H_ */
