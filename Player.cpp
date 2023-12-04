#include "Player.h"


Player::Player(GameMechs* thisGMRef) {
    mainGameMechsRef = thisGMRef;
    myDir = STOP;
    score = 0;

    int middleX = mainGameMechsRef->getBoardSizeX() / 2;
    int middleY = mainGameMechsRef->getBoardSizeY() / 2;
    objPos initialPos(middleX, middleY, '*'); 

    playerPosList = new objPosArrayList();
    playerPosList->insertHead(initialPos); 
    hasEatenFood = false;
}


int Player::getScore() const {
    if (playerPosList != nullptr) {
        return playerPosList->getSize() - 1; 
    }
    return 0; // Return 0 if playerPosList is null or empty
}


Player::~Player()
{ 
    delete playerPosList;
}


objPosArrayList* Player::getPlayerPosList() 
{
    return playerPosList;
}

void Player::updatePlayerDir()
{
    // PPA3 input processing logic
    char input = mainGameMechsRef->getInput();
    switch (input) {
                case '-': 
                    mainGameMechsRef->setExitTrue(); // Set the exit flag in GameMechs
                    break;
                case 'd':
                    if (myDir != LEFT)
                    {
                    myDir = RIGHT;
                    }
                    break;
                case 'a':
                    if (myDir != RIGHT)
                    {
                    myDir = LEFT;
                    }
                    break;
                case 's':
                    if (myDir != UP)
                    {
                        myDir = DOWN;
                    }
                    break;
                case 'w':
                     if (myDir != DOWN)
                    {
                        myDir = UP;
                    }
                    break;
                default:
                    break;
            }
        
        mainGameMechsRef->clearInput();
        
}

void Player::movePlayer() {
    objPos Brain; 
    
    playerPosList->getHeadElement(Brain); 

    switch (myDir) {
        case UP:
            Brain.y = (Brain.y - 1 + mainGameMechsRef->getBoardSizeY() - 2) % (mainGameMechsRef->getBoardSizeY() - 2);
            break;
        case DOWN:
            Brain.y = (Brain.y + 1) % (mainGameMechsRef->getBoardSizeY() - 2);
            break;
        case LEFT:
            Brain.x = (Brain.x - 1 + mainGameMechsRef->getBoardSizeX() - 2) % (mainGameMechsRef->getBoardSizeX() - 2);
            break;
        case RIGHT:
            Brain.x = (Brain.x + 1) % (mainGameMechsRef->getBoardSizeX() - 2);
            break;
        default:
            break;
    }

    if (Brain.x <= 0) Brain.x += (mainGameMechsRef->getBoardSizeX() - 2);
    if (Brain.y <= 0) Brain.y += (mainGameMechsRef->getBoardSizeY() - 2);

    if (checkSelfCollision(Brain)) {
        mainGameMechsRef->setLoseFlag();
        return;
    }

    playerPosList->insertHead(Brain);

    if (!hasEatenFood) {
        playerPosList->removeTail();
    } else {
        hasEatenFood = false; 
    }
}


void Player::setHasEatenFood(bool hasEaten) {
    hasEatenFood = hasEaten;
}

bool Player::checkSelfCollision(const objPos& BrainPos) const {
    for (int i = 1; i < playerPosList->getSize(); ++i) {
        objPos segment;
        playerPosList->getElement(segment, i);
        if (segment.x == BrainPos.x && segment.y == BrainPos.y) {
            return true; // Collision detected
        }
    }
    return false; // No collision
}
