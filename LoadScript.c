/*---------------------------------------------------------------------------
	Copyright © 2022 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>

	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"JsonTree.h"

int LoadScript ( char *ScriptFileName )
{
	FILE	*fp;
	char	*delim = " \t\r\n";
	char	xbuffer[1024];
	char	*tokens[4];
	int		tokcnt;
	int		lineno = 0;

	if (( fp = fopen ( ScriptFileName, "r" )) == NULL )
	{
		printf ( "Cannot open script file %s\n", ScriptFileName );
		return ( 1 );
	}

	RunMode = 0;

	while ( fgets ( xbuffer, sizeof(xbuffer), fp ) != (char *)0 )
	{
		lineno++;

		/*---------------------------------------------------------------------------
			get from one to three tokens
		---------------------------------------------------------------------------*/
		tokcnt = 0;
		if (( tokens[tokcnt] = strtok ( xbuffer, delim )) == NULL )
		{
			continue;
		}
		else
		{
			tokcnt++;
			if (( tokens[tokcnt] = strtok ( NULL, delim )) != NULL )
			{
				tokcnt++;
				if (( tokens[tokcnt] = strtok ( NULL, delim )) != NULL )
				{
					tokcnt++;
				}
			}
		}

		if ( tokens[0][0] == '#' )
		{
			continue;
		}

		if ( RunMode == 0 && strcmp ( tokens[0], "find" ) == 0 )
		{
			RunMode = MODE_FIND;
		}
		else if ( RunMode == 0 && strcmp ( tokens[0], "where" ) == 0 )
		{
			RunMode = MODE_WHERE;
		}
		else if ( tokens[0][0] == '{' )
		{
			continue;
		}
		else if ( tokens[0][0] == '}' )
		{
			break;
		}
		else if ( RunMode == MODE_WHERE && tokcnt < 3 )
		{
			printf ( "Syntax error, line %d\n", lineno );
		}
		else if ( RunMode == MODE_FIND )
		{
			if ( FindCount >= MAXFIND )
			{
				printf ( "Exceeds MAXFIND %d\n", MAXFIND );
				exit ( 1 );
			}
			snprintf ( FindArray[FindCount], MAXSTRING, "%s", tokens[0] );
			FindCount++;
		}
		else if ( RunMode == MODE_WHERE )
		{
			if ( WhereCount >= MAXFIND )
			{
				printf ( "Exceeds MAXFIND %d\n", MAXFIND );
				exit ( 1 );
			}
			snprintf ( WhereArray[WhereCount].Field, MAXSTRING, "%s", tokens[0] );
			snprintf ( WhereArray[WhereCount].Value, MAXSTRING, "%s", tokens[1] );
			snprintf ( WhereArray[WhereCount].Name,  MAXSTRING, "%s", tokens[2] );
			WhereCount++;
		}
		else
		{
			printf ( "Ignoring %s\n", tokens[0] );
		}
	}

	fclose ( fp );

	if ( RunMode == 0 )
	{
		printf ( "Invalid script\n" );
		return ( 2 );
	}

	return ( 0 );
}
