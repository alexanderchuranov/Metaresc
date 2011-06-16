#!/bin/sh

PP_DEPTH=$1

echo "#ifndef _RLPP_H_"
echo "#define _RLPP_H_" 
echo 

echo "#define RL_FOREACH0(...)" 
I=0
while [ $I -lt $PP_DEPTH ]
do
  NEXT=$(( $I + 1 ))
  echo "#define RL_FOREACH$NEXT(X, _0, ...) X (_0) RL_FOREACH$I (X, __VA_ARGS__)" 
  I=$(( $I + 1 ))
done
echo 

echo "#define RL_FOR0(NAME, OP, FUNC, X, ...) " 
echo "#define RL_FOR1(NAME, OP, FUNC, X, ...) FUNC (NAME, X, 1)" 
I=1
while [ $I -lt $PP_DEPTH ]
do
  NEXT=$(( $I + 1 ))
  echo "#define RL_FOR$NEXT(NAME, OP, FUNC, X, ...) OP (NAME, $I, FUNC (NAME, X, $NEXT), RL_FOR$I (NAME, OP, FUNC, __VA_ARGS__))" 
  I=$(( $I + 1 ))
done
echo 

echo -n "#define RL_ARG"$PP_DEPTH"(" 
I=1
while [ $I -le $PP_DEPTH ]
do
  echo -n "_$I, "
  I=$(( $I + 1 ))
done
echo "...) _"$PP_DEPTH 
echo 

echo "#define RL_NARG(...) RL_NARG_ (0, ##__VA_ARGS__)" 

echo -n "#define RL_NARG_(...) RL_ARG$PP_DEPTH (__VA_ARGS__, " 
I=$(( $PP_DEPTH - 2 ))
while [ $I -ge 0 ]
do
  echo -n "$I, "
  I=$(( $I - 1 ))
done
echo ")" 
echo 

echo -n "#define RL_HAS_COMMA(...) RL_ARG"$PP_DEPTH "(__VA_ARGS__, " 
I=2
while [ $I -lt $PP_DEPTH ]
do
  echo -n "1, "
  I=$(( $I + 1 ))
done
echo "0, ...)" 
echo 

echo "#endif /* _RLPP_H_ */" 
