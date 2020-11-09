set -e

cd /tmp

curl -L -O http://archive.apache.org/dist/apr/apr-1.5.2.tar.gz

tar -xzvf apr-1.5.2.tar.gz
cd apr-1.5.2

./configure
make
sudo make install

cd /tmp
rm -rf apr-1.5.2 apr-1.5.2.tar.gz

curl -L -O http://archive.apache.org/dist/apr/apr-util-1.5.4.tar.gz

tar -xzvf apr-util-1.5.4.tar.gz
cd apr-util-1.5.4

./configure --with-apr=/usr/local/apr
make
sudo make install

cd /tmp
rm -rf apr-util-1.5.4* apr-1.5.2*