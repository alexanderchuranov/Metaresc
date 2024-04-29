/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

#ifndef _LEXER_H_
#define _LEXER_H_

#include <metaresc.h>

TYPEDEF_STRUCT (mr_token_lloc_t, ATTRIBUTES ( , "token location"),
		(mr_lloc_t, start, , "start of the token"),
		(mr_lloc_t, end, , "end of the token"),
		)

#define MR_PARSE_ERROR(LLOCP, SCANNER, ERROR_MSG)			\
  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_PARSE_ERROR, ERROR_MSG,		\
	      (LLOCP)->start.lineno, (LLOCP)->start.column, (LLOCP)->end.lineno, (LLOCP)->end.column)

static inline void mr_calc_lloc (mr_load_t * mr_load, char * ptr)
{
  if (NULL == mr_load->buf)
    mr_load->buf = ptr;
  else
    for ( ; &mr_load->buf[mr_load->lloc.offset] < ptr; ++mr_load->lloc.offset)
      if ('\n' == mr_load->buf[mr_load->lloc.offset])
	{
	  ++mr_load->lloc.lineno;
	  mr_load->lloc.column = 0;
	}
      else
	++mr_load->lloc.column;
}

static inline int mr_substrcmp (char * str, mr_substr_t * substr)
{
  typeof (substr->length) str_len = strlen (str);
  if (str_len != substr->length)
    return (str_len - substr->length);
  return (strncmp (str, substr->str, str_len));
}

static inline void mr_get_id (mr_substr_t * substr, char * start)
{
  char * stop;
  while (isspace (*start))
    ++start;
  stop = start;
  while (isalnum (*stop) || ('_' == *stop))
    ++stop;
  substr->str = start;
  substr->length = stop - start;
}

#define YY_USER_ACTION ({			\
      mr_load_t * mr_load = yyextra;		\
      mr_calc_lloc (mr_load, yy_bp);		\
      yylloc->start = mr_load->lloc;		\
      mr_calc_lloc (mr_load, yy_cp);		\
      yylloc->end = mr_load->lloc;		\
    });

#define YYLLOC_DEFAULT(Current, Rhs, N)					\
  ({									\
    if (N)								\
      {									\
	(Current).start.lineno = YYRHSLOC (Rhs, 1).start.lineno;	\
	(Current).start.column = YYRHSLOC (Rhs, 1).start.column;	\
	(Current).start.offset = YYRHSLOC (Rhs, 1).start.offset;	\
	(Current).end.lineno = YYRHSLOC (Rhs, N).end.lineno;		\
	(Current).end.column = YYRHSLOC (Rhs, N).end.column;		\
	(Current).end.offset = YYRHSLOC (Rhs, N).end.offset;		\
      }									\
    else								\
      {									\
	(Current).start.lineno = (Current).end.lineno = YYRHSLOC (Rhs, 0).end.lineno; \
	(Current).start.column = (Current).end.column = YYRHSLOC (Rhs, 0).end.column; \
	(Current).start.offset = (Current).end.offset = YYRHSLOC (Rhs, 0).end.offset; \
      }									\
  })

#define YY_NO_INPUT
#define YYINITDEPTH 256
#define YYMAXDEPTH ((unsigned)-1)

#define MR_LOAD_FUNC(METHOD)						\
  mr_status_t mr_ ## METHOD ## _load (char * str, mr_ra_ptrdes_t * ptrs) { \
    mr_status_t status = MR_FAILURE;					\
    yyscan_t scanner;							\
    mr_load_t mr_load = { .lloc = { .lineno = 1, .column = 0, .offset = 0, }, .str = str, .buf = NULL, .parent = 0, .ptrs = ptrs }; \
    if (NULL == str)							\
      {									\
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_NULL_POINTER);	\
	return (MR_FAILURE);						\
      }									\
    if (mr_ ## METHOD ## _lex_init_extra (&mr_load, &scanner))		\
      return (MR_FAILURE);						\
    if (NULL != mr_ ## METHOD ## __scan_string (str, scanner))		\
      if (0 == mr_ ## METHOD ## _parse (scanner))			\
	status = MR_SUCCESS;						\
    if (status != MR_SUCCESS)						\
      mr_free_load_values (ptrs);					\
    mr_ ## METHOD ## _lex_destroy (scanner);				\
    return (status);							\
  }

#endif /* _LEXER_H_ */
