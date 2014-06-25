#!/bin/sh

TEMP_BIN=$(mktemp)
FAILED=0

./kamc -o $TEMP_BIN source/prog1.sm
diff $TEMP_BIN compiled/prog1.bin
if [ $? -ne 0 ]; then
	echo "--- $TEMP_BIN ---"
	xxd $TEMP_BIN

	echo ""
	echo "--- compiled/prog1.bin ---"
	xxd compiled/prog1.bin
	FAILED=1
fi
echo "[+] prog1 OK"

if [ $FAILED -ne 1 ]; then
	./kamc -o $TEMP_BIN source/prog2.sm
	diff $TEMP_BIN compiled/prog2.bin
	if [ $? -ne 0 ]; then
		FAILED=1
	fi
fi
echo "[+] prog2 OK"

rm $TEMP_BIN
exit $FAILED
