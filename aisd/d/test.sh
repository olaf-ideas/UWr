for((i=1;;i++)); do
	python3 gen.py > in
	diff -bwq <(./brute < in) <(./d < in) || break
	echo "$i: ok"
done
