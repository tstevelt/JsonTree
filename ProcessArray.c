/*---------------------------------------------------------------------------
	Copyright © 2022 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>

	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"JsonTree.h"

void ProcessArray ( struct json_object *Array, const size_t ArrayLength )
{
	json_type		type;
	json_object		*Elem;
	size_t			ndx;
	const char		*cp;
	long			intValue;
	double			dblValue;
	json_bool		boolValue;
	size_t			MyArrayLength;

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
				MyArrayLength = json_object_array_length ( Elem );
				ProcessArray ( Elem, MyArrayLength );
				break;

			case json_type_string:
				if ( RunMode == MODE_PRINT )
				{
					cp = json_object_get_string ( Elem );
					Indent(); printf ( "%s\n", cp );
				}
				break;

			case json_type_boolean:
				if ( RunMode == MODE_PRINT )
				{
					boolValue = json_object_get_boolean ( Elem );
					Indent(); printf ( "%s\n", boolValue ? "true" : "false" );
				}
				break;

			case json_type_double:
				if ( RunMode == MODE_PRINT )
				{
					dblValue = json_object_get_double ( Elem );
					Indent(); printf ( "%f\n", dblValue );
				}
				break;

			case json_type_int:
				if ( RunMode == MODE_PRINT )
				{
					intValue = json_object_get_int64 ( Elem );
					Indent(); printf ( "%ld\n", intValue );
				}
				break;

			case json_type_null:
				if ( RunMode == MODE_PRINT )
				{
					Indent(); printf ( "null\n" );
				}
				break;

		}
	}

	IndentValue -= 2;

}
