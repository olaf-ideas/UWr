exception Mye
exception Mye2

let f x =
  try
    if x = 0 then raise Mye else x
  with
  |Mye -> true
  