/*
 * Program: mopsolver.c
 * --------------------
 * C program that parses, solves, and outputs the solution
 * to mazes
 *
 * @author Brennan Reed
 */

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <getopt.h>
#include <ctype.h>
#include "mopsolver.h"
#include "queue.h"

/**
 * struct: Maze_S
 * --------------
 *  Structure representation of a maze
 */

struct Maze_S{
    size_t rows, columns;
    char** array;
};

/**
 * struct: Node_S
 * --------------
 * A single node in the queue
 */

struct Node_S{
    size_t row, column;
    Node *previousPath;
    Node *nextQueue;
    int distance;
};

/**
 * struct: Queue_S
 * ----------------
 * Queue data structure consisting of Nodes
 */

struct Queue_S{
    size_t size;
    Node *front, *rear;
};

/**
 * Function: createNode
 * --------------------
 * Creates and returns a node
 *
 * @param row the row the node is located
 * @param column the column the row is located
 * @param before the node before the node in the queue
 * @return created node
 */

Node* createNode(size_t row, size_t column, int distance, Node* previous){
        Node* new = malloc(sizeof( struct Node_S));
        if (new != 0){
                new->row = row;
                new->column = column;
                if (previous != NULL)
                        new->previousPath = previous;
                else
                        new->previousPath = NULL;
                new->nextQueue = NULL;
                new->distance = distance;
	}
        assert(new);
        return new;
}

/**
 * Function: createQueue
 * ----------------------
 * Creates and returns an empty queue
 *
 * @return empty queue
 */

Queue createQueue(){
        Queue new = malloc(sizeof(struct Queue_S));
        if (new != 0){
                new->size = 0;
                new->front = NULL;
                new->rear = NULL;
        }
        assert(new);
        return new;
}

/**
 * Function: createMaze
 * --------------------
 *  Creates and returns a maze structure
 *
 *  @param rows number of rows
 *  @param columns number of columns
 *  @param array character array containing maze data
 */

Maze createMaze(size_t rows, size_t columns, char** array){
	Maze new = malloc(sizeof(struct Maze_S));
	if (new != 0){
		new->rows = rows;
		new->columns = columns;
		new->array = array;
	}
	assert(new);
	return new;
}

/**
 * Function: destroyNode
 * ---------------------
 * Deletes all dynamic allocate storage for a Node
 *
 * @param node the node being destroyed
 */

void destroyNode(Node* node){
        assert(node);
        free(node);

}

/**
 * Function: destroyQueue
 * -----------------------
 * Deletes all dynamic queue storage
 */

void destroyQueue(Queue queue){
	free(queue);
}

/**
 * Function: destroyMaze
 * -----------------------
 * Deletes all dynamic maze storage
 *
 * @param maze the maze being destroyed
 */

void destroyMaze(Maze maze){
        assert(maze);
        for (size_t row = 0; row < maze->columns; row++){
                free(maze->array[row]);
        }
	free(maze->array);
        free(maze);
}

/**
 * Function: isEmpty
 * ----------------
 * Checks to see if the specified queue is empty
 *
 * @param queue the subject queue
 * @return false if empty : else true
 */

bool isEmpty(Queue queue){
        if (queue->front != NULL)
                return false;
        else
                return true;
}

/**
 * Function: enQueue
 * -----------------
 * Adds a key to the queue
 *
 * @param queue pointer to the specified queue
 * @param k key to be added
 */

void enQueue(Queue queue, Node *node){
        assert(queue);
        if (queue->size == 0){
                queue->front = node;
                queue->rear = node;
                queue->size++;
        } else {
                Node *temp = queue->rear;
                temp->nextQueue = node;
                queue->rear = node;
                queue->size++;
        }
}

/**
 * Function: deQueue
 * ------------------
 * Removes a key from a given queue
 *
 * @param queue pointer to the specified queue
 * @return the corresponding queue
 */

Node *deQueue(Queue queue){
        assert(queue && queue->size > 0);
        if (queue->front != NULL){
                Node * temp = queue->front;
                queue->front = temp->nextQueue;
                queue->size--;
                if (queue->size == 0)
                        queue->rear = NULL;
                return temp;
        } else if (queue->front == NULL)
                queue->rear = NULL;
        return NULL;
}

/**
 * Function: parseMaze
 * -------------------
 * Creates and returns the Maze specified by user input
 *
 * @param input source of input
 * @return created maze
 */

Maze parseMaze(FILE * input){
        size_t currentLine = 0;
        char *line = NULL;
	size_t len = 0;
        char** array = NULL;
        size_t column = 0;
	ssize_t nread;
        size_t columns = 0, rows = 0;

        while ((nread = getline(&line, &len, input)) != -1) {
                column = 0;
                if (currentLine == 0) {
                        for (ssize_t i = 0; i < nread; i++){
                               if (isdigit(line[i]) != 0)
                                       columns++;
                        }
                        
                        array = malloc(columns * sizeof(char*));
                        for (size_t i = 0; i < columns; i++)
                                array[i] = malloc(columns * sizeof(char*));
                }

                for (ssize_t i = 0; i < nread; i++){
                        if (isdigit(line[i]) != 0){
                                array[currentLine][column] = line[i];
                                column++;
                        }
                        if (column == columns){
                                break;
                        }
                }
                currentLine++;
        }
	if (line != NULL)
		free(line);
        rows = currentLine;
        Maze maze = createMaze(rows, columns, array);
        assert(maze != NULL);
        return maze;
}

/**
 * Function: validPoint
 * --------------------
 * Checks to see if a specified point in the maze is valid
 *
 * @param maze subject maze
 * @param row row value of the point
 * @param column column value of the point
 * @return true if valid : else false
 */

bool validPoint(Maze maze, int row, int column){
        size_t rows = maze->rows;
        size_t columns = maze->columns;
        if (row < 0 || column < 0 || row >= (int)rows || column >= (int)columns)
                return false;
        else
                if (maze->array[row][column] == '0')
                        return true;
                else
                        return false;
}

/**
 * Function: stepCount
 * -------------------
 * Uses a BFS to calculate the fewest number of steps necessary to solve a maze
 *
 * @return the fewest number of steps required to solve the maze
 */

int stepCount(Maze maze, size_t sRow,  size_t sColumn, bool optimalPath){
        int distance = 0;
	size_t rows = maze->rows;
	size_t nodesCreated = 0;
        size_t columns = maze->columns;
        int rowNum[] = {-1, 0, 0, 1};
        int colNum[] = {0, -1, 1, 0};
	bool solution = false;
        
	if (maze->array[sRow][sColumn] != '0' || maze->array[rows-1][columns-1] != '0')
	       	return distance;

	Node** nodes = malloc(rows * columns * sizeof(struct Node_S));
	assert (nodes != 0);
        bool ** visited = calloc(rows, sizeof(bool *));
        for (size_t row = 0; row < rows; row++){
                visited[row] = calloc(columns, sizeof(bool *));
        }
        visited[sRow][sColumn] = true;
        Queue queue = createQueue();
        Node* current = createNode(sRow, sColumn, 1, NULL);
        nodes[nodesCreated] = current;
	nodesCreated++;
	enQueue(queue, current);
        
	while (isEmpty(queue) == false){
                current = deQueue(queue);
                if (current->row == rows-1 && current->column == columns-1){
			for (size_t row = 0; row < rows; row++)
				free(visited[row]);
			free(visited);
			solution = true;
			break;
                }
                for (int i = 0; i < 4; i++){
                        int row = (int)current->row + rowNum[i];
                        int column = (int)current->column + colNum[i];

                        if (validPoint(maze, row, column) == true && visited[row][column] == false){
                                visited[row][column] = true;
                                Node* neighbor = createNode((size_t)row , (size_t)column , current->distance+1, current);
                                nodes[nodesCreated] = neighbor;
				nodesCreated++;
			       	enQueue(queue, neighbor);
                        }
                }
        }
	if (solution == true){
		distance = current->distance;
		if (optimalPath == true){
			while (current != NULL){
				maze->array[current->row][current->column] = '+';
				current = current->previousPath;
			}
		}
		for (size_t i = 0; i < nodesCreated; i++){
			Node* node = nodes[i];
			destroyNode(node);
		}
		free(nodes);
		destroyQueue(queue);
		return distance;
	} else{ 
		for (size_t row = 0; row < rows; row++)
			free(visited[row]);
		free(visited);
		for (size_t i = 0; i < nodesCreated; i++)
			destroyNode(nodes[i]);
		free(nodes);
		destroyQueue(queue);
        return distance;
	}
}

/**
 * Function: prettyPrint
 * ---------------------
 * Pretty-prints a maze with a nifty border and ascii graphics
 *
 * @param maze maze to be printed
 */

void prettyPrint(Maze maze, FILE* output){
        size_t rows = maze->rows;
        size_t columns = maze->columns;
        for (size_t row = 0; row < rows; row++) {
                if (row == 0) {
                        for (size_t i = 0; i < columns; i++) {
                                if (i == 0)
                                        fprintf(output, "|-");
                                fprintf(output,"--");
                                if (i == columns - 1)
                                        fprintf(output,"|\n");
                        }
                }
                if (row == 0)
                        fprintf(output," ");
                else
                        fprintf(output,"|");
                for (size_t column = 0; column < columns; column++) {
                        if (maze->array[row][column] == '0')
                                fprintf(output, " .");
                        else if (maze->array[row][column] == '1')
                                fprintf(output, " #");
                        else
                                fprintf(output, " %c", maze->array[row][column]);
                }
                if (row == rows - 1) {
                        fprintf(output," \n");
                        for (size_t i = 0; i < columns; i++) {
                                if (i == 0)
                                        fprintf(output,"|-");
                                fprintf(output,"--");
                                if (i == columns - 1)
                                        fprintf(output,"|\n");
                        }
                } else
                        fprintf(output," |\n");
        }
}

/**
 * Function: main
 * --------------
 * Controls the core logic of the program
 *
 * @return 0 if successful
 */

int main(int argc, char* argv[]) {
        int opt;
        FILE *infile= stdin, *outfile = stdout;
        bool pretty = false, shortestSolution = false, optimalPath = false;

        while ((opt = getopt(argc, argv, "hdspi:o:")) != -1){
                switch (opt){
                        case 'h':
                                printf("This helpful message.\n");
                                return 0;
                                break;
                        case 'd':
                                pretty = true;
                                break;
                        case 's':
                                shortestSolution = true;
                                break;
                        case 'p':
                                optimalPath = true;
                                break;
                        case 'i':
                                infile = fopen(optarg, "r");
				if (infile == NULL){
					exit(EXIT_FAILURE);
				}
                                break;
                        case 'o':
                                outfile = fopen(optarg, "w");
				if (outfile == NULL){
					exit(EXIT_FAILURE);
				}
                                break;
                        case '?':
                                printf("Invalid flag included as commandline argument\n");
                                break;
                        default:
                                break;
                }
        }

        Maze maze = parseMaze(infile);

        if (pretty == true)
                prettyPrint(maze, outfile);
        if (shortestSolution == true || optimalPath == true){
                int distance = stepCount(maze, 0, 0, optimalPath);
                if (shortestSolution == true){
                        if (distance == 0)
                                fprintf(outfile,"No solution.\n");
                        else
                                fprintf(outfile,"Solution in %d steps.\n", distance);
                }
                if (optimalPath == true){
                        prettyPrint(maze, outfile);
                }
        }
	fclose(infile);
	fclose(outfile);
	destroyMaze(maze);
        return 0;
}
