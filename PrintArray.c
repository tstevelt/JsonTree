/*---------------------------------------------------------------------------
	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>

	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"JsonTree.h"

void PrintArray ( struct json_object *Array, const size_t ArrayLength )
{
	json_type		type;
	json_object		*Elem;
	size_t			ndx;
	const char		*cp;
	long			intValue;
	double			dblValue;
	json_bool		boolValue;
	size_t			MyArrayLength;
	int				ArrayStart, ArrayEnd;

	switch ( ArrayIndex )
	{
		case ARRAY_FIRST:
			ArrayStart = 0;
			ArrayEnd = 1;
			break;
		case ARRAY_LAST:
			ArrayStart = ArrayLength - 1;
			ArrayEnd = ArrayLength;
			break;
		case ARRAY_ALL:
			ArrayStart = 0;
			ArrayEnd = ArrayLength;
			break;
	}

	IndentValue += 2;
	for ( ndx = ArrayStart; ndx < ArrayEnd; ndx++ )
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
				PrintObject ( Elem );
				break;

			case json_type_array:
				MyArrayLength = json_object_array_length ( Elem );
				PrintArray ( Elem, MyArrayLength );
				break;

			case json_type_string:
				cp = json_object_get_string ( Elem );
				if ( Format == FORMAT_STD )
				{
					Indent(); printf ( "%s\n", cp );
				}
				else
				{
					printf ( "%s,", cp );
				}
				break;

			case json_type_boolean:
				boolValue = json_object_get_boolean ( Elem );
				if ( Format == FORMAT_STD )
				{
					Indent(); printf ( "%s\n", boolValue ? "true" : "false" );
				}
				else
				{
					printf ( "%s,", boolValue ? "true" : "false" );
				}
				break;

			case json_type_double:
				dblValue = json_object_get_double ( Elem );
				if ( Format == FORMAT_STD )
				{
					Indent(); printf ( "%f\n", dblValue );
				}
				else
				{
					printf ( "%f,", dblValue );
				}
				break;

			case json_type_int:
				intValue = json_object_get_int64 ( Elem );
				if ( Format == FORMAT_STD )
				{
					Indent(); printf ( "%ld\n", intValue );
				}
				else
				{
					printf ( "%ld,", intValue );
				}
				break;

			case json_type_null:
				if ( Format == FORMAT_STD )
				{
					Indent(); printf ( "null\n" );
				}
				else
				{
					printf ( "null," );
				}
				break;

		}

		if ( Format == FORMAT_CSV )
		{
			printf ( "\n" );
		}
	}

	IndentValue -= 2;

}
