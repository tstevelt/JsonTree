/*---------------------------------------------------------------------------
	Copyright © 2022 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>

	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"JsonTree.h"

static void Usage ()
{
	printf ( "\n" );
	printf ( "USAGE: JsonTree  file|-  [options]\n" );
	printf ( "options:\n" );
	printf ( "  -print         default behavior\n" );
	printf ( "  -rm            remove input file after reading\n" );
	printf ( "  -find x [...]  find one or more name and print only those values\n" );
	printf ( "  -where fld val fld ... \n" );
	printf ( "  -script file   get -find or -where from script\n" );
	printf ( "  -d             debug\n" );
	printf ( "\n" );
	exit ( 1 );
}

void getargs ( int argc, char *argv[] )
{
	int		xa;

	if ( argc < 2 )
	{
		Usage ();
	}

	if ( strcmp ( argv[1], "-" ) == 0 )
	{
		fp = stdin;
		isStdin = 1;
	}
	else
	{
		InputFile = argv[1];
		if (( fp = fopen ( InputFile, "r" )) == NULL )
		{
			printf ( "Cannot open %s\n", argv[1] );
			exit ( 0 );
		}
		isStdin = 0;
	}

	RunMode = MODE_PRINT;
	DeleteFile = 0;
	Debug = 0;

	for ( xa = 2; xa < argc; xa++ )
	{
		if ( strcmp ( argv[xa], "-print" ) == 0 )
		{
			RunMode = MODE_PRINT;
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-find" ) == 0 )
		{
			RunMode = MODE_FIND;
			for ( xa++; xa < argc; )
			{
				if ( argv[xa][0] == '-' )
				{
					xa--;
					break;
				}
				if ( FindCount >= MAXFIND )
				{
					printf ( "Exceeds MAXFIND %d\n", MAXFIND );
					exit ( 1 );
				}

				snprintf ( FindArray[FindCount], MAXSTRING, "%s", argv[xa] );
				FindCount++;
				xa++;
			}
		}
		else if ( xa + 3 < argc && strcmp ( argv[xa], "-where" ) == 0 )
		{
			RunMode = MODE_WHERE;
			for ( xa++; xa < argc; )
			{
				if ( argv[xa][0] == '-' )
				{
					xa--;
					break;
				}
				if ( WhereCount >= MAXFIND )
				{
					printf ( "Exceeds MAXFIND %d\n", MAXFIND );
					exit ( 1 );
				}
				snprintf ( WhereArray[WhereCount].Field, MAXSTRING, "%s", argv[xa] );
				snprintf ( WhereArray[WhereCount].Value, MAXSTRING, "%s", argv[xa+1] );
				snprintf ( WhereArray[WhereCount].Name,  MAXSTRING, "%s", argv[xa+2] );
				WhereCount++;
				xa += 3;
			}
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-script" ) == 0 )
		{
			xa++;
			if ( LoadScript ( argv[xa] ) != 0 )
			{
				Usage ();
			}
		}
		else if ( strcmp ( argv[xa], "-rm" ) == 0 )
		{
			DeleteFile = 1;
		}
		else if ( strcmp ( argv[xa], "-d" ) == 0 )
		{
			Debug = 1;
		}
		else 
		{
			Usage ();
		}
	}
	if ( RunMode == MODE_FIND && FindCount == 0 )
	{
		printf ( "No search criteria\n" );
		exit ( 1 );
	}
	else if ( RunMode == MODE_WHERE && WhereCount == 0 )
	{
		printf ( "No search criteria\n" );
		exit ( 1 );
	}

	if ( Debug )
	{
		for ( xa = 0; xa < FindCount; xa++ )
		{
			printf ( "Find %s\n", FindArray[xa] );
		}
		for ( xa = 0; xa < WhereCount; xa++ )
		{
			printf ( "Where %s %s %s\n", WhereArray[xa].Field, WhereArray[xa].Value, WhereArray[xa].Name );
		}
	}
}
