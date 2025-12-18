# Structured Query Language (SQL)<br>[ISO/IEC 9075]

## Info
Used for statements that define a spesific selection of data from a database.

## Structure

## Known issues and exploits 
### SQL injection
Terminate an expected SQL querry early to chain a custom SQL querry to be executes instead. 
If not checked, a user could execute any command at will. 

## Optimization
### Querry time
Querrys can be slowed down by what you pull and how you do so.<br>

```SQL
EXPLAIN QUERY PLAN SELECT * FROM <tablename>;
```

This lets you see the sub commands for your sql commands and more interesting your views.
With this you have more vision on what is slowing the querry down.

When you see __SCAN__ then this means you are doing a full table scan which is not optimized for this kind of access.
If you just do a "select * from x" then this is a correct use. When we do a selection like __WHERE__ then this is a sign of trouble.
A solution is to build an index for the thing you compare against, this allows the querry to use an internal optimized lookup which increases speed significantly.
