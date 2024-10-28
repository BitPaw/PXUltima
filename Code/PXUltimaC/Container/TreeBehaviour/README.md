#  Behaviour-Tree
## Info
A container that holds scripting instructions for entitys.<br>
Similar to a hierarchical state machine, it describes a flow of conditions and actions. 

## Structure 
<p>
  Nodes of the tree are instructuions that define the behaviour of their children.
</p>
<p>
  Every node will return its current state. This value indicates if the action has succeeded, failed or is still running.
</p>
<p>
  A Task may not be compleated in one call Most implementations use a tick 
  based system, where each entiry will go through this behaviour tree each tick und execute tasks that are next to be executed. 
</p>

### Nodes
Those can be one of the following, or more:
  
#### Task Sequence
  A list of tasks that will be executed one after another.<br>
  If one task fails, the chain of events is cancelled and does return to the parent.<br>
  Use this for actions that need to happen in order, that depend on each other.
#### Task Set
  A list of tasks that will be executed at once in an ASYNC way. The order is irrelevant.<br>
  Failures must be handled by the parent.<br>
  Use this when tasks can be executed in parrallel, like searching or general calculating or computing of solutions.
#### Selector/fallback
  A list of tasks, where the first non-failing node will end the interation.<br>
  If no action succeded, the node fails.
  Use this when you have multible options to choose from and take the first option in order.

#### Loop/Repeat
  Use this to repeat the child nodes.

#### Condition
  Use this to let nodes be executed or skipped depending on the given condition. 

#### Inverter
  Negates the input and can be used if a condition cannot be inverted or would yield a too big change.

#### Sleep
  Delay the behaviour by some amount or a condition. Beware that this does not freeze the whole thread!

## Links
- [Wikipedia - Behavior Tree](https://en.wikipedia.org/wiki/Behavior_tree_(artificial_intelligence,_robotics_and_control))
