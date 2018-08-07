
if [ $# -ne 2 ]; then
    echo "need two argument"
    exit 1
fi

./gitcommit.sh $1 "$2"

b=`git branch | grep "*" | awk '{print $2}'`
git checkout $1
./mac-build.sh
git checkout $b


