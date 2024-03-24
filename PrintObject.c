/*---------------------------------------------------------------------------
	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>

	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"JsonTree.h"

void PrintObject ( struct json_object *Object )
{
	struct json_object_iterator	it;
	struct json_object_iterator	itEnd;
	const char					*name;
	json_type					type;
	struct json_object			*value;
	size_t						ArrayLength;
	const char 					*cp;
	long						intValue;
	double						dblValue;
	json_bool					boolValue;
	int							NeedNewline = 0;

	IndentValue += 2;

	it = json_object_iter_begin ( Object );
	itEnd = json_object_iter_end ( Object );

	while ( json_object_iter_equal ( &it, &itEnd ) == 0 )
	{
		name  = json_object_iter_peek_name(&it);
		value = json_object_iter_peek_value(&it);
		type  = json_object_get_type ( value );

		if ( Debug )
		{
			Indent(); printf ( "name: %s :: value type: %s\n", name, type_ascii(type) );
		}

		switch ( type )
		{
			case json_type_object:
				if ( Format == FORMAT_STD )
				{
					Indent(); printf ( "%s\n", json_object_iter_peek_name(&it) );
				}
				else if ( Format == FORMAT_CSV && NeedNewline )
				{
					NeedNewline = 0;
					printf ( "\n" );
				}
				PrintObject ( value );
				break;

			case json_type_array:
				if ( Format == FORMAT_STD )
				{
					Indent(); printf ( "%s\n", json_object_iter_peek_name(&it) );
				}
				else if ( Format == FORMAT_CSV && NeedNewline )
				{
					NeedNewline = 0;
					printf ( "\n" );
				}
				ArrayLength = json_object_array_length ( value );
				PrintArray ( value, ArrayLength );
				break;

			case json_type_boolean:
				boolValue = json_object_get_boolean ( value );
				if ( Format == FORMAT_STD )
				{
					Indent(); printf ( "%s:%s\n", json_object_iter_peek_name(&it), boolValue ? "true" : "false" );
				}
				else
				{
					printf ( "%s,", boolValue ? "true" : "false" );
					NeedNewline = 1;
				}
				break;

			case json_type_double:
				dblValue = json_object_get_double ( value );
				if ( Format == FORMAT_STD )
				{
					Indent(); printf ( "%s:%f\n", json_object_iter_peek_name(&it), dblValue );
				}
				else
				{
					printf ( "%f,", dblValue );
					NeedNewline = 1;
				}
				break;

			case json_type_int:
				intValue = json_object_get_int64 ( value );
				if ( Format == FORMAT_STD )
				{
					Indent(); printf ( "%s:%ld\n", json_object_iter_peek_name(&it), intValue );
				}
				else
				{
					printf ( "%ld,", intValue );
					NeedNewline = 1;
				}
				break;

			case json_type_string:
				cp = json_object_get_string ( value );
				if ( Format == FORMAT_STD )
				{
					Indent(); 
					if ( WrapLong == 0 )
					{
						printf ( "%s:%s\n", json_object_iter_peek_name(&it), cp );
					}
					else
					{
						char	Wrapper[32];
						int		asciiPercent = 37;
						sprintf ( Wrapper, "%cs:%c-.%ds\n", asciiPercent, asciiPercent, WrapLong );
						printf ( Wrapper, json_object_iter_peek_name(&it), cp );
					}
				}
				else
				{
					printf ( "%s,", cp );
					NeedNewline = 1;
				}
#ifdef NOT_USED
				len = json_object_get_string_len ( value );
				if ( len > 60 )
				{
					int		num_out, Indent2;

					Indent(); 
					Indent2 = IndentValue + printf ( "%s:", json_object_iter_peek_name(&it) );
					num_out = 0;
					while ( *cp )
					{
						printf ( "%c", *cp );
						cp++;
						num_out++;
						if ( num_out > 60 && *cp == ' ' )
						{
							num_out = 0;
							printf ( "\n" );
							printf ( "%*.*s", Indent2, Indent2, " " );
						}
					}
					printf ( "\n" );
				}
				else
				{
					Indent(); printf ( "%s:%s\n", json_object_iter_peek_name(&it), cp );
				}
#endif
				break;

			case json_type_null:
				if ( Format == FORMAT_STD )
				{
					Indent(); printf ( "%s:null\n", json_object_iter_peek_name(&it) );
				}
				else
				{
					printf ( "null," );
				}
				break;

			default:
				break;
		}

		json_object_iter_next ( &it );
	}

	if ( Format == FORMAT_CSV )
	{
		printf ( "\n" );
	}

	IndentValue -= 2;
}
