# Rule
## Info
Rules to define gamestate behaviour

## Structure
|Name|Behaviour|
|:-|:-|
|Condition|pure boolean checks|
|Operation|“what should happen” requests|
|ActionQueue|ordered list of pending operations|
|ActionProcessor|executes operations and mutates game state|

## Link