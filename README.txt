Read a JSON file or JSON from stdin.   
Output as indented tree or CSV file.
Supports FIND feature to find things in the file and print their values.
Also has a WHERE feature which looks for fields with certain values and then
print another field.  This feature needs a lot of improvment.

*** Sat Nov  4 15:07 EDT 2023 ***

* Bug fixes to CSV output in FIND mode.

* Added -array option to print only first or last element of array.

* Added DemoEdgar.sh and Json file for AXP (American Express).
  File is named 0000004962.json, which is the SEC Central Index Number for AXP.

USAGE: JsonTree file|- [options]
options:
  -print         default behavior
  -fmt {std|csv} Indented Standare or CSV
  -rm            remove input file after reading
  -error         print input file on error
  -find x [...]  find one or more name and print only those values
  -array {first|last|all} (currently only with -find}
  -where {sfld sval pfld} ... 
                 where sfld = sval print pfld
  -script file   get -find or -where from script
  -d             debug


*** Fri Nov  3 14:10 EDT 2023 ***

* See the Demo.sh for examples.

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


