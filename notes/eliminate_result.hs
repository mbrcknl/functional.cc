
-- Func: callable type.
-- Spec: list of parameter types.

-- fp::result_of
result_of :: Func -> Spec -> Maybe Type
result_of = undefined

-- std::common_type
common_type :: [Type] -> Maybe Type
common_type = undefined

partitionMaybe :: (a -> Maybe b) -> [a] -> ([a],[b])
partitionMaybe f = foldr step ([],[]) where
  step a (as,bs) = maybe (a:as,bs) ((as,).(:bs)) (f a)

-- Given:
--   1. a callable type, and
--   2. a list of candidate parameter-type lists;
-- Return:
--   A. the list of all parameter-type lists which are incompatible
--      with the callable type, and
--   B. the list of result types given by parameter-type lists which
--      are compatible with the callable type.

elim_res_one :: Func -> [Spec] -> ([Spec],[Type])
elim_res_one = partitionMaybe . result_of

-- Given:
--   1. a list of candidate parameter-type lists,
--   2. a list of callable types;
-- Return:
--   A. Nothing, if either:
--      - some parameter-type list does not match any callable type, or
--      - there is a callable type which does not match a parameter-type
--        list other than a parameter-type list matched by a preceding
--        callable type, or
--      - there is no common type to which the result types for matching calls
--        can be converted, where each matching call is given by the first
--        callable type which matches a given parameter-type list; or
--   B. The common return type, otherwise.

elim_res :: [Spec] -> [Func] -> Maybe Type
elim_res = first where
  first specs (f:funcs) = case elim_res_one f specs of
    (specr,ts) -> fmap (rest specr funcs) (common_type ts)
  first _ [] = Nothing

  rest specs (f:funcs) t = case elim_res_one f specs of
    (specr,[]) -> Nothing
    (specr,ts) -> fmap (rest specr funcs) (common_type (t:ts))
  rest (_:_) [] _ = Nothing
  rest [] [] t = Just t

