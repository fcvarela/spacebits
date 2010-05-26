#!/bin/sh

cd /servers/spacebits/repo
export COPYFILE_DISABLE
export COPY_EXTENDED_ATTRIBUTES_DISABLE
./dpkg-deb-fat -b SpacebitsMobile
rm -fr /tmp/repo
mkdir /tmp/repo
mv SpacebitsMobile.deb /tmp/repo
cp dpkg-gettext.pl /Library/Perl/
chmod 755 /Library/Perl/dpkg-gettext.pl
cd /tmp/repo
./dpkg-scanpackages -m . /dev/null >Packages
bzip2 Packages
cd ..
rm -fr /servers/spacebits/www/repo
mv repo /servers/spacebits/www/repo
