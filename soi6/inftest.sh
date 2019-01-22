if [ -e ./vsfs ]; then 
    ./fs rmfs
fi

./fs mkfs

x=0;
while [ $x -le 100 ]; do
    ./fs cp main.c 
    ./fs cp cat.c
    ./fs touch x
    ./fs rm main.c 
    ./fs cp makefile
    ./fs cp rm.c 
    ./fs rm rm.c 
    ./fs rm cat.c 
    ./fs rm makefile
    ./fs cp main.c 
    ./fs rm x
    ./fs rm main.c
    x=$[x+1];
done