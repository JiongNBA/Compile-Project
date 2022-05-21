#! /bin/bash
clear
. clean.sh
for i in {1..100};
do
	make run1 >> res.txt
	make run2 >> res.txt
	make run3 >> res.txt
done
grep fail res.txt > stat.txt
num=$(wc stat.txt -l | awk '{print $1}')
if [ "$num" -eq 0 ];
then
	echo pass!
else
	echo fail!
fi
rm res.txt
rm stat.txt
mv ./src/flySCC ./bin/flySCC
mv ./test/test1.o ./bin/test1.o
mv ./test/test2.o ./bin/test2.o
mv ./test/test3.o ./bin/test3.o