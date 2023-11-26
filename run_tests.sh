#! /bin/sh

testFiles=$(ls $2 | grep ".um")
for testFile in $testFiles ; do
	testName=$(echo $testFile | sed -E 's/(.*).um/\1/')
	if [ -f "$2/$testName.1" ] ; then
		um "$2/$testFile" > output.txt
		echo "diff output for $testName:"
		diff output.txt "$2/$testName.1"
	else
		echo "$testName has no output"
	fi
done


