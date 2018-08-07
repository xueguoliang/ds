b=`git branch | grep "*" | awk '{print $2}'`
git add . -A
git commit -m "no comment"
git push origin $b:$b
