# Parallel BFS

This is an implementation of parallel breadth first search in a shared memeory setting using the ideas from the paper "Ligra: A Lightweight Graph Processing
Framework for Shared Memory". In particular, it implements the edgeMap, edgeMapDense, and edgeMapSparse algorithms to acheive upwards of X10 speedup from a sequential version of bfs.

# Running the code

## Dependencies

You must have the following libraries installed on your system:

- g++ >= 5.3.0
- Cilk Plus

## Building the Project

There is a make file provided in the repository. To build the code just type in 

```shell
make
```

in your terminal.

## Running the Project

The executable takes in the following parameters:

- -g: The path to the graph. This is set to "tests/test1_sample.txt" by default.
- -s: The starting node of the bfs. This is set to "0" by default.

For example, if you want to run the bfs code on the second test graph, you would type the following into the terminal.

```shell
./bfs -g tests/test2_noedge/txt -s 0
```

## Graph Representation

The input graphs that the code takes in are represented in the Compressed Sparse Row (CSR) format. With the CSR format, the graphs are represented similar to an adjacency list. The first entry will be the graph type. This is not important for this project. Then the number of nodes, n, and the number of edges, m, will follow. The next n numbers represent the number of neighbors that the i-th vertex will have. This is usually called the offset. Then the next m numbers will represent the actual nodes that each 

![CSR Diagram](https://user-images.githubusercontent.com/24824185/131555604-62913731-a5fc-4d59-b4a6-93f1597af75a.png)

```shell
GraphType
<n> // The number of nodes
<m> // The number of edges
<o_0>
.
.
.
<o_(n-1)>
<e_0>
.
.
.
<e_(n-1)>
```

# References

Y. Shun, G. Blelloch. [Ligra: A Lightweight Graph Processing Framework for Shared Memory](https://www.cs.cmu.edu/~jshun/ligra.pdf)

