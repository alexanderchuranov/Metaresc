/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#ifndef _LEXER_H_
#define _LEXER_H_

TYPEDEF_STRUCT (mr_lloc_t, ATTRIBUTES ( , "parser location"),
		(int, lineno, , "parser location - line number"),
		(int, column, , "parser location - column number"),
		(int, offset, , "parser location - offset in string"),
		)

TYPEDEF_STRUCT (mr_token_lloc_t, ATTRIBUTES ( , "token location"),
		(mr_lloc_t, start, , "start of the token"),
		(mr_lloc_t, end, , "end of the token"),
		)

TYPEDEF_STRUCT (mr_load_t, ATTRIBUTES ( , "Metaresc load parser data"),
		(mr_lloc_t, lloc, , "current location of parser"),
		(char *, str, , "string to parse"),
		(char *, buf, , "parser internal buffer"),
		(int, parent, , "index of current parent"),
		(mr_ra_mr_ptrdes_t *, ptrs, , "resizable array with mr_ptrdes_t"),
		)

TYPEDEF_STRUCT (mr_substr_t, ATTRIBUTES ( , "substring and postprocessor"),
		RARRAY (char, substr, "substring descriptor"),
		(char *, unquote, (char *, int), "unquote handler"),
		)

TYPEDEF_ENUM (mr_value_type_t,
	      (MR_VT_INT, , "vt_int"),
	      (MR_VT_FLOAT, , "vt_float"),
	      (MR_VT_COMPLEX, , "vt_complex"),
	      (MR_VT_STRING, , "vt_string"),
	      )

TYPEDEF_STRUCT (mr_value_t, ATTRIBUTES ( , "value for expressions calculation"),
		(mr_value_type_t, value_type),
		ANON_UNION (),
		long long int vt_int,
		long double vt_float,
		complex long double vt_complex,
		(mr_substr_t, string),
		END_ANON_UNION ("value_type"),
		)

#define MR_PARSE_ERROR(ERROR_MSG, SCANNER, LANG) ({			\
      YYLTYPE * lloc = mr_## LANG ## _get_lloc (SCANNER);		\
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_PARSE_ERROR, ERROR_MSG,	\
		  lloc->start.lineno, lloc->start.column, lloc->end.lineno, lloc->end.column); \
    })

#define YYLTYPE mr_token_lloc_t

/**
 * Helper function for building tree within parsing.
 * @param mr_load structure with current parsing context
 * @return index of newly allocated element in mr_load->ptrs resizeable array
 */
static inline int mr_parse_add_node (mr_load_t * mr_load)
{
  int idx = mr_add_ptr_to_list (mr_load->ptrs);
  if (idx < 0)
    return (idx);
  mr_add_child (mr_load->parent, idx, mr_load->ptrs);
  return (idx);
}

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
