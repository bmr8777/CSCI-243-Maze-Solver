/*
 * queue.h
 *
 * @author Brennan Reed
 */

#ifndef QUEUE_H
#define QUEUE_H

/**
 * struct: QueueADT
 * -----------------
 * Queue data structure consisting of Nodes
 */

typedef struct Queue_S *Queue;

/**
 * struct: Node
 * ------------
 * A single node in the queue
 */

typedef struct Node_S Node;

/**
 * Function: createQueue
 * ----------------------
 * Creates and returns an empty queue
 *
 * @return empty queue
 */

Queue createQueue();


/**
 * Function: createNode
 * --------------------
 * Creates and returns a node
 *
 * @return created node
 */

Node* createNode(size_t row, size_t column, int distance, Node* previous);

/**
 * Function: destroyNode
 * ---------------------
 * Deletes all dynamic allocate storage for a Node
 *
 * @param node the node being destroyed
 */

void destroyNode(Node* node);

/**
 * Function: destroyQueue
 * -----------------------
 * Deletes all dynamic queue storage
 */

void destroyQueue(Queue queue);

/**
 * Function: sizeQueue
 * -------------------
 * Returns the number of items in a specified queue
 *
 * @param queue the subject queue
 * @return the current number of active entries in the queue
 */

size_t sizeQueue (Queue queue);

/**
 * Function: isEmpty
 * ----------------
 * Checks to see if the specified queue is empty
 *
 * @param queue the subject queue
 * @return false if empty : else true
 */

bool isEmpty(Queue queue);

/**
 * Function: enQueue
 * -----------------
 * Adds a key to the queue
 *
 * @param queue pointer to the specified queue
 * @param k key to be added
 */

void enQueue(Queue queue, Node *node);

/**
 * Function: deQueue
 * ------------------
 * Removes a key from a given queue
 *
 * @param queue pointer to the specified queue
 * @return the corresponding queue
 */

Node *deQueue(Queue queue);


#endif// QUEUE_H
