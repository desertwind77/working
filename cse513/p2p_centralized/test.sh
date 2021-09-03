#/bin/sh

set hostname = "sudwerk"
set iteration = 200
set port = $2 
set count = 0

while ($count <= $iteration) 
	echo $count
	cli -d $1 -p $port $hostname $1.txt
	set count = `expr $count + 1`
end
