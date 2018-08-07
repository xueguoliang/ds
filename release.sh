
p=`uname`
b=`git branch | grep "*" | awk '{print $2}'`
d=`date "+%Y-%m-%d %H:%M:%S"`


if [ $# -eq 2 ]; then
    git checkout master
    echo "$d $1 $2" >> release-note.txt
    ./gitcommit.sh
    git branch $1
    git checkout $1
    git push origin $1:$1
    git push origin master:master
elif [ $# -eq 1 ]; then
    git pull
    git checkout $1
else
    echo "need one or two argument"
    exit 1
fi


if [ "$p" = "MINGW32_NT-6.1" ];then
    echo "this is mingw"
    ./win-build.sh
elif [ "$p" = "Darwin" ]; then
    echo "this is mac"
    ./mac-build.sh
elif [ "$p" = "Linux" ]; then
    echo "this is Linux"
    ./linux-build.sh
else
    echo "unknown platform: $p"
fi
git checkout $b


