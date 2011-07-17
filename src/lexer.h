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

static inline char * rl_get_id (char * start)
{
  int size;
  char * id;
  char * stop;
  while (isspace (*start)) ++start;
  stop = start;
  while (isalnum (*stop) || ('_' == *stop)) ++stop;
  size = stop - start;
  id = RL_MALLOC (size + 1);
  if (id)
    {
      memcpy (id, start, size);
      id[size] = 0;
    }
  return (id);
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
  status = rl_ ## METHOD ## _parse (scanner);				\
  rl_ ## METHOD ## _lex_destroy (scanner);				\
  return (status);							\
}

#endif /* _LEXER_H_ */
