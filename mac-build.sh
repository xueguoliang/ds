
if [ -z "$QTDIR" ]; then
    echo can not find QTDIR
    exit 1
fi

date "+%Y-%m-%d %H:%M:%S" > date.txt
d=`date "+%Y%m%d%H%M%S"`
git branch | grep "*" | awk '{print $2}' > version.txt
v=`cat version.txt`

DSDIR=DSForMac-${v}-${d}
DSZIP=${DSDIR}.zip

rm -rf DSForMac*

mkdir ${DSDIR}

chmod u+x mac-make-one.sh
./mac-make-one.sh DSView ${DSDIR}
./mac-make-one.sh DS ${DSDIR}
./mac-make-one.sh DSCap ${DSDIR}
./mac-make-one.sh DSSend ${DSDIR}
#./mac-make-one.sh DSPlay ${DSDIR}

cp cn.xueguoliang.dscap.plist ${DSDIR}
cp cn.xueguoliang.dssend.plist ${DSDIR}

defaults write ${PWD}/${DSDIR}/DS.app/Contents/Info.plist LSUIElement 1
defaults write ${PWD}/${DSDIR}/DSCap.app/Contents/Info.plist LSUIElement 1
defaults write ${PWD}/${DSDIR}/DSSend.app/Contents/Info.plist LSUIElement 1

chmod u+x install.command
chmod u+x uninstall.command
cp install.command ${DSDIR}
cp uninstall.command ${DSDIR}

./${DSDIR}/install.command
echo start zip......
zip -q -r -m -o ${DSZIP} ${DSDIR}
echo end zip......
