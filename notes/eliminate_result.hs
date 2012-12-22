
{-# LANGUAGE TupleSections #-}

data Type = Type

-- Callable type
data Func = Func { func_args :: [Type], func_result :: Type }

-- List of parameter types
data Spec = Spec { spec_args :: [Type] }

-- fp::result_of
result_of :: Func -> Spec -> Maybe Type
result_of = undefined

-- std::common_type
common_type :: [Type] -> Maybe Type
common_type = undefined

partitionMaybe :: (a -> Maybe b) -> [a] -> ([b],[a])
partitionMaybe f = foldr step ([],[]) where
  step a (bs,as) = maybe (bs,a:as) ((,as).(:bs)) (f a)

-- Given:
--   1. a callable type, and
--   2. a list of candidate parameter-type lists;
-- Return:
--   A. the list of all parameter-type lists which are incompatible
--      with the callable type, and
--   B. the list of result types given by parameter-type lists which
--      are compatible with the callable type.

elim_res_one :: Func -> [Spec] -> ([Type],[Spec])
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

elim_res :: [Func] -> [Spec] -> Maybe Type
elim_res f s = elim f s Nothing where
  elim (f:funcs) specs t = case elim_res_one f specs of
    ([],specr) -> Nothing
    (ts,specr) -> common_type (maybe id (:) t ts) >>= elim funcs specr . Just
  elim [] (_:_) _ = Nothing
  elim [] [] t = t

