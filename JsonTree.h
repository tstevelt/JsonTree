
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include	<json-c/json.h>

#ifdef MAIN
#define	TYPE	/* */
#else
#define	TYPE	extern
#endif

#define		MODE_PRINT	1
#define		MODE_FIND	2
#define		MODE_WHERE	3
TYPE	int		RunMode;

#define		FORMAT_STD	1
#define		FORMAT_CSV	2
TYPE	int		Format;

#define		ARRAY_LAST	1
#define		ARRAY_FIRST	2
#define		ARRAY_ALL	3
TYPE	int		ArrayIndex;

TYPE	char	InputFile[512];
TYPE	int		isStdin;
TYPE	FILE	*fp;
TYPE	int		PrintFileOnError;
TYPE	int		DeleteFile;
TYPE	int		Debug;
TYPE	int		IndentValue;

#define		MAXSTRING	128
#define		MAXFIND		20
TYPE	char	FindArray[MAXFIND][MAXSTRING];
TYPE	int		FindCount;

typedef struct
{
	char	Field[MAXSTRING];
	char	Value[MAXSTRING];
	char	Name[MAXSTRING];
} WHERE_RECORD;

TYPE	WHERE_RECORD	WhereArray[MAXFIND];
TYPE	int				WhereCount;

/* Functions.c */
void Indent ( void );
char *type_ascii ( json_type type );

/* getargs.c */
long getargs ( int argc , char *argv []);

/* JsonTree.c */
int main ( int argc , char *argv []);

/* LoadScript.c */
int LoadScript ( char *ScriptFileName );

/* PrintArray.c */
void PrintArray ( struct json_object *Array , const size_t ArrayLength );

/* ProcessArray.c */
void ProcessArray ( struct json_object *Array , const size_t ArrayLength );

/* PrintObject.c */
void PrintObject ( struct json_object *Object );

/* ProcessObject.c */
void ProcessObject ( struct json_object *Object );
