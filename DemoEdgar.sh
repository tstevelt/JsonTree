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

Prompt 0 Expanding file
gunzip 0000004962.json.gz

Prompt 1 Printing JSON tree for 0000004962.json
JsonTree 0000004962.json | more
Prompt 1 Waiting ...

Prompt 0 Using -find for Outstanding Shares but only print the current one. Standard format.
JsonTree 0000004962.json -find EntityCommonStockSharesOutstanding  -array last
Prompt 1 Waiting ...

Prompt 0 Using -find for Dividends but only print the current one. CSV format.
JsonTree 0000004962.json -find DividendsCommonStock -fmt csv -array last

gzip 0000004962.json
