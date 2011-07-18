#!/bin/bash

if [ ! -d classes -o ! -d www ] ; then
   echo not at root of project, ignoring you...
   exit 1
fi

source /servers/spacebits/config/site.sh

path=`pwd`

exclude="--exclude=config --exclude=db --exclude=cache --exclude=svn"

./repo/update_repo.sh 

cd $path
echo "Syncing production farm"
for s in $SERVERS
do
  echo "Syncing code tree with $s"
  rsync -a --times $exclude --exclude=cusf --exclude=www/logs --exclude=.* --exclude=uploads --exclude=*.swp --exclude=main-*  --exclude=.svn -e ssh . $s:$PROD_PATH --delete
  echo "Syncing site.db from $s to local computer"
  rsync -a --times -e ssh $s:$PROD_PATH/db/*.db db/
  echo "Syncing images from $s to local computer"
  rsync -a --times --exclude=*.swp --exclude=.* --exclude=main-*  --exclude=.svn -e ssh $s:$PROD_PATH/www/uploads/* www/uploads
  echo "Syncing local images with remote $s"
  rsync -a --times --exclude=*.swp --exclude=.* --exclude=main-*  --exclude=.svn -e ssh www/uploads/* $s:$PROD_PATH/www/uploads
done
echo "Done."

