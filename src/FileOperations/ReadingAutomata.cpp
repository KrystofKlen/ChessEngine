#include "ReadingAutomata.h"
#include <fstream>


bool ReadingAutomata::readComment(std::istringstream & iss){
    char c1,c2,c3;
    if (! (iss >> c1 >> c2 ) || c1 !='/' || c2 != '*'){
        return false;
    }
    while ( iss >> c3 ){
        if(c2 = '*' && c3 =='/') return true;
        c2 = c3;
    }
    return false;
}

bool ReadingAutomata::readVariable(std::istringstream & iss){
    char c;
    while(iss >> c){
        if(c == '=')return true;
    }
    return false;
}

bool ReadingAutomata::readNumber(std::istringstream & iss, int & num){
    if (iss >> num) return true;
    return false;
}

bool ReadingAutomata::readChar(std::istringstream & iss, char & c){
    if (iss >> c) return true;
    return false;
}

bool ReadingAutomata::readToken(std::istringstream & iss, char token){
    char c;
    if (iss >> c && token == c) return true;
    return false;
}

bool ReadingAutomata::readChessPiece(
    std::istringstream & iss, 
    char & pieceSymbol,
    int & side,
    int & rowIndex,
    int & columnIndex,
    int & numOfSteps,
    int & id
    )
{
    char c1,c2;

    // Read openning bracket
    if( !readToken(iss,'{') ){
        return false;
    }

    // Read chess piece indicator
    if ( ! readVariable(iss) || ! readChar(iss, pieceSymbol) || ! readToken(iss,';') ) return false;

    // Read side
    if ( ! readVariable(iss) || ! readNumber(iss, side) || ! readToken(iss,';') ) return false;

    // Read Coordinates
    if ( ! readVariable(iss) || ! readToken(iss, '{')  ) return false;

    // Row Index
    if ( ! readNumber(iss, rowIndex) || ! readToken(iss,',') ) return false;
    // column Index
    if ( ! readNumber(iss, columnIndex) ) return false;

    if ( ! readToken(iss, '}')  || ! readToken(iss, ';') ) return false;
        
    // Read number of steps
    if( ! readVariable(iss) || ! readNumber(iss, numOfSteps) || ! readToken(iss,';') ) return false;

    // Read ID
    if( ! readVariable(iss) || ! readNumber(iss, id) || ! readToken(iss,';') ) return false;
    
    // Read closing bracket
    if( !readToken(iss,'}') ){
        return false;
    }
        
    return true;
}

bool ReadingAutomata::readFreePositions(std::istringstream & iss, int board[8][8] ){

    if( !readToken(iss,'{') ){
        return false;
    }
    
    for(int rowIndex = 0; rowIndex<8; rowIndex++){

        if( ! readVariable(iss) )return false;
        for(int colIndex = 0; colIndex<8; colIndex++){
            if ( ! readNumber(iss, board[rowIndex][colIndex]) ) {
                return false;
            }
        }
        
        if(!readToken(iss,';') ){
            return false;
        }
    }
    
    if( !readToken(iss,'}') ){
        return false;
    }
    return true;
}

bool ReadingAutomata::readHistoryMove(std::istringstream & iss, HistoryFileData & historyFileData){
    if( ! readToken(iss,'{') ) return false;

    if( ! readVariable(iss) ) return false;
    readChar(iss,historyFileData.mLetter);
    if( !readToken(iss,';') ){
        return false;
    }

    if( ! readVariable(iss) ) return false;
    readNumber(iss,historyFileData.mSide);
    if( !readToken(iss,';') ){
        return false;
    }

    if( ! readVariable(iss) ) return false;

    if( !readToken(iss,'{') 
    || ! readNumber(iss,historyFileData.mCoordinateFromRowIndex) 
    || ! readToken(iss,',') 
    || ! readNumber(iss,historyFileData.mCoordinateFromColIndex) 
    || ! readToken(iss,'}')
    ){
        return false;
    }

    if(!readToken(iss,';')) return false;

    if( ! readVariable(iss) ) return false;

    if( !readToken(iss,'{') 
    || ! readNumber(iss,historyFileData.mCoordinateToRowIndex) 
    || ! readToken(iss,',') 
    || ! readNumber(iss,historyFileData.mCoordinateToColIndex) 
    || ! readToken(iss,'}')
    ){
        return false;
    }

    if(!readToken(iss,';')) return false;
    if(!readToken(iss,'}')) return false;
    return true;
}


bool ReadingAutomata::readGameFromFile(
    const std::string & fileContent,
    int & gameType,
    int & difficulty,
    int & isCheck,
    int & isCheckMate,
    int & isStalmate,
    int & playingSide,
    std::list<PieceFileData> & piecesIn,
    std::list<PieceFileData> & piecesOut,
    int board[8][8],
    std::vector<HistoryFileData> & vctHistoryFileData){
    
    std::istringstream iss (fileContent);
    
    readComment(iss);
    if( ! readVariable(iss) || ! readNumber(iss,gameType) || ! readToken(iss,';') ){
        return false;
    }
    
    readComment(iss);
    if( ! readVariable(iss) || ! readNumber(iss,difficulty) || ! readToken(iss,';') ){
        return false;
    }
    readComment(iss);
    if( ! readVariable(iss) || ! readNumber(iss,isCheck) || ! readToken(iss,';') ){
        return false;
    }
    readComment(iss);
    if( ! readVariable(iss) || ! readNumber(iss, isCheckMate) || ! readToken(iss,';') ){
        return false;
    }
    readComment(iss);
    if( ! readVariable(iss) || ! readNumber(iss,isStalmate) || ! readToken(iss,';') ){
        return false;
    }
    readComment(iss);
    if( ! readVariable(iss) || ! readNumber(iss,playingSide) || ! readToken(iss,';') ){
        return false;
    }

    // read pieces in
    readComment(iss);
    if( ! readVariable(iss) || ! readToken(iss,'{')) return false;
    PieceFileData p ;
    while( readChessPiece(
        iss,
        p.mLetter,
        p.mSide,
        p.mRowIndex,
        p.mColumnIndex,
        p.mNumOfStepsDone,
        p.mId) ){
            piecesIn.push_back(p);
        }
        
    
    if( ! readToken(iss,';')) return false;

    // read pieces out
    readComment(iss);
    if( ! readVariable(iss) || ! readToken(iss,'{')) return false;
    while( readChessPiece(
        iss,
        p.mLetter,
        p.mSide,
        p.mRowIndex,
        p.mColumnIndex,
        p.mNumOfStepsDone,
        p.mId) ){
            piecesOut.push_back(p);
        }

    if(! readToken(iss,';')) return false;
 
    // read free positions
    readComment(iss);
    if( ! readVariable(iss) || ! readFreePositions(iss,board) || ! readToken(iss,';') ) return false;

    // read history moves
    readComment(iss);
    if( !readVariable(iss) || !readToken(iss,'{')  ) return false;
    HistoryFileData historyFileData;
    while( readHistoryMove(
        iss,
        historyFileData
    )){
        vctHistoryFileData.push_back(historyFileData);
    }
    
    return true;
}

bool ReadingAutomata::readVerification(std::string verificationPart, int & verification){
    std::istringstream iss (verificationPart);
    readComment(iss);
    if( ! readVariable(iss) || ! readNumber(iss,verification) ) return false;
    return true;
}

void ReadingAutomata::splitVerificationAndFile(std::string &fileContent, std::string &verificationPart, std::string &gamePart){
    size_t pos = fileContent.find(';');
    if(pos == std::string::npos) return;
    verificationPart = fileContent.substr(0,pos+1);
    gamePart = fileContent.substr(pos+2,fileContent.size());
}


