for((i=1;;i++)); do
	python3 gen.py > in
	diff -bwq <(./task4 < in) <(./brute < in) || break
	echo "$i: ok"
done
