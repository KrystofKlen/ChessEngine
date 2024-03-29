#pragma once
#include <set>
#include "GameControl.h"

#include "../Engine/AI/ComputerPlayer.h"
#include "../Engine/AI/EasyLevel.h"
#include "../Engine/AI/MidLevel.h"
#include "../Engine/AI/DifficultLevel.h"

class Tutorial : public GameControl{
public:

    /**
     * @param int difficulty
     */
    Tutorial(std::shared_ptr<ApiBase> apiBase);
    void startGameLoop();
private:
    std::unique_ptr<ComputerPlayer> ptrComputer;

    void handleKickout(const std::pair< Coordinates,Coordinates > & movementFromTo);

    void makeMoveForPC();

    void sendHelpMessage();

    void findPossibleKickouts(const std::list<Coordinates> & possibleMovesPlayer, std::set<char> & possibleKickoutsPlayer, int side);

    void createMessage(std::stringstream & ss, const std::set<char> & possibleKickoutsPlayer1, const std::set<char> & possibleKickoutsPlayer2);

};