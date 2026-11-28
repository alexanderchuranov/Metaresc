#ifndef _MR_LOAD_H_
#define _MR_LOAD_H_

#include <metaresc.h>

/*
  Set of the nodes that have a reference index (ref_idx) in the document,
  keyed by that index. Its size depends on the number of the nodes, not on
  the values of the indexes. The hash is seeded at random, so a document can
  not choose indexes that fall into one bucket. It keeps a pointer on itself,
  so it must not be moved, and the nodes must not be reallocated while it is
  used.
*/
typedef struct mr_ref_idx_set_t {
  mr_ic_t ic;
  mr_ptrdes_t * ra; /* nodes of the document */
  mr_idx_t ref_idx; /* the reference index that mr_ref_idx_set_find looks for */
  uint64_t seed; /* random odd multiplier of the hash */
  bool warn_collisions; /* report nodes with the same reference index */
} mr_ref_idx_set_t;

extern mr_status_t mr_ref_idx_set_new (mr_ref_idx_set_t * set, mr_ra_ptrdes_t * ptrs, bool warn_collisions);
extern mr_status_t mr_ref_idx_set_add (mr_ref_idx_set_t * set, mr_idx_t node);
extern mr_idx_t mr_ref_idx_set_find (mr_ref_idx_set_t * set, mr_idx_t ref_idx);
extern void mr_ref_idx_set_free (mr_ref_idx_set_t * set);

extern void mr_free_loaded_pointers (mr_ra_ptrdes_t *);
extern mr_status_t mr_set_crossref (mr_ra_ptrdes_t *, mr_idx_t, mr_idx_t);
extern mr_status_t mr_load_integer (mr_idx_t, mr_ra_ptrdes_t *);
extern mr_status_t mr_load_func (mr_idx_t, mr_ra_ptrdes_t *);
extern void mr_free_ptrdes_values (mr_ra_ptrdes_t *);
extern mr_status_t mr_validate_load (mr_ra_ptrdes_t *);

#endif /* _MR_LOAD_H_ */
