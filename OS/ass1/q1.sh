#! /bin/bash  

# Read input
echo -n "Enter uv1 : "
read uv1

echo -n "Enter uv2 : "
read uv2

# Print part a.
echo "============================="
echo $uv1$uv2
echo $uv2$uv1
echo "============================="

# Reverse uv1 and uv2

# Reverse function
function reverse() {
    local tmp
    local reverse
    local length

    tmp=$1
    reverse=""
    length=${#tmp}

    for ((i=length-1; i>=0; i--)); do
        reverse="$reverse${tmp:i:1}"
    done
    echo $reverse
}


uv1__reversed=$(reverse $uv1)
uv2__reversed=$(reverse $uv2)

# Print part b.
echo "============================="
echo $uv1__reversed
echo $uv2__reversed
echo "============================="