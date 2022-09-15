/*---------------------------------------------------------------------------
	Copyright © 2022 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>

	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"JsonTree.h"

void ProcessObject ( struct json_object *Object )
{
	struct json_object_iterator	it;
	struct json_object_iterator	itEnd;
	const char					*name, *name1, *name2;
	json_type					type, type1, type2;
	struct json_object			*value, *value1, *value2;
	size_t						ArrayLength;
	const char *cp;
	int 	ndx, len, found;
	long	intValue;
	double	dblValue;
	json_bool	boolValue;

	IndentValue += 2;

	it = json_object_iter_begin ( Object );
	itEnd = json_object_iter_end ( Object );

	while ( json_object_iter_equal ( &it, &itEnd ) == 0 )
	{
		name  = json_object_iter_peek_name(&it);
		value = json_object_iter_peek_value(&it);
		type  = json_object_get_type ( value );

		if ( RunMode == MODE_FIND )
		{
			for ( ndx = 0, found = 0; ndx < FindCount; ndx++ )
			{
				if ( strcmp ( name, FindArray[ndx] ) == 0 )
				{
					printf ( "%s : ", name );
					found = 1;
					break;
				}
			}

			switch ( type )
			{
				case json_type_object:
					if ( found )
					{
						printf ( "object\n" );
					}
					ProcessObject ( value );
					break;

				case json_type_array:
					ArrayLength = json_object_array_length ( value );
					if ( found )
					{
						printf ( "array[%ld]\n", ArrayLength );
					}
					ProcessArray ( value, ArrayLength );
					break;

				case json_type_boolean:
					if ( found )
					{
						boolValue = json_object_get_boolean ( value );
						printf ( "%s\n", boolValue ? "true" : "false" );
					}
					break;

				case json_type_double:
					if ( found )
					{
						dblValue = json_object_get_double ( value );
						printf ( "%f\n", dblValue );
					}
					break;

				case json_type_int:
					if ( found )
					{
						intValue = json_object_get_int64 ( value );
						printf ( "%ld\n", intValue );
					}
					break;

				case json_type_string:
					if ( found )
					{
						cp = json_object_get_string ( value );
						printf ( "%s\n", cp );
					}
					break;

				case json_type_null:
					if ( found )
					{
						printf ( "null\n" );
					}
					break;

				default:
					break;
			}
		}
		else if ( RunMode == MODE_WHERE )
		{
			name1  = json_object_iter_peek_name(&it);
			value1 = json_object_iter_peek_value(&it);
			type1  = json_object_get_type ( value1 );

			switch ( type1 )
			{
				case json_type_object:
					ProcessObject ( value1 );
					json_object_iter_next ( &it );
					continue;

				case json_type_array:
					ArrayLength = json_object_array_length ( value1 );
					ProcessArray ( value1, ArrayLength );
					json_object_iter_next ( &it );
					continue;

				case json_type_string:
					cp = json_object_get_string ( value1 );
					break;

				default:
					if ( Debug )
					{
						printf ( "%s type %s not supported at this time\n", name1, type_ascii(type1) );
					}
					json_object_iter_next ( &it );
					continue;
			}

			json_object_iter_next ( &it );
					
			name2  = json_object_iter_peek_name(&it);
			value2 = json_object_iter_peek_value(&it);
			type2  = json_object_get_type ( value2 );

			if ( Debug )
			{
				printf ( "%s %s %s\n", name1, cp, name2 );
			}

			for ( ndx = 0, found = 0; ndx < WhereCount; ndx++ )
			{
				if (( strcmp ( name1, WhereArray[ndx].Field ) == 0 ) &&
					( strcmp ( cp,    WhereArray[ndx].Value ) == 0 ))
				{
					printf ( "%s : ", cp );
					found = 1;
					break;
				}
			}

			switch ( type2 )
			{
				case json_type_object:
					if ( found )
					{
						printf ( "object\n" );
					}
					ProcessObject ( value2 );
					break;

				case json_type_array:
					ArrayLength = json_object_array_length ( value2 );
					if ( found )
					{
						printf ( "array[%ld]\n", ArrayLength );
					}
					ProcessArray ( value2, ArrayLength );
					break;

				case json_type_boolean:
					if ( found )
					{
						boolValue = json_object_get_boolean ( value2 );
						printf ( "%s\n", boolValue ? "true" : "false" );
					}
					break;

				case json_type_double:
					if ( found )
					{
						dblValue = json_object_get_double ( value2 );
						printf ( "%f\n", dblValue );
					}
					break;

				case json_type_int:
					if ( found )
					{
						intValue = json_object_get_int64 ( value2 );
						printf ( "%ld\n", intValue );
					}
					break;

				case json_type_string:
					if ( found )
					{
						cp = json_object_get_string ( value2 );
						printf ( "%s\n", cp );
					}
					break;

				case json_type_null:
					if ( found )
					{
						printf ( "null\n" );
					}
					break;

				default:
					break;
			}
		}
		else if ( RunMode == MODE_PRINT )
		{
			if ( Debug )
			{
				Indent(); printf ( "name: %s :: value type: %s\n", json_object_iter_peek_name(&it), type_ascii(type) );
			}

			switch ( type )
			{
				case json_type_object:
					Indent(); printf ( "%s\n", json_object_iter_peek_name(&it) );
					ProcessObject ( value );
					break;

				case json_type_array:
					Indent(); printf ( "%s\n", json_object_iter_peek_name(&it) );
					ArrayLength = json_object_array_length ( value );
					ProcessArray ( value, ArrayLength );
					break;

				case json_type_boolean:
					boolValue = json_object_get_boolean ( value );
					Indent(); printf ( "%s:%s\n", json_object_iter_peek_name(&it), boolValue ? "true" : "false" );
					break;

				case json_type_double:
					dblValue = json_object_get_double ( value );
					Indent(); printf ( "%s:%f\n", json_object_iter_peek_name(&it), dblValue );
					break;

				case json_type_int:
					intValue = json_object_get_int64 ( value );
					Indent(); printf ( "%s:%ld\n", json_object_iter_peek_name(&it), intValue );
					break;

				case json_type_string:
					cp = json_object_get_string ( value );
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
					break;

				case json_type_null:
					Indent(); printf ( "%s:null\n", json_object_iter_peek_name(&it) );
					break;

				default:
					break;
			}
		}

		json_object_iter_next ( &it );
	}

	IndentValue -= 2;
}
