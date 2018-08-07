
cd $1
cp ../date.txt .
cp ../version.txt .

${QTDIR}/bin/qmake
make
${QTDIR}/bin/macdeployqt $1.app
mv $1.app ../$2

