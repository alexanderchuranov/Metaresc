PP_DEPTH=$1

echo "#ifndef _RLPP_H_"
echo "#define _RLPP_H_" 
echo 

echo "#define RL_FOREACH0(...)" 
for (( I = 0; $I < $PP_DEPTH; I = $I + 1 )); do
  NEXT=$(( $I + 1 ))
  echo "#define RL_FOREACH$NEXT(X, _0, ...) X (_0) RL_FOREACH$I (X, __VA_ARGS__)" 
done
echo 

echo "#define RL_FOR0(NAME, OP, FUNC, X, ...) " 
echo "#define RL_FOR1(NAME, OP, FUNC, X, ...) FUNC (NAME, X, 1)" 
for (( I = 1; $I < $PP_DEPTH; I = $I + 1 )); do
  NEXT=$(( $I + 1 ))
  echo "#define RL_FOR$NEXT(NAME, OP, FUNC, X, ...) OP (NAME, $I, FUNC (NAME, X, $NEXT), RL_FOR$I (NAME, OP, FUNC, __VA_ARGS__))" 
done
echo 

echo -n "#define RL_ARG"$PP_DEPTH"(" 
for (( I = 1; $I <= $PP_DEPTH; I = $I + 1 )); do
  echo -n "_$I, "  
done
echo "...) _"$PP_DEPTH 
echo 

echo "#define RL_NARG(...) RL_NARG_ (0, ##__VA_ARGS__)" 

echo -n "#define RL_NARG_(...) RL_ARG$PP_DEPTH (__VA_ARGS__, " 
for (( I = $PP_DEPTH - 2; $I >= 0; I = $I - 1 )); do
  echo -n "$I, "  
done
echo ")" 
echo 

echo -n "#define RL_HAS_COMMA(...) RL_ARG"$PP_DEPTH "(__VA_ARGS__, " 
for (( I = 2; $I < $PP_DEPTH; I = $I + 1 )); do
  echo -n "1, "  
done
echo "0, ...)" 
echo 

echo "#endif /* _RLPP_H_ */" 
