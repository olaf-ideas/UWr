(* Zad1 *)

exception Found

let exists f xs =
	try
		List.fold_left (fun _ x ->
			if f x then raise (Found) else false
		) false xs
	with
	| Found -> true
