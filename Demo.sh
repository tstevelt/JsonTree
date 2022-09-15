#!/bin/sh

Prompt ()
{
	WAIT=$1
	shift
	echo "==== $* ===="
	if [ $WAIT = 1 ]
	then
		echo "Press ENTER to continue"
		read x
	fi
}

Prompt 0 Showing usage message
JsonTree

Prompt 1 Printing JSON tree for IBM.json
JsonTree IBM.json
Prompt 1

Prompt 0 Using -find option to FIND city state and zip in IBM.json
JsonTree IBM.json -find city state zip
Prompt 1

Prompt 0 Using -script option to FIND in IBM.json with this script
cat IBM_find.txt
Prompt 1
JsonTree IBM.json -script IBM_find.txt
Prompt 1

Prompt 1 Printing JSON tree for WMT_fundamentals.json
JsonTree WMT_fundamentals.json
Prompt 1

Prompt 0 Using -where option to find two fields in WMT_fundamentals.json
JsonTree WMT_fundamentals.json -where dataCode longTermDebtEquity value dataCode freeCashFlow value
Prompt 1

Prompt 0 Using -script option to WHERE in WMT_fundamentals.json with this script
cat WMT_where.txt
Prompt 1
JsonTree WMT_fundamentals.json -script WMT_where.txt

Prompt 0 Done

