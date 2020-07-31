# Toy OS

## RoundRobin

Take process from queue.

Execute in some ticks.

If still executing, put it to the back.

If finished, pop from queue.

## Variables

## Lifecycle

- allocate in `new`

### allocation

- takes first index from `unused_variables`
- take address of a variable from `arena` with the index
- return the address

### get_allocated

Return all used addresses of variables.

### kill_process

Substract from a pointer of a variable to a pointer of an array.
