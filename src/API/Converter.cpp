#include "Converter.h"


void Converter::readNextChunk(std::stringstream & strStream, char delimiter, const std::string & fileContent, std::string::const_iterator & it){
    strStream.str("");
    while(*it != delimiter){
        strStream<<*it;
        it++;
    }
    it++;
    it++;
}

void Converter::breakUpChunks(const std::string & strChunk, std::list<std::string> & chunks){
    std::stringstream strStream;
    std::string::const_iterator it = strChunk.begin();
    while(it != strChunk.end()){
        while(*it != '\n'){
            strStream<<*it;
            it++;
        }
        chunks.push_back(strStream.str());
        strStream.str("");
        it++;
    }
}

std::shared_ptr<Figure> Converter::createFigureFromChunk(const std::string & figureData){

    int ASCII_CONSTANT = 48;

    std::string::const_iterator it = figureData.begin();
    char letter = *it;
    it++;
    it++;
    int side = *it - ASCII_CONSTANT;
    it++;
    it++;
    it++;
    int rowIndex = *it - ASCII_CONSTANT;
    it++;
    it++;
    int columnIndexIndex = *it - ASCII_CONSTANT;
    it++;
    it++;
    it++;
    std::string buffer;
    while(*it != ','){
        buffer.push_back(*it);
        it++;
    }

    int numOfStepsDone;
    try{
        numOfStepsDone = std::stoi(buffer);
    }catch(...) {
        return nullptr;
    }
    it++;
    buffer.clear();
    while(*it != '}'){
        buffer.push_back(*it);
        it++;
    }

    int id;
    try{
        id = std::stoi(buffer);
    }catch(...){
        return nullptr;
    }
    
    Coordinates c;

    switch (letter)
    {
    case 'K':
        c = {rowIndex,columnIndexIndex};
        return std::make_shared<King> (side,c,numOfStepsDone,id);
        break;
    case 'Q':
        c = {rowIndex,columnIndexIndex};
        return std::make_shared<Queen> (side,c,numOfStepsDone,id);
        break;
    case 'B':
        c = {rowIndex,columnIndexIndex};
        return std::make_shared<Bishop> (side,c,numOfStepsDone,id);
        break;
    case 'H':
        c = {rowIndex,columnIndexIndex};
        return std::make_shared<Knight> (side,c,numOfStepsDone,id);
        break;
    case 'R':
        c = {rowIndex,columnIndexIndex};
        return std::make_shared<Rock> (side,c,numOfStepsDone,id);
        break;
    case 'P':
        c = {rowIndex,columnIndexIndex};
        return std::make_shared<Pawn> (side,c,numOfStepsDone,id);
        break;
    
    default:
        return nullptr;
    }
    return nullptr;
}


void Converter::setIsFreeData(const std::string & strIsFree){
    int strIsFreeIndex = 0;
    for(int rowIndex = 0; rowIndex < 8; rowIndex++,strIsFreeIndex++){
        for(int columnIndex = 0; columnIndex<8; columnIndex++, strIsFreeIndex++){
            if(strIsFree[strIsFreeIndex] == '1'){
                Board::playField[rowIndex][columnIndex].mIsFree = true;
            }else{
                Board::playField[rowIndex][columnIndex].mIsFree = false;
            }
        }
    }
}
