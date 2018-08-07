
if [ -z "$QTDIR" ]; then
    echo can not find QTDIR
    exit 1
fi

date "+%Y-%m-%d %H:%M:%S" > date.txt
d=`date "+%Y%m%d%H%M%S"`
git branch | grep "*" | awk '{print $2}' > version.txt
v=`cat version.txt`

DSDIR=DSForLinux-${v}-${d}
DSZIP=${DSDIR}.tar.gz

rm -rf DSForLinux*

mkdir ${DSDIR}

chmod u+x linux-make-one.sh
./linux-make-one.sh DSView ${DSDIR}
./linux-make-one.sh DS ${DSDIR}
./linux-make-one.sh DSCap ${DSDIR}
./linux-make-one.sh DSSend ${DSDIR}

tar zcvf ${DSZIP} ${DSDIR}
