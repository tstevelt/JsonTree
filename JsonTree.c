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
	tms		11/02/2023	Get file size and malloc xbuffer

----------------------------------------------------------------------------*/

#define		MAIN
#include	"JsonTree.h"

int main ( int argc, char *argv[] )
{
	char				*xbuffer;
	struct json_object	*root;
	json_type			type;
	json_tokener		*tok;
	size_t				ArrayLength;
	long				FileSize = 0L;

	FileSize = getargs ( argc, argv );

	xbuffer = malloc ( FileSize );
	fread ( xbuffer, 1, FileSize, fp );

	if ( isStdin == 0 )
	{
		fclose ( fp );
	}
	else if ( DeleteFile || isStdin )
	{
		unlink ( InputFile );
	}

	tok = json_tokener_new();

	if (( root = json_tokener_parse_ex ( tok, xbuffer, FileSize )) == NULL )
	{
		printf ( "json_tokener_parse failed\n" );
		enum json_tokener_error jerr = json_tokener_get_error (tok);
		printf ( "%s\n", json_tokener_error_desc(jerr) );

		if ( PrintFileOnError && isStdin == 0 )
		{
			char	cmdline[1024];
			sprintf ( cmdline, "cat %s", InputFile );
			system ( cmdline );
		}
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
					Indent(); printf ( "root array length %ld\n", ArrayLength );
				}
				ProcessArray ( root, ArrayLength );
				break;

			default:
				Indent(); printf ( "root is neither object or array, it is type %d\n", type );
				exit ( 1 );
		}
	}

	free ( xbuffer );
	return ( 0 );
}
