/**\file Domino.cpp
 * \brief Ficheiro com a implementa�o da classe Domino.
 *
 * @version Projecto 2 - Programa�o - Domino - 10-11 - MIEIC
 * @author Carlos Miguel Correia da Costa - ei09097
 */


#include "Domino.h"


//#ifdef USE_BOOST
//	BOOST_CLASS_EXPORT_GUID(Domino, "Domino")
//#endif


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <constructors-destructors>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
/**
 * Defautl contructor de Domino (explicit para evitar convers�es implicitas)
 * @param _scoreToCompleteGame score minimo que os players t�m que fazer para ganhar o jogo
 */
Domino::Domino(unsigned _scoreToCompleteGame):
			scoreToCompleteGame(_scoreToCompleteGame),
			gameBoneYard(Boneyard()),
			gameBoard(Board()),
			gamePlayers(vector<Player>()),
			currentPlayerIndex(0),
			winnerPlayerIndex(0),
			numberOfConsecutivePlayersUnableToAct(0) {}


/**
 * Destructor de Domino
 */
Domino::~Domino() {}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </constructors-destructors>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <Domino management>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
/**
 * M�todo que inicializa o Domino (Board, Boneyard e players)
 */
void Domino::initDomino() {
	this->gameBoard.initBoard();
	this->gameBoneYard.initBoneyard();
	this->initPlayers();
}


/**
 *  M�todo auxiliar que inicializa os players do Domino
 */
void Domino::initPlayers() {
	//inicializa cada um dos players
	for (size_t i = 0; i < gamePlayers.size(); ++i) {
		initPlayer(gamePlayers[i]);
	}
}


/**
 * M�todo auxiliar que faz o reset aos Bones dos players do Board
 */
void Domino::resetPlayersBones() {
	for (size_t i = 0; i < gamePlayers.size(); ++i) {
		gamePlayers[i].resetPlayerBones();
	}
}


/**
 * M�todo auxiliar que faz a inicializa�ao de um player do Domino
 * @param player Player a inicializar
 */
void Domino::initPlayer(Player& player) {
	//acrescenta os 7 bones ao player caso ainda existam bones disponiveis na bonePile
	for (size_t numPlayerBones = 0; numPlayerBones < 7; ++numPlayerBones) {
		if (!gameBoneYard.empty()) {
			player.getPlayerBones().push_back(gameBoneYard.getLastBone());
		}
	}
}


/**
 * M�todo que prepara a pr�xima round
 */
void Domino::initNextRound() {
	gameBoard.resetBoard();
	gameBoneYard.resetBoneyard();
	resetPlayersBones();
	initPlayers();
	currentPlayerIndex = 0;
	numberOfConsecutivePlayersUnableToAct = 0;
	calcFirstPlayerToAct();
}


/**
 * M�todo que faz a compra de um Bone* ao Boneyard do Domino
 * @return Bone* recebido ou NULL caso n�o haja mais bones para comprar
 */
Bone* Domino::getNewBoneFromBoneyard() {

	return gameBoneYard.getLastBone();
}


/**
 * M�todo que devolve o player que est� a jogar actualmente
 * @return Player que est� a jogar
 */
Player& Domino::getCurrentPlayer() {
	return gamePlayers[currentPlayerIndex];
}


/**
 * M�todo que trata da passagem da vez de um player
 */
void Domino::playerPassedTurn() {
	++numberOfConsecutivePlayersUnableToAct;
}


/**
 * M�todo que calcula qual deve ser o primeiro Player a come�ar uma round
 * Ou seja primeiro come�am os que t�m o maior doubleBone
 * Se nenhum player tiver doubleBones come�a o que tiver a pe�a com maior pontos
 */
void Domino::calcFirstPlayerToAct() {
	size_t candidadePlayerToStart = 0;
	char maxDoubleNumber = 0;

	//check for doublesBones
	for (size_t playerIndex = 0; playerIndex < gamePlayers.size(); ++playerIndex) {
		for (size_t boneIndex = 0; boneIndex < gamePlayers[playerIndex].getPlayerBones().size(); ++boneIndex) {
			Bone* currentBone = gamePlayers[playerIndex].getPlayerBones()[boneIndex];

			if (!currentBone->isDoubleBone())
				continue;

			char playerBoneNumber = currentBone->getFirstNumber();
			if (playerBoneNumber > maxDoubleNumber) {
				maxDoubleNumber = playerBoneNumber;
				candidadePlayerToStart = playerIndex;
			}
		}
	}


	//check for the greatest bone se n�o foi encontrado nenhum doubleBone (mesmo o '0' em ascii � maior do que 0)
	if (maxDoubleNumber == 0) {
		char greatestBoneSum = 0;

		for (size_t playerIndex = 0; playerIndex < gamePlayers.size(); ++playerIndex) {
			for (size_t boneIndex = 0; boneIndex < gamePlayers[playerIndex].getPlayerBones().size(); ++boneIndex) {
				char playerFirstBoneNumber = gamePlayers[playerIndex].getPlayerBones()[boneIndex]->getFirstNumber();
				char playerSecondBoneNumber = gamePlayers[playerIndex].getPlayerBones()[boneIndex]->getSecondNumber();
				char playerBoneNumbersSum = playerFirstBoneNumber + playerSecondBoneNumber;
				if (playerBoneNumbersSum > greatestBoneSum) {
					greatestBoneSum = playerBoneNumbersSum;
					candidadePlayerToStart = playerIndex;
				}
			}
		}
	}

	currentPlayerIndex = candidadePlayerToStart;
}


/**
 * M�todo que actualiza o indice do player que est� a jogar para o pr�ximo player
 */
void Domino::updateCurrentPlayerIndexToNextPlayer() {
	++currentPlayerIndex;

	if (currentPlayerIndex >= gamePlayers.size())
		currentPlayerIndex = 0;
}


/**
 * M�todo que actualiza o indice do player que est� a jogar para o player anterior
 */
void Domino::updateCurrentPlayerIndexToPreviousPlayer() {
	if (currentPlayerIndex > 0) {
		--currentPlayerIndex;
	} else {
		if (currentPlayerIndex == 0)
			currentPlayerIndex = gamePlayers.size() - 1;
	}
}


/**
 * M�todo que verifica se o winnerPlayerIndex est� correcto
 * Ou seja se � mesmo o indice do player que ganhou a round
 */
void Domino::confirmRoundWinnerPlayerIndex() {
	for (size_t i = 0; i < gamePlayers.size(); ++i) {
		if (gamePlayers[i].getPlayerBones().empty()) {
			currentPlayerIndex = i;
			winnerPlayerIndex = i;
			break;
		}
	}

}


/**
 * M�todo que verifica se um determinado player pode jogar um Bone*
 * @param player Player a verificar
 * @return true caso esse player tenha pelo menos um Bone* v�lido para jogar
 */
bool Domino::checkIfPlayerCanAct(Player& player) {
	for (size_t i = 0; i < player.getPlayerBones().size(); ++i) {
		NewBoneConection newBoneConection;
		newBoneConection.boneToInsert = player.getPlayerBones()[i];

		newBoneConection.wingOnBoardToInsertBone = NORTH_WING;
		if (gameBoard.positionValidToDrawBoneOnBoard(newBoneConection, BONE_CONECTION_POINT_UP)   ||
			gameBoard.positionValidToDrawBoneOnBoard(newBoneConection, BONE_CONECTION_POINT_DOWN) ||
			gameBoard.positionValidToDrawBoneOnBoard(newBoneConection, BONE_CONECTION_POINT_LEFT) ||
			gameBoard.positionValidToDrawBoneOnBoard(newBoneConection, BONE_CONECTION_POINT_RIGHT))
			return true;

		newBoneConection.wingOnBoardToInsertBone = SOUTH_WING;
		if (gameBoard.positionValidToDrawBoneOnBoard(newBoneConection, BONE_CONECTION_POINT_UP)   ||
			gameBoard.positionValidToDrawBoneOnBoard(newBoneConection, BONE_CONECTION_POINT_DOWN) ||
			gameBoard.positionValidToDrawBoneOnBoard(newBoneConection, BONE_CONECTION_POINT_LEFT) ||
			gameBoard.positionValidToDrawBoneOnBoard(newBoneConection, BONE_CONECTION_POINT_RIGHT))
			return true;

		newBoneConection.wingOnBoardToInsertBone = EAST_WING;
		if (gameBoard.positionValidToDrawBoneOnBoard(newBoneConection, BONE_CONECTION_POINT_UP)   ||
			gameBoard.positionValidToDrawBoneOnBoard(newBoneConection, BONE_CONECTION_POINT_DOWN) ||
			gameBoard.positionValidToDrawBoneOnBoard(newBoneConection, BONE_CONECTION_POINT_LEFT) ||
			gameBoard.positionValidToDrawBoneOnBoard(newBoneConection, BONE_CONECTION_POINT_RIGHT))
			return true;

		newBoneConection.wingOnBoardToInsertBone = WEST_WING;
		if (gameBoard.positionValidToDrawBoneOnBoard(newBoneConection, BONE_CONECTION_POINT_UP)   ||
			gameBoard.positionValidToDrawBoneOnBoard(newBoneConection, BONE_CONECTION_POINT_DOWN) ||
			gameBoard.positionValidToDrawBoneOnBoard(newBoneConection, BONE_CONECTION_POINT_LEFT) ||
			gameBoard.positionValidToDrawBoneOnBoard(newBoneConection, BONE_CONECTION_POINT_RIGHT))
			return true;
	}

	return false;
}


/**
 * M�todo que verifica se dois Bones se podem conectar
 * @param boneOnBoard Bone que est� no Board
 * @param boneToInsert Bone que se quer inserir
 * @return true caso seja possivel conectar os dois bones
 */
bool Domino::checkIfNodesCanConect(Bone* boneOnBoard, Bone* boneToInsert) {
	if (boneOnBoard == NULL || boneToInsert == NULL)
		return false;


	if (boneOnBoard->getBoneConectedToFirstNumber() == NULL) {
		if (boneOnBoard->getFirstNumber()  == boneToInsert->getFirstNumber()  ||
			boneOnBoard->getFirstNumber()  == boneToInsert->getSecondNumber()) {
			return true;
		}
	}


	if (boneOnBoard->getBoneConectedToSecondNumber() == NULL) {
		if (boneOnBoard->getSecondNumber() == boneToInsert->getFirstNumber()  ||
			boneOnBoard->getSecondNumber() == boneToInsert->getSecondNumber()) {
			return true;
		}
	}

	return false;
}


/**
 * M�todo que verifica se uma round est� bloqueada, ou seja ningu�m terminou a round mas ningu�m pode jogar, por n�o ter nenhum Bone* v�lido
 * @return true caso a round actual esteja bloqueada
 */
bool Domino::checkIfRoundIsBlocked() {
	if (numberOfConsecutivePlayersUnableToAct == gamePlayers.size())
		return true;
	else
		return false;
}


/**
 * M�todo que verifica se a round actual acabou, ou seja se algum player j� n�o tem Bones para jogar
 * @return true caso a round tenha terminado
 */
bool Domino::checkIfRoundIsFinish() {
	for (size_t i = 0; i < gamePlayers.size(); ++i) {
		if (gamePlayers[i].getPlayerBones().size() == 0) {
			winnerPlayerIndex = i;
			return true;
		}
	}

	return false;
}


/**
 * M�todo que verifica se o jogo j� terminou, ou seja se alguem j� atingiu o n�mero minimo de pontos para ganhar um jogo
 * @return true caso o jogo tenha terminado
 */
bool Domino::checkIfGameIsFinish() {
	for (size_t i = 0; i < gamePlayers.size(); ++i) {
		if (gamePlayers[i].getScore() >= scoreToCompleteGame) {
			winnerPlayerIndex = i;
			return true;
		}
	}

	return false;
}


/**
 * M�todo que determina qual foi o player que ganhou o jogo (actualiza a vari�vel winnerPlayerIndex)
 */
void Domino::calcWinner() {
	unsigned maxPlayerPoints = 0;
	size_t candidateToWinner = 0;

	for (size_t playerIndex = 0; playerIndex < gamePlayers.size(); ++playerIndex) {
		unsigned currentPlayerScore = gamePlayers[playerIndex].getScore();
		if (currentPlayerScore >= scoreToCompleteGame) {
			if (currentPlayerScore > maxPlayerPoints) {
				maxPlayerPoints = currentPlayerScore;
				candidateToWinner = playerIndex;
			}
		}
	}

	winnerPlayerIndex = candidateToWinner;
}


/**
 * M�todo que verifica qual � o player que tem menos pontos na sua hand de Bones
 * @return indice do player no vector de players gamePlayers do jogador que tem o menor n�mero de pontos na sua hand de bones
 */
size_t Domino::calcPlayerWithLowerPointsOnRoundBlocked() {
	size_t candidatePlayerIndex = 0;
	int currentMinimumPoints = INT_MAX;

	for (size_t playerIndex = 0; playerIndex < gamePlayers.size(); ++playerIndex) {
		int currentPlayerBonesSum = 0;
		for (size_t boneIndex = 0; boneIndex < gamePlayers[playerIndex].getPlayerBones().size(); ++boneIndex) {
			currentPlayerBonesSum += (int)(gamePlayers[playerIndex].getPlayerBones()[boneIndex]->getFirstNumber() + ASCII_CHAR_TO_NUMBER_OFFSET);
			currentPlayerBonesSum += (int)(gamePlayers[playerIndex].getPlayerBones()[boneIndex]->getSecondNumber() + ASCII_CHAR_TO_NUMBER_OFFSET);
		}

		if (currentPlayerBonesSum < currentMinimumPoints) {
			currentMinimumPoints = currentPlayerBonesSum;
			candidatePlayerIndex = playerIndex;
		}
	}

	return candidatePlayerIndex;
}


/**
 * M�todo que calcula os pontos de bonus a dar a quem ganhou a round
 * @param roundBlocked flag que indica se a round acabou normalmente ou se acabou por ficar bloqueada
 * @return Pontos de bonus a atribuir ao player que ganhou a round (ainda falta fazer o arredondamento para o mais pr�ximo multiplo de 5)
 */
int Domino::calcRemainScoreOnRoundCompletion(bool roundBlocked) {

	if (roundBlocked)
		winnerPlayerIndex = calcPlayerWithLowerPointsOnRoundBlocked();

	int pointsWinnerWonFromOtherPlayers = 0;

	for (size_t i = 0; i < gamePlayers.size(); ++i) {
		if (i != winnerPlayerIndex) {
			//acrescenta a pontua�o dos advers�rios
			pointsWinnerWonFromOtherPlayers += calcSumPlayerBonesPoints(gamePlayers[i]);
		}
	}


	//tira a pontua�o do pr�prio player (s� acontece na roundBlock, porque se acabar normalmente ir� tirar 0
	pointsWinnerWonFromOtherPlayers -= calcSumPlayerBonesPoints(gamePlayers[winnerPlayerIndex]);


	return pointsWinnerWonFromOtherPlayers;
}


/**
 * M�todo auxiliar que calculo o n�mero de pontos que um player tem na sua Bone hand
 * @param player player que onde se vai calcular o numero de pontos
 * @return numero de pontos que o player tem na sua bone hand
 */
unsigned int Domino::calcSumPlayerBonesPoints(Player& player) {
	unsigned sumOfPlayerBonesPoints = 0;

	for (size_t i = 0; i < player.getPlayerBones().size(); ++i) {
		sumOfPlayerBonesPoints += (player.getPlayerBones()[i]->getFirstNumber() + ASCII_CHAR_TO_NUMBER_OFFSET); //-48 para a convers�o de ascii para unsigned
		sumOfPlayerBonesPoints += (player.getPlayerBones()[i]->getSecondNumber() + ASCII_CHAR_TO_NUMBER_OFFSET);//-48 para a convers�o de ascii para unsigned
	}

	return sumOfPlayerBonesPoints;
}


/**
 * M�todo que calula o n�mero de pontos que um player obteria se inseri-se o bone no insertion point dado (�til para a AI)
 * @param newBoneConection Classe auxiliar que compacta o Bone* e o ponto de inser�o no Board
 * @param firstBone flag que indica se � o primeiro Bone* a inserir no Board
 * @return pontos que se obteria se o Bone fosse inserido naquela posi�o
 */
unsigned int Domino::calcScoreOnBoneInsertion(NewBoneConection newBoneConection, bool firstBone) {
	unsigned score = 0;

	if (firstBone) {
		score += newBoneConection.boneToInsert->getFirstNumber() + ASCII_CHAR_TO_NUMBER_OFFSET;
		score += newBoneConection.boneToInsert->getSecondNumber() + ASCII_CHAR_TO_NUMBER_OFFSET;
		return score;
	}

	Bone* boneOnBoardWhereBoneIsGoingToBeInserted = gameBoard.getCorrespondentBoneToBoardInsertPoint(newBoneConection.wingOnBoardToInsertBone);
	Bone* boneToInsert = newBoneConection.boneToInsert;

	// calculo dos pontos dos outros bones nas 3 outras possiveis posi�es de bones com pontua�o sem ser o primeiro doubleBone
	if (newBoneConection.wingOnBoardToInsertBone != EAST_WING &&
		gameBoard.getEastWingBone() != NULL &&
		gameBoard.getEastWingBone() != gameBoard.getFirstDoubleBone())
		score += calcBoneScoreOnBoard(gameBoard.getEastWingBone(), newBoneConection.wingOnBoardToInsertBone);

	if (newBoneConection.wingOnBoardToInsertBone != WEST_WING &&
		gameBoard.getWestWingBone() != NULL &&
		gameBoard.getWestWingBone() != gameBoard.getFirstDoubleBone())
		score += calcBoneScoreOnBoard(gameBoard.getWestWingBone(), newBoneConection.wingOnBoardToInsertBone);

	if (newBoneConection.wingOnBoardToInsertBone != NORTH_WING &&
		gameBoard.getNorthWingBone() != NULL &&
		gameBoard.getNorthWingBone() != gameBoard.getFirstDoubleBone())
		score += calcBoneScoreOnBoard(gameBoard.getNorthWingBone(), newBoneConection.wingOnBoardToInsertBone);

	if (newBoneConection.wingOnBoardToInsertBone != SOUTH_WING &&
		gameBoard.getSouthWingBone() != NULL &&
		gameBoard.getSouthWingBone() != gameBoard.getFirstDoubleBone())
		score += calcBoneScoreOnBoard(gameBoard.getSouthWingBone(), newBoneConection.wingOnBoardToInsertBone);


	//se for o primeiro doubleBone
	if (gameBoard.getFirstDoubleBone() != NULL) {
		unsigned doubleBonePointsCount = 0;
		if (newBoneConection.wingOnBoardToInsertBone != EAST_WING &&
			gameBoard.getEastWingBone() == gameBoard.getFirstDoubleBone())
			++doubleBonePointsCount;

		if (newBoneConection.wingOnBoardToInsertBone != WEST_WING &&
			gameBoard.getWestWingBone() == gameBoard.getFirstDoubleBone())
			++doubleBonePointsCount;

		if (newBoneConection.wingOnBoardToInsertBone != NORTH_WING &&
			gameBoard.getNorthWingBone() == gameBoard.getFirstDoubleBone())
			++doubleBonePointsCount;

		if (newBoneConection.wingOnBoardToInsertBone != SOUTH_WING &&
			gameBoard.getSouthWingBone() == gameBoard.getFirstDoubleBone())
			++doubleBonePointsCount;

		if (doubleBonePointsCount < 3)
			score += ((gameBoard.getFirstDoubleBone()->getFirstNumber() + ASCII_CHAR_TO_NUMBER_OFFSET) * doubleBonePointsCount);
		else
			score += ((gameBoard.getFirstDoubleBone()->getFirstNumber() + ASCII_CHAR_TO_NUMBER_OFFSET) * 2 ); //no m�ximo s� conta os 2 lados do bone (nunca pode ser os 3, quando por exemplo a primeira pe�a � um double...)
	}
	
	//calculo da pontua�o do bone que ir� ser inserido
	unsigned scoreOfBoneToBeInserted = 0;
	bool BonesCanBeConected = false;

	if (boneOnBoardWhereBoneIsGoingToBeInserted != NULL) {
		if (boneToInsert->getFirstNumber() == boneOnBoardWhereBoneIsGoingToBeInserted->getFirstNumber() || boneToInsert->getFirstNumber() == boneOnBoardWhereBoneIsGoingToBeInserted->getSecondNumber()) {
			scoreOfBoneToBeInserted += (boneToInsert->getSecondNumber() + ASCII_CHAR_TO_NUMBER_OFFSET);
			BonesCanBeConected = true;
		}

		if (boneToInsert->getSecondNumber() == boneOnBoardWhereBoneIsGoingToBeInserted->getFirstNumber() || boneToInsert->getSecondNumber() == boneOnBoardWhereBoneIsGoingToBeInserted->getSecondNumber()) {
			scoreOfBoneToBeInserted += (boneToInsert->getFirstNumber() + ASCII_CHAR_TO_NUMBER_OFFSET);
			BonesCanBeConected = true;
		}

		//bone n�o encaixa na posi�o que se pretendia
		if (!BonesCanBeConected)
			return 0;
	}


	score += scoreOfBoneToBeInserted;

	return score;
}


/**
 * M�todo auxiliar que calcula o n�mero de pontos que um Bone que est� no Board tem (para a contagem de pontos da jogada)
 * @param boneOnBoard Bone* que est� no Board
 * @return pontos que esse Bone* pode contribuir para uma jogada
 */
unsigned int Domino::calcBoneScoreOnBoard(Bone* boneOnBoard, BoardInsertPoint boardInsertionPoint) {
	unsigned points = 0;

	if (gameBoard.getNumberOfBonesOnBoard() == 1) {
		if (boardInsertionPoint == EAST_WING) {
			if (boneOnBoard->getBoneConectedToFirstNumber() == NULL)
				points += (boneOnBoard->getFirstNumber() + ASCII_CHAR_TO_NUMBER_OFFSET);
		} else if (boardInsertionPoint == WEST_WING) {
			if (boneOnBoard->getBoneConectedToSecondNumber() == NULL)
				points += (boneOnBoard->getSecondNumber() + ASCII_CHAR_TO_NUMBER_OFFSET);
		}
	} else {
		if (boneOnBoard->getBoneConectedToFirstNumber() == NULL)
			points += (boneOnBoard->getFirstNumber() + ASCII_CHAR_TO_NUMBER_OFFSET);
		else if (boneOnBoard->getBoneConectedToSecondNumber() == NULL)
			points += (boneOnBoard->getSecondNumber() + ASCII_CHAR_TO_NUMBER_OFFSET);
	}

	if (boneOnBoard->isDoubleBone() && boneOnBoard != gameBoard.getFirstDoubleBone())
		points *=2;

	return points;
}


/**
 * M�todo que verifica se a posi�o do bone a inserir na segunda pe�a jogada est� correcta (para garantir a correcta inicialia�o dos pointers)
 * @param boneOnBoardToCheck bone que est� no board
 * @param boneToInsert bone que se quer inserir
 * @param boardInsertionPoint Ponto de conec�o onde se quer inserir o bone
 * @return true caso a inser�o seja valida
 */
bool Domino::checkIfSecondBoneInsertionIsValid(Bone *boneOnBoardToCheck, Bone *boneToInsert, BoardInsertPoint boardInsertionPoint, BoneConectionPoint boneConectionPoint) {
	//caso especial do segundo bone a inserir num bone normal (n�o double), para evitar inser�es na posi�o contr�ria
	//na primeira pe�a o fisrtNumber de uma pe�a n�o double est� sempre do lado esquerdo

	if (boneOnBoardToCheck == NULL || boneToInsert == NULL)
		return false;

	bool invalid = false;
	if (gameBoard.getNumberOfBonesOnBoard() == 1) {
		//se for double
		if (boneOnBoardToCheck->isDoubleBone()) {
			if (boardInsertionPoint == EAST_WING && boneConectionPoint != BONE_CONECTION_POINT_RIGHT) {
				invalid = true;
			} else if (boardInsertionPoint == WEST_WING && boneConectionPoint != BONE_CONECTION_POINT_LEFT) {
				invalid = true;
			} /*else {
				invalid = true;
			}*/
		} else {
		//se n�o for double
			if (boardInsertionPoint == EAST_WING) {
				if ((boneOnBoardToCheck->getFirstNumber() == boneToInsert->getFirstNumber() || boneOnBoardToCheck->getFirstNumber() == boneToInsert->getSecondNumber())) {
					invalid = true;
				}
			} else if (boardInsertionPoint == WEST_WING) {
				if ((boneOnBoardToCheck->getSecondNumber() == boneToInsert->getFirstNumber() || boneOnBoardToCheck->getSecondNumber() == boneToInsert->getSecondNumber())) {
					invalid = true;
				}
			}
		}

	}

	return !invalid;
}


/**
 * M�todo usado na fun�o em baixo pra evitar repeti�o de c�digo
 */
void Domino::updateBestBoneConectionOnBoardWingAI(
		Bone* boneOnBoardToCheck,
		Bone* boneToInsert,
		BoardInsertPoint boardInsertionPoint,
		bool firstDoubleHasPrimaryConectionPointAvailable,
		bool firstBone,
		vector <NewBoneConection>& validBones,
		NewBoneConection& bestBoneConection,
		NewBoneConection& currentBoneConection,
		unsigned& bestScoreFound,
		unsigned& currentBoneScore) {

	if (boneOnBoardToCheck == NULL || boneToInsert == NULL)
		return;

	bool invalid = true;

	if (!firstBone) {
		//primeiro double tem posi�es disponiveis nas laterais (sem ser nas pontas)
		/*if (firstDoubleHasPrimaryConectionPointAvailable && gameBoard.getFirstDoubleBone() == boneOnBoardToCheck) {
			if (gameBoard.getFirstDoubleBone()->getVerticalDirection()) {
				if ((gameBoard.positionValidToDrawBoneOnBoard(currentBoneConection, BONE_CONECTION_POINT_LEFT)) ||
					(gameBoard.positionValidToDrawBoneOnBoard(currentBoneConection, BONE_CONECTION_POINT_RIGHT)) ) {
						invalid = false;
				} else {
//					invalid = true;
					return;
				}
			} else {
				if ((gameBoard.positionValidToDrawBoneOnBoard(currentBoneConection, BONE_CONECTION_POINT_UP)) ||
					(gameBoard.positionValidToDrawBoneOnBoard(currentBoneConection, BONE_CONECTION_POINT_DOWN)) ) {
						invalid = false;
				} else {
//					invalid = true;
					return;
				}
			}

		//inser�o numa pe�a sem ser o primeiro double bone
		} else {*/

			if (gameBoard.positionValidToDrawBoneOnBoard(currentBoneConection, BONE_CONECTION_POINT_UP)   ||
				gameBoard.positionValidToDrawBoneOnBoard(currentBoneConection, BONE_CONECTION_POINT_DOWN) ||
				gameBoard.positionValidToDrawBoneOnBoard(currentBoneConection, BONE_CONECTION_POINT_LEFT) ||
				gameBoard.positionValidToDrawBoneOnBoard(currentBoneConection, BONE_CONECTION_POINT_RIGHT)) {
				invalid = false;
			} else {
				invalid = true;
				return;
			}
		//}



		//caso especial do segundo bone a inserir num bone normal (n�o double), para evitar inser�es na posi�o contr�ria
		//na primeira pe�a o fisrtNumber de uma pe�a n�o double est� sempre do lado esquerdo
		if (checkIfSecondBoneInsertionIsValid(boneOnBoardToCheck, boneToInsert, boardInsertionPoint, BONE_CONECTION_POINT_UP) ||
				checkIfSecondBoneInsertionIsValid(boneOnBoardToCheck, boneToInsert, boardInsertionPoint, BONE_CONECTION_POINT_DOWN) ||
				checkIfSecondBoneInsertionIsValid(boneOnBoardToCheck, boneToInsert, boardInsertionPoint, BONE_CONECTION_POINT_LEFT) ||
				checkIfSecondBoneInsertionIsValid(boneOnBoardToCheck, boneToInsert, boardInsertionPoint, BONE_CONECTION_POINT_RIGHT)) {
			invalid = false;
		} else {
//			invalid = true;
			return;
		}

		//acrescenta�o das conec�es v�lidas e update � melhor
		if (!invalid) {
			currentBoneScore = calcScoreOnBoneInsertion(currentBoneConection, firstBone);

			if ((currentBoneScore >= bestScoreFound) && ((currentBoneScore % 5) == 0)) {
				bestBoneConection = currentBoneConection;
				bestScoreFound = currentBoneScore;
			}

			validBones.push_back(currentBoneConection);
		}


	//first bone
	} else {
		currentBoneScore = calcScoreOnBoneInsertion(currentBoneConection, firstBone);

		if ((currentBoneScore >= bestScoreFound) && ((currentBoneScore % 5) == 0)) {
			bestBoneConection = currentBoneConection;
			bestScoreFound = currentBoneScore;
		}

		validBones.push_back(currentBoneConection);
	}
}


/**
 * M�todo que calcula qual o melhor Bone* a inserir no Board dado um vector de Bones (�til para a AI)
 * @param playerBonesAI vector de Bones de um player
 * @param firstBone flag que indica que o Bone a inserir ser� o primeiro no Board
 * @return Retorna a melhor conec�o que se pode ter tendo em conta a heuristica sugerida no enunciado
 */
NewBoneConection Domino::calcBestBoneToPlaceOnBoardAI(vector <Bone*>& playerBonesAI, bool firstBone) {
	//1� pesquisa pela pe�a de melhor pontua�o
	//se nenhuma tiver pontua�o faz uma 2� pesquisa pelo maior double valido
	//se n�o houver nenhum double v�lido, retorna a primeira pe�a v�lida com maior pontua�o

	vector <NewBoneConection> validBones;

	NewBoneConection bestBoneConection;
	NewBoneConection currentBoneConection;

	unsigned bestScoreFound = 0;
	unsigned currentBoneScore = 0;

	bool firstDoubleHasPrimaryConectionPointAvailable = false;
	if (gameBoard.getFirstDoubleBone() != NULL &&
	   (gameBoard.getFirstDoubleBone()->getBoneConectedOnMiddleDownOrLeft() == NULL ||
		gameBoard.getFirstDoubleBone()->getBoneConectedOnMiddleUpOrRight() == NULL))
		firstDoubleHasPrimaryConectionPointAvailable = true;


#ifdef FIRST_BONE_MUST_BE_A_DOUBLE
	//se for o primeiro bone tem que ser usado um double
	if (!firstBone) {
#endif
		//adi�o das pe�as v�lidas aos vector validBones e verifica�o da pe�a de melhor pontua�o
		for (size_t bonesIndex = 0; bonesIndex < playerBonesAI.size(); ++bonesIndex) {
			Bone* boneOnBoardToCheck = NULL;
			Bone* boneToInsert = playerBonesAI[bonesIndex];
			currentBoneConection.boneToInsert = boneToInsert;


			//EAST_WING
			currentBoneConection.wingOnBoardToInsertBone = EAST_WING;
			boneOnBoardToCheck = gameBoard.getCorrespondentBoneToBoardInsertPoint(EAST_WING);
			updateBestBoneConectionOnBoardWingAI(boneOnBoardToCheck, boneToInsert, EAST_WING, firstDoubleHasPrimaryConectionPointAvailable, firstBone,
					                           validBones, bestBoneConection, currentBoneConection, bestScoreFound, currentBoneScore);


			//WEST_WING
			currentBoneConection.wingOnBoardToInsertBone = WEST_WING;
			boneOnBoardToCheck = gameBoard.getCorrespondentBoneToBoardInsertPoint(WEST_WING);
			updateBestBoneConectionOnBoardWingAI(boneOnBoardToCheck, boneToInsert, WEST_WING, firstDoubleHasPrimaryConectionPointAvailable, firstBone,
											   validBones, bestBoneConection, currentBoneConection, bestScoreFound, currentBoneScore);


			//NORTH_WING
			currentBoneConection.wingOnBoardToInsertBone = NORTH_WING;
			boneOnBoardToCheck = gameBoard.getCorrespondentBoneToBoardInsertPoint(NORTH_WING);
			updateBestBoneConectionOnBoardWingAI(boneOnBoardToCheck, boneToInsert, NORTH_WING, firstDoubleHasPrimaryConectionPointAvailable, firstBone,
											   validBones, bestBoneConection, currentBoneConection, bestScoreFound, currentBoneScore);



			//SOUTH_WING
			currentBoneConection.wingOnBoardToInsertBone = SOUTH_WING;
			boneOnBoardToCheck = gameBoard.getCorrespondentBoneToBoardInsertPoint(SOUTH_WING);
			updateBestBoneConectionOnBoardWingAI(boneOnBoardToCheck, boneToInsert, SOUTH_WING, firstDoubleHasPrimaryConectionPointAvailable, firstBone,
											   validBones, bestBoneConection, currentBoneConection, bestScoreFound, currentBoneScore);
		}


		//encontrou bones que fazem pontos
		if (bestScoreFound != 0) {
			return bestBoneConection;
		}
#ifdef FIRST_BONE_MUST_BE_A_DOUBLE
	}
#endif


	//se nenhuma das pe�as d� pontos tenta livrar-se dos doubleBones maiores
	char bigestDouble = 0;
	NewBoneConection bestDoubleBoneConection;
	bestDoubleBoneConection.boneToInsert = NULL;
	bestDoubleBoneConection.wingOnBoardToInsertBone = WEST_WING;

	//na primeira pe�a n�o vai haver validBones
	if (firstBone) {
		for (size_t i = 0; i < playerBonesAI.size(); ++i) {
			Bone* currentBone = playerBonesAI[i];
			if (currentBone->isDoubleBone()) {
				if (currentBone->getFirstNumber() > bigestDouble) {
					bigestDouble = currentBone->getFirstNumber();
					bestDoubleBoneConection.boneToInsert = currentBone;
				}
			}
		}

	//n�o � a primeira pe�a
	} else {
		for (size_t j = 0; j < validBones.size(); ++j) {
			Bone* currentBone = validBones[j].boneToInsert;
			if (currentBone->isDoubleBone()) {
				if (currentBone->getFirstNumber() > bigestDouble) {
					bigestDouble = currentBone->getFirstNumber();
					bestDoubleBoneConection = validBones[j];
				}
			}
		}
	}


	//encontrou um doubleBone v�lido
	if (bestDoubleBoneConection.boneToInsert != NULL) {
		return bestDoubleBoneConection;
	}


	//se n�o houver pe�as de pontos nem doubles seleciona a pe�a com maior pontos
	//(assim se perder o round, d� menos pontos ao advers�rio)
	char bigestBoneSum = 0;
	NewBoneConection bigestBoneConection;
	bigestBoneConection.boneToInsert = NULL;
	bigestBoneConection.wingOnBoardToInsertBone = WEST_WING;


	for (size_t i = 0; i < (firstBone ? (playerBonesAI.size()) : (validBones.size())); ++i) {
		Bone* currentBone = (firstBone ? (playerBonesAI[i]) : (validBones[i].boneToInsert));
		char currentBoneSum = currentBone->getFirstNumber() + currentBone->getSecondNumber();
		if (currentBoneSum > bigestBoneSum) {
			bigestBoneSum = currentBoneSum;
			if (firstBone)
				bigestBoneConection.boneToInsert = currentBone;
			else
				bigestBoneConection = validBones[i];
		}
	}

	return bigestBoneConection;
}


//responsabilidade de organiza�o das pe�as passada para o utilizador
//BoneConectionPoint Domino::calcBestBoneConectionPointOnBoardAI(NewBoneConection newBoneConection) {
//
//	return BONE_CONECTION_POINT_DOWN;
//}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </Domino management>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <CLI>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
/**
 * M�todo que faz a gest�o de um jogo de Domino usando a interface CLI
 * @param roundLoaded flag que indica se o jogo que se vai fazer teve a round actual carregada de um ficheiro
 */
void Domino::initDominoGameCLI(bool roundLoaded) {
	if (!roundLoaded) {
		utils::clearConsoleScreen();
		addPlayersCLI();

		utils::clearConsoleScreen();
		unsigned scoreToFinishGame = utils::getNumberCin<unsigned>("\nMinimum score to finish game: ");
		this->scoreToCompleteGame = scoreToFinishGame;
		initDomino();
	}

	//se n�o estiver a usar o boost o load apenas carrega as informa�es dos users, por isso � preciso inicializar o domino
#ifndef USE_BOOST
	if (roundLoaded)
		initDomino();
#endif


	bool firstRound = true;
	bool roundBlocked = false;

	while (!checkIfGameIsFinish() || roundLoaded) {

		if (!firstRound && !roundLoaded) {
			initNextRound();
		}

		if (!roundLoaded)
			calcFirstPlayerToAct();

//se o load tenha sido feito sem usar o boost � preciso ver quem come�a primeiro, porque a round come�a do inicio
#ifndef USE_BOOST
		if (roundLoaded)
			calcFirstPlayerToAct();
#endif


		firstRound = false;
		roundLoaded = false;


		bool firstBone = (gameBoard.getNumberOfBonesOnBoard() == 0) ? true : false;

		while (!checkIfRoundIsFinish()) {
			utils::clearConsoleScreen();

			if (checkIfRoundIsBlocked()) {
				roundBlocked = true;
				utils::clearConsoleScreen();
				gameBoard.showBoardCLI();

				for (size_t i = 0; i < gamePlayers.size(); ++i) {
					gamePlayers[i].showPlayerBonesCLI();
				}

				cout << "\n  >>>  A round ficou bloqueada por mais nenhum jogador ter bones validos e nao haver mais bones no Boneyard!\n" << endl;
				utils::getUserInput();
				break;
			}

			bool playerHasValidBone = false;

			if (!firstBone) {
				while (!playerHasValidBone) {
					playerHasValidBone = checkIfPlayerCanAct(getCurrentPlayer());

					if (playerHasValidBone)
						break;

					if (gameBoneYard.empty()) {
						utils::clearConsoleScreen();
						gameBoard.showBoardCLI();
						getCurrentPlayer().showPlayerBonesCLI();
						cout << "\n" << getCurrentPlayer().getName() << " nao tem Bones possiveis para jogar e nao existem mais bones disponiveis para compra!\nPassando a vez...\n\n";
						playerPassedTurn();
						utils::getUserInput();
						break;
					}

					utils::clearConsoleScreen();
					gameBoard.showBoardCLI();
					gamePlayers[currentPlayerIndex].showPlayerBonesCLI();

					cout << "\n" << getCurrentPlayer().getName() << " nao tem bones possiveis para jogar!\nComprando bone do boneyard...\n\n";
					getCurrentPlayer().addBone(gameBoneYard.getLastBone());
					utils::getUserInput();
				}
			} else {
				playerHasValidBone = true;
			}


			if (playerHasValidBone) {
				utils::clearConsoleScreen();
				currentPlayerTurnCLI(firstBone);
				numberOfConsecutivePlayersUnableToAct = 0;
				firstBone = false;
			}

			//passagem para o pr�ximo player
			updateCurrentPlayerIndexToNextPlayer();
		}

		//o player seguinte n�o chegou a jogar, porque n�o se entrou no anterior ciclo
		updateCurrentPlayerIndexToPreviousPlayer();
		confirmRoundWinnerPlayerIndex();


		if (roundBlocked)
			showScoresOnRoundBlock();
		else
			showScoresOnRoundEnd();

		roundBlocked = false;
		//endround
	}

	calcWinner();
	//endgame


	showScoresOnGameEnd();
	initNextRound();
	gameBoneYard.destroyBoneyard();
	gamePlayers.clear();
}


/**
 * M�todo que trata da adi�o dos players ao Domino usando a interface CLI
 */
void Domino::addPlayersCLI() {
	size_t numPlayers = utils::getNumberCin("  >>> How many players do you pretend to have in the game: ", 5, 2, false, "\nThe game can only have from 2 to 4 players!\n\n");

	cout << endl;

	for (size_t i = 0; i < numPlayers; ++i) {
		cout << "\n  >>> Enter name of player " << i << ": ";
		string name = utils::getlineCin();

		bool playerIsAI = utils::getYesNoCin("  >>> Would you like to have player to be controlled by AI (S/N): ", "\nEnter S to have player controlled by AI,\nor N to have it controlled by a human!\n\n");
		gamePlayers.push_back(Player(name, playerIsAI));
	}
}


/// M�todo que mostra o tabuleiro e os bones do player actual
void Domino::showBoardAndPlayerStatus() {
	gameBoard.showBoardCLI();

	cout << endl;

	cout << "\n" << getCurrentPlayer().getName() << " ten " << getCurrentPlayer().getScore() << (getCurrentPlayer().getScore() == 1 ? " point" : " points") << ".\n";
	cout << "Numero de bones no boneyard: " << gameBoneYard.getBonePile().size() << "\n";
	cout << "Controlled by AI: " << (getCurrentPlayer().getAIPlayer() ? "sim" : "nao") << "\n" << endl;


	getCurrentPlayer().showPlayerBonesCLI();

	cout << endl;
}


/**
 * M�todo que trata da jogada do player actual
 * @param firstBone flag que indica se o player vai inserir o primeiro bone na Board
 */
void Domino::currentPlayerTurnCLI(bool firstBone) {
	showBoardAndPlayerStatus();

	BoardInsertPoint boardInsertionPointSelected;
	BoneConectionPoint boneConectionPointSelected;
	NewBoneConection boneConection;
	unsigned newPoints = 0;

	bool playerUseInvalidBone;
	do {			
		playerUseInvalidBone = false;

		if (!getCurrentPlayer().getAIPlayer()) {
			size_t boneIndexSelected = selectBoneToUseFromPlayer();
			Bone* boneSelected = getCurrentPlayer().getBone(boneIndexSelected);
			boneConection.boneToInsert = boneSelected;

			if (firstBone) {
#ifdef FIRST_BONE_MUST_BE_A_DOUBLE
				//primeiro bone tem que ser double (se o player tiver doubles)
				if (getCurrentPlayer().playerHasDoubleBones() && !boneSelected->isDoubleBone()) {
					cout << "\nNa primeira jogada caso tenha um DoubleBone tem que o inserir!\n\n";
					playerUseInvalidBone = true;
					continue;
				}
#endif

				boardInsertionPointSelected = WEST_WING;
				boneConectionPointSelected = BONE_CONECTION_POINT_LEFT;
			} else {
				boardInsertionPointSelected = selectBoardInsertionPointToUseFromPlayer();
				boneConectionPointSelected = selectBoneConectionPointToUseFromPlayer();	
			}
		} else {
			//player com inteligencia artificial
			NewBoneConection AIBestBoneConectionFound = calcBestBoneToPlaceOnBoardAI(getCurrentPlayer().getPlayerBones(), firstBone);
			boneConection.boneToInsert = AIBestBoneConectionFound.boneToInsert;


			boardInsertionPointSelected = AIBestBoneConectionFound.wingOnBoardToInsertBone;

			cout << "A AI selecionou o bone |" << AIBestBoneConectionFound.boneToInsert->getFirstNumber() << "|" << AIBestBoneConectionFound.boneToInsert->getSecondNumber() << "|";
			cout << " para inserir na " << getBoardInsertionPointDescription(AIBestBoneConectionFound.wingOnBoardToInsertBone) << endl;

			if (firstBone) {
				boneConectionPointSelected = BONE_CONECTION_POINT_LEFT;
			} else {
				cout << "Selecione em que posicao quer inserir esse bone..." << endl;
				boneConectionPointSelected = selectBoneConectionPointToUseFromPlayer();
			}

		}

		boneConection.wingOnBoardToInsertBone = boardInsertionPointSelected;

		newPoints = 0;
		if (!firstBone)
			newPoints = calcScoreOnBoneInsertion(boneConection, firstBone);
		else {
			newPoints += boneConection.boneToInsert->getFirstNumber() + ASCII_CHAR_TO_NUMBER_OFFSET;
			newPoints += boneConection.boneToInsert->getSecondNumber() + ASCII_CHAR_TO_NUMBER_OFFSET;
		}

		//verifica se na segunda inser�o, os pointers s�o inicializados correctamente
		if(gameBoard.getNumberOfBonesOnBoard() == 1) {
			Bone* boneOnBoardToCheck = gameBoard.getCorrespondentBoneToBoardInsertPoint(boneConection.wingOnBoardToInsertBone);
			playerUseInvalidBone = !checkIfSecondBoneInsertionIsValid(boneOnBoardToCheck, boneConection.boneToInsert, boneConection.wingOnBoardToInsertBone, boneConectionPointSelected);
		}

		//se a verifica�o em cima passou ou n�o existiu (continuou valido), tenta inserir o bone
		if (!playerUseInvalidBone)
			playerUseInvalidBone = !(gameBoard.insertBone(boneConection, boneConectionPointSelected, firstBone));


		if (playerUseInvalidBone) {
			cout << "\n\nNao foi possivel inserir o bone que especificou na posicao indicada!\n\n";
			utils::getUserInput();
			utils::clearConsoleScreen();
			showBoardAndPlayerStatus();
		}

	} while (playerUseInvalidBone);


	cout << "\n\n\nNumero de pontos feitos nesta jogada: " << newPoints << endl;
	
	//s� os multiplos de  � que contam para a pontua�o
	if ((newPoints > 0) && ((newPoints % 5) == 0)) {
		getCurrentPlayer().addPoints(newPoints);
		cout << "\nForam acrescentados " << newPoints << " pontos a sua pontuacao!\n";
		cout << getCurrentPlayer().getName() << " tem agora " << getCurrentPlayer().getScore() << " pontos!\n" << endl;
	}

	utils::getUserInput();

	//retirada do bone que foi usado
	getCurrentPlayer().removeBone(boneConection.boneToInsert);
}


/**
 * M�todo auxiliar que seleciona o indice do Bone* que o player quer usar usando a interface CLI
 * @return retorna o indice que o player selecionou (supoem-se que foram mostrados os bones e respectivos indices antes de pedir o indice)
 */
size_t Domino::selectBoneToUseFromPlayer() {
	size_t boneIndexSelected = 0;
	bool invalidInput = false;

	cout << "\n   >>> Introduza o numero do bone que quer usar (S to exit and G to save and exit)\n";

	do {
		if (invalidInput)
			cout << "\nIntroduza um dos indices dos bones mostrados em cima (or S / G)!\n\n";


		cout << "  >>> Opcao: ";
		string input = utils::getlineCin();

		if (utils::toUpper(input) == "S")
			exit(0);

		if (utils::toUpper(input) == "G") {
			utils::clearConsoleScreen();
			saveDominoCLI();
			exit(0);
		}

		if (input.empty()) {
			invalidInput = true;
			continue;
		}


		stringstream ssInput(input);

		ssInput >> boneIndexSelected;

		if (ssInput.fail()) {
			ssInput.clear();
			invalidInput = true;
			continue;
		}


		if (boneIndexSelected < 0 || boneIndexSelected >= getCurrentPlayer().getPlayerBones().size())
			invalidInput = true;
		else
			invalidInput = false;

	} while (invalidInput);



	return boneIndexSelected;
}


/**
 * M�todo auxiliar que seleciona em que sitio do Board o player quer inserir o Bone que selecionou anteriormente
 * @return Retorna o sitio do Board onde o player quer inserir o Bone* que tinha selecionado
 */
BoardInsertPoint Domino::selectBoardInsertionPointToUseFromPlayer() {
	BoardInsertPoint insertionPointOnBoard;

	string title("");
	vector <string> colNames;
	colNames.push_back("Pontos de insercao no board");
	vector < vector <string> > columns;
	columns.push_back(vector <string>());
	columns[0].push_back("SOUTH_WING");
	columns[0].push_back("WEST_WING");
	columns[0].push_back("NORTH_WING");
	columns[0].push_back("EAST_WING");



	size_t insertionPointIndex = 0;

	utils::printMenu(title, columns, colNames);
	insertionPointIndex = utils::getNumberCin("\n  >>> Indice do ponto de insercao: ", 5, 0, false, "\nIntroduza um dos indices da tabela!\n\n");


	switch (insertionPointIndex) {
		case 2: {
			insertionPointOnBoard = NORTH_WING;
			break;
		}

		case 0: {
			insertionPointOnBoard = SOUTH_WING;
			break;
		}

		case 3: {
			insertionPointOnBoard = EAST_WING;
			break;
		}

		case 1: {
			insertionPointOnBoard = WEST_WING;
			break;
		}

		default: {
			insertionPointOnBoard = WEST_WING;
			break;
		}
	}

	return insertionPointOnBoard;
}


/**
 * M�todo auxiliar que seleciona em que direc�o o player quer inserir o Bone* que tinha selecionado antes
 * @return Retorna a dirc�o que o player selecionou da tabela mostrada
 */
BoneConectionPoint Domino::selectBoneConectionPointToUseFromPlayer() {
	BoneConectionPoint conectionPointOnBone;

	string title("");
	vector <string> colNames;
	colNames.push_back("Pontos de coneccao no bone selecionado");
	vector < vector <string> > columns;
	columns.push_back(vector <string>());
	columns[0].push_back("BONE_CONECTION_POINT_DOWN");
	columns[0].push_back("BONE_CONECTION_POINT_LEFT");
	columns[0].push_back("BONE_CONECTION_POINT_UP");
	columns[0].push_back("BONE_CONECTION_POINT_RIGHT");


	size_t insertionPointIndex = 0;

	utils::printMenu(title, columns, colNames);
	insertionPointIndex = utils::getNumberCin("\n  >>> Indice do ponto de coneccao (4 para guardar o jogo, 5 para sair): ", 6, 0, false, "\nIntroduza um dos indices da tabela!\n\n");


	switch (insertionPointIndex) {
		case 2: {
			conectionPointOnBone = BONE_CONECTION_POINT_UP;
			break;
		}

		case 0: {
			conectionPointOnBone = BONE_CONECTION_POINT_DOWN;
			break;
		}

		case 1: {
			conectionPointOnBone = BONE_CONECTION_POINT_LEFT;
			break;
		}

		case 3: {
			conectionPointOnBone = BONE_CONECTION_POINT_RIGHT;
			break;
		}

		case 4: {
			utils::clearConsoleScreen();
			saveDominoCLI();
			exit(0);
		}

		case 5: {
			exit(0);
		}

		default: {
			conectionPointOnBone = BONE_CONECTION_POINT_RIGHT;
			break;
		}
	}

	return conectionPointOnBone;
}


/**
 * M�todo auxiliar que mostra os scores quanda a round termina ou � bloqueada e actualiza a pontua�o do vencedor em conformidade
 * @param roundBlocked flag que indica se a round acabou normalmente (false), ou se acabou bloqueada (true)
 */
void Domino::calcAndShowScoresOnRoundEnd(bool roundBlocked) {
	int bonus = calcRemainScoreOnRoundCompletion(roundBlocked);
	int offsetToRound = bonus % 5;
	int roundedBonus = bonus;

	if (offsetToRound < 3)
		roundedBonus -= offsetToRound; //arredonda para o multiplo de 5 a baixo
	else
		roundedBonus += (5 - offsetToRound);//arredonda para o multiplo de 5 a cima

	if (roundedBonus < 0)
		roundedBonus = 0;

	gamePlayers[winnerPlayerIndex].addPoints((unsigned)roundedBonus);

	cout << "---------------------------------------------------------------------------------\n";

	if (roundBlocked) {
		for (size_t i = 0; i < gamePlayers.size(); ++i) {
			cout << " * " << gamePlayers[i].getName() << " tinha na sua mao de bones" << calcSumPlayerBonesPoints(gamePlayers[i]) << " pontos no fim da round!" << endl;
		}
	}

	cout << "\nTotal Score of adversaries: " << bonus;
	cout << "\nTotal Score of adversaries rounded: " << roundedBonus << endl;

	cout << "---------------------------------------------------------------------------------\n\n";

	cout << gamePlayers[winnerPlayerIndex].getName() << " won this round and got " << roundedBonus << " bonus points from its adversaries!\n" << endl;

	cout << "*********************************************************************************\n";
	cout << "Score: " << endl;
	for (size_t i = 0; i < gamePlayers.size(); ++i) {
		cout << " -> " << gamePlayers[i].getName() << " : " << (gamePlayers[i].getScore()) << ((gamePlayers[i].getScore() != 1) ? " points" : " point") << "\n";
	}
	cout << "*********************************************************************************\n";

	cout << "\n" << endl;

	utils::getUserInput();
}


/**
 * M�todo que mostra os scores quanda a round � bloqueada e actualiza a pontua�o do vencedor em conformidade
 */
void Domino::showScoresOnRoundBlock() {
	utils::clearConsoleScreen();

	cout << "############################   DOMINO - ALL FIVES   #############################\n";
	cout << "  >>>                 PONTUACOES NO FIM DESTA ROUND BLOCKED                 <<<  \n";
	cout << "#################################################################################\n\n\n";

	calcAndShowScoresOnRoundEnd(true);
}


/**
 * M�todo que mostra os scores quanda a round termina e actualiza a pontua�o do vencedor em conformidade
 */
void Domino::showScoresOnRoundEnd() {
	utils::clearConsoleScreen();

	cout << "############################   DOMINO - ALL FIVES   #############################\n";
	cout << "  >>>                     PONTUACOES NO FIM DESTA ROUND                     <<<  \n";
	cout << "#################################################################################\n\n\n";

	calcAndShowScoresOnRoundEnd(false);
}


/**
 * M�todo que mostra os scores quanda a jogo termina
 */
void Domino::showScoresOnGameEnd() {
	utils::clearConsoleScreen();

	cout << "############################   DOMINO - ALL FIVES   #############################\n";
	cout << "  >>>                          FINAL SCORE                            <<<  \n";
	cout << "#################################################################################\n\n\n";

	cout << gamePlayers[winnerPlayerIndex].getName() << " won this game!\n" << endl;

	cout << "*********************************************************************************\n";
	cout << "Score: " << endl;
	for (size_t i = 0; i < gamePlayers.size(); ++i) {
		cout << " -> " << gamePlayers[i].getName() << " : " << (gamePlayers[i].getScore()) << ((gamePlayers[i].getScore() != 1) ? " points" : " point") << "\n";
	}
	cout << "*********************************************************************************\n";

	cout << "\n" << endl;

	utils::getUserInput();
}


/**
 * M�todo que trata da interface CLI para o carregamento de um jogo de Domino
 * @return true caso tenha sido feito o carregamento de um jogo anterir com sucesso
 */
bool Domino::loadDominoCLI() {
	utils::clearConsoleScreen();
	cout << "\n  >>> Enter name of file where the game is saved or where you would like to have it loaded from: ";
	string filename = utils::getlineCin();

	try {
		loadDomino(filename);
	} catch (FileDoesntExistException& e) {
		cout << e.what() << endl;
		utils::getUserInput();

		if (utils::getYesNoCin("\nWould you like to try again with a different name? (S/N)? ", "\Enter S or N!\n\n"))
			return loadDominoCLI();

		return false;
	}

	return true;
}


/**
 * M�todo que trata da interface CLI para o backup de um jogo de Domino
 * @return true caso tenha sido feito o backup do jogo actual com sucesso
 */
bool Domino::saveDominoCLI() {
	utils::clearConsoleScreen();
	cout << "\n  >>> Enter name of file to save game:";
	string filename = utils::getlineCin();

	try {
		saveDomino(filename, false);
	} catch (FileAlreadyExistException& e) {
		cout << e.what() << endl;
		utils::getUserInput();

		if (utils::getYesNoCin("\nDo you wish to substitute the file that already exists (S/N)? ", "\nEnter S or N!\n\n"))
			saveDomino(filename, true);
		else if (utils::getYesNoCin("\nWould you like to try again with a different name? (S/N)? ", "\nEnter S or N!\n\n"))
			return saveDominoCLI();

		return false;
	}

	return true;
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </CLI>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <Load - save>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#ifdef USE_BOOST
/**
 * M�todo que trata do carregamento de um jogo a partir de um ficheiro usando a libraria de serializa�o do Boost
 * Caso o ficheiro n�o exista lan�a uma excep�o (FileDoesntExistException)
 * @param filename Nome do ficheiro onde est� o jogo a carregar
 */
void Domino::loadDomino(string filename)  throw(FileDoesntExistException)  {
	ifstream inputFileStream(filename.c_str());

	if (inputFileStream.is_open()) {
		boost::archive::text_iarchive inputArchive(inputFileStream);
		inputArchive >> (*this);

		inputFileStream.close();
	} else {
		throw FileDoesntExistException();
		//n�o conseguiu abrir o ficheiro
	}

}


/**
 * M�todo que trata do backup do jogo actual para um ficheiro usando a libraria de serializa�o do Boost
 * Caso o ficheiro para onde se ia guardar os dados j� exista lan�a uma excep�o (FileAlreadyExistException)
 * @param filename nome do ficheiro onde se vai tentar guardar o jogo actual
 * @param overWrite flag que indica que se o ficheiro existir, fa�a overwrite e n�o lance excep�o
 */
void Domino::saveDomino(string filename, bool overWrite) throw(FileAlreadyExistException) {
	if (!overWrite) {
		ifstream inputFileStream(filename.c_str());
		if (inputFileStream.is_open()) {
			inputFileStream.close();
			throw FileAlreadyExistException();
		} else {
			inputFileStream.close();
		}
	}

	ofstream outputFileStream(filename.c_str());

	boost::archive::text_oarchive outputArchive(outputFileStream);

	outputArchive << (*this);

	outputFileStream.close();
}

#else

/**
 * M�todo que trata do carregamento de um jogo a partir de um ficheiro
 * Caso o ficheiro n�o exista lan�a uma excep�o (FileDoesntExistException)
 * @param filename Nome do ficheiro onde est� o jogo a carregar
 */
void Domino::loadDomino(string filename) throw(FileDoesntExistException) {
	ifstream inputFileStream(filename.c_str());

	if (inputFileStream.is_open()) {
		//pontua�o minima para acabar o jogo
		size_t scoreToFininhGame = 0;
		string scoreToFininhGameStr;
		getline(inputFileStream, scoreToFininhGameStr);
		stringstream scoreToFininhGameSStr(scoreToFininhGameStr);
		scoreToFininhGameSStr >> scoreToFininhGame;
		this->scoreToCompleteGame = scoreToFininhGame;

		
		//player que estava a jogar
		size_t currentPlayer = 0;
		string currentPlayerStr;
		getline(inputFileStream, currentPlayerStr);
		stringstream currentPlayerSStr(currentPlayerStr);
		currentPlayerSStr >> currentPlayer;
		this->setCurrentPlayerIndex(currentPlayer);


		//numero de players que estavam a jogar
		size_t numPlayers = 0;
		string numPlayersStr;
		getline(inputFileStream, numPlayersStr);
		stringstream numPlayersSStr(numPlayersStr);
		numPlayersSStr >> numPlayers;

		for (size_t i = 0; i < numPlayers; ++i) {
			string name;
			string AIPlayerTemp;
			bool AIPlayer = false;
			unsigned score;

			getline(inputFileStream, name);

			getline(inputFileStream, AIPlayerTemp);
			if (AIPlayerTemp == "AI")
				AIPlayer = true;


			string scoreStr;
			getline(inputFileStream, scoreStr);
			stringstream scoreSStr(scoreStr);
			scoreSStr >> score;
			

			Player newPlayer(name, AIPlayer);
			newPlayer.setScore(score);

			gamePlayers.push_back(newPlayer);
		}

		inputFileStream.close();
	} else {
		throw FileDoesntExistException();
		//n�o conseguiu abrir o ficheiro
	}
}


/**
 * M�todo que trata do backup do jogo actual para um ficheiro
 * Caso o ficheiro para onde se ia guardar os dados j� exista lan�a uma excep�o (FileAlreadyExistException)
 * @param filename nome do ficheiro onde se vai tentar guardar o jogo actual
 * @param overWrite flag que indica que se o ficheiro existir, fa�a overwrite e n�o lance excep�o
 */
void Domino::saveDomino(string filename, bool overWrite) throw(FileAlreadyExistException) {
	if (!overWrite) {
		ifstream inputFileStream(filename.c_str());
		if (inputFileStream.is_open()) {
			inputFileStream.close();
			throw FileAlreadyExistException();
		} else {
			inputFileStream.close();
		}
	}


	ofstream outputFileStream(filename.c_str());

	if (outputFileStream.is_open()) {
		outputFileStream << scoreToCompleteGame << "\n";
		outputFileStream << currentPlayerIndex << "\n";
		outputFileStream << gamePlayers.size() << "\n";
		for (size_t i = 0; i < gamePlayers.size(); ++i) {
			outputFileStream << gamePlayers[i].getName() << "\n";
			outputFileStream << (gamePlayers[i].getAIPlayer() ? "AI" : "USER") << "\n";
			outputFileStream << gamePlayers[i].getScore() << "\n";
		}

		outputFileStream.close();
	}
}

#endif
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <Load - save>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <gets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
size_t Domino::getCurrentPlayerIndex() const {
	return currentPlayerIndex;
}

Board Domino::getGameBoard() const {
	return gameBoard;
}

Boneyard Domino::getGameBoneYard() const {
	return gameBoneYard;
}

vector<Player>& Domino::getGamePlayers() {
	return gamePlayers;
}

unsigned Domino::getNumberOfConsecutivePlayersUnableToAct() const {
	return numberOfConsecutivePlayersUnableToAct;
}

unsigned Domino::getScoreToCompleteGame() const {
	return scoreToCompleteGame;
}

size_t Domino::getWinnerPlayerIndex() const {
	return winnerPlayerIndex;
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </gets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <sets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
void Domino::setCurrentPlayerIndex(size_t currentPlayerIndex) {
	this->currentPlayerIndex = currentPlayerIndex;
}

void Domino::setGameBoard(Board gameBoard) {
	this->gameBoard = gameBoard;
}

void Domino::setGameBoneYard(Boneyard gameBoneYard) {
	this->gameBoneYard = gameBoneYard;
}

void Domino::setGamePlayers(const vector<Player>& gamePlayers) {
	this->gamePlayers = gamePlayers;
}

void Domino::setNumberOfConsecutivePlayersUnableToAct(unsigned numberOfConsecutivePlayersUnableToAct) {
	this->numberOfConsecutivePlayersUnableToAct = numberOfConsecutivePlayersUnableToAct;
}

void Domino::setScoreToCompleteGame(unsigned scoreToCompleteGame) {
	this->scoreToCompleteGame = scoreToCompleteGame;
}

void Domino::setWinnerPlayerIndex(size_t winnerPlayerIndex) {
	this->winnerPlayerIndex = winnerPlayerIndex;
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </sets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
