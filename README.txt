Read a JSON file or JSON from stdin.   
Output as indented tree or CSV file.
Supports decent FIND feature to find things in the file and print their values.
Also has a WHERE feature which looks for fields with certain values and then
printf another field.  This feature needs a lot of improvment.

USAGE: JsonTree  file  [options]
options:
  -print         default behavior
  -csv           output as CSV file
  -rm            remove input file after reading
  -error         print input file on error
  -find x [...]  find one or more name and print only those values
  -where {sfld sval pfld} ... 
                 where sfld = sval print pfld
  -script file   get -find or -where from script
  -d             debug

See the Demo.sh for examples.

