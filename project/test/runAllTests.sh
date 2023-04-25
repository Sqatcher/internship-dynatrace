i=1
echo "Running tests.."
while read line
do
	data=`echo $line | cut -d" " -f 1`
	count=`echo $line | cut -d" " -f 2`
	../build/internship $data $count > _output/output${i}.txt
	if cmp --silent _output/output${i}.txt answer${i}.txt; then
		echo "Test $((i)) - success!"
	else
		echo "Test $((i)) - failed."
	fi
	i=$((i+1))
done < tests.txt