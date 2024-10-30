for((i=0;i<4;i++)); do
	seed=$(python3 -c "import random; print(random.randint(0, 2**63 - 1))")
	./task $seed > out$i &
done

wait
