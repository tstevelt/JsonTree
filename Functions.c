/*---------------------------------------------------------------------------
	Copyright © 2022 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>

	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"JsonTree.h"

void Indent ()
{
	printf ( "%*.*s", IndentValue, IndentValue, " " );
}

char *type_ascii ( json_type type )
{
	switch ( type )
	{
		case json_type_null: return ( "null" );
		case json_type_boolean: return ( "boolean" );
		case json_type_double: return ( "double" );
		case json_type_int: return ( "int" );
		case json_type_object: return ( "object" );
		case json_type_array: return ( "array" );
		case json_type_string: return ( "string" );
	}
	return ( "unknown" );
}

