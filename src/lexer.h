/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#ifndef _LEXER_H_
#define _LEXER_H_

#define MR_PARSE_ERROR(ERROR_MSG, SCANNER, LANG) ({			\
      YYLTYPE * lloc = mr_## LANG ## _get_lloc (SCANNER);		\
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_PARSE_ERROR, ERROR_MSG,	\
		  lloc->start.lineno, lloc->start.column, lloc->end.lineno, lloc->end.column); \
    })

#define YYLTYPE mr_token_lloc_t

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
  int str_len = strlen (str);
  if (str_len != substr->substr.size)
    return (str_len - substr->substr.size);
  return (strncmp (str, substr->substr.data, str_len));
}

static inline char * mr_unquote (mr_substr_t * substr)
{
  if (NULL == substr->substr.data)
    return (NULL);
  else
    {
      if (substr->unquote)
	return (substr->unquote (substr->substr.data, substr->substr.size));
      else
	return (strndup (substr->substr.data, substr->substr.size));
    }
}

static inline void mr_get_id (mr_substr_t * substr, char * start)
{
  char * stop;
  while (isspace (*start))
    ++start;
  stop = start;
  while (isalnum (*stop) || ('_' == *stop))
    ++stop;
  substr->substr.data = start;
  substr->substr.size = stop - start;
  substr->unquote = NULL;
}

static inline void mr_set_lval (mr_substr_t * substr, char * str, char * (*unquote_func) (char*, int))
{
  substr->substr.data = str;
  substr->substr.size = (str ? strlen (str) : -1);
  substr->unquote = unquote_func;
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
    if (YYID (N))							\
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
#define YYMAXDEPTH (1 << 31)

#define MR_LOAD_FUNC(METHOD)						\
  int METHOD ## _load (char * str, mr_ra_mr_ptrdes_t * ptrs) {		\
  int status;								\
  yyscan_t scanner;							\
  mr_load_t mr_load = { .lloc = { .lineno = 1, .column = 0, .offset = 0, }, .str = str, .buf = NULL, .parent = -1, .ptrs = ptrs }; \
  mr_ ## METHOD ## _lex_init_extra (&mr_load, &scanner);		\
  mr_ ## METHOD ## __scan_string (str, scanner);			\
  status = !mr_ ## METHOD ## _parse (scanner);				\
  mr_ ## METHOD ## _lex_destroy (scanner);				\
  return (status);							\
}

#endif /* _LEXER_H_ */
