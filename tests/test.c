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

#include <reslib.h>
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
  RL_FREE_RECURSIVELY (sample_t, sample_);
}

#if 1
rl_conf_t x = {
  .rl_mem = {
    .mem_alloc_strategy = 2.0000000000,
  },
  .log_level = RL_LL_ALL,
  .hash = {
    .ra = { .size = 2488, .alloc_size = -1, .data = (rl_td_ptr_t[]){
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 11387 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 15535 */ NULL,
      },
      {
        .tdp = /* ref = 9464 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 4109 */ NULL,
      },
      {
        .tdp = /* ref = 9120 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 6855 */ NULL,
      },
      {
        .tdp = /* ref = 7541 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 11324 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 11553 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 7861 */ NULL,
      },
      {
        .tdp = /* ref = 7806 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 9257 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 11780 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 15622 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 7995 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 11612 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 5703 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 5598 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 10695 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 5293 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 9506 */ NULL,
      },
      {
        .tdp = /* ref = 6034 */ NULL,
      },
      {
        .tdp = /* ref = 6392 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 2157 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 9079 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 10010 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 8748 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 11693 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 5147 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 11490 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 10088 */ NULL,
      },
      {
        .tdp = /* ref = 7030 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 7617 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 2195 */ NULL,
      },
      {
        .tdp = /* ref = 7423 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 5463 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 5996 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 9375 */ NULL,
      },
      {
        .tdp = /* ref = 7461 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 8644 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 5354 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 9192 */ NULL,
      },
      {
        .tdp = /* ref = 5522 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 636 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 4998 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 7672 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 8699 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 6893 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 2416 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 8165 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = /* ref = 6354 */ NULL,
      },
      {
        .tdp = NULL,
      },
      {
        .tdp = NULL,
      },
    }},
  },
  .des = { .size = 408, .alloc_size = -1, .data = (rl_td_ptr_t[]){
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 636 */ {
          .rl_type = RL_TYPE_UNION,
          .type = /* ref_idx = 638 */ "rl_ptr_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 2488, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 2015 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 2100 */ NULL,
            },
            {
              .fdp = /* ref = 1913 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1352 */ NULL,
            },
            {
              .fdp = /* ref = 1845 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1573 */ NULL,
            },
            {
              .fdp = /* ref = 1658 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1998 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 2049 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1726 */ NULL,
            },
            {
              .fdp = /* ref = 1709 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1879 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 2134 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 2117 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1743 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 2066 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1488 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1471 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1981 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1403 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1964 */ NULL,
            },
            {
              .fdp = /* ref = 1522 */ NULL,
            },
            {
              .fdp = /* ref = 1556 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1301 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1828 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1930 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1811 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 2083 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1386 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 2032 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1947 */ NULL,
            },
            {
              .fdp = /* ref = 1607 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1675 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1318 */ NULL,
            },
            {
              .fdp = /* ref = 1624 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1437 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1505 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1267 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1896 */ NULL,
            },
            {
              .fdp = /* ref = 1641 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1777 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1420 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1862 */ NULL,
            },
            {
              .fdp = /* ref = 1454 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1284 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1369 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1692 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1794 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1590 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1335 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1760 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 1539 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 14242155953080141350ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 8,
          .fields = { .size = 5824, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 1267 */ {
              .type = "void",
              .name = "ptr",
              .hash_value = 9579906657643324566ULL,
              .size = 1,
              .offset = 0,
              .rl_type = RL_TYPE_VOID,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "default void pointer",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1284 */ {
              .type = /* ref_content = 638 */ NULL,
              .name = /* ref_content = 638 */ NULL,
              .hash_value = 14242155953080141350ULL,
              .size = 8,
              .offset = 0,
              .rl_type = RL_TYPE_UNION,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1301 */ {
              .type = /* ref_content = 16309 */ NULL,
              .name = /* ref_content = 16309 */ NULL,
              .hash_value = 2915236796245502698ULL,
              .size = 8,
              .offset = 0,
              .rl_type = RL_TYPE_CHAR_ARRAY,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1318 */ {
              .type = /* ref_content = 2197 */ NULL,
              .name = /* ref_content = 2197 */ NULL,
              .hash_value = 10039727766126216105ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1335 */ {
              .type = /* ref_content = 2418 */ NULL,
              .name = /* ref_content = 2418 */ NULL,
              .hash_value = 7137923791232533569ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1352 */ {
              .type = /* ref_content = 4111 */ NULL,
              .name = /* ref_content = 4111 */ NULL,
              .hash_value = 3994819707459131634ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1369 */ {
              .type = /* ref_content = 5000 */ NULL,
              .name = /* ref_content = 5000 */ NULL,
              .hash_value = 2396109789685783810ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1386 */ {
              .type = /* ref_content = 5149 */ NULL,
              .name = /* ref_content = 5149 */ NULL,
              .hash_value = 7029008206318397441ULL,
              .size = 32,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1403 */ {
              .type = /* ref_content = 5295 */ NULL,
              .name = /* ref_content = 5295 */ NULL,
              .hash_value = 7719247245401049831ULL,
              .size = 1,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1420 */ {
              .type = /* ref_content = 5356 */ NULL,
              .name = /* ref_content = 5356 */ NULL,
              .hash_value = 7260994937229123067ULL,
              .size = 32,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1437 */ {
              .type = /* ref_content = 5465 */ NULL,
              .name = /* ref_content = 5465 */ NULL,
              .hash_value = 13519412927678859711ULL,
              .size = 8,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1454 */ {
              .type = /* ref_content = 5524 */ NULL,
              .name = /* ref_content = 5524 */ NULL,
              .hash_value = 11993212420325884803ULL,
              .size = 40,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1471 */ {
              .type = /* ref_content = 5600 */ NULL,
              .name = /* ref_content = 5600 */ NULL,
              .hash_value = 572435035626151873ULL,
              .size = 40,
              .offset = 0,
              .rl_type = RL_TYPE_UNION,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1488 */ {
              .type = /* ref_content = 5705 */ NULL,
              .name = /* ref_content = 5705 */ NULL,
              .hash_value = 10483733335687766714ULL,
              .size = 112,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1505 */ {
              .type = /* ref_content = 5998 */ NULL,
              .name = /* ref_content = 5998 */ NULL,
              .hash_value = 10474170353971810799ULL,
              .size = 8,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1522 */ {
              .type = /* ref_content = 15588 */ NULL,
              .name = /* ref_content = 15588 */ NULL,
              .hash_value = 14712074649241264584ULL,
              .size = 136,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1539 */ {
              .type = /* ref_content = 6356 */ NULL,
              .name = /* ref_content = 6356 */ NULL,
              .hash_value = 17231367236236890109ULL,
              .size = 8,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1556 */ {
              .type = /* ref_content = 6394 */ NULL,
              .name = /* ref_content = 6394 */ NULL,
              .hash_value = 15060133741543493839ULL,
              .size = 40,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1573 */ {
              .type = /* ref_content = 6857 */ NULL,
              .name = /* ref_content = 6857 */ NULL,
              .hash_value = 9810431989290853996ULL,
              .size = 32,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1590 */ {
              .type = /* ref_content = 6895 */ NULL,
              .name = /* ref_content = 6895 */ NULL,
              .hash_value = 8797324492133635278ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1607 */ {
              .type = /* ref_content = 7032 */ NULL,
              .name = /* ref_content = 7032 */ NULL,
              .hash_value = 9288857365134653602ULL,
              .size = 192,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1624 */ {
              .type = /* ref_content = 7425 */ NULL,
              .name = /* ref_content = 7425 */ NULL,
              .hash_value = 1867922118188617489ULL,
              .size = 32,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1641 */ {
              .type = /* ref_content = 7463 */ NULL,
              .name = /* ref_content = 7463 */ NULL,
              .hash_value = 7091146785582921625ULL,
              .size = 24,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1658 */ {
              .type = /* ref_content = 7543 */ NULL,
              .name = /* ref_content = 7543 */ NULL,
              .hash_value = 5436509342830100602ULL,
              .size = 12,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1675 */ {
              .type = /* ref_content = 7619 */ NULL,
              .name = /* ref_content = 7619 */ NULL,
              .hash_value = 13699285310107456730ULL,
              .size = 24,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1692 */ {
              .type = /* ref_content = 7674 */ NULL,
              .name = /* ref_content = 7674 */ NULL,
              .hash_value = 7327166951249182128ULL,
              .size = 48,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1709 */ {
              .type = /* ref_content = 7808 */ NULL,
              .name = /* ref_content = 7808 */ NULL,
              .hash_value = 2891756773655109865ULL,
              .size = 64,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1726 */ {
              .type = /* ref_content = 7863 */ NULL,
              .name = /* ref_content = 7863 */ NULL,
              .hash_value = 7899483985752829880ULL,
              .size = 88,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1743 */ {
              .type = /* ref_content = 7997 */ NULL,
              .name = /* ref_content = 7997 */ NULL,
              .hash_value = 14284171889956797511ULL,
              .size = 16,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1760 */ {
              .type = /* ref_content = 8167 */ NULL,
              .name = /* ref_content = 8167 */ NULL,
              .hash_value = 16870479346664698198ULL,
              .size = 48,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1777 */ {
              .type = /* ref_content = 8646 */ NULL,
              .name = /* ref_content = 8646 */ NULL,
              .hash_value = 11906789587074292325ULL,
              .size = 64,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1794 */ {
              .type = /* ref_content = 8701 */ NULL,
              .name = /* ref_content = 8701 */ NULL,
              .hash_value = 6057791397001855945ULL,
              .size = 8,
              .offset = 0,
              .rl_type = RL_TYPE_FUNC_TYPE,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1811 */ {
              .type = /* ref_content = 8750 */ NULL,
              .name = /* ref_content = 8750 */ NULL,
              .hash_value = 15066973867535531574ULL,
              .size = 34944,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1828 */ {
              .type = /* ref_content = 9081 */ NULL,
              .name = /* ref_content = 9081 */ NULL,
              .hash_value = 3289538031139042843ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1845 */ {
              .type = /* ref_content = 13316 */ NULL,
              .name = /* ref_content = 13316 */ NULL,
              .hash_value = 4014832368315947173ULL,
              .size = 24,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1862 */ {
              .type = /* ref_content = 9194 */ NULL,
              .name = /* ref_content = 9194 */ NULL,
              .hash_value = 2647720504637857331ULL,
              .size = 1,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1879 */ {
              .type = /* ref_content = 9259 */ NULL,
              .name = /* ref_content = 9259 */ NULL,
              .hash_value = 4243970254013229759ULL,
              .size = 2,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1896 */ {
              .type = /* ref_content = 9377 */ NULL,
              .name = /* ref_content = 9377 */ NULL,
              .hash_value = 3675974750105805213ULL,
              .size = 20,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1913 */ {
              .type = /* ref_content = 9466 */ NULL,
              .name = /* ref_content = 9466 */ NULL,
              .hash_value = 11851814980081688881ULL,
              .size = 1,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1930 */ {
              .type = /* ref_content = 9995 */ NULL,
              .name = /* ref_content = 9995 */ NULL,
              .hash_value = 2272852052340759473ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_ANON_UNION,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1947 */ {
              .type = /* ref_content = 10073 */ NULL,
              .name = /* ref_content = 10073 */ NULL,
              .hash_value = 13570480650914066706ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_ANON_UNION,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1964 */ {
              .type = /* ref_content = 9508 */ NULL,
              .name = /* ref_content = 9508 */ NULL,
              .hash_value = 9481596970989224642ULL,
              .size = 176,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1981 */ {
              .type = /* ref_content = 10697 */ NULL,
              .name = /* ref_content = 10697 */ NULL,
              .hash_value = 10268092827064846727ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 1998 */ {
              .type = /* ref_content = 11326 */ NULL,
              .name = /* ref_content = 11326 */ NULL,
              .hash_value = 13407475883706848547ULL,
              .size = 64,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 2015 */ {
              .type = /* ref_content = 11389 */ NULL,
              .name = /* ref_content = 11389 */ NULL,
              .hash_value = 8860027222343181568ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 2032 */ {
              .type = /* ref_content = 11492 */ NULL,
              .name = /* ref_content = 11492 */ NULL,
              .hash_value = 10466231112475092992ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_UNION,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 2049 */ {
              .type = /* ref_content = 11555 */ NULL,
              .name = /* ref_content = 11555 */ NULL,
              .hash_value = 13583354161707468924ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_UNION,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 2066 */ {
              .type = /* ref_content = 11614 */ NULL,
              .name = /* ref_content = 11614 */ NULL,
              .hash_value = 17296971840426905799ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 2083 */ {
              .type = /* ref_content = 11695 */ NULL,
              .name = /* ref_content = 11695 */ NULL,
              .hash_value = 6874133897495585774ULL,
              .size = 8,
              .offset = 0,
              .rl_type = RL_TYPE_FUNC_TYPE,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 2100 */ {
              .type = /* ref_content = 15520 */ NULL,
              .name = /* ref_content = 15520 */ NULL,
              .hash_value = 6421365175777822323ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_ANON_UNION,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 2117 */ {
              .type = /* ref_content = 15607 */ NULL,
              .name = /* ref_content = 15607 */ NULL,
              .hash_value = 17718993774351129556ULL,
              .size = 8,
              .offset = 0,
              .rl_type = RL_TYPE_ANON_UNION,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 2134 */ {
              .type = /* ref_content = 11782 */ NULL,
              .name = /* ref_content = 11782 */ NULL,
              .hash_value = 5699203761841608469ULL,
              .size = 624,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "pointer on any type",
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 2157 */ {
          .rl_type = RL_TYPE_CHAR_ARRAY,
          .type = /* ref_content = 16309 */ NULL,
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 24, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = /* ref = 2172 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 2915236796245502698ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 8,
          .fields = { .size = 112, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 2172 */ {
              .type = "char",
              .name = NULL,
              .hash_value = 0ULL,
              .size = 1,
              .offset = 0,
              .rl_type = RL_TYPE_CHAR,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = /* ref_content = 11715 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "tricky way to declare type equivalent to char *",
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 2195 */ {
          .rl_type = RL_TYPE_ENUM,
          .type = /* ref_idx = 2197 */ "rl_log_level_t",
          .lookup_by_value = (rl_red_black_tree_node_t[]){
            {
              .key = /* ref = 2335 */ NULL,
              .left = (rl_red_black_tree_node_t[]){
                {
                  .key = /* ref = 2305 */ NULL,
                  .left = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 2290 */ NULL,
                      .left = NULL,
                      .right = NULL,
                      .red = RL_BLACK,
                    },
                  },
                  .right = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 2320 */ NULL,
                      .left = NULL,
                      .right = NULL,
                      .red = RL_BLACK,
                    },
                  },
                  .red = RL_RED,
                },
              },
              .right = (rl_red_black_tree_node_t[]){
                {
                  .key = /* ref = 2365 */ NULL,
                  .left = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 2350 */ NULL,
                      .left = NULL,
                      .right = NULL,
                      .red = RL_BLACK,
                    },
                  },
                  .right = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 2380 */ NULL,
                      .left = NULL,
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 2395 */ NULL,
                          .left = NULL,
                          .right = NULL,
                          .red = RL_RED,
                        },
                      },
                      .red = RL_BLACK,
                    },
                  },
                  .red = RL_RED,
                },
              },
              .red = RL_BLACK,
            },
          },
          .lookup_by_name = { .size = 184, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 2365 */ NULL,
            },
            {
              .fdp = /* ref = 2320 */ NULL,
            },
            {
              .fdp = /* ref = 2380 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 2290 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 2305 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 2335 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 2395 */ NULL,
            },
            {
              .fdp = /* ref = 2350 */ NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 10039727766126216105ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 4,
          .fields = { .size = 896, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 2290 */ {
              .type = "rl_log_level_t",
              .name = "RL_LL_ALL",
              .hash_value = 1896150841463107853ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 0,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 2305 */ {
              .type = "rl_log_level_t",
              .name = "RL_LL_TRACE",
              .hash_value = 843852082404576771ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 1,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 2320 */ {
              .type = "rl_log_level_t",
              .name = "RL_LL_DEBUG",
              .hash_value = 1240610847086562715ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 2,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 2335 */ {
              .type = "rl_log_level_t",
              .name = "RL_LL_INFO",
              .hash_value = 3626185169503994400ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 3,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 2350 */ {
              .type = "rl_log_level_t",
              .name = "RL_LL_WARN",
              .hash_value = 94028114581250860ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 4,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 2365 */ {
              .type = "rl_log_level_t",
              .name = "RL_LL_ERROR",
              .hash_value = 5039642306371590846ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 5,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 2380 */ {
              .type = "rl_log_level_t",
              .name = "RL_LL_FATAL",
              .hash_value = 1781767732375825980ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 6,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 2395 */ {
              .type = "rl_log_level_t",
              .name = "RL_LL_OFF",
              .hash_value = 9260955061462518799ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 7,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "Log levels enum",
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 2416 */ {
          .rl_type = RL_TYPE_ENUM,
          .type = /* ref_idx = 2418 */ "rl_message_id_t",
          .lookup_by_value = (rl_red_black_tree_node_t[]){
            {
              .key = /* ref = 3638 */ NULL,
              .left = (rl_red_black_tree_node_t[]){
                {
                  .key = /* ref = 3518 */ NULL,
                  .left = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 3458 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 3428 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3413 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3443 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .red = RL_BLACK,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 3488 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3473 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3503 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .red = RL_BLACK,
                        },
                      },
                      .red = RL_BLACK,
                    },
                  },
                  .right = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 3578 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 3548 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3533 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3563 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .red = RL_BLACK,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 3608 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3593 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3623 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .red = RL_BLACK,
                        },
                      },
                      .red = RL_BLACK,
                    },
                  },
                  .red = RL_BLACK,
                },
              },
              .right = (rl_red_black_tree_node_t[]){
                {
                  .key = /* ref = 3758 */ NULL,
                  .left = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 3698 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 3668 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3653 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3683 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .red = RL_BLACK,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 3728 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3713 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3743 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .red = RL_BLACK,
                        },
                      },
                      .red = RL_BLACK,
                    },
                  },
                  .right = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 3878 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 3818 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3788 */ NULL,
                              .left = (rl_red_black_tree_node_t[]){
                                {
                                  .key = /* ref = 3773 */ NULL,
                                  .left = NULL,
                                  .right = NULL,
                                  .red = RL_BLACK,
                                },
                              },
                              .right = (rl_red_black_tree_node_t[]){
                                {
                                  .key = /* ref = 3803 */ NULL,
                                  .left = NULL,
                                  .right = NULL,
                                  .red = RL_BLACK,
                                },
                              },
                              .red = RL_BLACK,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3848 */ NULL,
                              .left = (rl_red_black_tree_node_t[]){
                                {
                                  .key = /* ref = 3833 */ NULL,
                                  .left = NULL,
                                  .right = NULL,
                                  .red = RL_BLACK,
                                },
                              },
                              .right = (rl_red_black_tree_node_t[]){
                                {
                                  .key = /* ref = 3863 */ NULL,
                                  .left = NULL,
                                  .right = NULL,
                                  .red = RL_BLACK,
                                },
                              },
                              .red = RL_BLACK,
                            },
                          },
                          .red = RL_BLACK,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 3938 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3908 */ NULL,
                              .left = (rl_red_black_tree_node_t[]){
                                {
                                  .key = /* ref = 3893 */ NULL,
                                  .left = NULL,
                                  .right = NULL,
                                  .red = RL_BLACK,
                                },
                              },
                              .right = (rl_red_black_tree_node_t[]){
                                {
                                  .key = /* ref = 3923 */ NULL,
                                  .left = NULL,
                                  .right = NULL,
                                  .red = RL_BLACK,
                                },
                              },
                              .red = RL_BLACK,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3998 */ NULL,
                              .left = (rl_red_black_tree_node_t[]){
                                {
                                  .key = /* ref = 3968 */ NULL,
                                  .left = (rl_red_black_tree_node_t[]){
                                    {
                                      .key = /* ref = 3953 */ NULL,
                                      .left = NULL,
                                      .right = NULL,
                                      .red = RL_BLACK,
                                    },
                                  },
                                  .right = (rl_red_black_tree_node_t[]){
                                    {
                                      .key = /* ref = 3983 */ NULL,
                                      .left = NULL,
                                      .right = NULL,
                                      .red = RL_BLACK,
                                    },
                                  },
                                  .red = RL_BLACK,
                                },
                              },
                              .right = (rl_red_black_tree_node_t[]){
                                {
                                  .key = /* ref = 4028 */ NULL,
                                  .left = (rl_red_black_tree_node_t[]){
                                    {
                                      .key = /* ref = 4013 */ NULL,
                                      .left = NULL,
                                      .right = NULL,
                                      .red = RL_BLACK,
                                    },
                                  },
                                  .right = (rl_red_black_tree_node_t[]){
                                    {
                                      .key = /* ref = 4058 */ NULL,
                                      .left = (rl_red_black_tree_node_t[]){
                                        {
                                          .key = /* ref = 4043 */ NULL,
                                          .left = NULL,
                                          .right = NULL,
                                          .red = RL_BLACK,
                                        },
                                      },
                                      .right = (rl_red_black_tree_node_t[]){
                                        {
                                          .key = /* ref = 4073 */ NULL,
                                          .left = NULL,
                                          .right = (rl_red_black_tree_node_t[]){
                                            {
                                              .key = /* ref = 4088 */ NULL,
                                              .left = NULL,
                                              .right = NULL,
                                              .red = RL_RED,
                                            },
                                          },
                                          .red = RL_BLACK,
                                        },
                                      },
                                      .red = RL_RED,
                                    },
                                  },
                                  .red = RL_BLACK,
                                },
                              },
                              .red = RL_RED,
                            },
                          },
                          .red = RL_BLACK,
                        },
                      },
                      .red = RL_RED,
                    },
                  },
                  .red = RL_BLACK,
                },
              },
              .red = RL_BLACK,
            },
          },
          .lookup_by_name = { .size = 3032, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3953 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3968 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3773 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3923 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3458 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3803 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3473 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 4088 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3548 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3683 */ NULL,
            },
            {
              .fdp = /* ref = 3608 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3668 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3413 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 4058 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 4013 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3698 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3488 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 4073 */ NULL,
            },
            {
              .fdp = /* ref = 3788 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3893 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3818 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3533 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3443 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3743 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3428 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3833 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3938 */ NULL,
            },
            {
              .fdp = /* ref = 4028 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 4043 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3503 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3638 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3998 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3578 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3848 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3908 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3653 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3983 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3863 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3713 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3623 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3758 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3878 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3518 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3593 */ NULL,
            },
            {
              .fdp = /* ref = 3728 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 3563 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 7137923791232533569ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 4,
          .fields = { .size = 5152, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 3413 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_OUT_OF_MEMORY",
              .hash_value = 8482049838091757125ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 0,
              },
              .comment = "Out of memory.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3428 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_UNEXPECTED_END",
              .hash_value = 7328995559402797676ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 1,
              },
              .comment = "Unexpected end of file or end of string.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3443 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_UNEXPECTED_DATA",
              .hash_value = 4212155815339524335ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 2,
              },
              .comment = "Unexpected data in stream.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3458 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_UNBALANCED_TAGS",
              .hash_value = 16735522206745856348ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 3,
              },
              .comment = "Unbalanced tags.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3473 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_XDR_WRONG_ENCODING_MODE",
              .hash_value = 9799518308356626149ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 4,
              },
              .comment = "XDR wrong encoding mode.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3488 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_XDR_SAVE_FAILED",
              .hash_value = 9156761100990843649ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 5,
              },
              .comment = "XDR save to resizable array failed.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3503 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_XML_SAVE_FAILED",
              .hash_value = 12349086318915542276ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 6,
              },
              .comment = "XML save failed.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3518 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_LOAD_STRUCT_FAILED",
              .hash_value = 17240434204860829257ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 7,
              },
              .comment = "Structure load failed.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3533 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_LOAD_METHOD_MISSED",
              .hash_value = 7243975122098686693ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 8,
              },
              .comment = "Load method missed.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3548 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_XDR_LOAD_FAILED",
              .hash_value = 4149033888893123634ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 9,
              },
              .comment = "XDR load failed.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3563 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_NO_TYPE_DESCRIPTOR",
              .hash_value = 14804449997260177213ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 10,
              },
              .comment = "Can't find type descriptor for '%s'.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3578 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_NOT_A_UNION",
              .hash_value = 3494160842471716922ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 11,
              },
              .comment = "Type '%s' is not a union.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3593 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_STRING_IS_NULL",
              .hash_value = 3853654672481514061ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 12,
              },
              .comment = "Input string is NULL.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3608 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_NULL_POINTER",
              .hash_value = 12382403945225057340ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 13,
              },
              .comment = "Can't save/load to/from NULL pointer.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3623 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_IDS_COLLISION",
              .hash_value = 13958332417482615724ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 14,
              },
              .comment = "Two XML nodes have the same %s = %d.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3638 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_UNDEFINED_REF_IDX",
              .hash_value = 2533520799913135667ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 15,
              },
              .comment = "Reference on a node with %s = %d could not be found.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3653 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_UNKNOWN_ENUM",
              .hash_value = 8256591892709022949ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 16,
              },
              .comment = "Unknown enum value '%s'.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3668 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_READ_INT",
              .hash_value = 10896598171566164135ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 17,
              },
              .comment = "Can't read integer value from string `%s`.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3683 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_READ_ENUM",
              .hash_value = 12150513298856606641ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 18,
              },
              .comment = "Can't read enum value from string `%s`.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3698 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_READ_BITMASK",
              .hash_value = 14055229827739389799ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 19,
              },
              .comment = "Can't read bitmask from string `%s`.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3713 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_READ_FLOAT",
              .hash_value = 5479126332854533874ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 20,
              },
              .comment = "Can't read float from string `%s`.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3728 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_READ_DOUBLE",
              .hash_value = 17022615982577988311ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 21,
              },
              .comment = "Can't read double from string `%s`.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3743 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_READ_LONG_DOUBLE",
              .hash_value = 2843880305331114502ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 22,
              },
              .comment = "Can't read long double from string `%s`.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3758 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_READ_CHAR",
              .hash_value = 7373134931649213690ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 23,
              },
              .comment = "Can't read char from string `%s`.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3773 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_STRING_TRUNCATED",
              .hash_value = 11984176187620149665ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 24,
              },
              .comment = "Char array range check: string truncated.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3788 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_UNKNOWN_SUBNODE",
              .hash_value = 9249705093921184512ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 25,
              },
              .comment = "Struct type '%s': unknown subnode '%s'.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3803 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_SAVE_IDX_RANGE_CHECK",
              .hash_value = 16485784637784093149ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 26,
              },
              .comment = "Save node index is out of range.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3818 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_NODE_NAME_MISSMATCH",
              .hash_value = 16402923306101953807ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 27,
              },
              .comment = "Node name mismatch: expected '%s' got '%s'.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3833 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_NODE_TYPE_MISSMATCH",
              .hash_value = 11127960306535430960ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 28,
              },
              .comment = "Node type mismatch: expected '%s' got '%s'.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3848 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_UNSUPPORTED_NODE_TYPE",
              .hash_value = 10838265581288111792ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 29,
              },
              .comment = "Unsupported node type: %s(%d),:%s(%d).",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3863 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_RANGE_CHECK",
              .hash_value = 8785518452664077451ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 30,
              },
              .comment = "Range check error in array '%s'.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3878 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_READ_REF",
              .hash_value = 537110752643596537ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 31,
              },
              .comment = "Can't read '%s' as index.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3893 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_READ_RARRAY_SIZE",
              .hash_value = 4744840962185769479ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 32,
              },
              .comment = "Can't read '%s' as resizable array size.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3908 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_TYPE_NOT_ENUM",
              .hash_value = 2691553400864623303ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 33,
              },
              .comment = "Type '%s' is not a enum.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3923 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_SAVE_ENUM",
              .hash_value = 10239305430677094916ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 34,
              },
              .comment = "Can't find enum name for value %ld type '%s'.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3938 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_SAVE_BITMASK",
              .hash_value = 10165836153921455066ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 35,
              },
              .comment = "Can't decompose on bitmask %llu.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3953 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_DUPLICATED_ENUMS",
              .hash_value = 2769445295568142375ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 36,
              },
              .comment = "Duplicated enum definition for `%s` in enum '%s'.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3968 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_DUPLICATED_FIELDS",
              .hash_value = 10586990249361293078ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 37,
              },
              .comment = "Duplicated field definition for `%s` in type '%s'.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3983 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_INT_OF_UNKNOWN_SIZE",
              .hash_value = 18233103522326662857ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 38,
              },
              .comment = "Failed to stringify integer of unknown size: %d.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 3998 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_PARSE_ERROR",
              .hash_value = 15709022951381200773ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 39,
              },
              .comment = "Parser error: '%s'. Position: %d:%d-%d:%d.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4013 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_ANON_UNION_TYPE_ERROR",
              .hash_value = 12042077693221351743ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 40,
              },
              .comment = "Can't create type for anonymous union due to type name collision: '%s'.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4028 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_UNKNOWN_XML_ESC",
              .hash_value = 14836653130187492603ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 41,
              },
              .comment = "Unknown XML escape sequence '%s'.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4043 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_WRONG_XML_ESC",
              .hash_value = 10481015071798445208ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 42,
              },
              .comment = "Wrong XML escape sequence '%s'.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4058 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_TYPES_HASHES_MATCHED",
              .hash_value = 13828759418192619014ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 43,
              },
              .comment = "For types '%s' and '%s' hashes matched. Last type is not available.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4073 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_RARRAY_LOAD_FAILED",
              .hash_value = 14478296367857543157ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 44,
              },
              .comment = "Failed to find field .data in C-style data.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4088 */ {
              .type = "rl_message_id_t",
              .name = "RL_MESSAGE_LAST",
              .hash_value = 12343167619312624917ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 45,
              },
              .comment = "Last message ID.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "Messages enum. Message string saved in .comment field of meta info",
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 4109 */ {
          .rl_type = RL_TYPE_ENUM,
          .type = /* ref_idx = 4111 */ "rl_type_t",
          .lookup_by_value = (rl_red_black_tree_node_t[]){
            {
              .key = /* ref = 4677 */ NULL,
              .left = (rl_red_black_tree_node_t[]){
                {
                  .key = /* ref = 4617 */ NULL,
                  .left = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 4587 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 4572 */ NULL,
                          .left = NULL,
                          .right = NULL,
                          .red = RL_BLACK,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 4602 */ NULL,
                          .left = NULL,
                          .right = NULL,
                          .red = RL_BLACK,
                        },
                      },
                      .red = RL_BLACK,
                    },
                  },
                  .right = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 4647 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 4632 */ NULL,
                          .left = NULL,
                          .right = NULL,
                          .red = RL_BLACK,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 4662 */ NULL,
                          .left = NULL,
                          .right = NULL,
                          .red = RL_BLACK,
                        },
                      },
                      .red = RL_BLACK,
                    },
                  },
                  .red = RL_BLACK,
                },
              },
              .right = (rl_red_black_tree_node_t[]){
                {
                  .key = /* ref = 4797 */ NULL,
                  .left = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 4737 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 4707 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 4692 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 4722 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .red = RL_BLACK,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 4767 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 4752 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 4782 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .red = RL_BLACK,
                        },
                      },
                      .red = RL_BLACK,
                    },
                  },
                  .right = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 4857 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 4827 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 4812 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 4842 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .red = RL_BLACK,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 4917 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 4887 */ NULL,
                              .left = (rl_red_black_tree_node_t[]){
                                {
                                  .key = /* ref = 4872 */ NULL,
                                  .left = NULL,
                                  .right = NULL,
                                  .red = RL_BLACK,
                                },
                              },
                              .right = (rl_red_black_tree_node_t[]){
                                {
                                  .key = /* ref = 4902 */ NULL,
                                  .left = NULL,
                                  .right = NULL,
                                  .red = RL_BLACK,
                                },
                              },
                              .red = RL_RED,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 4947 */ NULL,
                              .left = (rl_red_black_tree_node_t[]){
                                {
                                  .key = /* ref = 4932 */ NULL,
                                  .left = NULL,
                                  .right = NULL,
                                  .red = RL_BLACK,
                                },
                              },
                              .right = (rl_red_black_tree_node_t[]){
                                {
                                  .key = /* ref = 4962 */ NULL,
                                  .left = NULL,
                                  .right = (rl_red_black_tree_node_t[]){
                                    {
                                      .key = /* ref = 4977 */ NULL,
                                      .left = NULL,
                                      .right = NULL,
                                      .red = RL_RED,
                                    },
                                  },
                                  .red = RL_BLACK,
                                },
                              },
                              .red = RL_RED,
                            },
                          },
                          .red = RL_BLACK,
                        },
                      },
                      .red = RL_BLACK,
                    },
                  },
                  .red = RL_RED,
                },
              },
              .red = RL_BLACK,
            },
          },
          .lookup_by_name = { .size = 1256, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 4707 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 4887 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 4572 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 4842 */ NULL,
            },
            {
              .fdp = /* ref = 4902 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 4797 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 4932 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 4662 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 4782 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 4752 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 4827 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 4602 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 4917 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 4977 */ NULL,
            },
            {
              .fdp = /* ref = 4962 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 4812 */ NULL,
            },
            {
              .fdp = /* ref = 4692 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 4737 */ NULL,
            },
            {
              .fdp = /* ref = 4722 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 4872 */ NULL,
            },
            {
              .fdp = /* ref = 4632 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 4617 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 4647 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 4587 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 4947 */ NULL,
            },
            {
              .fdp = /* ref = 4857 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 4767 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 4677 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 3994819707459131634ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 4,
          .fields = { .size = 3136, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 4572 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_NONE",
              .hash_value = 2933804931442949742ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 0,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4587 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_STRING",
              .hash_value = 4676598876058151765ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 1,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4602 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_CHAR_ARRAY",
              .hash_value = 14386104040312687738ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 2,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4617 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_CHAR",
              .hash_value = 16285334709245950620ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 3,
              },
              .comment = "RL_TYPE_CHAR | RL_TYPE_STRING should be the same as RL_TYPE_CHAR for proper char* auto-detection",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4632 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_VOID",
              .hash_value = 12082443409800080400ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 4,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4647 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_INT8",
              .hash_value = 15027558264823727969ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 5,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4662 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_UINT8",
              .hash_value = 2052287231818910102ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 6,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4677 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_INT16",
              .hash_value = 9213459167364790192ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 7,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4692 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_UINT16",
              .hash_value = 3990702955468316549ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 8,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4707 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_INT32",
              .hash_value = 15700518619103084462ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 9,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4722 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_UINT32",
              .hash_value = 10477762407206610819ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 10,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4737 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_INT64",
              .hash_value = 6943370958392730803ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 11,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4752 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_UINT64",
              .hash_value = 1720614746496257160ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 12,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4767 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_FLOAT",
              .hash_value = 9208084924385763028ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 13,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4782 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_DOUBLE",
              .hash_value = 1133996463875224825ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 14,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4797 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_LONG_DOUBLE",
              .hash_value = 2461718287433070440ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 15,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4812 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_STRUCT",
              .hash_value = 11228790061406888899ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 16,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4827 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_ENUM",
              .hash_value = 2615969002743791955ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 17,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4842 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_FUNC_TYPE",
              .hash_value = 13673807370196629163ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 18,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4857 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_ENUM_VALUE",
              .hash_value = 733875388565491791ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 19,
              },
              .comment = /* ref_content = 5648 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4872 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_FUNC",
              .hash_value = 2761806987214984298ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 20,
              },
              .comment = /* ref_content = 5682 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4887 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_BITFIELD",
              .hash_value = 11787266559213920833ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 21,
              },
              .comment = /* ref_content = 5665 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4902 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_BITMASK",
              .hash_value = 11607033393711479753ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 22,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4917 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_UNION",
              .hash_value = 2605667790189848263ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 23,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4932 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_ANON_UNION",
              .hash_value = 1623175926977673362ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 24,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4947 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_END_ANON_UNION",
              .hash_value = 14650846495040406504ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 25,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4962 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_TRAILING_RECORD",
              .hash_value = 14253207407634959542ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 26,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 4977 */ {
              .type = "rl_type_t",
              .name = "RL_TYPE_LAST",
              .hash_value = 11614625279952890962ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 27,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "ResLib types",
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 4998 */ {
          .rl_type = RL_TYPE_ENUM,
          .type = /* ref_idx = 5000 */ "rl_type_ext_t",
          .lookup_by_value = (rl_red_black_tree_node_t[]){
            {
              .key = /* ref = 5081 */ NULL,
              .left = (rl_red_black_tree_node_t[]){
                {
                  .key = /* ref = 5066 */ NULL,
                  .left = NULL,
                  .right = NULL,
                  .red = RL_BLACK,
                },
              },
              .right = (rl_red_black_tree_node_t[]){
                {
                  .key = /* ref = 5111 */ NULL,
                  .left = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 5096 */ NULL,
                      .left = NULL,
                      .right = NULL,
                      .red = RL_RED,
                    },
                  },
                  .right = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 5126 */ NULL,
                      .left = NULL,
                      .right = NULL,
                      .red = RL_RED,
                    },
                  },
                  .red = RL_BLACK,
                },
              },
              .red = RL_BLACK,
            },
          },
          .lookup_by_name = { .size = 136, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = /* ref = 5066 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 5081 */ NULL,
            },
            {
              .fdp = /* ref = 5096 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 5111 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 5126 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 2396109789685783810ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 4,
          .fields = { .size = 560, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 5066 */ {
              .type = "rl_type_ext_t",
              .name = "RL_TYPE_EXT_NONE",
              .hash_value = 17678890354004018206ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 0,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5081 */ {
              .type = "rl_type_ext_t",
              .name = "RL_TYPE_EXT_ARRAY",
              .hash_value = 1923680109568032557ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 1,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5096 */ {
              .type = "rl_type_ext_t",
              .name = "RL_TYPE_EXT_RARRAY",
              .hash_value = 920487576262934719ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 2,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5111 */ {
              .type = "rl_type_ext_t",
              .name = "RL_TYPE_EXT_POINTER",
              .hash_value = 15700573640675791727ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 3,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5126 */ {
              .type = "rl_type_ext_t",
              .name = "RL_TYPE_EXT_LAST",
              .hash_value = 7912966628804407810ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 4,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "ResLib types extension",
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 5147 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_idx = 5149 */ "rl_rarray_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 184, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 5270 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 5253 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 5219 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 5202 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 5236 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 7029008206318397441ULL,
          .attr = /* ref_content = 9395 */ NULL,
          .size = 32,
          .fields = { .size = 560, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 5202 */ {
              .type = "void",
              .name = /* ref_content = 7147 */ NULL,
              .hash_value = 14190807189472276762ULL,
              .size = 1,
              .offset = 0,
              .rl_type = RL_TYPE_VOID,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "pointer on data array",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5219 */ {
              .type = "int32_t",
              .name = /* ref_content = 6265 */ NULL,
              .hash_value = 16483305478199314875ULL,
              .size = 4,
              .offset = 8,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "used space in bytes",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5236 */ {
              .type = "int32_t",
              .name = "alloc_size",
              .hash_value = 15113774998545711525ULL,
              .size = 4,
              .offset = 12,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "allocated space in bytes",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5253 */ {
              .type = "rl_ptr_t",
              .name = /* ref_content = 16205 */ NULL,
              .hash_value = 1991805435669457073ULL,
              .size = 8,
              .offset = 16,
              .rl_type = RL_TYPE_UNION,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 16216 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5270 */ {
              .type = "char *",
              .name = /* ref_content = 16216 */ NULL,
              .hash_value = 8773534286945746647ULL,
              .size = 8,
              .offset = 24,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_CHAR,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 7413 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "resizable array type",
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 5293 */ {
          .rl_type = RL_TYPE_ENUM,
          .type = /* ref_idx = 5295 */ "rl_red_black_t",
          .lookup_by_value = (rl_red_black_tree_node_t[]){
            {
              .key = /* ref = 5318 */ NULL,
              .left = NULL,
              .right = (rl_red_black_tree_node_t[]){
                {
                  .key = /* ref = 5333 */ NULL,
                  .left = NULL,
                  .right = NULL,
                  .red = RL_RED,
                },
              },
              .red = RL_BLACK,
            },
          },
          .lookup_by_name = { .size = 24, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 5318 */ NULL,
            },
            {
              .fdp = /* ref = 5333 */ NULL,
            },
          }},
          .hash_value = 7719247245401049831ULL,
          .attr = "__attribute__ ((packed, aligned (sizeof (unsigned char))))",
          .size = 1,
          .fields = { .size = 224, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 5318 */ {
              .type = "rl_red_black_t",
              .name = "RL_BLACK",
              .hash_value = 16514996913048944602ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 0,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5333 */ {
              .type = "rl_red_black_t",
              .name = "RL_RED",
              .hash_value = 17018067808740834008ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 1,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "Red/Black enum",
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 5354 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_idx = 5356 */ "rl_red_black_tree_node_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 104, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = /* ref = 5406 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 5440 */ NULL,
            },
            {
              .fdp = /* ref = 5423 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 5389 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 7260994937229123067ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 32,
          .fields = { .size = 448, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 5389 */ {
              .type = "void",
              .name = "key",
              .hash_value = 16390057921852437961ULL,
              .size = 1,
              .offset = 0,
              .rl_type = RL_TYPE_VOID,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 16063 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5406 */ {
              .type = "rl_red_black_tree_node_t",
              .name = "left",
              .hash_value = 13963671236797617803ULL,
              .size = 32,
              .offset = 8,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "left child",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5423 */ {
              .type = "rl_red_black_tree_node_t",
              .name = "right",
              .hash_value = 1470002992116559358ULL,
              .size = 32,
              .offset = 16,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "right child",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5440 */ {
              .type = "rl_red_black_t",
              .name = "red",
              .hash_value = 16029480301089786299ULL,
              .size = 1,
              .offset = 24,
              .rl_type = RL_TYPE_ENUM,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "red/black tree node",
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 5463 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_idx = 5465 */ "rl_array_param_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 40, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = /* ref = 5482 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 5499 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 13519412927678859711ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 8,
          .fields = { .size = 224, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 5482 */ {
              .type = "int",
              .name = "count",
              .hash_value = 9756913025002090345ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "array size",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5499 */ {
              .type = "int",
              .name = "row_count",
              .hash_value = 14197097130345295552ULL,
              .size = 4,
              .offset = 4,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "row size",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "array parameters",
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 5522 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_idx = 5524 */ "rl_bitfield_param_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 40, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = /* ref = 5575 */ NULL,
            },
            {
              .fdp = /* ref = 5558 */ NULL,
            },
            {
              .fdp = /* ref = 5541 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 11993212420325884803ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 40,
          .fields = { .size = 336, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 5541 */ {
              .type = "int",
              .name = "width",
              .hash_value = 5493078662958471552ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "bit-field width in bits",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5558 */ {
              .type = "int",
              .name = "shift",
              .hash_value = 18425316097591891806ULL,
              .size = 4,
              .offset = 4,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "bit-field shift in first byte",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5575 */ {
              .type = "uint8_t",
              .name = "bitfield",
              .hash_value = 11125291243449888195ULL,
              .size = 1,
              .offset = 8,
              .rl_type = RL_TYPE_UINT8,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_RARRAY,
              .param = {
                .array_param = {
                  .count = 32,
                  .row_count = 1,
                },
              },
              .comment = "zero-struct with flagged bit-fields",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = /* ref_content = 5676 */ NULL,
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 5598 */ {
          .rl_type = RL_TYPE_UNION,
          .type = /* ref_idx = 5600 */ "rl_param_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 88, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 5629 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 5680 */ NULL,
            },
            {
              .fdp = /* ref = 5646 */ NULL,
            },
            {
              .fdp = /* ref = 5663 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 572435035626151873ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 40,
          .fields = { .size = 448, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 5629 */ {
              .type = "rl_array_param_t",
              .name = "array_param",
              .hash_value = 605704667523314415ULL,
              .size = 8,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "array parameters - default for serialization",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5646 */ {
              .type = "int64_t",
              .name = /* ref_idx = 5648 */ "enum_value",
              .hash_value = 15833688092908594289ULL,
              .size = 8,
              .offset = 0,
              .rl_type = RL_TYPE_INT64,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "enum value",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5663 */ {
              .type = "rl_bitfield_param_t",
              .name = /* ref_idx = 5665 */ "bitfield_param",
              .hash_value = 7399649345190398931ULL,
              .size = 40,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_idx = 5676 */ "bit-field parameters",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5680 */ {
              .type = "rl_fd_t",
              .name = /* ref_idx = 5682 */ "func_param",
              .hash_value = 8755317417798125884ULL,
              .size = 112,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_RARRAY,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 1,
                },
              },
              .comment = "function arguments descriptors",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "optional parameters for different types",
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 5703 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_idx = 5705 */ "rl_fd_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 296, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 5837 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 5888 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 5786 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 5973 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 5820 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 5939 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 5803 */ NULL,
            },
            {
              .fdp = /* ref = 5956 */ NULL,
            },
            {
              .fdp = /* ref = 5854 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 5922 */ NULL,
            },
            {
              .fdp = /* ref = 5871 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 5905 */ NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 10483733335687766714ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 112,
          .fields = { .size = 1344, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 5786 */ {
              .type = "char *",
              .name = /* ref_content = 6180 */ NULL,
              .hash_value = 11711308902943563202ULL,
              .size = 8,
              .offset = 0,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_CHAR,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 6191 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5803 */ {
              .type = "char *",
              .name = "name",
              .hash_value = 15968983230813922017ULL,
              .size = 8,
              .offset = 8,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_CHAR,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "name of the field",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5820 */ {
              .type = "uint64_t",
              .name = /* ref_content = 6231 */ NULL,
              .hash_value = 2617208015517793568ULL,
              .size = 8,
              .offset = 16,
              .rl_type = RL_TYPE_UINT64,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "hash of the name",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5837 */ {
              .type = "int",
              .name = /* ref_content = 6265 */ NULL,
              .hash_value = 16483305478199314875ULL,
              .size = 4,
              .offset = 24,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "size of field",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5854 */ {
              .type = "int",
              .name = /* ref_content = 7596 */ NULL,
              .hash_value = 13919737092692143207ULL,
              .size = 4,
              .offset = 28,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "offset in structure",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5871 */ {
              .type = "rl_type_t",
              .name = /* ref_content = 6163 */ NULL,
              .hash_value = 6092410040724578847ULL,
              .size = 4,
              .offset = 32,
              .rl_type = RL_TYPE_ENUM,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 6174 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5888 */ {
              .type = "rl_type_t",
              .name = "rl_type_aux",
              .hash_value = 4615979996815055468ULL,
              .size = 4,
              .offset = 36,
              .rl_type = RL_TYPE_ENUM,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "ResLib type if field is a pointer on builtin types or bit-field",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5905 */ {
              .type = "rl_type_ext_t",
              .name = "rl_type_ext",
              .hash_value = 15715818202503479855ULL,
              .size = 4,
              .offset = 40,
              .rl_type = RL_TYPE_ENUM,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "ResLib type extension",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5922 */ {
              .type = "rl_param_t",
              .name = "param",
              .hash_value = 13651251626867812401ULL,
              .size = 40,
              .offset = 48,
              .rl_type = RL_TYPE_UNION,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 6163 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5939 */ {
              .type = "char *",
              .name = /* ref_content = 16199 */ NULL,
              .hash_value = 6560990630759044179ULL,
              .size = 8,
              .offset = 88,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_CHAR,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "field comments",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5956 */ {
              .type = "rl_ptr_t",
              .name = /* ref_content = 16205 */ NULL,
              .hash_value = 1991805435669457073ULL,
              .size = 8,
              .offset = 96,
              .rl_type = RL_TYPE_UNION,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 16216 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 5973 */ {
              .type = "char *",
              .name = /* ref_content = 16216 */ NULL,
              .hash_value = 8773534286945746647ULL,
              .size = 8,
              .offset = 104,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_CHAR,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 7413 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "ResLib field descriptor",
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 5996 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_idx = 5998 */ "rl_fd_ptr_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 24, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 6011 */ NULL,
            },
          }},
          .hash_value = 10474170353971810799ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 8,
          .fields = { .size = 112, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 6011 */ {
              .type = "rl_fd_t",
              .name = "fdp",
              .hash_value = 8596217961761203322ULL,
              .size = 112,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "pointer on field descriptor",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "rl_fd_t pointer wrapper",
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 6034 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_content = 15588 */ NULL,
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 472, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 6297 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 6212 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 6161 */ NULL,
            },
            {
              .fdp = /* ref = 6195 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 6178 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 6229 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 6331 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 6263 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 6280 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 6246 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 6314 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 14712074649241264584ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 136,
          .fields = { .size = 1232, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 6161 */ {
              .type = "rl_type_t",
              .name = /* ref_idx = 6163 */ "rl_type",
              .hash_value = 6092410040724578847ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_idx = 6174 */ "ResLib type",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 6178 */ {
              .type = "char *",
              .name = /* ref_idx = 6180 */ "type",
              .hash_value = 11711308902943563202ULL,
              .size = 8,
              .offset = 8,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_CHAR,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_idx = 6191 */ "stringified type name",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 6195 */ {
              .type = "rl_red_black_tree_node_t",
              .name = "lookup_by_value",
              .hash_value = 3397501239002488432ULL,
              .size = 32,
              .offset = 16,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "B-tree for enums values lookup",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 6212 */ {
              .type = "rl_fd_ptr_t",
              .name = "lookup_by_name",
              .hash_value = 12643811366992802868ULL,
              .size = 8,
              .offset = 24,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_RARRAY,
              .param = {
                .array_param = {
                  .count = 4,
                  .row_count = 1,
                },
              },
              .comment = "hash for lookup by field name",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 6229 */ {
              .type = "uint64_t",
              .name = /* ref_idx = 6231 */ "hash_value",
              .hash_value = 2617208015517793568ULL,
              .size = 8,
              .offset = 56,
              .rl_type = RL_TYPE_UINT64,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "type name hash value",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 6246 */ {
              .type = "char *",
              .name = "attr",
              .hash_value = 11597434149126835963ULL,
              .size = 8,
              .offset = 64,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_CHAR,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "stringified typedef attributes",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 6263 */ {
              .type = "int",
              .name = /* ref_idx = 6265 */ "size",
              .hash_value = 16483305478199314875ULL,
              .size = 4,
              .offset = 72,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "size of type",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 6280 */ {
              .type = "rl_fd_t",
              .name = "fields",
              .hash_value = 5344086084105438967ULL,
              .size = 112,
              .offset = 76,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_RARRAY,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 1,
                },
              },
              .comment = "fields or enums descriptors",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 6297 */ {
              .type = "char *",
              .name = /* ref_content = 16199 */ NULL,
              .hash_value = 6560990630759044179ULL,
              .size = 8,
              .offset = 112,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_CHAR,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "type comments",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 6314 */ {
              .type = "rl_ptr_t",
              .name = /* ref_content = 16205 */ NULL,
              .hash_value = 1991805435669457073ULL,
              .size = 8,
              .offset = 120,
              .rl_type = RL_TYPE_UNION,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 16216 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 6331 */ {
              .type = "char *",
              .name = /* ref_content = 16216 */ NULL,
              .hash_value = 8773534286945746647ULL,
              .size = 8,
              .offset = 128,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_CHAR,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 7413 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "ResLib type descriptor",
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 6354 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_idx = 6356 */ "rl_td_ptr_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 24, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 6369 */ NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 17231367236236890109ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 8,
          .fields = { .size = 112, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 6369 */ {
              .type = "rl_td_t",
              .name = "tdp",
              .hash_value = 742321678401540040ULL,
              .size = 136,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "pointer on type descriptor",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "rl_td_t pointer wrapper",
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 6392 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_idx = 6394 */ "rl_mem_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 56, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = /* ref = 6415 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 6532 */ NULL,
            },
            {
              .fdp = /* ref = 6749 */ NULL,
            },
            {
              .fdp = /* ref = 6649 */ NULL,
            },
            {
              .fdp = /* ref = 6432 */ NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 15060133741543493839ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 40,
          .fields = { .size = 560, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 6415 */ {
              .type = "float",
              .name = "mem_alloc_strategy",
              .hash_value = 17931455811001359963ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_FLOAT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "memory allocation strategy",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 6432 */ {
              .type = "void*",
              .name = "malloc",
              .hash_value = 17977260687194959736ULL,
              .size = 8,
              .offset = 8,
              .rl_type = RL_TYPE_FUNC,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .func_param = { .size = 560, .alloc_size = -1, .data = (rl_fd_t[]){
                  {
                    .type = "void",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 1,
                    .offset = 0,
                    .rl_type = RL_TYPE_VOID,
                    .rl_type_aux = RL_TYPE_VOID,
                    .rl_type_ext = RL_TYPE_EXT_POINTER,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 11715 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "char *",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 8,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRING,
                    .rl_type_aux = RL_TYPE_CHAR,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "char *",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 8,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRING,
                    .rl_type_aux = RL_TYPE_CHAR,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "int",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 4,
                    .offset = 0,
                    .rl_type = RL_TYPE_INT32,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "size_t",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 8,
                    .offset = 0,
                    .rl_type = RL_TYPE_UINT64,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                }},
              },
              .comment = "pointer on malloc() function",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 6532 */ {
              .type = "void*",
              .name = "realloc",
              .hash_value = 3374279725547937698ULL,
              .size = 8,
              .offset = 16,
              .rl_type = RL_TYPE_FUNC,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .func_param = { .size = 672, .alloc_size = -1, .data = (rl_fd_t[]){
                  {
                    .type = "void",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 1,
                    .offset = 0,
                    .rl_type = RL_TYPE_VOID,
                    .rl_type_aux = RL_TYPE_VOID,
                    .rl_type_ext = RL_TYPE_EXT_POINTER,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 11715 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "char *",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 8,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRING,
                    .rl_type_aux = RL_TYPE_CHAR,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "char *",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 8,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRING,
                    .rl_type_aux = RL_TYPE_CHAR,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "int",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 4,
                    .offset = 0,
                    .rl_type = RL_TYPE_INT32,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "void",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 1,
                    .offset = 0,
                    .rl_type = RL_TYPE_VOID,
                    .rl_type_aux = RL_TYPE_VOID,
                    .rl_type_ext = RL_TYPE_EXT_POINTER,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "size_t",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 8,
                    .offset = 0,
                    .rl_type = RL_TYPE_UINT64,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                }},
              },
              .comment = "pointer on realloc() function",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 6649 */ {
              .type = "char*",
              .name = "strdup",
              .hash_value = 11749497784942182178ULL,
              .size = 8,
              .offset = 24,
              .rl_type = RL_TYPE_FUNC,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .func_param = { .size = 560, .alloc_size = -1, .data = (rl_fd_t[]){
                  {
                    .type = "char*",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 8,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRING,
                    .rl_type_aux = RL_TYPE_CHAR,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 11715 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "char *",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 8,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRING,
                    .rl_type_aux = RL_TYPE_CHAR,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "char *",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 8,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRING,
                    .rl_type_aux = RL_TYPE_CHAR,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "int",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 4,
                    .offset = 0,
                    .rl_type = RL_TYPE_INT32,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "char*",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 8,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRING,
                    .rl_type_aux = RL_TYPE_CHAR,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                }},
              },
              .comment = "pointer on strdup() function",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 6749 */ {
              .type = "void",
              .name = "free",
              .hash_value = 5819777968959969346ULL,
              .size = 1,
              .offset = 32,
              .rl_type = RL_TYPE_FUNC,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .func_param = { .size = 560, .alloc_size = -1, .data = (rl_fd_t[]){
                  {
                    .type = "void",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 1,
                    .offset = 0,
                    .rl_type = RL_TYPE_VOID,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 11715 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "char *",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 8,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRING,
                    .rl_type_aux = RL_TYPE_CHAR,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "char *",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 8,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRING,
                    .rl_type_aux = RL_TYPE_CHAR,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "int",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 4,
                    .offset = 0,
                    .rl_type = RL_TYPE_INT32,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "void",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 1,
                    .offset = 0,
                    .rl_type = RL_TYPE_VOID,
                    .rl_type_aux = RL_TYPE_VOID,
                    .rl_type_ext = RL_TYPE_EXT_POINTER,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                }},
              },
              .comment = "pointer on free() function",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "ResLib memory operations",
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 6855 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_idx = 6857 */ "rl_ra_rl_td_ptr_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 24, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 6870 */ NULL,
            },
          }},
          .hash_value = 9810431989290853996ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 32,
          .fields = { .size = 112, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 6870 */ {
              .type = "rl_td_ptr_t",
              .name = /* ref_content = 7440 */ NULL,
              .hash_value = 4926783735716085875ULL,
              .size = 8,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_RARRAY,
              .param = {
                .array_param = {
                  .count = 4,
                  .row_count = 1,
                },
              },
              .comment = "non-collision hash table",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = /* ref_content = 13140 */ NULL,
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 6893 */ {
          .rl_type = RL_TYPE_ENUM,
          .type = /* ref_idx = 6895 */ "rl_ptrdes_flags_t",
          .lookup_by_value = (rl_red_black_tree_node_t[]){
            {
              .key = /* ref = 6964 */ NULL,
              .left = (rl_red_black_tree_node_t[]){
                {
                  .key = /* ref = 6949 */ NULL,
                  .left = NULL,
                  .right = NULL,
                  .red = RL_BLACK,
                },
              },
              .right = (rl_red_black_tree_node_t[]){
                {
                  .key = /* ref = 6994 */ NULL,
                  .left = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 6979 */ NULL,
                      .left = NULL,
                      .right = NULL,
                      .red = RL_RED,
                    },
                  },
                  .right = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 7009 */ NULL,
                      .left = NULL,
                      .right = NULL,
                      .red = RL_RED,
                    },
                  },
                  .red = RL_BLACK,
                },
              },
              .red = RL_BLACK,
            },
          },
          .lookup_by_name = { .size = 88, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 6979 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 6994 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 7009 */ NULL,
            },
            {
              .fdp = /* ref = 6964 */ NULL,
            },
            {
              .fdp = /* ref = 6949 */ NULL,
            },
          }},
          .hash_value = 8797324492133635278ULL,
          .attr = "__attribute__ ((packed, aligned (sizeof (int32_t))))",
          .size = 4,
          .fields = { .size = 560, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 6949 */ {
              .type = "rl_ptrdes_flags_t",
              .name = "RL_PDF_NONE",
              .hash_value = 9221200086642040870ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 0,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 6964 */ {
              .type = "rl_ptrdes_flags_t",
              .name = "RL_PDF_IS_NULL",
              .hash_value = 6014036373388444300ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 1,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 6979 */ {
              .type = "rl_ptrdes_flags_t",
              .name = "RL_PDF_IS_REFERENCED",
              .hash_value = 10558076412693465988ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 2,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 6994 */ {
              .type = "rl_ptrdes_flags_t",
              .name = "RL_PDF_CONTENT_REFERENCE",
              .hash_value = 5590349953383442847ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 4,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7009 */ {
              .type = "rl_ptrdes_flags_t",
              .name = "RL_PDF_RARRAY_SIZE",
              .hash_value = 4680749297360302625ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 8,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "bitmask values",
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 7030 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_idx = 7032 */ "rl_ptrdes_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 424, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 7298 */ NULL,
            },
            {
              .fdp = /* ref = 7162 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 7315 */ NULL,
            },
            {
              .fdp = /* ref = 7179 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 7145 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 7400 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 7281 */ NULL,
            },
            {
              .fdp = /* ref = 7213 */ NULL,
            },
            {
              .fdp = /* ref = 7196 */ NULL,
            },
            {
              .fdp = /* ref = 7366 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 7264 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 7349 */ NULL,
            },
            {
              .fdp = /* ref = 7230 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 7247 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 7332 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 7383 */ NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 9288857365134653602ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 192,
          .fields = { .size = 1792, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 7145 */ {
              .type = "void",
              .name = /* ref_idx = 7147 */ "data",
              .hash_value = 14190807189472276762ULL,
              .size = 1,
              .offset = 0,
              .rl_type = RL_TYPE_VOID,
              .rl_type_aux = RL_TYPE_VOID,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "pointer on binary data",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7162 */ {
              .type = "rl_fd_t",
              .name = "fd",
              .hash_value = 15687657730477379274ULL,
              .size = 112,
              .offset = 8,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "field descriptor",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7179 */ {
              .type = "int",
              .name = "level",
              .hash_value = 2757014581963577112ULL,
              .size = 4,
              .offset = 120,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "level from the root element",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7196 */ {
              .type = "int",
              .name = "idx",
              .hash_value = 14777994882308554149ULL,
              .size = 4,
              .offset = 124,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "public index",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7213 */ {
              .type = "int",
              .name = "ref_idx",
              .hash_value = 14087956278693882881ULL,
              .size = 4,
              .offset = 128,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "reference index (internal enumeration)",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7230 */ {
              .type = "int",
              .name = /* ref_content = 7923 */ NULL,
              .hash_value = 18037092245690240010ULL,
              .size = 4,
              .offset = 132,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "parent index",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7247 */ {
              .type = "int",
              .name = "first_child",
              .hash_value = 8717841750135131851ULL,
              .size = 4,
              .offset = 136,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "first child index",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7264 */ {
              .type = "int",
              .name = "last_child",
              .hash_value = 7225592420567089079ULL,
              .size = 4,
              .offset = 140,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "last child index",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7281 */ {
              .type = "int",
              .name = /* ref_content = 16086 */ NULL,
              .hash_value = 15176331950943812221ULL,
              .size = 4,
              .offset = 144,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "previous sibling index",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7298 */ {
              .type = "int",
              .name = /* ref_content = 16069 */ NULL,
              .hash_value = 956248190986344479ULL,
              .size = 4,
              .offset = 148,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "next sibling index",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7315 */ {
              .type = "rl_ptrdes_flags_t",
              .name = "flags",
              .hash_value = 9228631820639449677ULL,
              .size = 4,
              .offset = 152,
              .rl_type = RL_TYPE_BITMASK,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7332 */ {
              .type = "int32_t",
              .name = "rarray_size",
              .hash_value = 8717841521772970475ULL,
              .size = 4,
              .offset = 156,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "size of resizable array",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7349 */ {
              .type = "char *",
              .name = "union_field_name",
              .hash_value = 3846041642840733516ULL,
              .size = 8,
              .offset = 160,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_CHAR,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "field descriptor for unions",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7366 */ {
              .type = "char *",
              .name = "value",
              .hash_value = 214844141176482237ULL,
              .size = 8,
              .offset = 168,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_CHAR,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "stringified value",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7383 */ {
              .type = "rl_ptr_t",
              .name = /* ref_content = 16205 */ NULL,
              .hash_value = 1991805435669457073ULL,
              .size = 8,
              .offset = 176,
              .rl_type = RL_TYPE_UNION,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 16216 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7400 */ {
              .type = "char *",
              .name = /* ref_content = 16216 */ NULL,
              .hash_value = 8773534286945746647ULL,
              .size = 8,
              .offset = 184,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_CHAR,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_idx = 7413 */ "union discriminator",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "pointer descriptor type",
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 7423 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_idx = 7425 */ "rl_ra_rl_ptrdes_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 24, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 7438 */ NULL,
            },
          }},
          .hash_value = 1867922118188617489ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 32,
          .fields = { .size = 112, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 7438 */ {
              .type = "rl_ptrdes_t",
              .name = /* ref_idx = 7440 */ "ra",
              .hash_value = 4926783735716085875ULL,
              .size = 192,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_RARRAY,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 7796 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "rl_ptrdes_t resizable array",
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 7461 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_idx = 7463 */ "rl_save_type_data_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 56, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 7484 */ NULL,
            },
            {
              .fdp = /* ref = 7501 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 7518 */ NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 7091146785582921625ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 24,
          .fields = { .size = 336, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 7484 */ {
              .type = "char *",
              .name = "prefix",
              .hash_value = 11608723575658648014ULL,
              .size = 8,
              .offset = 0,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_CHAR,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_idx = 7497 */ "statically allocated string",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7501 */ {
              .type = "char *",
              .name = "content",
              .hash_value = 3874494049717291899ULL,
              .size = 8,
              .offset = 8,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_CHAR,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "dynamically formed string. Need to be freed.",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7518 */ {
              .type = "char *",
              .name = "suffix",
              .hash_value = 12114290965759838581ULL,
              .size = 8,
              .offset = 16,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_CHAR,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 7497 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "serialization of the node",
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 7541 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_idx = 7543 */ "rl_lloc_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 40, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = /* ref = 7577 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 7594 */ NULL,
            },
            {
              .fdp = /* ref = 7560 */ NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 5436509342830100602ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 12,
          .fields = { .size = 336, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 7560 */ {
              .type = "int",
              .name = "lineno",
              .hash_value = 18213422598021737573ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "parser location - line number",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7577 */ {
              .type = "int",
              .name = "column",
              .hash_value = 7065782845240653070ULL,
              .size = 4,
              .offset = 4,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "parser location - column number",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7594 */ {
              .type = "int",
              .name = /* ref_idx = 7596 */ "offset",
              .hash_value = 13919737092692143207ULL,
              .size = 4,
              .offset = 8,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "parser location - offset in string",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "parser location",
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 7617 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_idx = 7619 */ "rl_token_lloc_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 24, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 7649 */ NULL,
            },
            {
              .fdp = /* ref = 7632 */ NULL,
            },
          }},
          .hash_value = 13699285310107456730ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 24,
          .fields = { .size = 224, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 7632 */ {
              .type = "rl_lloc_t",
              .name = "start",
              .hash_value = 773323544036302190ULL,
              .size = 12,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "start of the token",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7649 */ {
              .type = "rl_lloc_t",
              .name = "end",
              .hash_value = 2022777746706796567ULL,
              .size = 12,
              .offset = 12,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "end of the token",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "token location",
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 7672 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_idx = 7674 */ "rl_load_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 136, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = /* ref = 7715 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 7749 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 7732 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 7766 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 7783 */ NULL,
            },
          }},
          .hash_value = 7327166951249182128ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 48,
          .fields = { .size = 560, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 7715 */ {
              .type = "rl_lloc_t",
              .name = "lloc",
              .hash_value = 3482799889551823690ULL,
              .size = 12,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "current location of parser",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7732 */ {
              .type = "char *",
              .name = /* ref_content = 9979 */ NULL,
              .hash_value = 6579304305944143033ULL,
              .size = 8,
              .offset = 16,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_CHAR,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "string to parse",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7749 */ {
              .type = "char *",
              .name = "buf",
              .hash_value = 18422730613418745341ULL,
              .size = 8,
              .offset = 24,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_CHAR,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "parser internal buffer",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7766 */ {
              .type = "int",
              .name = /* ref_content = 7923 */ NULL,
              .hash_value = 18037092245690240010ULL,
              .size = 4,
              .offset = 32,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 7934 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7783 */ {
              .type = "rl_ra_rl_ptrdes_t",
              .name = /* ref_content = 7906 */ NULL,
              .hash_value = 4879723586695586089ULL,
              .size = 32,
              .offset = 40,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_idx = 7796 */ "resizable array with rl_ptrdes_t",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "ResLib load parser data",
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 7806 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_idx = 7808 */ "rl_load_data_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 24, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = /* ref = 7838 */ NULL,
            },
            {
              .fdp = /* ref = 7821 */ NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 2891756773655109865ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 64,
          .fields = { .size = 224, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 7821 */ {
              .type = "rl_ra_rl_ptrdes_t",
              .name = /* ref_content = 7906 */ NULL,
              .hash_value = 4879723586695586089ULL,
              .size = 32,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "internal representation of a loaded tree",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7838 */ {
              .type = "int",
              .name = /* ref_content = 7974 */ NULL,
              .hash_value = 3498189851131014420ULL,
              .size = 4,
              .offset = 32,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_RARRAY,
              .param = {
                .array_param = {
                  .count = 8,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 7985 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = /* ref_content = 13140 */ NULL,
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 7861 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_idx = 7863 */ "rl_save_data_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 136, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 7938 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 7955 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 7972 */ NULL,
            },
            {
              .fdp = /* ref = 7921 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 7904 */ NULL,
            },
          }},
          .hash_value = 7899483985752829880ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 88,
          .fields = { .size = 560, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 7904 */ {
              .type = "rl_ra_rl_ptrdes_t",
              .name = /* ref_idx = 7906 */ "ptrs",
              .hash_value = 4879723586695586089ULL,
              .size = 32,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "internal representation of a saved tree",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7921 */ {
              .type = "int",
              .name = /* ref_idx = 7923 */ "parent",
              .hash_value = 18037092245690240010ULL,
              .size = 4,
              .offset = 32,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_idx = 7934 */ "index of current parent",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7938 */ {
              .type = "rl_red_black_tree_node_t",
              .name = "typed_ptrs_tree",
              .hash_value = 17758210358149786461ULL,
              .size = 32,
              .offset = 40,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "B-tree with typed nodes",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7955 */ {
              .type = "rl_red_black_tree_node_t",
              .name = "untyped_ptrs_tree",
              .hash_value = 2549409341173862368ULL,
              .size = 32,
              .offset = 48,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "B-tree with untyped nodes",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 7972 */ {
              .type = "int",
              .name = /* ref_idx = 7974 */ "rl_ra_idx",
              .hash_value = 3498189851131014420ULL,
              .size = 4,
              .offset = 56,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_RARRAY,
              .param = {
                .array_param = {
                  .count = 8,
                  .row_count = 1,
                },
              },
              .comment = /* ref_idx = 7985 */ "indexes of postponed nodes",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "save routines data and lookup structures",
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 7995 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_idx = 7997 */ "rl_load_io_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 24, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = /* ref = 8076 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 8010 */ NULL,
            },
          }},
          .hash_value = 14284171889956797511ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 16,
          .fields = { .size = 224, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 8010 */ {
              .type = "int",
              .name = /* ref_content = 8210 */ NULL,
              .hash_value = 17552555235055067870ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_FUNC,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .func_param = { .size = 336, .alloc_size = -1, .data = (rl_fd_t[]){
                  {
                    .type = "int",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 4,
                    .offset = 0,
                    .rl_type = RL_TYPE_INT32,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 11715 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "int",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 4,
                    .offset = 0,
                    .rl_type = RL_TYPE_INT32,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "rl_load_data_t",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 64,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRUCT,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_POINTER,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                }},
              },
              .comment = "handler for internal format parsing",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 8076 */ {
              .type = "int",
              .name = /* ref_content = 8259 */ NULL,
              .hash_value = 11334942743337298830ULL,
              .size = 4,
              .offset = 8,
              .rl_type = RL_TYPE_FUNC,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .func_param = { .size = 448, .alloc_size = -1, .data = (rl_fd_t[]){
                  {
                    .type = "int",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 4,
                    .offset = 0,
                    .rl_type = RL_TYPE_INT32,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 11715 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "XDR",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 0,
                    .offset = 0,
                    .rl_type = RL_TYPE_NONE,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_POINTER,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "int",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 4,
                    .offset = 0,
                    .rl_type = RL_TYPE_INT32,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "rl_ra_rl_ptrdes_t",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 32,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRUCT,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_POINTER,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                }},
              },
              .comment = "handler for XDR parsing",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = /* ref_content = 8672 */ NULL,
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 8165 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_idx = 8167 */ "rl_save_io_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 136, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = /* ref = 8572 */ NULL,
            },
            {
              .fdp = /* ref = 8340 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 8208 */ NULL,
            },
            {
              .fdp = /* ref = 8489 */ NULL,
            },
            {
              .fdp = /* ref = 8257 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 8406 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 16870479346664698198ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 48,
          .fields = { .size = 672, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 8208 */ {
              .type = "void",
              .name = /* ref_idx = 8210 */ "rl",
              .hash_value = 17552555235055067870ULL,
              .size = 1,
              .offset = 0,
              .rl_type = RL_TYPE_FUNC,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .func_param = { .size = 224, .alloc_size = -1, .data = (rl_fd_t[]){
                  {
                    .type = "void",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 1,
                    .offset = 0,
                    .rl_type = RL_TYPE_VOID,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 11715 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "rl_save_data_t",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 88,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRUCT,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_POINTER,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                }},
              },
              .comment = "handler for internal format",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 8257 */ {
              .type = "int",
              .name = /* ref_idx = 8259 */ "xdr",
              .hash_value = 11334942743337298830ULL,
              .size = 4,
              .offset = 8,
              .rl_type = RL_TYPE_FUNC,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .func_param = { .size = 448, .alloc_size = -1, .data = (rl_fd_t[]){
                  {
                    .type = "int",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 4,
                    .offset = 0,
                    .rl_type = RL_TYPE_INT32,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 11715 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "XDR",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 0,
                    .offset = 0,
                    .rl_type = RL_TYPE_NONE,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_POINTER,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "int",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 4,
                    .offset = 0,
                    .rl_type = RL_TYPE_INT32,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "rl_ra_rl_ptrdes_t",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 32,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRUCT,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_POINTER,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                }},
              },
              .comment = "handler for XDR",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 8340 */ {
              .type = "char *",
              .name = "xml",
              .hash_value = 8183419000255542545ULL,
              .size = 8,
              .offset = 16,
              .rl_type = RL_TYPE_FUNC,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .func_param = { .size = 336, .alloc_size = -1, .data = (rl_fd_t[]){
                  {
                    .type = "char *",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 8,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRING,
                    .rl_type_aux = RL_TYPE_CHAR,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 11715 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "int",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 4,
                    .offset = 0,
                    .rl_type = RL_TYPE_INT32,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "rl_ra_rl_ptrdes_t",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 32,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRUCT,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_POINTER,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                }},
              },
              .comment = "handler for XML",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 8406 */ {
              .type = "int",
              .name = "cinit",
              .hash_value = 10528583703661595511ULL,
              .size = 4,
              .offset = 24,
              .rl_type = RL_TYPE_FUNC,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .func_param = { .size = 448, .alloc_size = -1, .data = (rl_fd_t[]){
                  {
                    .type = "int",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 4,
                    .offset = 0,
                    .rl_type = RL_TYPE_INT32,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 11715 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "int",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 4,
                    .offset = 0,
                    .rl_type = RL_TYPE_INT32,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "rl_ra_rl_ptrdes_t",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 32,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRUCT,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_POINTER,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "rl_save_type_data_t",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 24,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRUCT,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_POINTER,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                }},
              },
              .comment = "handler for CINIT",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 8489 */ {
              .type = "int",
              .name = "json",
              .hash_value = 4989451604664205914ULL,
              .size = 4,
              .offset = 32,
              .rl_type = RL_TYPE_FUNC,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .func_param = { .size = 448, .alloc_size = -1, .data = (rl_fd_t[]){
                  {
                    .type = "int",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 4,
                    .offset = 0,
                    .rl_type = RL_TYPE_INT32,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 11715 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "int",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 4,
                    .offset = 0,
                    .rl_type = RL_TYPE_INT32,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "rl_ra_rl_ptrdes_t",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 32,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRUCT,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_POINTER,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "rl_save_type_data_t",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 24,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRUCT,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_POINTER,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                }},
              },
              .comment = "handler for JSON",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 8572 */ {
              .type = "char *",
              .name = "scm",
              .hash_value = 8132292419369330371ULL,
              .size = 8,
              .offset = 40,
              .rl_type = RL_TYPE_FUNC,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .func_param = { .size = 336, .alloc_size = -1, .data = (rl_fd_t[]){
                  {
                    .type = "char *",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 8,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRING,
                    .rl_type_aux = RL_TYPE_CHAR,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 11715 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "int",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 4,
                    .offset = 0,
                    .rl_type = RL_TYPE_INT32,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "rl_ra_rl_ptrdes_t",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 32,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRUCT,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_POINTER,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                }},
              },
              .comment = "handler for SCHEME",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = /* ref_content = 8689 */ NULL,
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 8644 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_idx = 8646 */ "rl_io_handler_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 24, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = /* ref = 8659 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 8676 */ NULL,
            },
          }},
          .hash_value = 11906789587074292325ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 64,
          .fields = { .size = 224, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 8659 */ {
              .type = "rl_load_io_t",
              .name = "load",
              .hash_value = 16315971700400293632ULL,
              .size = 16,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_idx = 8672 */ "load handlers",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 8676 */ {
              .type = "rl_save_io_t",
              .name = "save",
              .hash_value = 14626242956147045807ULL,
              .size = 48,
              .offset = 16,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_idx = 8689 */ "save handlers",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "input/ouput handlers",
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 8699 */ {
          .rl_type = RL_TYPE_FUNC_TYPE,
          .type = /* ref_idx = 8701 */ "rl_output_format_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 0, .alloc_size = -1, .data = NULL,          },
          .hash_value = 6057791397001855945ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 8,
          .fields = { .size = 224, .alloc_size = -1, .data = (rl_fd_t[]){
            {
              .type = "char*",
              .name = NULL,
              .hash_value = 0ULL,
              .size = 8,
              .offset = 0,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_CHAR,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = /* ref_content = 11715 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            {
              .type = "rl_ptrdes_t",
              .name = NULL,
              .hash_value = 0ULL,
              .size = 192,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "formater handler",
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 8748 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_idx = 8750 */ "rl_conf_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 184, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 8803 */ NULL,
            },
            {
              .fdp = /* ref = 9005 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 9039 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 8820 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 9022 */ NULL,
            },
            {
              .fdp = /* ref = 8971 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 8988 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 9056 */ NULL,
            },
            {
              .fdp = /* ref = 8837 */ NULL,
            },
          }},
          .hash_value = 15066973867535531574ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 34944,
          .fields = { .size = 1008, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 8803 */ {
              .type = "rl_mem_t",
              .name = "rl_mem",
              .hash_value = 15338099223607283900ULL,
              .size = 40,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "memory operations",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 8820 */ {
              .type = "rl_log_level_t",
              .name = "log_level",
              .hash_value = 7478390273985736601ULL,
              .size = 4,
              .offset = 40,
              .rl_type = RL_TYPE_ENUM,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 8837 */ {
              .type = "void",
              .name = "msg_handler",
              .hash_value = 4243879310078101380ULL,
              .size = 1,
              .offset = 48,
              .rl_type = RL_TYPE_FUNC,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .func_param = { .size = 784, .alloc_size = -1, .data = (rl_fd_t[]){
                  {
                    .type = "void",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 1,
                    .offset = 0,
                    .rl_type = RL_TYPE_VOID,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 11715 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "char*",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 8,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRING,
                    .rl_type_aux = RL_TYPE_CHAR,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "char*",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 8,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRING,
                    .rl_type_aux = RL_TYPE_CHAR,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "int",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 4,
                    .offset = 0,
                    .rl_type = RL_TYPE_INT32,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "rl_log_level_t",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 4,
                    .offset = 0,
                    .rl_type = RL_TYPE_ENUM,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "rl_message_id_t",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 4,
                    .offset = 0,
                    .rl_type = RL_TYPE_ENUM,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "va_list",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 24,
                    .offset = 0,
                    .rl_type = RL_TYPE_NONE,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                }},
              },
              .comment = "handler for error messages",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 8971 */ {
              .type = "rl_ra_rl_td_ptr_t",
              .name = "hash",
              .hash_value = 3400491683286421188ULL,
              .size = 32,
              .offset = 56,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "hash for type descriptors lookup",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 8988 */ {
              .type = "rl_td_ptr_t",
              .name = "des",
              .hash_value = 4206849572492594524ULL,
              .size = 8,
              .offset = 88,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_RARRAY,
              .param = {
                .array_param = {
                  .count = 4,
                  .row_count = 1,
                },
              },
              .comment = "types descriptors",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 9005 */ {
              .type = "rl_red_black_tree_node_t",
              .name = "enum_by_name",
              .hash_value = 2723351322354628783ULL,
              .size = 32,
              .offset = 120,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "B-tree with enums mapping",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 9022 */ {
              .type = "rl_output_format_t",
              .name = "output_format",
              .hash_value = 3647808662117094713ULL,
              .size = 8,
              .offset = 128,
              .rl_type = RL_TYPE_FUNC_TYPE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_ARRAY,
              .param = {
                .array_param = {
                  .count = 256,
                  .row_count = 1,
                },
              },
              .comment = "formaters",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 9039 */ {
              .type = "rl_io_handler_t",
              .name = "io_handlers",
              .hash_value = 183834207772096040ULL,
              .size = 64,
              .offset = 2176,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_ARRAY,
              .param = {
                .array_param = {
                  .count = 256,
                  .row_count = 1,
                },
              },
              .comment = /* ref_idx = 9052 */ "io handlers",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 9056 */ {
              .type = "rl_io_handler_t",
              .name = "io_ext_handlers",
              .hash_value = 417960812827973816ULL,
              .size = 64,
              .offset = 18560,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_ARRAY,
              .param = {
                .array_param = {
                  .count = 256,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 9052 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "ResLib configuration",
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 9079 */ {
          .rl_type = RL_TYPE_ENUM,
          .type = /* ref_idx = 9081 */ "meta_type_t",
          .lookup_by_value = (rl_red_black_tree_node_t[]){
            {
              .key = /* ref = 9099 */ NULL,
              .left = NULL,
              .right = NULL,
              .red = RL_RED,
            },
          },
          .lookup_by_name = { .size = 24, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 9099 */ NULL,
            },
          }},
          .hash_value = 3289538031139042843ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 4,
          .fields = { .size = 112, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 9099 */ {
              .type = "meta_type_t",
              .name = "SIMPLE",
              .hash_value = 1622489241385021898ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 0,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = /* ref_content = 13140 */ NULL,
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 9120 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_content = 13316 */ NULL,
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 24, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = /* ref = 9169 */ NULL,
            },
            {
              .fdp = /* ref = 9135 */ NULL,
            },
            {
              .fdp = /* ref = 9152 */ NULL,
            },
          }},
          .hash_value = 4014832368315947173ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 24,
          .fields = { .size = 336, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 9135 */ {
              .type = "meta_type_t",
              .name = "meta_type",
              .hash_value = 14104073037921463048ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 9152 */ {
              .type = "char *",
              .name = "info",
              .hash_value = 2269677728008001996ULL,
              .size = 8,
              .offset = 8,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 9169 */ {
              .type = "char *",
              .name = "format",
              .hash_value = 5282192654278043913ULL,
              .size = 8,
              .offset = 16,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = /* ref_content = 13140 */ NULL,
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 9192 */ {
          .rl_type = RL_TYPE_ENUM,
          .type = /* ref_idx = 9194 */ "boolean_t",
          .lookup_by_value = (rl_red_black_tree_node_t[]){
            {
              .key = /* ref = 9221 */ NULL,
              .left = NULL,
              .right = (rl_red_black_tree_node_t[]){
                {
                  .key = /* ref = 9236 */ NULL,
                  .left = NULL,
                  .right = NULL,
                  .red = RL_RED,
                },
              },
              .red = RL_BLACK,
            },
          },
          .lookup_by_name = { .size = 40, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 9221 */ NULL,
            },
            {
              .fdp = /* ref = 9236 */ NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 2647720504637857331ULL,
          .attr = "__attribute__ ((packed,aligned(1)))",
          .size = 1,
          .fields = { .size = 224, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 9221 */ {
              .type = "boolean_t",
              .name = "FALSE",
              .hash_value = 8603604455186103947ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 0,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 9236 */ {
              .type = "boolean_t",
              .name = "TRUE",
              .hash_value = 16836302487955935968ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 1,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = /* ref_content = 13140 */ NULL,
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 9257 */ {
          .rl_type = RL_TYPE_ENUM,
          .type = /* ref_idx = 9259 */ "mask_t",
          .lookup_by_value = (rl_red_black_tree_node_t[]){
            {
              .key = /* ref = 9323 */ NULL,
              .left = (rl_red_black_tree_node_t[]){
                {
                  .key = /* ref = 9308 */ NULL,
                  .left = NULL,
                  .right = NULL,
                  .red = RL_BLACK,
                },
              },
              .right = (rl_red_black_tree_node_t[]){
                {
                  .key = /* ref = 9338 */ NULL,
                  .left = NULL,
                  .right = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 9353 */ NULL,
                      .left = NULL,
                      .right = NULL,
                      .red = RL_RED,
                    },
                  },
                  .red = RL_BLACK,
                },
              },
              .red = RL_RED,
            },
          },
          .lookup_by_name = { .size = 88, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = /* ref = 9353 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 9323 */ NULL,
            },
            {
              .fdp = /* ref = 9308 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 9338 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 4243970254013229759ULL,
          .attr = "__attribute__ ((packed,aligned(2)))",
          .size = 2,
          .fields = { .size = 448, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 9308 */ {
              .type = "mask_t",
              .name = "NONE",
              .hash_value = 14826750325628006384ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 0,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 9323 */ {
              .type = "mask_t",
              .name = "READ",
              .hash_value = 18322459845098251196ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 1,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 9338 */ {
              .type = "mask_t",
              .name = "WRITE",
              .hash_value = 10500505324831237867ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 2,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 9353 */ {
              .type = "mask_t",
              .name = "EXEC",
              .hash_value = 8934489515751720261ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 4,
              },
              .comment = "RL_ENUM_DEF_ARGN test",
              .ext = {
                (string_t[]){
                  "RL_ENUM_DEF_ARGN meta",
                },
              },
              .ptr_type = /* ref_content = 16309 */ NULL,
            },
          }},
          .comment = /* ref_content = 13140 */ NULL,
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 9375 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_idx = 9377 */ "point_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 56, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 9398 */ NULL,
            },
            {
              .fdp = /* ref = 9419 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 9437 */ NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 3675974750105805213ULL,
          .attr = /* ref_idx = 9395 */ "__attribute__((packed))",
          .size = 20,
          .fields = { .size = 336, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 9398 */ {
              .type = "float",
              .name = /* ref_content = 10113 */ NULL,
              .hash_value = 8608480567731124088ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_FLOAT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13309 */ NULL,
              .ext = {
                (meta_info_t[]){
                  {
                    .meta_type = SIMPLE,
                    .info = "comment field",
                    .format = NULL,
                  },
                },
              },
              .ptr_type = /* ref_content = 13316 */ NULL,
            },
            /* ref_idx = 9419 */ {
              .type = "double",
              .name = /* ref_content = 10130 */ NULL,
              .hash_value = 9756277976761940633ULL,
              .size = 8,
              .offset = 4,
              .rl_type = RL_TYPE_DOUBLE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13309 */ NULL,
              .ext = {
                (string_t[]){
                  "ext comment",
                },
              },
              .ptr_type = /* ref_content = 16309 */ NULL,
            },
            /* ref_idx = 9437 */ {
              .type = "sample_t",
              .name = "sample",
              .hash_value = 15113130414041442690ULL,
              .size = 624,
              .offset = 12,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = "Comment on struct",
          .ext = {
            (meta_info_t[]){
              {
                .meta_type = SIMPLE,
                .info = "comment struct",
                .format = NULL,
              },
            },
          },
          .ptr_type = /* ref_content = 13316 */ NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 9464 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_idx = 9466 */ "char_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 24, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = /* ref = 9479 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 11851814980081688881ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 1,
          .fields = { .size = 112, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 9479 */ {
              .type = "char",
              .name = "c",
              .hash_value = 2951479051793528259ULL,
              .size = 1,
              .offset = 0,
              .rl_type = RL_TYPE_CHAR,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = /* ref_content = 13140 */ NULL,
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = /* ref = 10010 */ NULL,
    },
    {
      .tdp = /* ref = 10088 */ NULL,
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 9506 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_idx = 9508 */ "empty_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 776, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 9960 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 9977 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 9994 */ NULL,
            },
            {
              .fdp = /* ref = 10478 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 10216 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 10672 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 10233 */ NULL,
            },
            {
              .fdp = /* ref = 10284 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 9926 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 10150 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 10267 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 10072 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 10167 */ NULL,
            },
            {
              .fdp = /* ref = 9943 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 9909 */ NULL,
            },
            {
              .fdp = /* ref = 10250 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 9709 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 9481596970989224642ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 176,
          .fields = { .size = 1904, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 9709 */ {
              .type = "int",
              .name = "_x_",
              .hash_value = 2336008623130224310ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_FUNC,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .func_param = { .size = 1232, .alloc_size = -1, .data = (rl_fd_t[]){
                  {
                    .type = "int",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 4,
                    .offset = 0,
                    .rl_type = RL_TYPE_INT32,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 11715 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "int",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 4,
                    .offset = 0,
                    .rl_type = RL_TYPE_INT32,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "char*",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 8,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRING,
                    .rl_type_aux = RL_TYPE_CHAR,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "char *",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 8,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRING,
                    .rl_type_aux = RL_TYPE_STRING,
                    .rl_type_ext = RL_TYPE_EXT_POINTER,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "int",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 4,
                    .offset = 0,
                    .rl_type = RL_TYPE_INT32,
                    .rl_type_aux = RL_TYPE_INT32,
                    .rl_type_ext = RL_TYPE_EXT_POINTER,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "void",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 1,
                    .offset = 0,
                    .rl_type = RL_TYPE_VOID,
                    .rl_type_aux = RL_TYPE_VOID,
                    .rl_type_ext = RL_TYPE_EXT_POINTER,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "void*",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 0,
                    .offset = 0,
                    .rl_type = RL_TYPE_NONE,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_POINTER,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "char*",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 8,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRING,
                    .rl_type_aux = RL_TYPE_CHAR,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "char*",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 8,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRING,
                    .rl_type_aux = RL_TYPE_STRING,
                    .rl_type_ext = RL_TYPE_EXT_POINTER,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "sample_t",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 624,
                    .offset = 0,
                    .rl_type = RL_TYPE_STRUCT,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_POINTER,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "int (*) (struct sample_t*)",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 8,
                    .offset = 0,
                    .rl_type = RL_TYPE_FUNC,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .func_param = { .size = 0, .alloc_size = -1, .data = NULL,                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                }},
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 9909 */ {
              .type = "str_t",
              .name = "p",
              .hash_value = 17872845369194143344ULL,
              .size = 16,
              .offset = 8,
              .rl_type = RL_TYPE_CHAR_ARRAY,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 9926 */ {
              .type = "int32_t",
              .name = /* ref_content = 13408 */ NULL,
              .hash_value = 5989520103281828746ULL,
              .size = 4,
              .offset = 24,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 9943 */ {
              .type = "unsigned long long int",
              .name = "_int_",
              .hash_value = 1994272790543604745ULL,
              .size = 8,
              .offset = 32,
              .rl_type = RL_TYPE_UINT64,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 9960 */ {
              .type = "long double",
              .name = /* ref_content = 15470 */ NULL,
              .hash_value = 15727647091175455598ULL,
              .size = 16,
              .offset = 48,
              .rl_type = RL_TYPE_LONG_DOUBLE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 9977 */ {
              .type = "string_t",
              .name = /* ref_idx = 9979 */ "str",
              .hash_value = 6579304305944143033ULL,
              .size = 8,
              .offset = 64,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_CHAR,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 9994 */ {
              .type = /* ref_idx = 9995 */ "rl_type_anonymous_union_0_t",
              .name = "anon_union_",
              .hash_value = 9912587899895408755ULL,
              .size = 4,
              .offset = 72,
              .rl_type = RL_TYPE_ANON_UNION,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = /* ref_idx = 10007 */ "xxx",
              .ext = {
                (rl_td_t[]){
                  /* ref_idx = 10010 */ {
                    .rl_type = RL_TYPE_ANON_UNION,
                    .type = /* ref_content = 9995 */ NULL,
                    .lookup_by_value = NULL,
                    .lookup_by_name = { .size = 56, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
                      {
                        .fdp = NULL,
                      },
                      {
                        .fdp = /* ref = 10033 */ NULL,
                      },
                      {
                        .fdp = /* ref = 10050 */ NULL,
                      },
                      {
                        .fdp = NULL,
                      },
                      {
                        .fdp = NULL,
                      },
                      {
                        .fdp = NULL,
                      },
                      {
                        .fdp = NULL,
                      },
                    }},
                    .hash_value = 2272852052340759473ULL,
                    .attr = /* ref_content = 11569 */ NULL,
                    .size = 4,
                    .fields = { .size = 224, .alloc_size = -1, .data = (rl_fd_t[]){
                      /* ref_idx = 10033 */ {
                        .type = "int32_t",
                        .name = /* ref_content = 10113 */ NULL,
                        .hash_value = 8608480567731124088ULL,
                        .size = 4,
                        .offset = 0,
                        .rl_type = RL_TYPE_INT32,
                        .rl_type_aux = RL_TYPE_NONE,
                        .rl_type_ext = RL_TYPE_EXT_NONE,
                        .param = {
                          .array_param = {
                            .count = 1,
                            .row_count = 1,
                          },
                        },
                        .comment = /* ref_content = 13140 */ NULL,
                        .ext = {
                          NULL,
                        },
                        .ptr_type = NULL,
                      },
                      /* ref_idx = 10050 */ {
                        .type = "float",
                        .name = /* ref_content = 10130 */ NULL,
                        .hash_value = 9756277976761940633ULL,
                        .size = 4,
                        .offset = 0,
                        .rl_type = RL_TYPE_FLOAT,
                        .rl_type_aux = RL_TYPE_NONE,
                        .rl_type_ext = RL_TYPE_EXT_NONE,
                        .param = {
                          .array_param = {
                            .count = 1,
                            .row_count = 1,
                          },
                        },
                        .comment = /* ref_content = 13140 */ NULL,
                        .ext = {
                          NULL,
                        },
                        .ptr_type = NULL,
                      },
                    }},
                    .comment = /* ref_content = 10007 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                },
              },
              .ptr_type = /* ref_content = 15588 */ NULL,
            },
            /* ref_idx = 10072 */ {
              .type = /* ref_idx = 10073 */ "rl_type_anonymous_union_1_t",
              .name = "huh",
              .hash_value = 14717120728082015365ULL,
              .size = 4,
              .offset = 72,
              .rl_type = RL_TYPE_ANON_UNION,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = /* ref_idx = 10085 */ "com1",
              .ext = {
                (rl_td_t[]){
                  /* ref_idx = 10088 */ {
                    .rl_type = RL_TYPE_ANON_UNION,
                    .type = /* ref_content = 10073 */ NULL,
                    .lookup_by_value = NULL,
                    .lookup_by_name = { .size = 56, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
                      {
                        .fdp = NULL,
                      },
                      {
                        .fdp = /* ref = 10111 */ NULL,
                      },
                      {
                        .fdp = /* ref = 10128 */ NULL,
                      },
                      {
                        .fdp = NULL,
                      },
                      {
                        .fdp = NULL,
                      },
                      {
                        .fdp = NULL,
                      },
                      {
                        .fdp = NULL,
                      },
                    }},
                    .hash_value = 13570480650914066706ULL,
                    .attr = /* ref_content = 13140 */ NULL,
                    .size = 4,
                    .fields = { .size = 224, .alloc_size = -1, .data = (rl_fd_t[]){
                      /* ref_idx = 10111 */ {
                        .type = "int32_t",
                        .name = /* ref_idx = 10113 */ "x",
                        .hash_value = 8608480567731124088ULL,
                        .size = 4,
                        .offset = 0,
                        .rl_type = RL_TYPE_INT32,
                        .rl_type_aux = RL_TYPE_NONE,
                        .rl_type_ext = RL_TYPE_EXT_NONE,
                        .param = {
                          .array_param = {
                            .count = 1,
                            .row_count = 1,
                          },
                        },
                        .comment = /* ref_content = 13140 */ NULL,
                        .ext = {
                          NULL,
                        },
                        .ptr_type = NULL,
                      },
                      /* ref_idx = 10128 */ {
                        .type = "float",
                        .name = /* ref_idx = 10130 */ "y",
                        .hash_value = 9756277976761940633ULL,
                        .size = 4,
                        .offset = 0,
                        .rl_type = RL_TYPE_FLOAT,
                        .rl_type_aux = RL_TYPE_NONE,
                        .rl_type_ext = RL_TYPE_EXT_NONE,
                        .param = {
                          .array_param = {
                            .count = 1,
                            .row_count = 1,
                          },
                        },
                        .comment = /* ref_content = 13140 */ NULL,
                        .ext = {
                          NULL,
                        },
                        .ptr_type = NULL,
                      },
                    }},
                    .comment = /* ref_content = 10085 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                },
              },
              .ptr_type = /* ref_content = 15588 */ NULL,
            },
            /* ref_idx = 10150 */ {
              .type = "double",
              .name = "z",
              .hash_value = 10904075385792757178ULL,
              .size = 8,
              .offset = 80,
              .rl_type = RL_TYPE_DOUBLE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_ARRAY,
              .param = {
                .array_param = {
                  .count = 2,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 10167 */ {
              .type = "void",
              .name = "e",
              .hash_value = 5247073869855161349ULL,
              .size = 1,
              .offset = 96,
              .rl_type = RL_TYPE_FUNC,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .func_param = { .size = 224, .alloc_size = -1, .data = (rl_fd_t[]){
                  {
                    .type = "void",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 1,
                    .offset = 0,
                    .rl_type = RL_TYPE_VOID,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 11715 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                  {
                    .type = "void",
                    .name = NULL,
                    .hash_value = 0ULL,
                    .size = 1,
                    .offset = 0,
                    .rl_type = RL_TYPE_VOID,
                    .rl_type_aux = RL_TYPE_NONE,
                    .rl_type_ext = RL_TYPE_EXT_NONE,
                    .param = {
                      .array_param = {
                        .count = 0,
                        .row_count = 0,
                      },
                    },
                    .comment = /* ref_content = 13140 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                }},
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 10216 */ {
              .type = "long double",
              .name = "union_",
              .hash_value = 12541914222104491144ULL,
              .size = 16,
              .offset = 104,
              .rl_type = RL_TYPE_LONG_DOUBLE,
              .rl_type_aux = RL_TYPE_LONG_DOUBLE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 10233 */ {
              .type = "sample_t",
              .name = "forward",
              .hash_value = 18306250434741876373ULL,
              .size = 624,
              .offset = 112,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 10250 */ {
              .type = "int32_t",
              .name = "q",
              .hash_value = 573898704515408273ULL,
              .size = 4,
              .offset = 120,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_ARRAY,
              .param = {
                .array_param = {
                  .count = 3,
                  .row_count = 1,
                },
              },
              .comment = "com",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 10267 */ {
              .type = "long double",
              .name = "w",
              .hash_value = 7460683158700307543ULL,
              .size = 16,
              .offset = 136,
              .rl_type = RL_TYPE_LONG_DOUBLE,
              .rl_type_aux = RL_TYPE_LONG_DOUBLE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 16199 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 10284 */ {
              .type = "short",
              .name = "b1",
              .hash_value = 8780467989020821043ULL,
              .size = 2,
              .offset = 144,
              .rl_type = RL_TYPE_BITFIELD,
              .rl_type_aux = RL_TYPE_INT16,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .bitfield_param = {
                  .width = 12,
                  .shift = 0,
                  .bitfield = { .size = 176, .alloc_size = -1, .data = (uint8_t[]){
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    255,
                    15,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                  }},
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 10478 */ {
              .type = "long int",
              .name = "b2",
              .hash_value = 9928265398051637588ULL,
              .size = 8,
              .offset = 145,
              .rl_type = RL_TYPE_BITFIELD,
              .rl_type_aux = RL_TYPE_INT64,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .bitfield_param = {
                  .width = 24,
                  .shift = 4,
                  .bitfield = { .size = 176, .alloc_size = -1, .data = (uint8_t[]){
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    240,
                    255,
                    255,
                    15,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                  }},
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 10672 */ {
              .type = "_Decimal128",
              .name = "d128",
              .hash_value = 46668547445688799ULL,
              .size = 16,
              .offset = 160,
              .rl_type = RL_TYPE_NONE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = /* ref_content = 13140 */ NULL,
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 10695 */ {
          .rl_type = RL_TYPE_ENUM,
          .type = /* ref_idx = 10697 */ "rl_type_class_t",
          .lookup_by_value = (rl_red_black_tree_node_t[]){
            {
              .key = /* ref = 11153 */ NULL,
              .left = (rl_red_black_tree_node_t[]){
                {
                  .key = /* ref = 11093 */ NULL,
                  .left = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 11063 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 11048 */ NULL,
                          .left = NULL,
                          .right = NULL,
                          .red = RL_BLACK,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 11078 */ NULL,
                          .left = NULL,
                          .right = NULL,
                          .red = RL_BLACK,
                        },
                      },
                      .red = RL_BLACK,
                    },
                  },
                  .right = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 11123 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 11108 */ NULL,
                          .left = NULL,
                          .right = NULL,
                          .red = RL_BLACK,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 11138 */ NULL,
                          .left = NULL,
                          .right = NULL,
                          .red = RL_BLACK,
                        },
                      },
                      .red = RL_BLACK,
                    },
                  },
                  .red = RL_BLACK,
                },
              },
              .right = (rl_red_black_tree_node_t[]){
                {
                  .key = /* ref = 11213 */ NULL,
                  .left = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 11183 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 11168 */ NULL,
                          .left = NULL,
                          .right = NULL,
                          .red = RL_BLACK,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 11198 */ NULL,
                          .left = NULL,
                          .right = NULL,
                          .red = RL_BLACK,
                        },
                      },
                      .red = RL_BLACK,
                    },
                  },
                  .right = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 11243 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 11228 */ NULL,
                          .left = NULL,
                          .right = NULL,
                          .red = RL_BLACK,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 11273 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 11258 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 11288 */ NULL,
                              .left = NULL,
                              .right = (rl_red_black_tree_node_t[]){
                                {
                                  .key = /* ref = 11303 */ NULL,
                                  .left = NULL,
                                  .right = NULL,
                                  .red = RL_RED,
                                },
                              },
                              .red = RL_BLACK,
                            },
                          },
                          .red = RL_RED,
                        },
                      },
                      .red = RL_BLACK,
                    },
                  },
                  .red = RL_BLACK,
                },
              },
              .red = RL_RED,
            },
          },
          .lookup_by_name = { .size = 1016, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 11273 */ NULL,
            },
            {
              .fdp = /* ref = 11288 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 11303 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 11153 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 11198 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 11048 */ NULL,
            },
            {
              .fdp = /* ref = 11138 */ NULL,
            },
            {
              .fdp = /* ref = 11213 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 11123 */ NULL,
            },
            {
              .fdp = /* ref = 11228 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 11183 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 11108 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 11243 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 11168 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 11258 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 11093 */ NULL,
            },
            {
              .fdp = /* ref = 11078 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 11063 */ NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 10268092827064846727ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 4,
          .fields = { .size = 2016, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 11048 */ {
              .type = "rl_type_class_t",
              .name = "no_type_class",
              .hash_value = 15728845672731636083ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = -1,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 11063 */ {
              .type = "rl_type_class_t",
              .name = "void_type_class",
              .hash_value = 601663583595076136ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 0,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 11078 */ {
              .type = "rl_type_class_t",
              .name = "integer_type_class",
              .hash_value = 12621318300398315780ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 1,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 11093 */ {
              .type = "rl_type_class_t",
              .name = "char_type_class",
              .hash_value = 16332327780735812916ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 2,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 11108 */ {
              .type = "rl_type_class_t",
              .name = "enumeral_type_class",
              .hash_value = 12260491976724432687ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 3,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 11123 */ {
              .type = "rl_type_class_t",
              .name = "boolean_type_class",
              .hash_value = 14439121097703488118ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 4,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 11138 */ {
              .type = "rl_type_class_t",
              .name = "pointer_type_class",
              .hash_value = 880147102353759383ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 5,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 11153 */ {
              .type = "rl_type_class_t",
              .name = "reference_type_class",
              .hash_value = 7603802696655722053ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 6,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 11168 */ {
              .type = "rl_type_class_t",
              .name = "offset_type_class",
              .hash_value = 8631336491525502557ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 7,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 11183 */ {
              .type = "rl_type_class_t",
              .name = "real_type_class",
              .hash_value = 2628975624928806938ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 8,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 11198 */ {
              .type = "rl_type_class_t",
              .name = "complex_type_class",
              .hash_value = 415331404305575438ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 9,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 11213 */ {
              .type = "rl_type_class_t",
              .name = "function_type_class",
              .hash_value = 7040231030308591004ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 10,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 11228 */ {
              .type = "rl_type_class_t",
              .name = "method_type_class",
              .hash_value = 9957387851557333847ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 11,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 11243 */ {
              .type = "rl_type_class_t",
              .name = "record_type_class",
              .hash_value = 2126461002074430517ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 12,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 11258 */ {
              .type = "rl_type_class_t",
              .name = "union_type_class",
              .hash_value = 18252071987918682591ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 13,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 11273 */ {
              .type = "rl_type_class_t",
              .name = "array_type_class",
              .hash_value = 4652137184758062805ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 14,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 11288 */ {
              .type = "rl_type_class_t",
              .name = "string_type_class",
              .hash_value = 16033503701473202701ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 15,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 11303 */ {
              .type = "rl_type_class_t",
              .name = "lang_type_class",
              .hash_value = 13575909629082516600ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 16,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = /* ref_content = 13140 */ NULL,
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 11324 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_idx = 11326 */ "rarray_test_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 56, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 11347 */ NULL,
            },
            {
              .fdp = /* ref = 11364 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 13407475883706848547ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 64,
          .fields = { .size = 224, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 11347 */ {
              .type = "char",
              .name = "x1",
              .hash_value = 1513999439531124681ULL,
              .size = 1,
              .offset = 0,
              .rl_type = RL_TYPE_CHAR,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_RARRAY,
              .param = {
                .array_param = {
                  .count = 32,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 11364 */ {
              .type = "char",
              .name = "x2",
              .hash_value = 2661796848561941226ULL,
              .size = 1,
              .offset = 32,
              .rl_type = RL_TYPE_CHAR,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_RARRAY,
              .param = {
                .array_param = {
                  .count = 32,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = /* ref_content = 13140 */ NULL,
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 11387 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_idx = 11389 */ "ieee_float_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 88, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 11418 */ NULL,
            },
            {
              .fdp = /* ref = 11440 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 11462 */ NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 8860027222343181568ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 4,
          .fields = { .size = 336, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 11418 */ {
              .type = "unsigned int",
              .name = "mantissa",
              .hash_value = 18174536776068718016ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_BITFIELD,
              .rl_type_aux = RL_TYPE_UINT32,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .bitfield_param = {
                  .width = 23,
                  .shift = 0,
                  .bitfield = { .size = 4, .alloc_size = -1, .data = (uint8_t[]){
                    255,
                    255,
                    127,
                    0,
                  }},
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 11440 */ {
              .type = "int",
              .name = "exponent",
              .hash_value = 1433032357063652273ULL,
              .size = 4,
              .offset = 2,
              .rl_type = RL_TYPE_BITFIELD,
              .rl_type_aux = RL_TYPE_INT32,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .bitfield_param = {
                  .width = 8,
                  .shift = 7,
                  .bitfield = { .size = 4, .alloc_size = -1, .data = (uint8_t[]){
                    0,
                    0,
                    128,
                    127,
                  }},
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 11462 */ {
              .type = "unsigned int",
              .name = "negative",
              .hash_value = 535384111770234131ULL,
              .size = 4,
              .offset = 3,
              .rl_type = RL_TYPE_BITFIELD,
              .rl_type_aux = RL_TYPE_UINT32,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .bitfield_param = {
                  .width = 1,
                  .shift = 7,
                  .bitfield = { .size = 4, .alloc_size = -1, .data = (uint8_t[]){
                    0,
                    0,
                    0,
                    128,
                  }},
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = /* ref_content = 13140 */ NULL,
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 11490 */ {
          .rl_type = RL_TYPE_UNION,
          .type = /* ref_idx = 11492 */ "ieee754_float_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 56, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 11513 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 11530 */ NULL,
            },
          }},
          .hash_value = 10466231112475092992ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 4,
          .fields = { .size = 224, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 11513 */ {
              .type = "ieee_float_t",
              .name = "b",
              .hash_value = 1803681642762711714ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 11530 */ {
              .type = "float",
              .name = "f",
              .hash_value = 6394871278885977894ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_FLOAT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = /* ref_content = 13140 */ NULL,
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 11553 */ {
          .rl_type = RL_TYPE_UNION,
          .type = /* ref_idx = 11555 */ "union_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 40, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = /* ref = 11572 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 11589 */ NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 13583354161707468924ULL,
          .attr = /* ref_idx = 11569 */ "__attribute__((__transparent_union__))",
          .size = 4,
          .fields = { .size = 224, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 11572 */ {
              .type = "uint32_t",
              .name = /* ref_content = 11668 */ NULL,
              .hash_value = 6233045883771592845ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_UINT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 11589 */ {
              .type = "float",
              .name = /* ref_content = 15569 */ NULL,
              .hash_value = 13213587925970429118ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_FLOAT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = /* ref_content = 16199 */ NULL,
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 11612 */ {
          .rl_type = RL_TYPE_ENUM,
          .type = /* ref_idx = 11614 */ "union_enum_discriminator_t",
          .lookup_by_value = (rl_red_black_tree_node_t[]){
            {
              .key = /* ref = 11657 */ NULL,
              .left = (rl_red_black_tree_node_t[]){
                {
                  .key = /* ref = 11642 */ NULL,
                  .left = NULL,
                  .right = NULL,
                  .red = RL_RED,
                },
              },
              .right = (rl_red_black_tree_node_t[]){
                {
                  .key = /* ref = 11672 */ NULL,
                  .left = NULL,
                  .right = NULL,
                  .red = RL_RED,
                },
              },
              .red = RL_BLACK,
            },
          },
          .lookup_by_name = { .size = 24, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = /* ref = 11642 */ NULL,
            },
            {
              .fdp = /* ref = 11672 */ NULL,
            },
            {
              .fdp = /* ref = 11657 */ NULL,
            },
          }},
          .hash_value = 17296971840426905799ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 4,
          .fields = { .size = 336, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 11642 */ {
              .type = "union_enum_discriminator_t",
              .name = "UED_DEFAULT",
              .hash_value = 6516876578134947522ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 0,
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 11657 */ {
              .type = "union_enum_discriminator_t",
              .name = "UED_INT32",
              .hash_value = 13384791308680664333ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 1,
              },
              .comment = /* ref_idx = 11668 */ "union_uint32",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 11672 */ {
              .type = "union_enum_discriminator_t",
              .name = "UED_FLOAT",
              .hash_value = 6892357613963342899ULL,
              .size = 0,
              .offset = 0,
              .rl_type = RL_TYPE_ENUM_VALUE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .enum_value = 2,
              },
              .comment = /* ref_content = 15569 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = /* ref_content = 13140 */ NULL,
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 11693 */ {
          .rl_type = RL_TYPE_FUNC_TYPE,
          .type = /* ref_idx = 11695 */ "msg_handler_x_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 0, .alloc_size = -1, .data = NULL,          },
          .hash_value = 6874133897495585774ULL,
          .attr = /* ref_content = 13140 */ NULL,
          .size = 8,
          .fields = { .size = 448, .alloc_size = -1, .data = (rl_fd_t[]){
            {
              .type = "int",
              .name = NULL,
              .hash_value = 0ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = /* ref_idx = 11715 */ "return value",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            {
              .type = "int",
              .name = NULL,
              .hash_value = 0ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            {
              .type = "int",
              .name = NULL,
              .hash_value = 0ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            {
              .type = "char*",
              .name = NULL,
              .hash_value = 0ULL,
              .size = 8,
              .offset = 0,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_CHAR,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = /* ref_content = 16199 */ NULL,
          .ext = {
            NULL,
          },
          .ptr_type = NULL,
        },
      },
    },
    {
      .tdp = /* ref = 15535 */ NULL,
    },
    {
      .tdp = /* ref = 15622 */ NULL,
    },
    {
      .tdp = (rl_td_t[]){
        /* ref_idx = 11780 */ {
          .rl_type = RL_TYPE_STRUCT,
          .type = /* ref_idx = 11782 */ "sample_t",
          .lookup_by_value = NULL,
          .lookup_by_name = { .size = 5416, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
            {
              .fdp = /* ref = 15965 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15999 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15863 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 13245 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15795 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 13440 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15897 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 16254 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15434 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 13228 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15880 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 16084 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 13317 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 13457 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15693 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 13143 */ NULL,
            },
            {
              .fdp = /* ref = 15812 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15400 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 13194 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15502 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15727 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 13355 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15676 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 13423 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15761 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15417 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 16203 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15931 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15519 */ NULL,
            },
            {
              .fdp = /* ref = 16135 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15778 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 16152 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15846 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15606 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 16033 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15383 */ NULL,
            },
            {
              .fdp = /* ref = 16186 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 16288 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15829 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15451 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15948 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15982 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15485 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 13338 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 16169 */ NULL,
            },
            {
              .fdp = /* ref = 16050 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 14741 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15589 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15914 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 16237 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 16016 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 13406 */ NULL,
            },
            {
              .fdp = /* ref = 16101 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 16271 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 13160 */ NULL,
            },
            {
              .fdp = /* ref = 15744 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15710 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 13389 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 16118 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 13262 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 15468 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 13211 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 16220 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 13296 */ NULL,
            },
            {
              .fdp = /* ref = 16067 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 14099 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 13372 */ NULL,
            },
            {
              .fdp = /* ref = 13177 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = /* ref = 13279 */ NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
            {
              .fdp = NULL,
            },
          }},
          .hash_value = 5699203761841608469ULL,
          .attr = /* ref_idx = 13140 */ "",
          .size = 624,
          .fields = { .size = 7728, .alloc_size = -1, .data = (rl_fd_t[]){
            /* ref_idx = 13143 */ {
              .type = "point_t",
              .name = "ptr_fwd",
              .hash_value = 1692421036708828470ULL,
              .size = 20,
              .offset = 0,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 13160 */ {
              .type = "point_t",
              .name = "ptr_fwd_mismatch",
              .hash_value = 13943128777073916043ULL,
              .size = 20,
              .offset = 8,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 13177 */ {
              .type = "point_t",
              .name = "point",
              .hash_value = 5467508524432179722ULL,
              .size = 20,
              .offset = 16,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 13194 */ {
              .type = "point_t",
              .name = "ptr_bkw",
              .hash_value = 4074283255379509273ULL,
              .size = 20,
              .offset = 40,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 13211 */ {
              .type = "point_t",
              .name = "ptr_bkw_mismatch",
              .hash_value = 13977113550321435598ULL,
              .size = 20,
              .offset = 48,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 13228 */ {
              .type = "point_t",
              .name = "ptr_external",
              .hash_value = 16467392045250926104ULL,
              .size = 20,
              .offset = 56,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 13245 */ {
              .type = "double",
              .name = "type_renamed",
              .hash_value = 13342940756111628893ULL,
              .size = 8,
              .offset = 64,
              .rl_type = RL_TYPE_DOUBLE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 13262 */ {
              .type = "long double",
              .name = "typename_with_spaces",
              .hash_value = 17793889834290454428ULL,
              .size = 16,
              .offset = 72,
              .rl_type = RL_TYPE_LONG_DOUBLE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 13279 */ {
              .type = "void",
              .name = "pointer_on_pointer",
              .hash_value = 17110325265035710685ULL,
              .size = 1,
              .offset = 80,
              .rl_type = RL_TYPE_VOID,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 8,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 13296 */ {
              .type = "int8_t",
              .name = "_int8",
              .hash_value = 12570406059470414338ULL,
              .size = 1,
              .offset = 88,
              .rl_type = RL_TYPE_INT8,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_idx = 13309 */ "Comment",
              .ext = {
                (meta_info_t[]){
                  {
                    .meta_type = SIMPLE,
                    .info = NULL,
                    .format = /* ref_content = 13336 */ NULL,
                  },
                },
              },
              .ptr_type = /* ref_idx = 13316 */ "meta_info_t",
            },
            /* ref_idx = 13317 */ {
              .type = "uint8_t",
              .name = "_uint8",
              .hash_value = 2286594382740431735ULL,
              .size = 1,
              .offset = 89,
              .rl_type = RL_TYPE_UINT8,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13309 */ NULL,
              .ext = {
                (meta_info_t[]){
                  {
                    .meta_type = SIMPLE,
                    .info = NULL,
                    .format = /* ref_idx = 13336 */ "0x%02hhx ",
                  },
                },
              },
              .ptr_type = /* ref_content = 13316 */ NULL,
            },
            /* ref_idx = 13338 */ {
              .type = "int16_t",
              .name = "_int16",
              .hash_value = 16663221649596860785ULL,
              .size = 2,
              .offset = 90,
              .rl_type = RL_TYPE_INT16,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 13355 */ {
              .type = "uint16_t",
              .name = "_uint16",
              .hash_value = 11124485362417883270ULL,
              .size = 2,
              .offset = 92,
              .rl_type = RL_TYPE_UINT16,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 13372 */ {
              .type = "int32_t",
              .name = "_int32",
              .hash_value = 4703537027625603439ULL,
              .size = 4,
              .offset = 96,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 13389 */ {
              .type = "uint32_t",
              .name = "_uint32",
              .hash_value = 17611544814156177540ULL,
              .size = 4,
              .offset = 100,
              .rl_type = RL_TYPE_UINT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 13406 */ {
              .type = "int",
              .name = /* ref_idx = 13408 */ "_int",
              .hash_value = 5989520103281828746ULL,
              .size = 4,
              .offset = 104,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 13423 */ {
              .type = "int64_t",
              .name = "_int64",
              .hash_value = 14393133440624801396ULL,
              .size = 8,
              .offset = 112,
              .rl_type = RL_TYPE_INT64,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 13440 */ {
              .type = "uint64_t",
              .name = "_uint64",
              .hash_value = 8854397153445823881ULL,
              .size = 8,
              .offset = 120,
              .rl_type = RL_TYPE_UINT64,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 13457 */ {
              .type = "mask_t",
              .name = "b1",
              .hash_value = 8780467989020821043ULL,
              .size = 2,
              .offset = 128,
              .rl_type = RL_TYPE_BITFIELD,
              .rl_type_aux = RL_TYPE_ENUM,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .bitfield_param = {
                  .width = 3,
                  .shift = 0,
                  .bitfield = { .size = 624, .alloc_size = -1, .data = (uint8_t[]){
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    7,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                  }},
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 14099 */ {
              .type = "mask_t",
              .name = "b2",
              .hash_value = 9928265398051637588ULL,
              .size = 2,
              .offset = 128,
              .rl_type = RL_TYPE_BITFIELD,
              .rl_type_aux = RL_TYPE_ENUM,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .bitfield_param = {
                  .width = 3,
                  .shift = 3,
                  .bitfield = { .size = 624, .alloc_size = -1, .data = (uint8_t[]){
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    56,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                  }},
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 14741 */ {
              .type = "mask_t",
              .name = "b3",
              .hash_value = 11076062807082454133ULL,
              .size = 2,
              .offset = 128,
              .rl_type = RL_TYPE_BITFIELD,
              .rl_type_aux = RL_TYPE_ENUM,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .bitfield_param = {
                  .width = 3,
                  .shift = 6,
                  .bitfield = { .size = 624, .alloc_size = -1, .data = (uint8_t[]){
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    192,
                    1,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                  }},
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15383 */ {
              .type = "float",
              .name = "_float",
              .hash_value = 9278421016874310869ULL,
              .size = 4,
              .offset = 132,
              .rl_type = RL_TYPE_FLOAT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15400 */ {
              .type = "double",
              .name = "_double",
              .hash_value = 888352481081268794ULL,
              .size = 8,
              .offset = 136,
              .rl_type = RL_TYPE_DOUBLE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15417 */ {
              .type = "long double",
              .name = "ld",
              .hash_value = 12028916846642957392ULL,
              .size = 16,
              .offset = 144,
              .rl_type = RL_TYPE_LONG_DOUBLE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15434 */ {
              .type = "msg_handler_x_t",
              .name = "handler",
              .hash_value = 7576815047979530590ULL,
              .size = 8,
              .offset = 160,
              .rl_type = RL_TYPE_FUNC_TYPE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15451 */ {
              .type = "long double",
              .name = "_ld",
              .hash_value = 15392477435953150799ULL,
              .size = 16,
              .offset = 176,
              .rl_type = RL_TYPE_LONG_DOUBLE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_ARRAY,
              .param = {
                .array_param = {
                  .count = 2,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15468 */ {
              .type = "long_double_t",
              .name = /* ref_idx = 15470 */ "_ld_",
              .hash_value = 15727647091175455598ULL,
              .size = 16,
              .offset = 208,
              .rl_type = RL_TYPE_LONG_DOUBLE,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = "RL_AUTO for any type",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15485 */ {
              .type = "char",
              .name = "_char",
              .hash_value = 13664211445459663197ULL,
              .size = 1,
              .offset = 224,
              .rl_type = RL_TYPE_CHAR,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15502 */ {
              .type = "char",
              .name = /* ref_idx = 15504 */ "union_static_str_discriminator",
              .hash_value = 1204278905114584095ULL,
              .size = 1,
              .offset = 225,
              .rl_type = RL_TYPE_CHAR_ARRAY,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 16,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15519 */ {
              .type = /* ref_idx = 15520 */ "rl_type_anonymous_union_2_t",
              .name = "anon_union_138",
              .hash_value = 13700136655908639823ULL,
              .size = 4,
              .offset = 241,
              .rl_type = RL_TYPE_ANON_UNION,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = /* ref_content = 15504 */ NULL,
              .ext = {
                (rl_td_t[]){
                  /* ref_idx = 15535 */ {
                    .rl_type = RL_TYPE_ANON_UNION,
                    .type = /* ref_content = 15520 */ NULL,
                    .lookup_by_value = NULL,
                    .lookup_by_name = { .size = 24, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
                      {
                        .fdp = /* ref = 15567 */ NULL,
                      },
                      {
                        .fdp = /* ref = 15550 */ NULL,
                      },
                      {
                        .fdp = NULL,
                      },
                    }},
                    .hash_value = 6421365175777822323ULL,
                    .attr = /* ref_content = 15634 */ NULL,
                    .size = 4,
                    .fields = { .size = 224, .alloc_size = -1, .data = (rl_fd_t[]){
                      /* ref_idx = 15550 */ {
                        .type = "int32_t",
                        .name = "union_uint1",
                        .hash_value = 16633923502330385785ULL,
                        .size = 4,
                        .offset = 0,
                        .rl_type = RL_TYPE_INT32,
                        .rl_type_aux = RL_TYPE_NONE,
                        .rl_type_ext = RL_TYPE_EXT_ARRAY,
                        .param = {
                          .array_param = {
                            .count = 1,
                            .row_count = 1,
                          },
                        },
                        .comment = /* ref_content = 13140 */ NULL,
                        .ext = {
                          NULL,
                        },
                        .ptr_type = NULL,
                      },
                      /* ref_idx = 15567 */ {
                        .type = "float",
                        .name = /* ref_idx = 15569 */ "union_float",
                        .hash_value = 13213587925970429118ULL,
                        .size = 4,
                        .offset = 0,
                        .rl_type = RL_TYPE_FLOAT,
                        .rl_type_aux = RL_TYPE_NONE,
                        .rl_type_ext = RL_TYPE_EXT_NONE,
                        .param = {
                          .array_param = {
                            .count = 1,
                            .row_count = 1,
                          },
                        },
                        .comment = /* ref_content = 13140 */ NULL,
                        .ext = {
                          NULL,
                        },
                        .ptr_type = NULL,
                      },
                    }},
                    .comment = /* ref_content = 15504 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                },
              },
              .ptr_type = /* ref_idx = 15588 */ "rl_td_t",
            },
            /* ref_idx = 15589 */ {
              .type = "char *",
              .name = /* ref_idx = 15591 */ "union_str_discriminator",
              .hash_value = 14709546884934511128ULL,
              .size = 8,
              .offset = 248,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15606 */ {
              .type = /* ref_idx = 15607 */ "rl_type_anonymous_union_3_t",
              .name = "union_double_",
              .hash_value = 3314783824767948002ULL,
              .size = 8,
              .offset = 256,
              .rl_type = RL_TYPE_ANON_UNION,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = /* ref_content = 15591 */ NULL,
              .ext = {
                (rl_td_t[]){
                  /* ref_idx = 15622 */ {
                    .rl_type = RL_TYPE_ANON_UNION,
                    .type = /* ref_content = 15607 */ NULL,
                    .lookup_by_value = NULL,
                    .lookup_by_name = { .size = 24, .alloc_size = -1, .data = (rl_fd_ptr_t[]){
                      {
                        .fdp = /* ref = 15654 */ NULL,
                      },
                      {
                        .fdp = /* ref = 15637 */ NULL,
                      },
                      {
                        .fdp = NULL,
                      },
                    }},
                    .hash_value = 17718993774351129556ULL,
                    .attr = /* ref_idx = 15634 */ "__attribute__ ((packed))",
                    .size = 8,
                    .fields = { .size = 224, .alloc_size = -1, .data = (rl_fd_t[]){
                      /* ref_idx = 15637 */ {
                        .type = "int64_t",
                        .name = "union_uint2",
                        .hash_value = 17781720911361202330ULL,
                        .size = 8,
                        .offset = 0,
                        .rl_type = RL_TYPE_INT64,
                        .rl_type_aux = RL_TYPE_NONE,
                        .rl_type_ext = RL_TYPE_EXT_ARRAY,
                        .param = {
                          .array_param = {
                            .count = 1,
                            .row_count = 1,
                          },
                        },
                        .comment = /* ref_content = 13140 */ NULL,
                        .ext = {
                          NULL,
                        },
                        .ptr_type = NULL,
                      },
                      /* ref_idx = 15654 */ {
                        .type = "double",
                        .name = "union_double",
                        .hash_value = 7956597624406235715ULL,
                        .size = 8,
                        .offset = 0,
                        .rl_type = RL_TYPE_DOUBLE,
                        .rl_type_aux = RL_TYPE_NONE,
                        .rl_type_ext = RL_TYPE_EXT_NONE,
                        .param = {
                          .array_param = {
                            .count = 1,
                            .row_count = 1,
                          },
                        },
                        .comment = /* ref_content = 13140 */ NULL,
                        .ext = {
                          NULL,
                        },
                        .ptr_type = NULL,
                      },
                    }},
                    .comment = /* ref_content = 15591 */ NULL,
                    .ext = {
                      NULL,
                    },
                    .ptr_type = NULL,
                  },
                },
              },
              .ptr_type = /* ref_content = 15588 */ NULL,
            },
            /* ref_idx = 15676 */ {
              .type = "union_enum_discriminator_t",
              .name = /* ref_idx = 15678 */ "union_enum_discriminator",
              .hash_value = 10704986693400388276ULL,
              .size = 4,
              .offset = 264,
              .rl_type = RL_TYPE_ENUM,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15693 */ {
              .type = "union_t",
              .name = "_union_enum",
              .hash_value = 1487844585651313116ULL,
              .size = 4,
              .offset = 268,
              .rl_type = RL_TYPE_UNION,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 15678 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15710 */ {
              .type = "uint8_t",
              .name = /* ref_idx = 15712 */ "union_int_discriminator",
              .hash_value = 11690070187653009546ULL,
              .size = 1,
              .offset = 272,
              .rl_type = RL_TYPE_UINT8,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15727 */ {
              .type = "union_t",
              .name = "_union_int",
              .hash_value = 9872867282616831058ULL,
              .size = 4,
              .offset = 276,
              .rl_type = RL_TYPE_UNION,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 15712 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15744 */ {
              .type = "boolean_t",
              .name = "_bool",
              .hash_value = 16850218403796428619ULL,
              .size = 1,
              .offset = 280,
              .rl_type = RL_TYPE_ENUM,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15761 */ {
              .type = "mask_t",
              .name = "mask",
              .hash_value = 1765882924608851628ULL,
              .size = 2,
              .offset = 282,
              .rl_type = RL_TYPE_BITMASK,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15778 */ {
              .type = "char *",
              .name = "string",
              .hash_value = 9910220252161132055ULL,
              .size = 8,
              .offset = 288,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15795 */ {
              .type = "char *",
              .name = "string1",
              .hash_value = 12833787981114894152ULL,
              .size = 8,
              .offset = 296,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15812 */ {
              .type = "char *",
              .name = "string_empty",
              .hash_value = 8990314215915260645ULL,
              .size = 8,
              .offset = 304,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15829 */ {
              .type = "char *",
              .name = "string_zero",
              .hash_value = 16436688102736767798ULL,
              .size = 8,
              .offset = 312,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15846 */ {
              .type = "char *",
              .name = "string_full",
              .hash_value = 14811913468926664777ULL,
              .size = 8,
              .offset = 320,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15863 */ {
              .type = "char",
              .name = "char_array",
              .hash_value = 11039172670946238620ULL,
              .size = 1,
              .offset = 328,
              .rl_type = RL_TYPE_CHAR_ARRAY,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 16,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15880 */ {
              .type = "str_t",
              .name = "char_array_type",
              .hash_value = 6927301882303760541ULL,
              .size = 16,
              .offset = 344,
              .rl_type = RL_TYPE_CHAR_ARRAY,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15897 */ {
              .type = "char",
              .name = "array",
              .hash_value = 17606021492913633247ULL,
              .size = 1,
              .offset = 360,
              .rl_type = RL_TYPE_CHAR,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_ARRAY,
              .param = {
                .array_param = {
                  .count = 2,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15914 */ {
              .type = "char_t",
              .name = "array_",
              .hash_value = 5243337325051623934ULL,
              .size = 1,
              .offset = 362,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_ARRAY,
              .param = {
                .array_param = {
                  .count = 2,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15931 */ {
              .type = "int",
              .name = "array2d",
              .hash_value = 6024644460091514677ULL,
              .size = 4,
              .offset = 364,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_ARRAY,
              .param = {
                .array_param = {
                  .count = 6,
                  .row_count = 2,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15948 */ {
              .type = "int",
              .name = "xxx",
              .hash_value = 18323753633880492904ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_VOID,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15965 */ {
              .type = "char_t",
              .name = "array_none",
              .hash_value = 2215081321836923246ULL,
              .size = 1,
              .offset = 0,
              .rl_type = RL_TYPE_VOID,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15982 */ {
              .type = "char_t",
              .name = "rarray",
              .hash_value = 13943900487504955857ULL,
              .size = 1,
              .offset = 394,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_RARRAY,
              .param = {
                .array_param = {
                  .count = 32,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 15999 */ {
              .type = "char",
              .name = "rarray_1",
              .hash_value = 15915670746705645441ULL,
              .size = 1,
              .offset = 426,
              .rl_type = RL_TYPE_CHAR,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_RARRAY,
              .param = {
                .array_param = {
                  .count = 32,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 16016 */ {
              .type = "char",
              .name = "rarray_2",
              .hash_value = 17063468155736461986ULL,
              .size = 1,
              .offset = 458,
              .rl_type = RL_TYPE_CHAR,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_RARRAY,
              .param = {
                .array_param = {
                  .count = 32,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 16033 */ {
              .type = "char_t",
              .name = "rarray_empty",
              .hash_value = 5134239390527374879ULL,
              .size = 1,
              .offset = 490,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_RARRAY,
              .param = {
                .array_param = {
                  .count = 32,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 16050 */ {
              .type = "void",
              .name = "_void",
              .hash_value = 9461320146013792977ULL,
              .size = 1,
              .offset = 528,
              .rl_type = RL_TYPE_VOID,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 8,
                  .row_count = 1,
                },
              },
              .comment = /* ref_idx = 16063 */ "void pointer",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 16067 */ {
              .type = "sample_t",
              .name = /* ref_idx = 16069 */ "next",
              .hash_value = 956248190986344479ULL,
              .size = 624,
              .offset = 536,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 1,
                },
              },
              .comment = "linked list example",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 16084 */ {
              .type = "sample_t",
              .name = /* ref_idx = 16086 */ "prev",
              .hash_value = 15176331950943812221ULL,
              .size = 624,
              .offset = 544,
              .rl_type = RL_TYPE_STRUCT,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 16101 */ {
              .type = "int8_t",
              .name = "ptr_int8",
              .hash_value = 13750789230346198616ULL,
              .size = 1,
              .offset = 552,
              .rl_type = RL_TYPE_INT8,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 8,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 16118 */ {
              .type = "uint8_t",
              .name = "ptr_uint8",
              .hash_value = 3685594526846823565ULL,
              .size = 1,
              .offset = 560,
              .rl_type = RL_TYPE_UINT8,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 8,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 16135 */ {
              .type = "char",
              .name = "ptr_char",
              .hash_value = 14844594616335447475ULL,
              .size = 1,
              .offset = 568,
              .rl_type = RL_TYPE_CHAR,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 8,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 16152 */ {
              .type = "char",
              .name = "ptr_char_",
              .hash_value = 14823371235040243282ULL,
              .size = 1,
              .offset = 576,
              .rl_type = RL_TYPE_CHAR,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 8,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 16169 */ {
              .type = "char",
              .name = "ptr_null",
              .hash_value = 10894936984197790512ULL,
              .size = 1,
              .offset = 584,
              .rl_type = RL_TYPE_CHAR,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_POINTER,
              .param = {
                .array_param = {
                  .count = 8,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 16186 */ {
              .type = "int",
              .name = "arr",
              .hash_value = 13504669328277671781ULL,
              .size = 4,
              .offset = 592,
              .rl_type = RL_TYPE_INT32,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_ARRAY,
              .param = {
                .array_param = {
                  .count = 2,
                  .row_count = 1,
                },
              },
              .comment = /* ref_idx = 16199 */ "comment",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 16203 */ {
              .type = "rl_ptr_t",
              .name = /* ref_idx = 16205 */ "ext",
              .hash_value = 1991805435669457073ULL,
              .size = 8,
              .offset = 600,
              .rl_type = RL_TYPE_UNION,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_idx = 16216 */ "ptr_type",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 16220 */ {
              .type = "char *",
              .name = /* ref_content = 16216 */ NULL,
              .hash_value = 8773534286945746647ULL,
              .size = 8,
              .offset = 608,
              .rl_type = RL_TYPE_STRING,
              .rl_type_aux = RL_TYPE_CHAR,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 16237 */ {
              .type = "void*",
              .name = "ext_info",
              .hash_value = 2456408258523798748ULL,
              .size = 8,
              .offset = 0,
              .rl_type = RL_TYPE_VOID,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = "user extended info",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 16254 */ {
              .type = "int",
              .name = "res1",
              .hash_value = 12199634756763804987ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_VOID,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = "test of memory cleanup",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 16271 */ {
              .type = "int",
              .name = "res2",
              .hash_value = 13347432165794621532ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_VOID,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = /* ref_content = 13140 */ NULL,
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
            /* ref_idx = 16288 */ {
              .type = "int",
              .name = "res3",
              .hash_value = 14495229574825438077ULL,
              .size = 4,
              .offset = 0,
              .rl_type = RL_TYPE_VOID,
              .rl_type_aux = RL_TYPE_NONE,
              .rl_type_ext = RL_TYPE_EXT_NONE,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 0,
                },
              },
              .comment = "array with flexible number of elements",
              .ext = {
                NULL,
              },
              .ptr_type = NULL,
            },
          }},
          .comment = /* ref_content = 13309 */ NULL,
          .ext = {
            (string_t[]){
              "One more comment",
            },
          },
          .ptr_type = /* ref_idx = 16309 */ "string_t",
        },
      },
    },
  }},
  .enum_by_name = (rl_red_black_tree_node_t[]){
    {
      .key = /* ref = 3503 */ NULL,
      .left = (rl_red_black_tree_node_t[]){
        {
          .key = /* ref = 3668 */ NULL,
          .left = (rl_red_black_tree_node_t[]){
            {
              .key = /* ref = 3533 */ NULL,
              .left = (rl_red_black_tree_node_t[]){
                {
                  .key = /* ref = 2380 */ NULL,
                  .left = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 5318 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 9308 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 9221 */ NULL,
                              .left = (rl_red_black_tree_node_t[]){
                                {
                                  .key = /* ref = 9353 */ NULL,
                                  .left = NULL,
                                  .right = NULL,
                                  .red = RL_RED,
                                },
                              },
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 9323 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .red = RL_RED,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 2320 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 2290 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 2365 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .red = RL_RED,
                        },
                      },
                      .red = RL_BLACK,
                    },
                  },
                  .right = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 3953 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 2305 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 2335 */ NULL,
                              .left = NULL,
                              .right = (rl_red_black_tree_node_t[]){
                                {
                                  .key = /* ref = 2395 */ NULL,
                                  .left = NULL,
                                  .right = NULL,
                                  .red = RL_RED,
                                },
                              },
                              .red = RL_BLACK,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 2350 */ NULL,
                              .left = NULL,
                              .right = (rl_red_black_tree_node_t[]){
                                {
                                  .key = /* ref = 4013 */ NULL,
                                  .left = NULL,
                                  .right = NULL,
                                  .red = RL_RED,
                                },
                              },
                              .red = RL_BLACK,
                            },
                          },
                          .red = RL_RED,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 3623 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3968 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3983 */ NULL,
                              .left = NULL,
                              .right = (rl_red_black_tree_node_t[]){
                                {
                                  .key = /* ref = 4088 */ NULL,
                                  .left = NULL,
                                  .right = NULL,
                                  .red = RL_RED,
                                },
                              },
                              .red = RL_BLACK,
                            },
                          },
                          .red = RL_RED,
                        },
                      },
                      .red = RL_BLACK,
                    },
                  },
                  .red = RL_BLACK,
                },
              },
              .right = (rl_red_black_tree_node_t[]){
                {
                  .key = /* ref = 3413 */ NULL,
                  .left = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 3578 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 3818 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3518 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_RED,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3833 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_RED,
                            },
                          },
                          .red = RL_BLACK,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 3563 */ NULL,
                          .left = NULL,
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3608 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_RED,
                            },
                          },
                          .red = RL_BLACK,
                        },
                      },
                      .red = RL_BLACK,
                    },
                  },
                  .right = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 3758 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 3863 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3998 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3698 */ NULL,
                              .left = (rl_red_black_tree_node_t[]){
                                {
                                  .key = /* ref = 4073 */ NULL,
                                  .left = NULL,
                                  .right = NULL,
                                  .red = RL_RED,
                                },
                              },
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .red = RL_RED,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 3683 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3728 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3713 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .red = RL_RED,
                        },
                      },
                      .red = RL_BLACK,
                    },
                  },
                  .red = RL_BLACK,
                },
              },
              .red = RL_BLACK,
            },
          },
          .right = (rl_red_black_tree_node_t[]){
            {
              .key = /* ref = 3443 */ NULL,
              .left = (rl_red_black_tree_node_t[]){
                {
                  .key = /* ref = 3593 */ NULL,
                  .left = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 3878 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 3743 */ NULL,
                          .left = NULL,
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3893 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_RED,
                            },
                          },
                          .red = RL_BLACK,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 3923 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3938 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_RED,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3803 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_RED,
                            },
                          },
                          .red = RL_BLACK,
                        },
                      },
                      .red = RL_BLACK,
                    },
                  },
                  .right = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 3458 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 4058 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3773 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_RED,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 3908 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_RED,
                            },
                          },
                          .red = RL_BLACK,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 3638 */ NULL,
                          .left = NULL,
                          .right = NULL,
                          .red = RL_BLACK,
                        },
                      },
                      .red = RL_BLACK,
                    },
                  },
                  .red = RL_BLACK,
                },
              },
              .right = (rl_red_black_tree_node_t[]){
                {
                  .key = /* ref = 3848 */ NULL,
                  .left = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 3653 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 3428 */ NULL,
                          .left = NULL,
                          .right = NULL,
                          .red = RL_BLACK,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 3788 */ NULL,
                          .left = NULL,
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 4028 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_RED,
                            },
                          },
                          .red = RL_BLACK,
                        },
                      },
                      .red = RL_BLACK,
                    },
                  },
                  .right = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 3488 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 3548 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 4043 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_RED,
                            },
                          },
                          .right = NULL,
                          .red = RL_BLACK,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 3473 */ NULL,
                          .left = NULL,
                          .right = NULL,
                          .red = RL_BLACK,
                        },
                      },
                      .red = RL_BLACK,
                    },
                  },
                  .red = RL_BLACK,
                },
              },
              .red = RL_BLACK,
            },
          },
          .red = RL_RED,
        },
      },
      .right = (rl_red_black_tree_node_t[]){
        {
          .key = /* ref = 4662 */ NULL,
          .left = (rl_red_black_tree_node_t[]){
            {
              .key = /* ref = 4767 */ NULL,
              .left = (rl_red_black_tree_node_t[]){
                {
                  .key = /* ref = 4602 */ NULL,
                  .left = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 4932 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 6949 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 6964 */ NULL,
                              .left = (rl_red_black_tree_node_t[]){
                                {
                                  .key = /* ref = 6994 */ NULL,
                                  .left = NULL,
                                  .right = NULL,
                                  .red = RL_RED,
                                },
                              },
                              .right = (rl_red_black_tree_node_t[]){
                                {
                                  .key = /* ref = 6979 */ NULL,
                                  .left = NULL,
                                  .right = NULL,
                                  .red = RL_RED,
                                },
                              },
                              .red = RL_BLACK,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 5333 */ NULL,
                              .left = (rl_red_black_tree_node_t[]){
                                {
                                  .key = /* ref = 7009 */ NULL,
                                  .left = NULL,
                                  .right = NULL,
                                  .red = RL_RED,
                                },
                              },
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .red = RL_BLACK,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 4902 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 4887 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 4617 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .red = RL_BLACK,
                        },
                      },
                      .red = RL_RED,
                    },
                  },
                  .right = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 5081 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 4827 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 4782 */ NULL,
                              .left = NULL,
                              .right = (rl_red_black_tree_node_t[]){
                                {
                                  .key = /* ref = 4947 */ NULL,
                                  .left = NULL,
                                  .right = NULL,
                                  .red = RL_RED,
                                },
                              },
                              .red = RL_BLACK,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 4857 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .red = RL_RED,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 5111 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 5066 */ NULL,
                              .left = (rl_red_black_tree_node_t[]){
                                {
                                  .key = /* ref = 5126 */ NULL,
                                  .left = NULL,
                                  .right = NULL,
                                  .red = RL_RED,
                                },
                              },
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 5096 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .red = RL_RED,
                        },
                      },
                      .red = RL_BLACK,
                    },
                  },
                  .red = RL_BLACK,
                },
              },
              .right = (rl_red_black_tree_node_t[]){
                {
                  .key = /* ref = 4572 */ NULL,
                  .left = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 4707 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 4842 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 4872 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_RED,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 4677 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_RED,
                            },
                          },
                          .red = RL_BLACK,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 4647 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 4737 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 4797 */ NULL,
                              .left = (rl_red_black_tree_node_t[]){
                                {
                                  .key = /* ref = 4977 */ NULL,
                                  .left = NULL,
                                  .right = NULL,
                                  .red = RL_RED,
                                },
                              },
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .red = RL_RED,
                        },
                      },
                      .red = RL_BLACK,
                    },
                  },
                  .right = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 4692 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 4812 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 4587 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_RED,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 4962 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_RED,
                            },
                          },
                          .red = RL_BLACK,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 4722 */ NULL,
                          .left = NULL,
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 4752 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_RED,
                            },
                          },
                          .red = RL_BLACK,
                        },
                      },
                      .red = RL_BLACK,
                    },
                  },
                  .red = RL_BLACK,
                },
              },
              .red = RL_BLACK,
            },
          },
          .right = (rl_red_black_tree_node_t[]){
            {
              .key = /* ref = 11108 */ NULL,
              .left = (rl_red_black_tree_node_t[]){
                {
                  .key = /* ref = 9236 */ NULL,
                  .left = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 4632 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 4917 */ NULL,
                          .left = NULL,
                          .right = NULL,
                          .red = RL_BLACK,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 9099 */ NULL,
                          .left = NULL,
                          .right = NULL,
                          .red = RL_BLACK,
                        },
                      },
                      .red = RL_BLACK,
                    },
                  },
                  .right = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 11273 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 11657 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 11642 */ NULL,
                              .left = NULL,
                              .right = (rl_red_black_tree_node_t[]){
                                {
                                  .key = /* ref = 11672 */ NULL,
                                  .left = NULL,
                                  .right = NULL,
                                  .red = RL_RED,
                                },
                              },
                              .red = RL_BLACK,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 9338 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .red = RL_RED,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 11093 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 11123 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 11198 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .red = RL_RED,
                        },
                      },
                      .red = RL_BLACK,
                    },
                  },
                  .red = RL_BLACK,
                },
              },
              .right = (rl_red_black_tree_node_t[]){
                {
                  .key = /* ref = 11048 */ NULL,
                  .left = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 11078 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 11213 */ NULL,
                          .left = NULL,
                          .right = NULL,
                          .red = RL_BLACK,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 11228 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 11303 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_RED,
                            },
                          },
                          .right = NULL,
                          .red = RL_BLACK,
                        },
                      },
                      .red = RL_BLACK,
                    },
                  },
                  .right = (rl_red_black_tree_node_t[]){
                    {
                      .key = /* ref = 11153 */ NULL,
                      .left = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 11138 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 11168 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_BLACK,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 11183 */ NULL,
                              .left = NULL,
                              .right = (rl_red_black_tree_node_t[]){
                                {
                                  .key = /* ref = 11243 */ NULL,
                                  .left = NULL,
                                  .right = NULL,
                                  .red = RL_RED,
                                },
                              },
                              .red = RL_BLACK,
                            },
                          },
                          .red = RL_RED,
                        },
                      },
                      .right = (rl_red_black_tree_node_t[]){
                        {
                          .key = /* ref = 11258 */ NULL,
                          .left = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 11288 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_RED,
                            },
                          },
                          .right = (rl_red_black_tree_node_t[]){
                            {
                              .key = /* ref = 11063 */ NULL,
                              .left = NULL,
                              .right = NULL,
                              .red = RL_RED,
                            },
                          },
                          .red = RL_BLACK,
                        },
                      },
                      .red = RL_BLACK,
                    },
                  },
                  .red = RL_BLACK,
                },
              },
              .red = RL_BLACK,
            },
          },
          .red = RL_RED,
        },
      },
      .red = RL_BLACK,
    },
  },
  .output_format = {
  },
  .io_handlers = {
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
  },
  .io_ext_handlers = {
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
    {
      .load = {
      },
      .save = {
      },
    },
  },
};
#endif

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
  rl_output_format_t old_uint8 = rl_conf.output_format[RL_TYPE_UINT8];
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
    .b1 = NONE,
    .b2 = READ,
    .b3 = WRITE,
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
    .rarray_1 = { .data = "ijk", .size = 4 },
    .rarray_empty = { .data = NULL, .size = 0 },
    ._void = NULL,
    .next = &sample,
    .prev = &sample,
    .ptr_int8 = (int8_t*)"x",
    .ptr_uint8 = (uint8_t*)"y",
    .ptr_char = "z",
    .ptr_null = NULL,
    .arr = {2, 3},
    .ext = { "test string_t" },
    .ptr_type = "string_t",
  };

  //test_hash ();

#if 1
  point.sample = &sample;
  sample.string1 = sample.string;
  memcpy (&sample.rarray_2, &sample.rarray_1, sizeof (sample.rarray_1));
  sample.ptr_char_ = &sample.rarray_1.data[3];
  sample._void = sample.string;
  
#ifdef HAVE_LIBXML2
  LIBXML_TEST_VERSION;
  xmlKeepBlanksDefault(0);
#endif /* HAVE_LIBXML2 */

  char * format_float(rl_ptrdes_t * ptrdes) { char str[RL_FLOAT_TO_STRING_BUF_SIZE] = ""; sprintf (str, "%.10f", *(float*)ptrdes->data); return RL_STRDUP (str); }
  char * format_double(rl_ptrdes_t * ptrdes) { char str[RL_FLOAT_TO_STRING_BUF_SIZE] = ""; sprintf (str, "%.20g", *(double*)ptrdes->data); return RL_STRDUP (str); }
  char * format_uint8(rl_ptrdes_t * ptrdes) {
    char str[RL_INT_TO_STRING_BUF_SIZE] = "";
    if ((NULL == ptrdes->fd.ext.ptr) || (NULL == ((meta_info_t*)ptrdes->fd.ext.ptr)->format))
      return (old_uint8 (ptrdes));
    sprintf (str, ((meta_info_t*)ptrdes->fd.ext.ptr)->format, *(int8_t*)ptrdes->data);
    return (RL_STRDUP (str));
  }
  char * format_uint64(rl_ptrdes_t * ptrdes) {
    char str[RL_INT_TO_STRING_BUF_SIZE] = "";
    sprintf (str, "%" SCNu64 "ULL", *(uint64_t*)ptrdes->data);
    return (RL_STRDUP (str));
  }
  rl_conf.output_format[RL_TYPE_FLOAT] = format_float;
  rl_conf.output_format[RL_TYPE_DOUBLE] = format_double;
  rl_conf.output_format[RL_TYPE_UINT8] = format_uint8;
  //rl_conf.output_format[RL_TYPE_UINT64] = format_uint64;

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

#if 0
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

  rl_type_t x = RL_TYPE_DETECT (typeof (.0f));
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

  ieee754_float_t ie;
  ie.f = M_PI * 2;
  str = RL_SAVE_CINIT (ieee754_float_t, &ie);
  if (str)
    {
      printf ("emp = %s\n", str);
      RL_FREE (str);
    }

  _Bool tc = 1;
  printf ("%d\n", tc);
#ifdef HAVE_LIBXML2 
  /* Clean up everything else before quitting. */
  xmlCleanupParser();
#endif /* HAVE_LIBXML2 */
  return (EXIT_SUCCESS);
}
