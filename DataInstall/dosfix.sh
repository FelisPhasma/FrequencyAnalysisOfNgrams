#!/bin/sh
if [ $(dpkg-query -W -f='${Status}' dos2unix 2>/dev/null | grep -c "ok installed") -eq 0 ];
then
  sudo apt-get install dos2unix;
fi
dos2unix install.sh
exit