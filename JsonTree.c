/*----------------------------------------------------------------------------
	Program : JsonTree.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Sep 2022
	Synopsis: Read JSON from file or stdin.  Print indented tree.

	Copyright © 2022 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>

	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.

	Who		Date		Modification
	---------------------------------------------------------------------
	tms		09/13/2022	Refactored into modules.  Added -find option.
	tms		09/14/2022	Added -where and -script options.

----------------------------------------------------------------------------*/

#define		MAIN
#include	"JsonTree.h"

int main ( int argc, char *argv[] )
{
	char	xbuffer[20000];
	struct json_object	*root;
	json_type					type;
	size_t						ArrayLength;

	getargs ( argc, argv );

	memset ( xbuffer, '\0', sizeof(xbuffer) );
	fread ( xbuffer, 1, sizeof(xbuffer), fp );

	if ( isStdin == 0 )
	{
		fclose ( fp );
	}
	else if ( DeleteFile )
	{
		unlink ( InputFile );
	}

	if (( root = json_tokener_parse ( xbuffer )) == NULL )
	{
		printf ( "json_tokener_parse failed\n" );
	}
	else
	{
		type = json_object_get_type ( root );

		switch ( type )
		{
			case json_type_object:
				ProcessObject ( root );
				break;

			case json_type_array:
				ArrayLength = json_object_array_length ( root );
				if ( Debug )
				{
					Indent(); printf ( "array length %ld\n", ArrayLength );
				}
				ProcessArray ( root, ArrayLength );
				break;

			default:
				Indent(); printf ( "root is neither object or array, it is type %d\n", type );
				exit ( 1 );
		}
	}

	return ( 0 );
}
