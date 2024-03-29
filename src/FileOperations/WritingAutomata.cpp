#include "WritingAutomata.h"

std::string WritingAutomata::getFileContent(
        const SaveData & saveData,
        const std::vector<PieceFileData> & piecesIn,
        const std::vector<PieceFileData> & piecesOut,
        int board[8][8],
        const std::vector<HistoryFileData> & historyMoves
    ){  
        std::vector<std::string> params;
        setParamsGame(saveData,params);
        for(auto piece : piecesIn){
            setParamsPiece(piece, params);
        }
        for(auto piece : piecesOut){
            setParamsPiece(piece, params);
        }
        for(int i = 0; i<8;i++){
            for(int j = 0; j<8;j++){
                params.push_back(std::to_string(board[i][j]));
            }
        }
        for(auto move : historyMoves){
            setParamsHistory(move,params);
        }
        std::string fileTemplate = getFileTemplate(piecesIn,piecesOut,historyMoves);
        return replace_nth_hashtags(fileTemplate,params);
    }


std::string WritingAutomata::getFileTemplate(
    const std::vector<PieceFileData> & piecesIn,
    const std::vector<PieceFileData> & piecesOut,
    const std::vector<HistoryFileData> & historyMoves
    ){
    std::string fileContent = GAME_FILE_SAVING_TEMPLATE;
    std::string strPieces;
    std::string strHistoryMoves;
    
    fileContent.append("PIECES_IN = ");
    for(int i = 0; i<piecesIn.size(); i++){
        strPieces.append(PIECES_FILE_SAVING_TEMPLATE);
    }
    fileContent.append( replaceHashesWithNumbers(COMPOSED_BRACKET_TEMPLATE,{strPieces}) );
    strPieces.clear();
    fileContent.append("PIECES_OUT = ");
    for(int i = 0; i<piecesOut.size(); i++){
        strPieces.append(PIECES_FILE_SAVING_TEMPLATE);
    }
    fileContent.append( replaceHashesWithNumbers(COMPOSED_BRACKET_TEMPLATE,{strPieces}) );

    fileContent.append("FREE_POSITIONS = ");
    fileContent.append(FREE_POSITIONS_TEMPLATE);

    fileContent.append("HISTORY_MOVES = {");
    for(int i = 0; i<historyMoves.size(); i++){
        strHistoryMoves.append(HISTORY_MOVE_TEMPLATE);
    }
    fileContent.append(strHistoryMoves);
    fileContent.append("}");
    
    return fileContent;
}

std::string WritingAutomata::addVerification(std::string & fileContent){
    int key = fileVerifier.getVerificationKeyForFile(fileContent);
    std::string strHash = VERIFICATION_TEMPLATE;
    return replace_nth_hashtags(VERIFICATION_TEMPLATE,{std::to_string(key)});
}

std::string WritingAutomata::getFreePositions(int board[8][8]){
    std::vector<std::string> params;
    for(int rowIndex = 0; rowIndex<8; rowIndex++){
        for(int colIndex = 0; colIndex<8; colIndex++){
            params.push_back(std::to_string(board[rowIndex][colIndex]));
        }
    }
    return replace_nth_hashtags(FREE_POSITIONS_TEMPLATE, params);
}

void WritingAutomata::setParamsPiece(
    const PieceFileData & piece,
    std::vector<std::string> & params
){
    params.push_back(std::string(1,piece.mLetter));
    params.push_back(std::to_string(piece.mSide));
    params.push_back(std::to_string(piece.mRowIndex));
    params.push_back(std::to_string(piece.mColumnIndex));
    params.push_back(std::to_string(piece.mNumOfStepsDone));
    params.push_back(std::to_string(piece.mId));
}

void WritingAutomata::setParamsHistory(
    const HistoryFileData & historyMove,
    std::vector<std::string> & params
){
    params.push_back(std::string(1,historyMove.mLetter));
    params.push_back(std::to_string(historyMove.mSide));
    params.push_back(std::to_string(historyMove.mCoordinateFromRowIndex));
    params.push_back(std::to_string(historyMove.mCoordinateFromColIndex));
    params.push_back(std::to_string(historyMove.mCoordinateToRowIndex));
    params.push_back(std::to_string(historyMove.mCoordinateToColIndex));

}

void WritingAutomata::setParamsGame(
    const SaveData & saveData,
    std::vector<std::string> & params
    )
    {
    params.push_back(std::to_string(saveData.gameType));
    params.push_back(std::to_string(saveData.difficulty));
    params.push_back(std::to_string(saveData.isCheck));
    params.push_back(std::to_string(saveData.isCheckMate));
    params.push_back(std::to_string(saveData.isStalmate));
    params.push_back(std::to_string(saveData.playingSide));
}



std::string WritingAutomata::replace_nth_hashtags(std::string str, std::vector<std::string> params) {
    int param_index = 0;
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == '#') {
            if (param_index < params.size()) {
                str.replace(i, 1, params[param_index]);
                param_index++;
            } else {
                break;
            }
        }
    }
    return str;
}

std::string WritingAutomata::replaceHashesWithNumbers(const std::string& input, const std::vector<std::string>& vct ) {
    std::regex regex("#");
    std::string output = input;
    int count = 0;
    for (std::sregex_iterator iter(output.begin(), output.end(), regex), end; iter != end; ++iter) {
        output.replace(iter->position(), 1, vct[count]);
        ++count;
    }
    return output;
}

std::string WritingAutomata::replace_hashtags(std::string str, std::string replacement) {
    std::regex pattern("#");
    std::string result = std::regex_replace(str, pattern, replacement);
    return result;
}

