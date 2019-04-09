/*
 * file: mopsolver.h
 * -----------------
 * Header file for the mopsolver program
 *
 * @author Brennan Reed
 */

#ifndef MOPSOLVER_H
#define MOPSOLVER_H

#include <stdbool.h>
#include <stdio.h>

typedef struct Maze_S *Maze;

/**
 * Function: parseMaze
 * -------------------
 * Parses a plain-text encoding of a maze
 */

Maze parseMaze(FILE * input);

#include "queue.h"

/**
 * Function: stepCount
 * -------------------
 * Uses a BFS to calculate the fewest number of steps necessary to solve a maze
 *
 * @return the fewest number of steps required to solve the maze
 */

int stepCount(Maze maze, size_t sRow, size_t sColumn, bool optimalPath);

/**
 * Function: prettyPrint
 * ---------------------
 * Pretty-prints a maze with a nifty border and ascii graphics
 */

void prettyPrint(Maze maze, FILE* output);

#endif// MOPSOLVER_H
