# Warning:
Could not insert hardware watchpoint 2.
Could not insert hardware watchpoint 3.
Could not insert hardware breakpoints:
You may have requested too many hardware breakpoints/watchpoints.

Command aborted.

## solved:
```vbnet
set can-use-hw-watchpoints off
```