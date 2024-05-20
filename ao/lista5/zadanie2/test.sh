for((i=1;;i++)); do
	./gen > in
	diff -bwq <(./rozw < in) <(./rozw2 < in) || break
	echo "$i: ok"
done
