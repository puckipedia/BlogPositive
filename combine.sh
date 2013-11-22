cat $1.h $1.cpp > $1.lzz
sed -i 's/#include "[^.]*\.h"//g' $1.lzz
sed -i 's/#pragma once//g' $1.lzz
rm $1.h $1.cpp
