echo "export SUGSORTLIB="$(pwd) > bin/thisSSlib.sh
echo "export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:\$SUGSORTLIB/bin:\$SUGSORTLIB/bin/build:\$SUGSORTLIB">> bin/thisSSlib.sh
echo "export CPATH=\$CPATH:\$SUGSORTLIB/include">> bin/thisSSlib.sh
echo "export PATH=\$PATH:\$SUGSORTLIB/include:\$SUGSORTLIB/src:\$SUGSORTLIB/bin:\$SUGSORTLIB/bin/build">> bin/thisSSlib.sh
echo "export LIBRARY_PATH=\$LIBRARY_PATH:\$SUGSORTLIB/bin:\$SUGSORTLIB/bin/build:\$SUGSORTLIB/src:\$SUGSORTLIB/include:\$SUGSORTLIB">> bin/thisSSlib.sh
