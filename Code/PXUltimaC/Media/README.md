# Pascal 
## Info
Pascal has a preprocessor.
## Syntax
### Basic program
```pascal
programm XXXX

begin
  XXXX
end
```
### Comments
```pascal
// A line as a comment
```
or
```pascal
{Mutible lines as a comment} 
```
### Variables
```pascal
var xxxx: Integer;
```
### Condition
```pascal
if XXXX then
  XXXX
else
  XXXX
```
### Loops
```pascal
for i:=0 to 10
  XXXX


while XXXX do
  XXXXX


repeat

until XXXX

```
### Arrays
```pascal
var myArray : array[1..5] of Integer
var XXXXX : array[min..max] of Type
```
### Class
```pascal
type 
  MyClass=class
  XXXX
end
```

### Exceptions
```pascal
try XXXX
  XXXX
except
  XXXX
end;
```
### Functions
Those can be stacked, o create private functions that are not visible outside the parent function.
Prcedures are functions that do not return anything.
```pascal
procedure XXXX(...)

begin
  XXXX
end
```
Functions always return something 
```pascal
function XXXX(...) : Type

begin
  XXXX
end
```
### Switch
```pascal
...
```

### Preprocessor
A preprocessor directive is closely the same as a comment with an added hashtag. 
```pascal
{#XXXX}
```
