for((i=1;;i++)); do
	python3 gen.py > in
	diff -bwq <(./task < in) <(./brute < in) || break
	echo "$i: ok"
done
