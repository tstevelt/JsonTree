/*---------------------------------------------------------------------------
	Copyright © 2022 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>

	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"JsonTree.h"

void ProcessArray ( struct json_object *Array, size_t ArrayLength )
{
	json_type		type;
	json_object		*Elem;
	size_t			ndx;
	const char		*cp;
	long			intValue;
	double			dblValue;
	json_bool		boolValue;

	IndentValue += 2;
	for ( ndx = 0; ndx < ArrayLength; ndx++ )
	{
		Elem = json_object_array_get_idx ( Array, ndx );
		type = json_object_get_type ( Elem );

		if ( Debug )
		{
			Indent (); printf ( "ndx %ld, type %s\n", ndx, type_ascii(type) );
		}

		switch ( type )
		{
			case json_type_object:
				ProcessObject ( Elem );
				break;

			case json_type_array:
				ArrayLength = json_object_array_length ( Elem );
				ProcessArray ( Elem, ArrayLength );
				break;

			case json_type_string:
				cp = json_object_get_string ( Elem );
				Indent(); printf ( "%s\n", cp );
				break;

			case json_type_boolean:
				boolValue = json_object_get_boolean ( Elem );
				Indent(); printf ( "%s\n", boolValue ? "true" : "false" );
				break;

			case json_type_double:
				dblValue = json_object_get_double ( Elem );
				Indent(); printf ( "%f\n", dblValue );
				break;

			case json_type_int:
				intValue = json_object_get_int64 ( Elem );
				Indent(); printf ( "%ld\n", intValue );
				break;

			case json_type_null:
				Indent(); printf ( "null\n" );
				break;

		}
	}

	IndentValue -= 2;

}
