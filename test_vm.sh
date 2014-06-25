#!/bin/sh

STATUS=0

RESULT="$(./kamvm compiled/prog1.bin | grep '^>' | awk '{print $2;}')"
if [ "$RESULT" != "20" ]; then
	echo "[!] prog1.bin: expected a result of 20, got $RESULT."
	STATUS=1
fi
echo "[+] prog1.bin OK"

if [ $STATUS -ne 1 ]; then
	RESULT="$(./kamvm compiled/prog2.bin | grep '^>' | awk '{print $2;}')"
	if [ "$RESULT" != "8" ]; then
		echo "[!] prog2.bin: expected a result of 8, got $RESULT."
		STATUS=1
	fi
	echo "[+] prog2.bin OK"
fi

exit $STATUS

