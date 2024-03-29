#include "Rock.h"
//---------- ROCK -----------------------------------------------------------------
Rock::Rock(int side, Coordinates initCoordinates, int numOfStepsDone, int id) 
: Piece(8,'R',side,initCoordinates,numOfStepsDone, id, ROCK_RANK){

}
Rock::Rock(const Rock & ref) 
: Piece(ref.mMaxNumberOfSteps, ref.mLetter, ref.mSide, ref.mCoordinates, ref.mNumOfStepsDone, ref.mId, ROCK_RANK ){

}
void Rock::modifyRank(){
    //To be implemented
}

void Rock::getPossibleMovePositions(std::list<Coordinates> & possiblePositions){
    addMovesVertical(possiblePositions,*this);
    addMovesHorizontal(possiblePositions,*this);
}