
if [ -z "$QTDIR" ]; then
    echo can not find QTDIR
    exit 1
fi

if [ -z "$QTTOOL" ]; then
    echo can not find QTTOOL
    exit 1
fi

date "+%Y-%m-%d %H:%M:%S" > date.txt
d=`date "+%Y%m%d%H%M%S"`
git branch | grep "*" | awk '{print $2}' > version.txt
v=`cat version.txt`

DSDIR=DSForWin-${v}-${d}
DSZIP=${DSDIR}.rar

rm -rf DSForWin*

mkdir ${DSDIR}

${QTDIR}/qmake.exe -r -spec win32-g++
${QTTOOL}/mingw32-make

#apps="DS DSCap DSPlay DSSend DSView"
apps="DS DSCap DSSend DSView"
for app in $apps; do
	cp $app/release/$app.exe $DSDIR
	$QTDIR/windeployqt.exe $DSDIR/$app.exe
done

cp $MINGWDIR/msys/1.0/local/bin/avcodec-57.dll $DSDIR
cp $MINGWDIR/msys/1.0/local/bin/avdevice-57.dll $DSDIR
cp $MINGWDIR/msys/1.0/local/bin/avformat-57.dll $DSDIR
cp $MINGWDIR/msys/1.0/local/bin/avutil-55.dll $DSDIR
cp $MINGWDIR/msys/1.0/local/bin/postproc-54.dll $DSDIR
cp $MINGWDIR/msys/1.0/local/bin/swresample-2.dll $DSDIR
cp $MINGWDIR/msys/1.0/local/bin/avfilter-6.dll $DSDIR
cp $MINGWDIR/msys/1.0/local/bin/swscale-4.dll $DSDIR
cp $MINGWDIR/bin/libiconv-2.dll $DSDIR
cp $MINGWDIR/bin/pthreadGC-3.dll $DSDIR
cp $MINGWDIR/bin/libgcc_s_dw2-1.dll $DSDIR
cp $MINGWDIR/bin/libstdc++-6.dll $DSDIR
cp c:/OpenSSL-Win32/libeay32.dll $DSDIR
cp c:/OpenSSL-Win32/libssl32.dll $DSDIR
cp c:/OpenSSL-Win32/ssleay32.dll $DSDIR
cp c:/Windows/System32/msvcr120.dll $DSDIR
"C:/Program Files/WinRAR/WinRAR.exe" a -r -ep1 -df $DSZIP $DSDIR
