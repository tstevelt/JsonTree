/*---------------------------------------------------------------------------
	Copyright © 2022 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>

	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include <errno.h>
#include	"JsonTree.h"

static void Usage ()
{
	fprintf ( stderr, "\n" );
	fprintf ( stderr, "USAGE: JsonTree  file  [options]\n" );
	fprintf ( stderr, "options:\n" );
	fprintf ( stderr, "  -print         default behavior\n" );
	fprintf ( stderr, "  -csv           output as CSV file\n" );
	fprintf ( stderr, "  -rm            remove input file after reading\n" );
	fprintf ( stderr, "  -error         print input file on error\n" );
	fprintf ( stderr, "  -find x [...]  find one or more name and print only those values\n" );
	fprintf ( stderr, "  -where {sfld sval pfld} ... \n" );
	fprintf ( stderr, "                 where sfld = sval print pfld\n" );
	fprintf ( stderr, "  -script file   get -find or -where from script\n" );
	fprintf ( stderr, "  -d             debug\n" );
	fprintf ( stderr, "\n" );
	exit ( 1 );
}

long getargs ( int argc, char *argv[] )
{
	int		xa;
	long	FileSize, nr, nw;
	struct stat statbuf;
	unsigned char	Buffer[10240];

	if ( argc < 2 )
	{
		Usage ();
	}

	if ( strcmp ( argv[1], "-" ) == 0 )
	{
		isStdin = 1;
		sprintf ( InputFile, "/var/local/tmp/JsonTree_%d.json", getpid() );

		if (( fp = fopen ( InputFile, "w" )) == NULL )
		{
			fprintf ( stderr, "Cannot create %s for output\n", InputFile );
			exit ( 1 );
		}

		/*---------------------------------------------------------------------------
		   fread, fwrite - binary stream input/output
		   #include <stdio.h>
		   size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
		   size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
		---------------------------------------------------------------------------*/

		while (( nr = fread ( Buffer, 1, sizeof(Buffer), stdin )) > 0 )
		{
			nw = fwrite ( Buffer, 1, nr, fp );
			if ( nw != nr )
			{
				fprintf ( stderr, "fwrite error: %s\n", strerror(errno) );
				exit ( 1 );
			}
		}

		fclose ( fp );
	}
	else
	{
		isStdin = 0;
		sprintf ( InputFile, "%s", argv[1] );
	}

	stat ( InputFile, &statbuf );
	FileSize = statbuf.st_size;
	// fprintf ( stderr,File size: %ld bytes\n", FileSize );

	if (( fp = fopen ( InputFile, "r" )) == NULL )
	{
		fprintf ( stderr, "Cannot open %s\n", InputFile );
		exit ( 0 );
	}

	RunMode = MODE_PRINT;
	PrintFileOnError = 0;
	DeleteFile = 0;
	Debug = 0;

	for ( xa = 2; xa < argc; xa++ )
	{
		if ( strcmp ( argv[xa], "-print" ) == 0 )
		{
			RunMode = MODE_PRINT;
		}
		else if ( strcmp ( argv[xa], "-csv" ) == 0 )
		{
			RunMode = MODE_CSV;
		}
		else if ( strcmp ( argv[xa], "-error" ) == 0 )
		{
			PrintFileOnError = 1;
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
					fprintf ( stderr, "Exceeds MAXFIND %d\n", MAXFIND );
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
					fprintf ( stderr, "Exceeds MAXFIND %d\n", MAXFIND );
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
			fprintf ( stderr, "Unknown argument %s\n", argv[xa] );
		}
	}
	if ( RunMode == MODE_FIND && FindCount == 0 )
	{
		fprintf ( stderr, "No search criteria\n" );
		exit ( 1 );
	}
	else if ( RunMode == MODE_WHERE && WhereCount == 0 )
	{
		fprintf ( stderr, "No search criteria\n" );
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

	return ( FileSize );
}
