// ======================================================================
// FILE:        MyAI.cpp
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

#include "MyAI.hpp"
#include "ctime"
#include "iostream"

MyAI::MyAI ( int _rowDimension, int _colDimension, int _totalMines, int _agentX, int _agentY ) : Agent()
{

    // ======================================================================
    // YOUR CODE BEGINS
    // ======================================================================

    rowDimension = _rowDimension;
    colDimension = _colDimension;
    totalMines = _totalMines;
    agentX = _agentX;
    agentY = _agentY; //var correct

    ucX=agentX;
    ucY=agentY;
    uncoveredX=agentX;
    uncoveredY=agentY;
    
    //covered = rowDimension * colDimension;
    //cout << agentX << agentY << endl;
    int default_val = 16;
    int entry_num=0;
    total_time_elapsed=0.0;

    std::vector<int> temp;
    //temp.clear();

    //board = new int*[rowDimension];
    for(int x = 0; x < colDimension; ++x)
    {
        for(int y=0; y<rowDimension; ++y) {
            temp.push_back(default_val);
        }
        board.push_back(temp);
        temp.clear();
    }
    /*
    for(int x=0; x<rowDimension; ++x)
        for(int y=0; y<colDimension; ++y)
            board[x][y]=default_val;*/
    //cout << rowDimension << "x" << colDimension << endl;
    //cout << entry_num << endl;
}; //dimension fixed twice

MyAI::~MyAI() {
    /*
    for(int x=0; x<rowDimension; ++x)
        delete [] board[x];

    delete [] board;*/
} 

Agent::Action MyAI::getAction( int number )
{
   
    if(number>=0) board[uncoveredX%colDimension][uncoveredY%rowDimension]=number;  //store label of last uncovered tiles
    double remaining_time = 300-total_time_elapsed;
    

    int covered=0;
    int coverUnmarked=0;
    int coverMarked=0;
    for(int x=0; x<colDimension; ++x) {
        for(int y=0; y<rowDimension; ++y) {
            if(board[x%colDimension][y%rowDimension]==15) ++coverMarked;
            if(board[x%colDimension][y%rowDimension]==16) ++coverUnmarked;
        }
    }
    covered = coverMarked+coverUnmarked;


    if(covered==totalMines) return {LEAVE,-1,-1}; 

    

    if(remaining_time < 5.0) {

        //int tx, ty;
        ucX = rand() % colDimension;
        ucY = rand() % rowDimension;
        if(ucX<0) ucX=0;
        if(ucY<0) ucY=0;
        
        Action_type myAction= actions[rand() % 4];
        
        if(myAction==UNCOVER) {
            uncoveredX=ucX;
            uncoveredY=ucY;
            //--covered;
        }

        return{myAction, ucX, ucY};

    } else{
        auto time_start = std::chrono::steady_clock::now();
        //add frontier nodes + subtract the node that we just uncovered from frontier 
        //scan for frontier
        //then do rule of thumb
        // then to model checking


        ucX=uncoveredX;
        ucY=uncoveredY;
        //doFrontierCovered();
        //doFrontierUncovered();
        if(number==0) {
            //ucX=uncoveredX;
            //ucY=uncoveredY;
            findSafeTile();
        } //good
        
        doFrontierUncovered();
        doFrontierCovered();
        //view covered and uncovered
        /*
        cout << "uncovered frontier" << endl;
        for(auto it=uncovered_frontier.begin(); it!=uncovered_frontier.end(); ++it)
            cout << it->first+1 << "," <<it->second+1 << endl;
        */
        //cout << "covered frontier" << endl;
        //cout << covered_frontier.size() << endl;
        /*
        for(auto it=covered_frontier.begin(); it!=covered_frontier.end(); ++it) {
            pair<int, int> ptr = it->first;
            cout << ptr.first+1 << "," << ptr.second+1 << endl;

        }*/
      //  modelChecking();
        
        //ucX=uncoveredX;
        //ucY=uncoveredY;
        //set<pair<int, int>>::iterator iter_covered = covered_frontier.begin();
        //set<pair<int, int>>::iterator iter_uncovered = uncovered_frontier.begin();

        for(set<pair<int, int>>::iterator pt=uncovered_frontier.begin(); pt!=uncovered_frontier.end(); ++pt) {
            pair<int, int> ptr = *pt;
            ucX=ptr.first;
            ucY=ptr.second;
            int flabel = board[ucX][ucY];
            int effectiveLabel = getEffectiveLabel(flabel);
            int unmarkedNeighbor=findUnmarkedNeighbor();
            if(effectiveLabel==unmarkedNeighbor) {
                findUnmarkDangerTile();
            } else if(effectiveLabel==0) {
                findUnmarkSafeTile();
            } else if(effectiveLabel!=unmarkedNeighbor) {
                //ucX-1, ucY-1
                
                //if(adjEL(ucX, ucY, 1)==true) {
                    ucX=ptr.first;
                    ucY=ptr.second;
                    if((ucX-1)>=0 && (ucY-1)>=0) {
                        pair<int, int> key = make_pair(ucX-1, ucY-1);
                        if(covered_frontier.count(key)==1)
                            covered_frontier[key] += double(effectiveLabel);

                    }
                        //safeToUncover.insert(make_pair((ucX-1)%colDimension, (ucY-1)%rowDimension));
                    //ucX-1, ucY
                    if((ucX-1)>=0) {
                        pair<int, int> key = make_pair(ucX-1, ucY);
                        if(covered_frontier.count(key)==1)
                            covered_frontier[key] += double(effectiveLabel);
                    }
                        //safeToUncover.insert(make_pair((ucX-1)%colDimension, ucY%rowDimension));
                    //ucX-1, ucY+1
                    if((ucX-1)>=0 && (ucY+1)<rowDimension) {
                        pair<int, int> key = make_pair(ucX-1, ucY+1);
                        if(covered_frontier.count(key)==1)
                            covered_frontier[key] += double(effectiveLabel);
                    }

                        //safeToUncover.insert(make_pair((ucX-1)%colDimension, (ucY+1)%rowDimension));
                    //ucX, ucY+1
                    if((ucY+1)<rowDimension) {
                        pair<int, int> key = make_pair(ucX, ucY+1);
                        if(covered_frontier.count(key)==1)
                            covered_frontier[key] += double(effectiveLabel);
                    }
                        //safeToUncover.insert(make_pair(ucX%colDimension, (ucY+1)%rowDimension));
                    //ucX+1, ucY+1
                    if((ucX+1)<colDimension && (ucY+1)<rowDimension) {
                        pair<int, int> key = make_pair(ucX+1, ucY+1);
                        if(covered_frontier.count(key)==1)
                            covered_frontier[key] += double(effectiveLabel);
                    }
                        //safeToUncover.insert(make_pair((ucX+1)%colDimension, (ucY+1)%rowDimension));
                    //ucX+1, ucY
                    if((ucX+1)<colDimension) {
                        pair<int, int> key = make_pair(ucX+1, ucY);
                        if(covered_frontier.count(key)==1)
                            covered_frontier[key] += double(effectiveLabel);
                    }
                        //safeToUncover.insert(make_pair((ucX+1)%colDimension, ucY%rowDimension));
                    //ucX+1, ucY-1
                    if((ucX+1)<colDimension && (ucY-1)>=0) {
                        pair<int, int> key = make_pair(ucX+1, ucY-1);
                        if(covered_frontier.count(key)==1)
                            covered_frontier[key] += double(effectiveLabel);
                    }
                        //safeToUncover.insert(make_pair((ucX+1)%colDimension, (ucY-1)%rowDimension));
                    //ucX, ucY-1
                    if((ucY-1)>=0) {
                        pair<int, int> key = make_pair(ucX, ucY-1);
                        if(covered_frontier.count(key)==1)
                            covered_frontier[key] += double(effectiveLabel);
                    }
                //}
            }

        }
        
        

        //uncovered_frontier.clear();
        //covered_frontier.clear();

        if(!dangerTile.empty()) {
            std::pair<int, int> coord=*dangerTile.begin();
            
            dangerTile.erase(dangerTile.begin());
            
            //std::cout << "unflag " << coord.first+1 <<" , " << coord.second+1 << endl;
            board[coord.first][coord.second]=15;

            
            uncovered_frontier.clear();
            covered_frontier.clear();

            auto time_end = std::chrono::steady_clock::now();
            double dt = (double)std::chrono::duration_cast<std::chrono::seconds>(time_end - time_start).count();
            total_time_elapsed+=dt;
            return {FLAG, coord.first, coord.second};
        }

        if(!safeToUncover.empty()) {
            std::pair<int, int> coord=*safeToUncover.begin();
            if(board[coord.first][coord.second]==15) {
                safe_queue.push(make_pair(coord.first,coord.second));
                if(safe_queue.size() > 2){
                    while(!safe_queue.empty()){
                        int tmpx = safe_queue.front().first;
                        int tmpy = safe_queue.front().second;
                        if(!safe_queue.empty()) safe_queue.pop();
                        if(!safe_queue.empty()) safe_queue.pop();
                        if(safe_queue.empty()) break;
                        if(tmpx == safe_queue.front().first && tmpy == safe_queue.front().second){
                            safeToUncover.erase(safeToUncover.begin());
                            modelChecking();
                            //NEED TO CHANGE THIS

                            //std::cout << "entered queue condition; will return x,y" << endl;
                            uncovered_frontier.clear();
                            covered_frontier.clear();

                            auto time_end = std::chrono::steady_clock::now();
                            double dt = (double)std::chrono::duration_cast<std::chrono::seconds>(time_end - time_start).count();
                            total_time_elapsed+=dt;

                            return {FLAG, tmpx, tmpy};
                        }
                    }
                }
                //std::cout << "skipped safe_queue" << endl;
                //std::cout << "queue size: " << safe_queue.size() << endl;
                board[coord.first][coord.second]=16;


                

                //std::cout << "frontier cleared safetouncover " << endl;
                uncovered_frontier.clear();
                covered_frontier.clear();

                auto time_end = std::chrono::steady_clock::now();
                double dt = (double)std::chrono::duration_cast<std::chrono::seconds>(time_end - time_start).count();
                total_time_elapsed+=dt;

                return {UNFLAG, coord.first, coord.second};
            }
            safeToUncover.erase(safeToUncover.begin());
            uncoveredX=coord.first;
            uncoveredY=coord.second;
            if(uncoveredX<0) uncoveredX=0;
            if(uncoveredY<0) uncoveredY=0;
            if(uncoveredX>=colDimension) uncoveredX=colDimension-1;
            if(uncoveredY>=rowDimension) uncoveredY=rowDimension-1;
            
            //cout << uncoveredX<<" "<<rowDimension<<" " << uncoveredY<<" "<<colDimension<<endl;

            //std::cout << "frontier cleared safetouncover erase " << endl;
            

            uncovered_frontier.clear();
            covered_frontier.clear();

            auto time_end = std::chrono::steady_clock::now();
            double dt = (double)std::chrono::duration_cast<std::chrono::seconds>(time_end - time_start).count();
            total_time_elapsed+=dt;
            return {UNCOVER, uncoveredX, uncoveredY};
        } else {
            //find uncovered node

            /*
            //first iteration, scrapped for frontier node
            for(int x = 0; x < colDimension; x++)
            {
                for(int y = 0; y < rowDimension; y++)
                {
                    if(board[x%colDimension][y%rowDimension] == 16)
                    {
                        lowestPMineCoordinate.insert(make_pair(x%colDimension,y%rowDimension));
                    }
                }
            }
            */
           //std::cout << "else :" << endl;
           //modelChecking();
           probabilities_covered.clear();
           next_to_probabilities.clear();
            double min=100.0;
            double max=0.0;
            pair<int, int> minProbPair=make_pair(-1, -1);
            pair<int, int> flagThis=make_pair(-1, -1);
            for(map<pair<int, int>, double>::iterator pt=covered_frontier.begin(); pt!=covered_frontier.end(); ++pt) {
                pair<int, int> tempPair=pt->first;
                //cout << "model check " << tempPair.first+1 << " , " << tempPair.second+1 << " : " << pt->second << endl;
                if((0<= tempPair.first<colDimension) && (0<=tempPair.second<rowDimension)) {
                    //cout << "probe Model Checking" << endl;
                    if(pt->second>0.0 && pt->second<min && board[tempPair.first][tempPair.second]==16) {
                        min = pt->second;
                        minProbPair=tempPair;
                    }
                    
                    else if(pt->second>max && board[tempPair.first][tempPair.second]==16) {
                        max = pt->second;
                        flagThis = tempPair;
                    }
                }

            }
            //safeToUncover.insert(safeToUncover.begin(), minProbPair);
            if(colDimension>16 && rowDimension>16) { 
                if((flagThis.first>=0 && flagThis.first<colDimension) && (flagThis.second>=0&&flagThis.second<rowDimension)) {
                    //cout << "MC Flag "<< endl;
                    //cout << flagThis.first << " , "<< flagThis.second <<endl;
                    dangerTile.insert(flagThis);
                }
            }

           
         
            //findLowestProbMine();
            /*
            if(flagThis.first>=0 && flagThis.first<colDimension 
                && flagThis.second>=0 && flagThis.second<rowDimension) {
                dangerTile.insert(flagThis);
            }*/
            
            //cout << uncoveredX<<" "<<rowDimension<<" " << uncoveredY<<" "<<colDimension<<endl;
            //return{UNCOVER, lowestPMineCoordinate.first, lowestPMineCoordinate.second};
            if(minProbPair.first>=0 && minProbPair.second>=0) {
                uncoveredX = minProbPair.first;
                uncoveredY = minProbPair.second;
                uncovered_frontier.clear();
                covered_frontier.clear();
                auto time_end = std::chrono::steady_clock::now();
                double dt = (double)std::chrono::duration_cast<std::chrono::seconds>(time_end - time_start).count();
                total_time_elapsed+=dt;
                return {UNCOVER, uncoveredX, uncoveredY};
            } else if(covered_frontier.size()>0) {
                int index = rand() % covered_frontier.size();
                map<pair<int, int>, double>::iterator it = covered_frontier.begin();
                for(int i=0; i<=index && it!=covered_frontier.end(); ++i) {
                    ++it;
                }
                //if(it!=covered_frontier.end()) {
                pair<int, int> guess =it->first;
                if(guess.first<0) uncoveredX=0;
                else if(guess.first>=colDimension) uncoveredX=colDimension-1;
                else uncoveredX=guess.first;

                if(guess.second<0) uncoveredY=0;
                else if(guess.second>=rowDimension) uncoveredY=rowDimension-1;
                else uncoveredY=guess.second;
                
                uncovered_frontier.clear();
                covered_frontier.clear();
                //cout << "random guess " << uncoveredX <<" , "<< uncoveredY<<  endl;
                auto time_end = std::chrono::steady_clock::now();
                double dt = (double)std::chrono::duration_cast<std::chrono::seconds>(time_end - time_start).count();
                total_time_elapsed+=dt;
                return {UNCOVER, uncoveredX, uncoveredY};
            }else {
                //std::cout<< " entered this" << endl;

                set<pair<int, int> > remainingTiles;
                for(int x = 0; x < colDimension; x++){
                    for(int y = 0; y < rowDimension; y++){
                        if(board[x%colDimension][y%rowDimension] == 16){
                            remainingTiles.insert(make_pair(x,y));
                        }
                    }
                }
                if(remainingTiles.size() > 1){
                    std::pair<int, int> coords = *remainingTiles.begin();
                     auto time_end = std::chrono::steady_clock::now();
                      double dt = (double)std::chrono::duration_cast<std::chrono::seconds>(time_end - time_start).count();
                      remainingTiles.clear();
                    return {UNCOVER, coords.first, coords.second};
                }
                if(remainingTiles.size() == 0){
                    return {LEAVE,-1, -1};
                }
                
                //std::cout << "no" << endl;
                uncovered_frontier.clear();
                covered_frontier.clear();
                auto time_end = std::chrono::steady_clock::now();
                double dt = (double)std::chrono::duration_cast<std::chrono::seconds>(time_end - time_start).count();
                total_time_elapsed+=dt;
                return {UNCOVER, uncoveredX, uncoveredY};
            //return{UNCOVER, minpair.first, minpair.second};
            }
        } 


    }
    
    
    


    //return {LEAVE,-1,-1};

}

void MyAI::doFrontierCovered()
{
    
    //check to see if last uncovered node is in our covered frontier
    for(set<pair<int, int>>::iterator pt = uncovered_frontier.begin(); pt!=uncovered_frontier.end(); ++pt) {
        pair<int, int> ptr=*pt;
        ucX=ptr.first;
        ucY=ptr.second;
        helperFrontierCovered();
    }
}

bool MyAI::adjEL(int x, int y, int el) {
    //getEffectiveLabel()
    if((x-1)>=0 && (y-1)>=0 ) {
        pair<int, int> key = make_pair(x-1, y-1);
        ucX=key.first;
        ucY=key.second;
        int EL=getEffectiveLabel(board[ucX][ucY]);
        if(uncovered_frontier.find(key)!=uncovered_frontier.end() && EL==el) return true;
    }
        //safeToUncover.insert(make_pair((ucX-1)%colDimension, (ucY-1)%rowDimension));
    //ucX-1, ucY
    if((x-1)>=0) {
        pair<int, int> key = make_pair(x-1, y);
        ucX=key.first;
        ucY=key.second;
        int EL=getEffectiveLabel(board[ucX][ucY]);
        if(uncovered_frontier.find(key)!=uncovered_frontier.end() && EL==el) return true;
    }
        //safeToUncover.insert(make_pair((ucX-1)%colDimension, ucY%rowDimension));
    //ucX-1, ucY+1
    if((x-1)>=0 && (y+1)<rowDimension) {
        pair<int, int> key = make_pair(x-1, y+1);
        ucX=key.first;
        ucY=key.second;
        int EL=getEffectiveLabel(board[ucX][ucY]);
        if(uncovered_frontier.find(key)!=uncovered_frontier.end() && EL==el) return true;
    }

        //safeToUncover.insert(make_pair((ucX-1)%colDimension, (ucY+1)%rowDimension));
    //ucX, ucY+1
    if((y+1)<rowDimension) {
        pair<int, int> key = make_pair(x, y+1);
        ucX=key.first;
        ucY=key.second;
        int EL=getEffectiveLabel(board[ucX][ucY]);
        if(uncovered_frontier.find(key)!=uncovered_frontier.end() && EL==el) return true;
    }
        //safeToUncover.insert(make_pair(ucX%colDimension, (ucY+1)%rowDimension));
    //ucX+1, ucY+1
    if((x+1)<colDimension && (y+1)<rowDimension) {
        pair<int, int> key = make_pair(x+1, y+1);
        ucX=key.first;
        ucY=key.second;
        int EL=getEffectiveLabel(board[ucX][ucY]);
        if(uncovered_frontier.find(key)!=uncovered_frontier.end() && EL==el) return true;
    }
        //safeToUncover.insert(make_pair((ucX+1)%colDimension, (ucY+1)%rowDimension));
    //ucX+1, ucY
    if((x+1)<colDimension ) {
        pair<int, int> key = make_pair(x+1, y);
        ucX=key.first;
        ucY=key.second;
        int EL=getEffectiveLabel(board[ucX][ucY]);
        if(uncovered_frontier.find(key)!=uncovered_frontier.end() && EL==el) return true;
    }
        //safeToUncover.insert(make_pair((ucX+1)%colDimension, ucY%rowDimension));
    //ucX+1, ucY-1
    if((x+1)<colDimension && (y-1)>=0) {
        pair<int, int> key = make_pair(x+1, y-1);
        ucX=key.first;
        ucY=key.second;
        int EL=getEffectiveLabel(board[ucX][ucY]);
        if(uncovered_frontier.find(key)!=uncovered_frontier.end() && EL==el) return true;
    }
        //safeToUncover.insert(make_pair((ucX+1)%colDimension, (ucY-1)%rowDimension));
    //ucX, ucY-1
    if((y-1)>=0 ) {
        pair<int, int> key = make_pair(x, y-1);
        ucX=key.first;
        ucY=key.second;
        int EL=getEffectiveLabel(board[ucX][ucY]);
        if(uncovered_frontier.find(key)!=uncovered_frontier.end() && EL==el) return true;
    }
    return false;
}

void MyAI::helperFrontierCovered() {
    //ucX-1, ucY-1
    if((ucX-1)>=0 && (ucY-1)>=0 && board[(ucX-1)%colDimension][(ucY-1)%rowDimension]==16) {
        pair<int, int> key = make_pair(ucX-1, ucY-1);
        if(covered_frontier.count(key)==0)
            covered_frontier[key] = 0.0;

    }
        //safeToUncover.insert(make_pair((ucX-1)%colDimension, (ucY-1)%rowDimension));
    //ucX-1, ucY
    if((ucX-1)>=0 && board[(ucX-1)%colDimension][ucY%rowDimension]==16 ) {
        pair<int, int> key = make_pair(ucX-1, ucY);
        if(covered_frontier.count(key)==0)
            covered_frontier[key] = 0.0;
    }
        //safeToUncover.insert(make_pair((ucX-1)%colDimension, ucY%rowDimension));
    //ucX-1, ucY+1
    if((ucX-1)>=0 && (ucY+1)<rowDimension && board[(ucX-1)%colDimension][(ucY+1)%rowDimension]==16) {
        pair<int, int> key = make_pair(ucX-1, ucY+1);
        if(covered_frontier.count(key)==0)
            covered_frontier[key] = 0.0;
    }

        //safeToUncover.insert(make_pair((ucX-1)%colDimension, (ucY+1)%rowDimension));
    //ucX, ucY+1
    if((ucY+1)<rowDimension && board[ucX%colDimension][(ucY+1)%rowDimension]==16) {
        pair<int, int> key = make_pair(ucX, ucY+1);
        if(covered_frontier.count(key)==0)
            covered_frontier[key] = 0.0;
    }
        //safeToUncover.insert(make_pair(ucX%colDimension, (ucY+1)%rowDimension));
    //ucX+1, ucY+1
    if((ucX+1)<colDimension && (ucY+1)<rowDimension && board[(ucX+1)%colDimension][(ucY+1)%rowDimension]==16) {
        pair<int, int> key = make_pair(ucX+1, ucY+1);
        if(covered_frontier.count(key)==0)
            covered_frontier[key] = 0.0;
    }
        //safeToUncover.insert(make_pair((ucX+1)%colDimension, (ucY+1)%rowDimension));
    //ucX+1, ucY
    if((ucX+1)<colDimension && board[(ucX+1)%colDimension][ucY%rowDimension]==16) {
        pair<int, int> key = make_pair(ucX+1, ucY);
        if(covered_frontier.count(key)==0)
            covered_frontier[key] = 0.0;
    }
        //safeToUncover.insert(make_pair((ucX+1)%colDimension, ucY%rowDimension));
    //ucX+1, ucY-1
    if((ucX+1)<colDimension && (ucY-1)>=0 && board[(ucX+1)%colDimension][(ucY-1)%rowDimension]==16) {
        pair<int, int> key = make_pair(ucX+1, ucY-1);
        if(covered_frontier.count(key)==0)
            covered_frontier[key] = 0.0;
    }
        //safeToUncover.insert(make_pair((ucX+1)%colDimension, (ucY-1)%rowDimension));
    //ucX, ucY-1
    if((ucY-1)>=0 && board[ucX%colDimension][(ucY-1)%rowDimension]==16) {
        pair<int, int> key = make_pair(ucX, ucY-1);
        if(covered_frontier.count(key)==0)
            covered_frontier[key] = 0.0;
    }
   
        //safeToUncover.insert(make_pair(ucX%colDimension, (ucY-1)%rowDimension));
}

void MyAI::doFrontierUncovered()
{
    for(int x=0; x<colDimension; ++x) {
        for(int y=0; y<rowDimension; ++y) {
            
            if(board[x][y]<9 && is_next_to_covered(x,y) == true) {
                ucX=x;
                ucY=y;
                if(findUnmarkedNeighbor()>0) uncovered_frontier.insert(make_pair(x, y));
            } 
        }
    }  

    /*
    for(auto it = uncovered_frontier.begin(); it != uncovered_frontier.end(); it++){
        std::cout << "uncovered: " << it->first << " " << it->second << endl;
    }
    */
    
}
bool MyAI::is_next_to_covered(int x, int y){
    auto it = covered_frontier.find(make_pair(x,y));
    if(it != covered_frontier.end()){
        return false;
    }
            //std::cout << "uncovered pair found: " << x << " " << y << endl;

    //std::cout << "true" << endl;
    return true;
}

void MyAI::modelChecking()
{
    //std::cout << "entered model check" << endl;

    //std::cout << "covered frontier :" << endl;
    /*
    for(auto it = covered_frontier.begin(); it!= covered_frontier.end(); it++){
        cout << it->first.first << " " << it->first.second << endl;
    }
    //std::cout << "uncovered frontier :" << endl;
    for(auto it = uncovered_frontier.begin(); it!= uncovered_frontier.end(); it++){
        cout << it->first << " " << it->second << endl;
    }*/



    //map<pair<int,int>, double> probabilities;
    for(auto it = covered_frontier.begin(); it != covered_frontier.end(); it++){
        //std::cout << "model checking covered frontier:" <<  it->first.first << " "<< it->first.second <<endl;
      //  int tmpx;
      //  int tmpy;
        //first condition - – all covered/unmarked tiles adjacent to U are in C
        // probably satisfies second condition too - – no other covered/unmarked tiles in C <- which is probabilities
        if(isAdj(it->first.first, it->first.second) == true){
            //std::cout << "entered isadj == true " << endl;
            probabilities_covered.insert(make_pair(it->first.first,it->first.second));
            //std::cout << "probabilities_covered: " << it->first.first << " " << it->first.second << endl;
            
           // pair<int,int> mypair = make_pair(it->first.first, it->first.second);
            //add the coordinates to this to check possible worlds
            //double pmine = findLowestProbMine(mypair);
            //check probability of worlds
            }
        //std::cout << "isadj is false" << std::endl;
    }
    //std::cout << "is fakse>???" << endl;

    /*
    for(auto it = probabilities_covered.begin(); it!= probabilities_covered.end(); it++)
    {
        std::cout << "probabilities first: " << it->first << " probabilities second: " << it->second << std::endl;
    }
    for(auto it = next_to_probabilities.begin(); it != next_to_probabilities.end(); it++)
    {
        std::cout << "next_to first: " << it->first << " next_to second: " << it->second << std::endl;
    }
    */

    //findLowestProbMine();


    /*
    //find the probabilities with lowest pmine
    double min = 1.0;rtf
    //pair<int,int> minpair;
    for(auto itr = probabilities.begin(); itr != probabilities.end(); itr++){
        if(itr->second < min){
            min = itr->second;
            minpair = make_pair(itr->first.first, itr->first.second);
        }
    }
    probabilities.empty();

    */

}

void MyAI::findLowestProbMine()
{
    //just need to get the #sol with mine / #solutions with each coord
    //use probabilities var here.. ?
    //store in minpair...?
    //A+B+whatever = 1 here with whatever coordinates are in probabilities.
    int tmpx;
    int tmpy;
    //probabilities_covered = a covered tile that has an adjacent uncovered tile
    //next_to_probabilities = each uncovered tile that is next a probabilities(covered) tile

    /*
    for(auto itrpc = probabilities_covered.begin(); itrpc != probabilities_covered.end(); itrpc++){
        for(auto itr = next_to_probabilities.begin(); itr != next_to_probabilities.end(); itr++){
            if(abs(itrpc->first - itr->first) >= 2 || abs(itrpc->second - itr->second) >= 2)
            {

            }
        }

    }
    */

   //need to section the probabilities_covered tile and next_to so that if they aren't adjacent ...?
    


}

bool MyAI::isAdj(int x, int y)
{
    //check if all all covered/unmarked tiles adjacent to Uncovered are in Covered

    //ucX-1, ucY-1
    set<pair<int, int>> temp;
    if((x-1)>=0 && (y-1)>=0 && board[(x-1)%colDimension][(y-1)%rowDimension] < 9){
        temp.insert(make_pair(((x-1)%colDimension), (y-1)%rowDimension));
    }
    //ucX-1, ucY
    if((x-1)>=0 && board[(x-1)%colDimension][y%rowDimension] < 9 ){
        temp.insert(make_pair(((x-1)%colDimension),y%rowDimension));
    }
    //ucX-1, ucY+1
    if((x-1)>=0 && (y+1)<rowDimension && board[(x-1)%colDimension][(y+1)%rowDimension] < 9){
        temp.insert(make_pair(((x-1)%colDimension), (y+1)%rowDimension));
    }
    //ucX, ucY+1
    if((y+1)<rowDimension && board[x%colDimension][(y+1)%rowDimension] < 9){
        temp.insert(make_pair((x%colDimension),(y+1)%rowDimension));
    }
    //ucX+1, ucY+1
    if((x+1)<colDimension && (y+1)<rowDimension && board[(x+1)%colDimension][(y+1)%rowDimension] < 9){
        temp.insert(make_pair(((x+1)%colDimension), (y+1)%rowDimension));
    }
    //ucX+1, ucY
    if((x+1)<colDimension && board[(x+1)%colDimension][y%rowDimension] < 9){
        temp.insert(make_pair(((x+1)%colDimension), y%rowDimension));
    }
    //ucX+1, ucY-1
    if((x+1)<colDimension && (y-1)>=0 && board[(x+1)%colDimension][(y-1)%rowDimension] < 9){
        temp.insert(make_pair(((x+1)%colDimension), (y-1)%rowDimension));
    }
    //ucX, ucY-1
    if((y-1)>=0 && board[x%colDimension][(y-1)%rowDimension] < 9){
        temp.insert(make_pair((x%colDimension), (y-1)%rowDimension));
    }

    int count = 0;
    int tempsize = temp.size();
    //std::cout << "tempsize: " << tempsize << endl;
    while(!temp.empty()){
        pair<int, int> coord = *temp.begin();
        temp.erase(temp.begin());
        
        if(uncovered_frontier.find(coord)!=uncovered_frontier.end()) {
            next_to_probabilities.insert(make_pair(coord.first, coord.second));
            ++count;
        }
        
        /*
        for(auto itr = uncovered_frontier.begin(); itr != uncovered_frontier.end(); itr++){
            if(coord.first == itr->first && coord.second == itr->second){
                //if the number of coords in covered_frontier is equal to the size in temp (which is number of adj covered squares)
                next_to_probabilities.insert(make_pair(coord.first,coord.second));
                std::cout << "next_to_probabilities: " << coord.first << " " << coord.second << endl;
                count++;
            }
        }*/
    }
    //
   // std::cout << "count: " << count << endl;
    
    //99 is key so we can isolate each one from probabilities
   // next_to_probabilities.insert(make_pair(99,99));
    if(count != tempsize){
  //      std::cout << " false" << endl;
        next_to_probabilities.clear();
        return false;
    }

    return true;
    
}

int MyAI::findLowestProbMineHelper(int x_coord, int y_coord)
{
    //calculates the mine probability for each square that is passed in
    int temp;


    return temp;
}


//dimension fixed
void MyAI::findSafeTile() {

    //all check twice

    //ucX-1, ucY-1
    if((ucX-1)>=0 && (ucY-1)>=0 && board[(ucX-1)%colDimension][(ucY-1)%rowDimension]>14) safeToUncover.insert(make_pair((ucX-1)%colDimension, (ucY-1)%rowDimension));
    //ucX-1, ucY
    if((ucX-1)>=0 && board[(ucX-1)%colDimension][ucY%rowDimension]>14) safeToUncover.insert(make_pair((ucX-1)%colDimension, ucY%rowDimension));
    //ucX-1, ucY+1
    if((ucX-1)>=0 && (ucY+1)<rowDimension && board[(ucX-1)%colDimension][(ucY+1)%rowDimension]>14) safeToUncover.insert(make_pair((ucX-1)%colDimension, (ucY+1)%rowDimension));
    //ucX, ucY+1
    if((ucY+1)<rowDimension && board[ucX%colDimension][(ucY+1)%rowDimension]>14) safeToUncover.insert(make_pair(ucX%colDimension, (ucY+1)%rowDimension));
    //ucX+1, ucY+1
    if((ucX+1)<colDimension && (ucY+1)<rowDimension && board[(ucX+1)%colDimension][(ucY+1)%rowDimension]>14) safeToUncover.insert(make_pair((ucX+1)%colDimension, (ucY+1)%rowDimension));
    //ucX+1, ucY
    if((ucX+1)<colDimension && board[(ucX+1)%colDimension][ucY%rowDimension]>14) safeToUncover.insert(make_pair((ucX+1)%colDimension, ucY%rowDimension));
    //ucX+1, ucY-1
    if((ucX+1)<colDimension && (ucY-1)>=0 && board[(ucX+1)%colDimension][(ucY-1)%rowDimension]>14) safeToUncover.insert(make_pair((ucX+1)%colDimension, (ucY-1)%rowDimension));
    //ucX, ucY-1
    if((ucY-1)>=0 && board[ucX%colDimension][(ucY-1)%rowDimension]>14) safeToUncover.insert(make_pair(ucX%colDimension, (ucY-1)%rowDimension));
} //update

void MyAI::findUnmarkSafeTile() {
    //ucX-1, ucY-1
    if((ucX-1)>=0 && (ucY-1)>=0 && board[(ucX-1)%colDimension][(ucY-1)%rowDimension]==16) safeToUncover.insert(make_pair((ucX-1)%colDimension, (ucY-1)%rowDimension));
    //ucX-1, ucY
    if((ucX-1)>=0 && board[(ucX-1)%colDimension][ucY%rowDimension]==16 ) safeToUncover.insert(make_pair((ucX-1)%colDimension, ucY%rowDimension));
    //ucX-1, ucY+1
    if((ucX-1)>=0 && (ucY+1)<rowDimension && board[(ucX-1)%colDimension][(ucY+1)%rowDimension]==16) safeToUncover.insert(make_pair((ucX-1)%colDimension, (ucY+1)%rowDimension));
    //ucX, ucY+1
    if((ucY+1)<rowDimension && board[ucX%colDimension][(ucY+1)%rowDimension]==16) safeToUncover.insert(make_pair(ucX%colDimension, (ucY+1)%rowDimension));
    //ucX+1, ucY+1
    if((ucX+1)<colDimension && (ucY+1)<rowDimension && board[(ucX+1)%colDimension][(ucY+1)%rowDimension]==16) safeToUncover.insert(make_pair((ucX+1)%colDimension, (ucY+1)%rowDimension));
    //ucX+1, ucY
    if((ucX+1)<colDimension && board[(ucX+1)%colDimension][ucY%rowDimension]==16) safeToUncover.insert(make_pair((ucX+1)%colDimension, ucY%rowDimension));
    //ucX+1, ucY-1
    if((ucX+1)<colDimension && (ucY-1)>=0 && board[(ucX+1)%colDimension][(ucY-1)%rowDimension]==16) safeToUncover.insert(make_pair((ucX+1)%colDimension, (ucY-1)%rowDimension));
    //ucX, ucY-1
    if((ucY-1)>=0 && board[ucX%colDimension][(ucY-1)%rowDimension]==16) safeToUncover.insert(make_pair(ucX%colDimension, (ucY-1)%rowDimension));
} //update

void MyAI::findUnmarkDangerTile() {
    //ucX-1, ucY-1
    if((ucX-1)>=0 && (ucY-1)>=0 && board[(ucX-1)%colDimension][(ucY-1)%rowDimension]==16) dangerTile.insert(make_pair((ucX-1)%colDimension, (ucY-1)%rowDimension));
    //ucX-1, ucY
    if((ucX-1)>=0 && board[(ucX-1)%colDimension][ucY%rowDimension]==16 ) dangerTile.insert(make_pair((ucX-1)%colDimension, ucY%rowDimension));
    //ucX-1, ucY+1
    if((ucX-1)>=0 && (ucY+1)<rowDimension && board[(ucX-1)%colDimension][(ucY+1)%rowDimension]==16) dangerTile.insert(make_pair((ucX-1)%colDimension, (ucY+1)%rowDimension));
    //ucX, ucY+1
    if((ucY+1)<rowDimension && board[ucX%colDimension][(ucY+1)%rowDimension]==16) dangerTile.insert(make_pair(ucX%colDimension, (ucY+1)%rowDimension));
    //ucX+1, ucY+1
    if((ucX+1)<colDimension && (ucY+1)<rowDimension && board[(ucX+1)%colDimension][(ucY+1)%rowDimension]==16) dangerTile.insert(make_pair((ucX+1)%colDimension, (ucY+1)%rowDimension));
    //ucX+1, ucY
    if((ucX+1)<colDimension && board[(ucX+1)%colDimension][ucY%rowDimension]==16) dangerTile.insert(make_pair((ucX+1)%colDimension, ucY%rowDimension));
    //ucX+1, ucY-1
    if((ucX+1)<colDimension && (ucY-1)>=0 && board[(ucX+1)%colDimension][(ucY-1)%rowDimension]==16) dangerTile.insert(make_pair((ucX+1)%colDimension, (ucY-1)%rowDimension));
    //ucX, ucY-1
    if((ucY-1)>=0 && board[ucX%colDimension][(ucY-1)%rowDimension]==16) dangerTile.insert(make_pair(ucX%colDimension, (ucY-1)%rowDimension));
} //update

//dim and all checked ok

int MyAI::findUnmarkedNeighbor() {
    //int effectiveLabel;
    int unmarked = 0;

    //ucX-1, ucY-1
    if((ucX-1)>=0 && (ucY-1)>=0 && board[(ucX-1)%colDimension][(ucY-1)%rowDimension]==16) ++unmarked;
    //ucX-1, ucY
    if((ucX-1)>=0 && board[(ucX-1)%colDimension][ucY%rowDimension]==16 ) ++unmarked;
    //ucX-1, ucY+1
    if((ucX-1)>=0 &&  (ucY+1)<rowDimension && board[(ucX-1)%colDimension][(ucY+1)%rowDimension]==16) ++unmarked;
    //ucX, ucY+1
    if((ucY+1)<rowDimension && board[ucX%colDimension][(ucY+1)%rowDimension]==16) ++unmarked;
    //ucX+1, ucY+1
    if((ucX+1)<colDimension && (ucY+1)<rowDimension && board[(ucX+1)%colDimension][(ucY+1)%rowDimension]==16) ++unmarked;
    //ucX+1, ucY
    if((ucX+1)<colDimension && board[(ucX+1)%colDimension][ucY%rowDimension]==16) ++unmarked;
    //ucX+1, ucY-1
    if((ucX+1)<colDimension && (ucY-1)>=0 && board[(ucX+1)%colDimension][(ucY-1)%rowDimension]==16) ++unmarked;
    //ucX, ucY-1
    if((ucY-1)>=0 && board[ucX%colDimension][(ucY-1)%rowDimension]==16) ++unmarked;
    return unmarked;
} //update


int MyAI::getEffectiveLabel(int label)
{
    int effectiveLabel;
    int marked = 0;
    //ucX-1, ucY-1
    if((ucX-1)>=0 && (ucY-1)>=0 && board[(ucX-1)%colDimension][(ucY-1)%rowDimension]==15) ++marked;
    //ucX-1, ucY
    if((ucX-1)>=0 && board[(ucX-1)%colDimension][ucY%rowDimension]==15 ) ++marked;
    //ucX-1, ucY+1
    if((ucX-1)>=0 && (ucY+1)<rowDimension && board[(ucX-1)%colDimension][(ucY+1)%rowDimension]==15) ++marked;
    //ucX, ucY+1
    if((ucY+1)<rowDimension && board[ucX%colDimension][(ucY+1)%rowDimension]==15) ++marked;
    //ucX+1, ucY+1
    if((ucX+1)<colDimension && (ucY+1)<rowDimension && board[(ucX+1)%colDimension][(ucY+1)%rowDimension]==15) ++marked;
    //ucX+1, ucY
    if((ucX+1)<colDimension && board[(ucX+1)%colDimension][ucY%rowDimension]==15) ++marked;
    //ucX+1, ucY-1
    if((ucX+1)<colDimension && (ucY-1)>=0 && board[(ucX+1)%colDimension][(ucY-1)%rowDimension]==15) ++marked;
    //ucX, ucY-1
    if((ucY-1)>=0 && board[ucX%colDimension][(ucY-1)%rowDimension]==15) ++marked;
    
    effectiveLabel = label - marked;
    return effectiveLabel;


//checked

} //updated

//dimension fixed ok
