for((i=1;i<=6;i++)); do
	for j in {a..z}; do
		FILE=obl/in/obl$i$j.in
		if test -f $FILE; then
			echo -n "$FILE:"
			./inwer 
		fi
	done
done
