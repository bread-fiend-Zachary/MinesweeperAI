// ======================================================================
// FILE:        MyAI.hpp
//
// AUTHOR:      Jian Li
//
// DESCRIPTION: This file contains your agent class, which you will
//              implement. You are responsible for implementing the
//              'getAction' function and any helper methods you feel you
//              need.
//
// NOTES:       - If you are having trouble understanding how the shell
//                works, look at the other parts of the code, as well as
//                the documentation.
//
//              - You are only allowed to make changes to this portion of
//                the code. Any changes to other portions of the code will
//                be lost when the tournament runs your code.
// ======================================================================

#ifndef MINE_SWEEPER_CPP_SHELL_MYAI_HPP
#define MINE_SWEEPER_CPP_SHELL_MYAI_HPP

#include "Agent.hpp"
#include <iostream> // temporary use
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <time.h>
//#include <unistd.h>
#include <chrono>
#include <queue>
//#include <stdio.h>
//#include <string.h>
using namespace std;


class MyAI : public Agent
{
public:
    MyAI ( int _rowDimension, int _colDimension, int _totalMines, int _agentX, int _agentY );
    ~MyAI();
    Action getAction ( int number ) override;

        //because number represents the label of the last uncovered tiles.
   // int initialize_board(vector<int> myvec);

    
private:
    int getEffectiveLabel(int label);
    //void ROTMarked(); // return coordinates of unmarked tile
    int findUnmarkedNeighbor();
    void findSafeTile();
    void findUnmarkDangerTile();
    void findUnmarkSafeTile();
    void localSearch();
    void findLowestProbMine();
    void doFrontierCovered();
    void helperFrontierCovered();
    void modelChecking();
    void doFrontierUncovered();
    bool isAdj(int x, int y);
    int findLowestProbMineHelper(int x_coord, int y_coord);
    bool is_next_to_covered(int,int);
    bool adjEL(int x, int y, int el);
    
    std::vector<std::vector<int>> board; //stores rows of data, index is row number aka y
    //int **board;
    set<pair<int, int> > safeToUncover;
    set<pair<int, int> > dangerTile;    
    set<pair<int, int> > uncovered_frontier;
    map<pair<int, int>, double> covered_frontier;
    //map<pair<int, int>, double> el1; 
    set<pair<int,int> > probabilities_covered;
    set<pair< int, int> > next_to_probabilities;
    queue<pair<int,int>> safe_queue;
    pair<int, int> minpair;


    double total_time_elapsed=0.0;
    int ucX=-1; //user chosen coordinate, 
    int ucY=-1;
    int uncoveredX=-1;
    int uncoveredY=-1;
    pair<int, int> lowestPMineCoordinate;
    double lowestPMine;
    //int covered=0;
    const Action_type actions[4] =
            {
                    LEAVE,
                    UNCOVER,
                    FLAG,
                    UNFLAG,
            };
    

};

#endif //MINE_SWEEPER_CPP_SHELL_MYAI_HPP
