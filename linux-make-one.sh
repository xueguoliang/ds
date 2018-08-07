
cd $1
cp ../date.txt .
cp ../version.txt .

${QTDIR}/bin/qmake
make
mv $1.bin ../$2/$1
cd ../$2; 
linuxdeployqt $1 -bundle-non-qt-libs; 
mv AppRun $1.bin

