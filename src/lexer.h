/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#ifndef _LEXER_H_
#define _LEXER_H_

#define RL_PARSE_ERROR(ERROR_MSG, SCANNER, LANG) ({			\
      YYLTYPE * lloc = rl_## LANG ## _get_lloc (SCANNER);		\
      RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_PARSE_ERROR, ERROR_MSG,	\
		  lloc->start.lineno, lloc->start.column, lloc->end.lineno, lloc->end.column); \
    })

#define YYLTYPE rl_token_lloc_t

static inline void rl_calc_lloc (rl_load_t * rl_load, char * ptr)
{
  if (NULL == rl_load->buf)
    rl_load->buf = ptr;
  else
    for ( ; &rl_load->buf[rl_load->lloc.offset] < ptr; ++rl_load->lloc.offset)
      if ('\n' == rl_load->buf[rl_load->lloc.offset])
	{
	  ++rl_load->lloc.lineno;
	  rl_load->lloc.column = 0;
	}
      else
	++rl_load->lloc.column;
}

static inline int rl_substrcmp (char * str, rl_substr_t * substr)
{
  int str_len = strlen (str);
  if (str_len != substr->substr.size)
    return (str_len - substr->substr.size);
  return (strncmp (str, substr->substr.data, str_len));
}

static inline char * rl_unquote (rl_substr_t * substr)
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
  
static inline void rl_get_id (rl_substr_t * substr, char * start)
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

static inline void rl_set_lval (rl_substr_t * substr, char * str, char * (*unquote_func) (char*, int))
{
  substr->substr.data = str;
  substr->substr.size = (str ? strlen (str) : -1);
  substr->unquote = unquote_func;
}

#define YY_USER_ACTION ({			\
      rl_load_t * rl_load = yyextra;		\
      rl_calc_lloc (rl_load, yy_bp);		\
      yylloc->start = rl_load->lloc;		\
      rl_calc_lloc (rl_load, yy_cp);		\
      yylloc->end = rl_load->lloc;		\
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

#define RL_LOAD_FUNC(METHOD)						\
  int METHOD ## _load (char * str, rl_ra_rl_ptrdes_t * ptrs) {		\
  int status;								\
  yyscan_t scanner;							\
  rl_load_t rl_load = { .lloc = { .lineno = 1, .column = 0, .offset = 0, }, .str = str, .buf = NULL, .parent = -1, .ptrs = ptrs }; \
  rl_ ## METHOD ## _lex_init_extra (&rl_load, &scanner);		\
  rl_ ## METHOD ## __scan_string (str, scanner);			\
  status = !rl_ ## METHOD ## _parse (scanner);				\
  rl_ ## METHOD ## _lex_destroy (scanner);				\
  return (status);							\
}

#endif /* _LEXER_H_ */
