#include "OnePlayerGame.h"

OnePlayerGame::OnePlayerGame(int difficulty){
    mDifficulty = difficulty;
    if(difficulty == 1){
        ptrComputer = std::make_unique<EasyLevel>();
    }else if(difficulty == 2){
        ptrComputer = std::make_unique<MidLevel>();
    }else{
        ptrComputer = std::make_unique<DifficultLevel>();
    }
}

void OnePlayerGame::startGameLoop(){
    
    Board::setFiguresOnBoard(g.figuresINplayer1, g.figuresINplayer2);
    
    bool gameRunning = true;

    while(gameRunning){

        b.refreshFigureCoordinates();

        //refresh UI
        api.showBoard();
        api.updateFigures(g);

        //check game state
        checkDetected = kingIsInCheck(g,b,1);
        if(checkDetected){
            checkMateDetected = isCheckMate(g,b,1);
        }
        if(!checkDetected){
            stalemateDetected = isStalmate(g,b,1);
        }

        b.refreshFigureCoordinates();

        //update UI if game state changed, (+ end if stalmate or check mate)
        if(checkMateDetected){
            api.showAlert("CHECK MATE,  PRESS ANY KEY TO END");
            return;
        }else if(checkDetected){
            api.showAlert("CHECK");
        }else if(stalemateDetected){
            api.showAlert("STALMATE");
            return;
        }
        
        //pick figure and position to move to
        std::pair< Coordinates,Coordinates > movementFromTo =  api.pickPosition(playingSide, gameRunning);
        if(!gameRunning) break;
        
        //check whether move is OK, if not -> new iteration
        bool moveIsValid = g.moveIsValid(movementFromTo.first,movementFromTo.second);
        if(!moveIsValid) continue;
        if( Board::playField[movementFromTo.first.mRowIndex][movementFromTo.first.mColumnIndex].mFigure->mLetter == 'K'
        && !kingWillNotLandIntoCheck(g,b, movementFromTo.second, 1))
            moveIsValid = false;
        if(!moveIsValid) continue;

        //move is OK ... checking for en passant and castling
        g.checkEnPassant(movementFromTo.second,movementFromTo.first);

        bool castleDetected = g.detectCastlingAttempt(movementFromTo.second, movementFromTo.first);
        if(castleDetected){
            bool succesfullyCasteled = g.castle(b, movementFromTo.second, movementFromTo.first);
            if(!succesfullyCasteled){
                continue;
            }
        }

        //checking if we are kicking out opponents figure
        if(g.checkIfFigureWasKickedOut(movementFromTo.second) && !castleDetected){
            char letter = Board::playField[movementFromTo.second.mRowIndex][movementFromTo.second.mColumnIndex].mFigure
            ->mLetter;

            g.kickout(movementFromTo.second, g.figuresOUTplayer2);
            
            //end game if king was the target
            if(letter == 'K'){
                b.moveFigure(movementFromTo.first, movementFromTo.second, true);
                api.showBoard();
                api.updateFigures(g);
                api.showAlert("GAME OVER , RED WINS, PRESS ANY KEY TO END");
                return;
            }      
        }
        if(!castleDetected)
            b.moveFigure(movementFromTo.first, movementFromTo.second, false);

        //promote pawn if it is at the end
        if(!castleDetected){
            bool pawnAtEnd = g.checkIfPawnReachedEnd(playingSide);
            if(pawnAtEnd){
                api.promotePawn(movementFromTo.second,  g,b);
            }
        }
        

        //updating game state
        stalemateDetected = isStalmate(g,b, 1);
        checkMateDetected = isCheckMate(g,b, 1);

        b.refreshFigureCoordinates();
        //no stalmate or check mate -> pc can make a move
        if( !stalemateDetected && !checkMateDetected ){
            bool pcWin = false;
            ptrComputer->makeNextMove(g, b, pcWin);
            if(pcWin){
                api.showBoard();
                api.updateFigures(g);
                api.showAlert("GAME OVER , BLUE WINS, PRESS ANY KEY TO END");
                return;
            }
        } 
    } 
    

    api.saveGame(
        b,
        g,
        "1",
        std::to_string(mDifficulty),
        checkDetected ? "1" : "0",
        checkMateDetected ? "1" : "0",
        stalemateDetected ? "1" : "0",
        std::to_string(playingSide) 
        );
}