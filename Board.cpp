/**\file Board.cpp
 * \brief Ficheiro com a implementa��o da classe Board.
 *
 * @version Projecto 2 - Programa��o - Domino - 10-11 - MIEIC
 * @author Carlos Miguel Correia da Costa - ei09097
 */


#include "Board.h"


//#ifdef USE_BOOST
//	BOOST_CLASS_EXPORT_GUID(Board, "Board")
//	BOOST_CLASS_EXPORT_GUID(NewBoneConection, "NewBoneConection")
//#endif


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <constructors-destructors>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
/**
 * Default contructor de Board
 */
Board::Board() :
				eastWingBone(NULL),
				westWingBone(NULL),
				northWingBone(NULL),
				southWingBone(NULL),
				firstDoubleBone(NULL),
				numberOfBonesOnBoard(0),
				boardMatrix(vector< vector<char> >()) {}

/**
 * Destructor de Board
 */
Board::~Board() {}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </constructors-destructors>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <Enums>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
/**
 * Operador que devolve o BoneConectionPoint logicamente oposto
 * @param boneConectionPoint BoneConectionPoint que se pretende "inverter"
 * @return BoneConectionPoint que � o inverso do que foi dado
 */
BoneConectionPoint operator!(const BoneConectionPoint& boneConectionPoint) {
	switch (boneConectionPoint) {
		case BONE_CONECTION_POINT_UP: {
			return BONE_CONECTION_POINT_DOWN;
		}

		case BONE_CONECTION_POINT_DOWN: {
			return BONE_CONECTION_POINT_UP;
		}

		case BONE_CONECTION_POINT_LEFT: {
			return BONE_CONECTION_POINT_RIGHT;
		}

		case BONE_CONECTION_POINT_RIGHT: {
			return BONE_CONECTION_POINT_LEFT;
		}

		default:
			return BONE_CONECTION_POINT_LEFT;
		}
}


/**
 * M�todo que devolve a string associada � enumera��o BoardInsertPoint
 * @param boneInsertionPoint BoardInsertPoint que se pretende obter a descri��o
 * @return descri��o da BoardInsertPoint dada
 */
string getBoardInsertionPointDescription(BoardInsertPoint boneInsertionPoint) {
	switch (boneInsertionPoint) {
		case NORTH_WING: {
			return (string)"NORTH_WING";
		}

		case SOUTH_WING: {
			return (string)"SOUTH_WING";
		}

		case EAST_WING: {
			return (string)"EAST_WING";
		}

		case WEST_WING: {
			return (string)"WEST_WING";
		}

		default: {
			return (string)"DEFAULT_WEST_WING";
		}
	}
}


/**
 * Operador que permite mostrar de forma mais legivel a enumera��o BoardInsertPoint
 * @param out stream de output
 * @param boneInsertionPoint BoardInsertPoint que se pretende mostrar
 * @return o stream de output, de forma a permitir encadeamentos de streams
 */
ostream& operator<<(ostream& out, const BoardInsertPoint& boneInsertionPoint) {
	out << getBoardInsertionPointDescription(boneInsertionPoint);
	return out;
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </Enums>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <Board management>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
/**
 * M�todo qye inicializa a charMatrix do Board
 */
void Board::initBoard() {
	boardMatrix.clear();

	for (size_t y = 0; y < BOARD_HEIGHT; ++y) {
		//acrescenata��o de uma nova linha
		boardMatrix.push_back(vector<char>());

		//inicializa��o da linha
		for (size_t x = 0; x < BOARD_WIDTH; ++x) {
			if (y == 0 || y == (BOARD_HEIGHT - 1) || x == 0 || x == (BOARD_WIDTH - 1)) {
				boardMatrix[y].push_back('#');
			} else {
				boardMatrix[y].push_back(BOARD_BACKGROUND_CHAR);
			}
		}
	}
}


/**
 * M�todo que reinicializa o Board
 */
void Board::resetBoard() {
	boardMatrix.clear();
	initBoard();
	eastWingBone = NULL;
	westWingBone = NULL;
	northWingBone = NULL;
	southWingBone = NULL;
	firstDoubleBone = NULL;

	numberOfBonesOnBoard = 0;
}


/**
 * M�todo que retorna o Bone* correspondente ao BoardInsertPoint no Board
 * @param insertionPoint enumera��o com os 4 pontos possiveis de inserir bones
 * @return o apontador para o Bone pretendido ou NULL se esse local de inser��o ainda n�o tiver Bones associados
 */
Bone* Board::getCorrespondentBoneToBoardInsertPoint(BoardInsertPoint insertionPoint) {
	Bone* correspondentBone = NULL;

	switch (insertionPoint) {
		case NORTH_WING: {
			correspondentBone = northWingBone;
			break;
		}

		case SOUTH_WING: {
			correspondentBone = southWingBone;
			break;
		}

		case EAST_WING: {
			correspondentBone = eastWingBone;
			break;
		}

		case WEST_WING: {
			correspondentBone = westWingBone;
			break;
		}

		default: {
			correspondentBone = westWingBone;
			break;
		}
	}

	return correspondentBone;
}


/**
 * M�todo que retorna a enumer��o BoneFirstNumberPositionOnBoard correspospondente � enumer��o BoneConectionPoint
 * @param boneConectionPoint Enumer��o que se quer converter
 * @return Enumer��o correspondente
 */
BoneFirstNumberPositionOnBoard Board::getCorrespondentBoneFirstNumberPositionOnBoard(BoneConectionPoint boneConectionPoint) {
	BoneFirstNumberPositionOnBoard boneFirstNumberPositionOnBoard;

	switch (boneConectionPoint) {
		case BONE_CONECTION_POINT_UP: {
			boneFirstNumberPositionOnBoard = BONE_FIRST_NUMBER_UP;
			break;
		}

		case BONE_CONECTION_POINT_DOWN: {
			boneFirstNumberPositionOnBoard = BONE_FIRST_NUMBER_DOWN;
			break;
		}

		case BONE_CONECTION_POINT_LEFT: {
			boneFirstNumberPositionOnBoard = BONE_FIRST_NUMBER_LEFT;
			break;
		}

		case BONE_CONECTION_POINT_RIGHT: {
			boneFirstNumberPositionOnBoard = BONE_FIRST_NUMBER_RIGHT;
			break;
		}

		default: {
			boneFirstNumberPositionOnBoard = BONE_FIRST_NUMBER_LEFT;
			break;
		}
	}


	return boneFirstNumberPositionOnBoard;
}


/**
 * M�todo que mostra na consola a charMatrix do Board
 */
void Board::showBoardCLI() {
	for (size_t y = 0; y < boardMatrix.size(); ++y) {
		for (size_t x = 0; x < boardMatrix[y].size(); ++x) {
			cout << boardMatrix[y][x];
		}

		cout << "\n";
	}
}


/**
 * M�todo que trata da inser��o de um Bone, num dado ponto de inser��o, numa determinada direc��o no Board
 * @param newBoneConection NewConection com o Bone* a inserir e em que ponto de inser��o do Board
 * @param boneConectionPoint Direc��o em que inserir o Bone*
 * @param firstBone Flag que indica se o Bone a inserir � o primeiro bone do Board
 * @return true caso a inser��o tenha sido feita com sucesso, ou false, caso n�o possa desenhar no sitio pretendido
 * 		   tenha desrespeitado alguma regra do jogo ou fosse colocar o Bone numa posi��o que saisse fora da charMatrix do Board
 */
bool Board::insertBone(NewBoneConection newBoneConection, BoneConectionPoint boneConectionPoint, bool firstBone) {
	Bone* boneOnBoardToCheck = getCorrespondentBoneToBoardInsertPoint(newBoneConection.wingOnBoardToInsertBone);

	if (firstBone) {
		newBoneConection.boneToInsert->setXCenterPosOnBoardMatrix(BOARD_WIDTH / 2);
		newBoneConection.boneToInsert->setYCenterPosOnBoardMatrix(BOARD_HEIGHT / 2);
		
		if (newBoneConection.boneToInsert->isDoubleBone()) {
			newBoneConection.boneToInsert->setVerticalDirection(true);
			newBoneConection.boneToInsert->setBoneFirstNumberPositionOnBoard(BONE_FIRST_NUMBER_DOWN);
		}
		else {
			newBoneConection.boneToInsert->setVerticalDirection(false);
			newBoneConection.boneToInsert->setBoneFirstNumberPositionOnBoard(BONE_FIRST_NUMBER_LEFT);
		}

		drawBoneOnBoard(newBoneConection.boneToInsert);
		updateBoardWingPointers(boneOnBoardToCheck, newBoneConection, boneConectionPoint, firstBone);
		++numberOfBonesOnBoard;
		return true;
	}

	if (boneOnBoardToCheck == NULL)
		return false;

	if (positionValidToDrawBoneOnBoard(newBoneConection, boneConectionPoint)) {
		drawBoneOnBoard(newBoneConection.boneToInsert);
		updateBoardWingPointers(boneOnBoardToCheck, newBoneConection, boneConectionPoint, firstBone);
		
		++numberOfBonesOnBoard;
		return true;
	}

	return false;
}


/**
 * M�todo que faz o update dos pointers que o Board tem para a gest�o das inser��es. S� deve ser executado depois da inser��o.
 * @param boneOnBoardToCheck Bone* onde se inseriu o novo Bone*
 * @param newBoneConection Bone* que foi inserido e sitio de inser��o no Board
 * @param boneConectionPoint Direc��o em que o Bone* foi inserido
 * @param firstBone Flag que indica se o Bone* inserido foi o primeiro da Board
 */
void Board::updateBoardWingPointers(Bone* boneOnBoardToCheck, NewBoneConection newBoneConection, BoneConectionPoint boneConectionPoint, bool firstBone) {		
	Bone* boneToInsert = newBoneConection.boneToInsert;
	BoardInsertPoint wingOnBoardToInsertBone = newBoneConection.wingOnBoardToInsertBone;

	if (firstBone) {
		eastWingBone = boneToInsert;
		westWingBone = boneToInsert;
	}

	if (boneToInsert->isDoubleBone()) {
		if (firstDoubleBone == NULL) {
			firstDoubleBone = boneToInsert;
			northWingBone = boneToInsert;
			southWingBone = boneToInsert;
		}
	}


	if (firstBone)
		return;

	if (boneOnBoardToCheck == NULL || newBoneConection.boneToInsert == NULL)
		return;

	switch (wingOnBoardToInsertBone) {
		case NORTH_WING: {
			northWingBone = boneToInsert;
			break;
		}

		case SOUTH_WING: {
			southWingBone = boneToInsert;
			break;
		}

		case EAST_WING: {
			eastWingBone = boneToInsert;
			break;
		}

		case WEST_WING: {
			westWingBone = boneToInsert;
			break;
		}

		default: {
			westWingBone = boneToInsert;
			break;
		}
	}


	bool pointerSet = false;
	if (boneOnBoardToCheck != NULL && boneOnBoardToCheck->isDoubleBone()) {
		DoubleBone* doubleBoneOnBoardToCheck = (DoubleBone*)boneOnBoardToCheck;
		//laterais do doublebone
		if (boneConectionPoint == BONE_CONECTION_POINT_DOWN || boneConectionPoint == BONE_CONECTION_POINT_LEFT) {
			if (doubleBoneOnBoardToCheck->getBoneConectedOnMiddleDownOrLeft() == NULL) {
				doubleBoneOnBoardToCheck->setBoneConectedOnMiddleDownOrLeft(boneToInsert);
				pointerSet = true;
			}
		} else if (boneConectionPoint == BONE_CONECTION_POINT_UP || boneConectionPoint == BONE_CONECTION_POINT_RIGHT) {
			if (doubleBoneOnBoardToCheck->getBoneConectedOnMiddleUpOrRight() == NULL) {
				doubleBoneOnBoardToCheck->setBoneConectedOnMiddleUpOrRight(boneToInsert);
				pointerSet = true;
			}
		}
	}


	//s� o primeiro doubleBone pode inserir pe�as nas pontas
	if (boneOnBoardToCheck != NULL && boneOnBoardToCheck != firstDoubleBone && boneOnBoardToCheck->isDoubleBone())
		return;


	//inser��o nas pontas do bone caso elas estejam livres
	if (!pointerSet && !firstBone) {
		if ((boneOnBoardToCheck->getBoneConectedToFirstNumber() == NULL) &&
			(boneOnBoardToCheck->getFirstNumber() == boneToInsert->getFirstNumber() ||
			 boneOnBoardToCheck->getFirstNumber() == boneToInsert->getSecondNumber()) ) {

			if (boneOnBoardToCheck->isDoubleBone()) {
				if (boneOnBoardToCheck->getBoneFirstNumberPositionOnBoard() == getCorrespondentBoneFirstNumberPositionOnBoard(boneConectionPoint)) {
					boneOnBoardToCheck->setBoneConectedToFirstNumber(boneToInsert);
				}
			} else {
				boneOnBoardToCheck->setBoneConectedToFirstNumber(boneToInsert);
			}
		}

		if ((boneOnBoardToCheck->getBoneConectedToSecondNumber() == NULL) &&
		   ( boneOnBoardToCheck->getSecondNumber() == boneToInsert->getFirstNumber() ||
			 boneOnBoardToCheck->getSecondNumber() == boneToInsert->getSecondNumber()) ) {

			if (boneOnBoardToCheck->isDoubleBone()) {
				if (boneOnBoardToCheck->getBoneFirstNumberPositionOnBoard() != getCorrespondentBoneFirstNumberPositionOnBoard(boneConectionPoint)) {
					boneOnBoardToCheck->setBoneConectedToSecondNumber(boneToInsert);
				}
			} else {
				boneOnBoardToCheck->setBoneConectedToSecondNumber(boneToInsert);
			}
		}
	}
}


/**
 * M�todo que verifica se � possivel "desenhar" on dado Bone* numa dada direc��o, noutro que j� esteja no Board
 * @param boneOnBoardToCheck Bone* onde se quer inserir o novo Bone*
 * @param boneToInsert Bone* que se quer inserir
 * @param boneConectionPoint Direc��o em que se quer inserir o novo Bone*
 * @return true caso seja possivel desenhar o Bone* na Board
 */
bool Board::positionValidToDrawBoneOnBoard(NewBoneConection newBoneConection, BoneConectionPoint boneConectionPoint) {
	Bone* boneOnBoardToCheck = getCorrespondentBoneToBoardInsertPoint(newBoneConection.wingOnBoardToInsertBone);
	Bone* boneToInsert = newBoneConection.boneToInsert;

	if (boneOnBoardToCheck == NULL || boneToInsert == NULL)
		return false;


	//se a pe�a a inserir for doubleBone, verifica se o user selecionou a direc��o correcta de inser��o
	//(os doubleBones t�m que ser perpendiculares com as outras pe�as
	if (boneToInsert->isDoubleBone()) {
		//se a pe�a onde vai ser inserido o bone est� na horizontal, tem que se inserir o bone na vertical
		//(caso contr�rio tem que se inserir na horizontal)
		if (boneOnBoardToCheck->getVerticalDirection()) {
			if (boneConectionPoint == BONE_CONECTION_POINT_LEFT || boneConectionPoint == BONE_CONECTION_POINT_RIGHT) {
				return false;
			}
		} else {
			if (boneConectionPoint == BONE_CONECTION_POINT_UP || boneConectionPoint == BONE_CONECTION_POINT_DOWN) {
				return false;
			}
		}
	}


	//*********************************************************  <Bone no board � double>  *********************************************************
	if (boneOnBoardToCheck->isDoubleBone()) {
		//n�o se pode ligar 2 doubleBones
		if (boneToInsert->isDoubleBone())
			return false;


		//verifica se est� a inserir na posi��o correspondente ao wing que o user introduziu

		//o primeiro double est� na vertical
		if (boneOnBoardToCheck == firstDoubleBone) {
			if (boneOnBoardToCheck->getVerticalDirection()) {
				if (newBoneConection.wingOnBoardToInsertBone == NORTH_WING && boneConectionPoint != BONE_CONECTION_POINT_UP)
					return false;

				if (newBoneConection.wingOnBoardToInsertBone == SOUTH_WING && boneConectionPoint != BONE_CONECTION_POINT_DOWN)
					return false;

				if (newBoneConection.wingOnBoardToInsertBone == EAST_WING && boneConectionPoint != BONE_CONECTION_POINT_RIGHT)
					return false;

				if (newBoneConection.wingOnBoardToInsertBone == WEST_WING && boneConectionPoint != BONE_CONECTION_POINT_LEFT)
					return false;

			} else {
				//est� na horizontal
				if (newBoneConection.wingOnBoardToInsertBone == NORTH_WING && boneConectionPoint != BONE_CONECTION_POINT_LEFT)
					return false;

				if (newBoneConection.wingOnBoardToInsertBone == SOUTH_WING && boneConectionPoint != BONE_CONECTION_POINT_RIGHT)
					return false;

				if (newBoneConection.wingOnBoardToInsertBone == EAST_WING && boneConectionPoint != BONE_CONECTION_POINT_UP)
					return false;

				if (newBoneConection.wingOnBoardToInsertBone == WEST_WING && boneConectionPoint != BONE_CONECTION_POINT_DOWN)
					return false;
			}
		}



		//se existir uma posi��o livre nas laterais no double bone que est� no board
		if (boneOnBoardToCheck->getFirstNumber() == boneToInsert->getFirstNumber()) {
			return checkIfBonePositionIsValidToDrawOnBoard(boneOnBoardToCheck, boneToInsert, boneConectionPoint, false, true, true);

		} else { if (boneOnBoardToCheck->getFirstNumber() == boneToInsert->getSecondNumber())
			return checkIfBonePositionIsValidToDrawOnBoard(boneOnBoardToCheck, boneToInsert, boneConectionPoint, false, false, true);
		}
	}
	//*********************************************************  </Bone no board � double>  ********************************************************


	//bone no board n�o � double >>>

	//*************************************  <primeiro n�mero no board est� livre para inserir>  ***************************************************
	if (boneOnBoardToCheck->getBoneConectedToFirstNumber() == NULL) {

		//***************************  <primeiro n�mero no board � igual ao primeiro numero da pe�a a inserir>  ************************************
		if (boneOnBoardToCheck->getFirstNumber() == boneToInsert->getFirstNumber()) {
			if (boneToInsert->isDoubleBone()) {
				if (checkIfDoubleBonePositionIsValidToDrawOnBoard(boneOnBoardToCheck, boneToInsert, boneConectionPoint, true, true))
					return true;
			} else {
				if (checkIfBonePositionIsValidToDrawOnBoard(boneOnBoardToCheck, boneToInsert, boneConectionPoint, true, true, false))
					return true;
			}
			//***************************  </primeiro n�mero no board � igual ao primeiro numero da pe�a a inserir>  ***********************************

		} else {
			//****************************  <primeiro n�mero no board � igual ao segundo numero da pe�a a inserir>  ************************************
			if (boneOnBoardToCheck->getFirstNumber() == boneToInsert->getSecondNumber()) {
				if (boneToInsert->isDoubleBone()) {
					if (checkIfDoubleBonePositionIsValidToDrawOnBoard(boneOnBoardToCheck, boneToInsert, boneConectionPoint, true, false))
						return true;
				} else {
					if (checkIfBonePositionIsValidToDrawOnBoard(boneOnBoardToCheck, boneToInsert, boneConectionPoint, true, false, false))
						return true;
				}
			}
			//****************************  </primeiro n�mero no board � igual ao segundo numero da pe�a a inserir>  ***********************************
		}
	}
	//*************************************  </primeiro n�mero no board est� livre para inserir>  **************************************************


	//----------------------------------------------------------------------------------------------------------------------------------------------


	//*************************************  <segundo n�mero no board est� livre para inserir>  ***************************************************
	if (boneOnBoardToCheck->getBoneConectedToSecondNumber() == NULL) {

		//***************************  <segundo n�mero no board � igual ao primeiro numero da pe�a a inserir>  ************************************
		if (boneOnBoardToCheck->getSecondNumber() == boneToInsert->getFirstNumber()) {
			if (boneToInsert->isDoubleBone()) {
				if (checkIfDoubleBonePositionIsValidToDrawOnBoard(boneOnBoardToCheck, boneToInsert, boneConectionPoint, false, true))
					return true;
			} else {
				if (checkIfBonePositionIsValidToDrawOnBoard(boneOnBoardToCheck, boneToInsert, boneConectionPoint, false, true, false))
					return true;
			}
			//***************************  </segundo n�mero no board � igual ao primeiro numero da pe�a a inserir>  ***********************************

		} else {
			//****************************  <segundo n�mero no board � igual ao segundo numero da pe�a a inserir>  ************************************
			if (boneOnBoardToCheck->getSecondNumber() == boneToInsert->getSecondNumber()) {
				if (boneToInsert->isDoubleBone()) {
					if (checkIfDoubleBonePositionIsValidToDrawOnBoard(boneOnBoardToCheck, boneToInsert, boneConectionPoint, false, false))
						return true;
				} else {
					if (checkIfBonePositionIsValidToDrawOnBoard(boneOnBoardToCheck, boneToInsert, boneConectionPoint, false, false, false))
						return true;
				}
			}
			//****************************  </primeiro n�mero no board � igual ao segundo numero da pe�a a inserir>  ***********************************
		}
	}
	//*************************************  </primeiro n�mero no board est� livre para inserir>  **************************************************


	//n�o � possivel inserir nesta pe�a do board
	return false;
}


/**
 * M�todo auxiliar que verifica se um dado doubleBone � v�lido para inserir no Board
 * @param boneOnBoardToCheck Bone* onde o doubleBone seria inserido
 * @param boneToInsert doubleBone a inserir
 * @param boneConectionPoint Dire��o do doubleBone a inserir
 * @param useFirstNumberOnBoneOnBoardToCheck flag que indica para verificar o primeiro n�mero (true) ou o segundo n�mero (false) do bone onde se vai inserir o doubleBone
 * @param useFirstNumberOnBoneToInsert flag que indica para verificar o primeiro n�mero (true) ou o segundo n�mero (false) do doubleBone
 * @return true caso seja possivel inserir o doubleBone que se pretendia
 */
bool Board::checkIfDoubleBonePositionIsValidToDrawOnBoard(Bone* boneOnBoardToCheck, Bone* boneToInsert, BoneConectionPoint boneConectionPoint, bool useFirstNumberOnBoneOnBoardToCheck, bool useFirstNumberOnBoneToInsert) {
	if (boneOnBoardToCheck == NULL || boneToInsert == NULL)
		return false;

	if (!updateBoneToInsertOnBone(boneOnBoardToCheck, boneToInsert, boneConectionPoint, useFirstNumberOnBoneOnBoardToCheck, useFirstNumberOnBoneToInsert))
		return false;

	size_t boneToInsertNewXpos;
	size_t boneToInsertNewYPos;
	bool boneToInsertNewVerticalDirection = !boneOnBoardToCheck->getVerticalDirection();	//doubleBones s�o sempre perpendiculares �s outras pe�as
	BoneFirstNumberPositionOnBoard boneToInsertNewBoneFirstNumberPositionOnBoard = BONE_FIRST_NUMBER_LEFT;
	if (boneToInsertNewVerticalDirection)
		boneToInsertNewBoneFirstNumberPositionOnBoard = BONE_FIRST_NUMBER_DOWN;

	//testar a coloca��o do doubleBone na vertical
	if (boneToInsertNewVerticalDirection) {
		if (boneConectionPoint == BONE_CONECTION_POINT_LEFT)
			boneToInsertNewXpos = boneOnBoardToCheck->getXCenterPosOnBoardMatrix() - 7;
		else
			boneToInsertNewXpos = boneOnBoardToCheck->getXCenterPosOnBoardMatrix() + 7;

		boneToInsertNewYPos = boneOnBoardToCheck->getYCenterPosOnBoardMatrix();
	} else {
		//testar a coloca��o do double bone na horizontal
		boneToInsertNewXpos = boneOnBoardToCheck->getXCenterPosOnBoardMatrix();

		if (boneConectionPoint == BONE_CONECTION_POINT_DOWN)
			boneToInsertNewYPos = boneOnBoardToCheck->getYCenterPosOnBoardMatrix() + 4;
		else
			boneToInsertNewYPos = boneOnBoardToCheck->getYCenterPosOnBoardMatrix() - 4;
	}

	if (checkIfPositionIsClearAndValidToDrawBone(this->boardMatrix, boneToInsertNewXpos, boneToInsertNewYPos, boneToInsertNewVerticalDirection, BOARD_BACKGROUND_CHAR)) {
		boneToInsert->setXCenterPosOnBoardMatrix(boneToInsertNewXpos);
		boneToInsert->setYCenterPosOnBoardMatrix(boneToInsertNewYPos);
		boneToInsert->setVerticalDirection(boneToInsertNewVerticalDirection);
		boneToInsert->setBoneFirstNumberPositionOnBoard(boneToInsertNewBoneFirstNumberPositionOnBoard);
		return true;
	}


	return false;
}


/**
 * M�todo auxiliar que verifica se um dado Bone* � v�lido para inserir no Board
 * @param boneOnBoardToCheck Bone* onde o Bone* seria inserido
 * @param boneToInsert Bone* a inserir
 * @param boneConectionPoint Dire��o do Bone* a inserir
 * @param useFirstNumberOnBoneOnBoardToCheck flag que indica para verificar o primeiro n�mero (true) ou o segundo n�mero (false) do bone onde se vai inserir o Bone*
 * @param useFirstNumberOnBoneToInsert flag que indica para verificar o primeiro n�mero (true) ou o segundo n�mero (false) do Bone* a inserir
 * @param boneOnBoardToCheckIsDouble flag que indica se o bone que est� no Board onde se quer inserir o Bone* � um doubleBone
 * @return true caso seja possivel inserir o Bone* nas condi��es especificadas
 */
bool Board::checkIfBonePositionIsValidToDrawOnBoard(Bone* boneOnBoardToCheck, Bone* boneToInsert, BoneConectionPoint boneConectionPoint, bool useFirstNumberOnBoneOnBoardToCheck, bool useFirstNumberOnBoneToInsert, bool boneOnBoardToCheckIsDouble) {
	if (boneOnBoardToCheck == NULL || boneToInsert == NULL)
		return false;

	if (boneOnBoardToCheckIsDouble) {
	//*********************************************************  <Bone no board � double>  *********************************************************
		if (!updateBoneToInsertOnDoubleBone(boneOnBoardToCheck, boneToInsert, boneConectionPoint, useFirstNumberOnBoneToInsert))
			return false;
	//*********************************************************  <Bone no board � double>  *********************************************************
	} else {
	//*******************************************************  <Bone no board n�o � double>  *******************************************************
		if (!updateBoneToInsertOnBone(boneOnBoardToCheck, boneToInsert, boneConectionPoint, useFirstNumberOnBoneOnBoardToCheck, useFirstNumberOnBoneToInsert))
			return false;
	//*******************************************************  <Bone no board n�o � double>  *******************************************************
	}

	return checkIfPositionIsClearAndValidToDrawBone(this->boardMatrix, boneToInsert->getXCenterPosOnBoardMatrix(), boneToInsert->getYCenterPosOnBoardMatrix(),
			boneToInsert->getVerticalDirection(), BOARD_BACKGROUND_CHAR);
}


/**
 * M�todo auxiliar que faz o update de um Bone* que ir� ser inserido noutro Bone* do Board
 * @param boneOnBoardToCheck bone do Board onde o Bone* ser� inserido
 * @param boneToInsert Bone* que se quer inserir
 * @param boneConectionPoint Direc��o em que se quer inserir o Bone*
 * @param useFirstNumberOnBoneOnBoardToCheck flag que indica para verificar o primeiro n�mero (true) ou o segundo n�mero (false) do bone onde se vai inserir o Bone*
 * @param useFirstNumberOnBoneToInsert flag que indica para verificar o primeiro n�mero (true) ou o segundo n�mero (false) do Bone* a inserir
 * @return true caso o update tenha sido feito com sucesso e o Bone* pode ser inserido, false se as condi��es dadas forem contra as regras ou inv�lidas
 */
bool Board::updateBoneToInsertOnBone(Bone* boneOnBoardToCheck, Bone* boneToInsert, BoneConectionPoint boneConectionPoint, bool useFirstNumberOnBoneOnBoardToCheck, bool useFirstNumberOnBoneToInsert) {
	if (boneOnBoardToCheck == NULL || boneToInsert == NULL)
		return false;

	//propriedades do bone que ser�o alterdadas
	Bone*  boneToInsertNewBoneConectedToFirstNumber = NULL;
	Bone*  boneToInsertNewBoneConectedToSecondNumber = NULL;
	Bone* boneConectedOnMiddleDownOrLeft = NULL;
	Bone* boneConectedOnMiddleUpOrRight = NULL;
	size_t boneToInsertNewXpos = 0;
	size_t boneToInsertNewYPos = 0;
	bool   boneToInsertNewVerticalDirection = false;
	BoneFirstNumberPositionOnBoard boneToInsertNewBoneFirstNumberPositionOnBoard = BONE_FIRST_NUMBER_LEFT;


	//actualiza��o dos pointers
	if (boneToInsert->isDoubleBone()) {
		if (boneConectionPoint == BONE_CONECTION_POINT_UP || boneConectionPoint == BONE_CONECTION_POINT_RIGHT)
			boneConectedOnMiddleDownOrLeft = boneOnBoardToCheck;
		else
			boneConectedOnMiddleUpOrRight = boneOnBoardToCheck;
	} else {
		if (useFirstNumberOnBoneToInsert)
			boneToInsertNewBoneConectedToFirstNumber = boneOnBoardToCheck;
		else
			boneToInsertNewBoneConectedToSecondNumber = boneOnBoardToCheck;
	}


	//actualiza��o das restantes propriedades
	if (boneOnBoardToCheck->getVerticalDirection()) {
	//*****************************************************  <Bone no board est� na vertical>  *****************************************************
		if ((useFirstNumberOnBoneOnBoardToCheck && (boneOnBoardToCheck->getBoneFirstNumberPositionOnBoard() == BONE_FIRST_NUMBER_UP)) ||
		   (!useFirstNumberOnBoneOnBoardToCheck && (boneOnBoardToCheck->getBoneFirstNumberPositionOnBoard() == BONE_FIRST_NUMBER_DOWN)) ) {
			if (boneConectionPoint == BONE_CONECTION_POINT_DOWN) //posi��o inv�lida (ficava em cima do bone do board
				return false;

			if (boneConectionPoint == BONE_CONECTION_POINT_LEFT) {
				boneToInsertNewXpos = boneOnBoardToCheck->getXCenterPosOnBoardMatrix() - 7;
				boneToInsertNewYPos = boneOnBoardToCheck->getYCenterPosOnBoardMatrix() - 1;
				boneToInsertNewVerticalDirection = false;
				boneToInsertNewBoneFirstNumberPositionOnBoard = (useFirstNumberOnBoneToInsert ? BONE_FIRST_NUMBER_RIGHT : BONE_FIRST_NUMBER_LEFT);
			} else if (boneConectionPoint == BONE_CONECTION_POINT_RIGHT) {
				boneToInsertNewXpos = boneOnBoardToCheck->getXCenterPosOnBoardMatrix() + 7;
				boneToInsertNewYPos = boneOnBoardToCheck->getYCenterPosOnBoardMatrix() - 1;
				boneToInsertNewVerticalDirection = false;
				boneToInsertNewBoneFirstNumberPositionOnBoard = (useFirstNumberOnBoneToInsert ? BONE_FIRST_NUMBER_LEFT : BONE_FIRST_NUMBER_RIGHT);
			} else if (boneConectionPoint == BONE_CONECTION_POINT_UP) {
				boneToInsertNewXpos = boneOnBoardToCheck->getXCenterPosOnBoardMatrix();
				boneToInsertNewYPos = boneOnBoardToCheck->getYCenterPosOnBoardMatrix() - 5;
				boneToInsertNewVerticalDirection = true;
				boneToInsertNewBoneFirstNumberPositionOnBoard = (useFirstNumberOnBoneToInsert ? BONE_FIRST_NUMBER_DOWN : BONE_FIRST_NUMBER_UP);
			}

		} else if ((useFirstNumberOnBoneOnBoardToCheck && (boneOnBoardToCheck->getBoneFirstNumberPositionOnBoard() == BONE_FIRST_NUMBER_DOWN)) ||
				  (!useFirstNumberOnBoneOnBoardToCheck && (boneOnBoardToCheck->getBoneFirstNumberPositionOnBoard() == BONE_FIRST_NUMBER_UP)) ) {
			if (boneConectionPoint == BONE_CONECTION_POINT_UP) //posi��o inv�lida (ficava em cima do bone do board
				return false;

			if (boneConectionPoint == BONE_CONECTION_POINT_LEFT) {
				boneToInsertNewXpos = boneOnBoardToCheck->getXCenterPosOnBoardMatrix() - 7;
				boneToInsertNewYPos = boneOnBoardToCheck->getYCenterPosOnBoardMatrix() + 1;
				boneToInsertNewVerticalDirection = false;
				boneToInsertNewBoneFirstNumberPositionOnBoard = (useFirstNumberOnBoneToInsert ? BONE_FIRST_NUMBER_RIGHT : BONE_FIRST_NUMBER_LEFT);
			} else if (boneConectionPoint == BONE_CONECTION_POINT_RIGHT) {
				boneToInsertNewXpos = boneOnBoardToCheck->getXCenterPosOnBoardMatrix() + 7;
				boneToInsertNewYPos = boneOnBoardToCheck->getYCenterPosOnBoardMatrix() + 1;
				boneToInsertNewVerticalDirection = false;
				boneToInsertNewBoneFirstNumberPositionOnBoard = (useFirstNumberOnBoneToInsert ? BONE_FIRST_NUMBER_LEFT : BONE_FIRST_NUMBER_RIGHT);
			} else if (boneConectionPoint == BONE_CONECTION_POINT_DOWN) {
				boneToInsertNewXpos = boneOnBoardToCheck->getXCenterPosOnBoardMatrix();
				boneToInsertNewYPos = boneOnBoardToCheck->getYCenterPosOnBoardMatrix() + 5;
				boneToInsertNewVerticalDirection = true;
				boneToInsertNewBoneFirstNumberPositionOnBoard = (useFirstNumberOnBoneToInsert ? BONE_FIRST_NUMBER_UP : BONE_FIRST_NUMBER_DOWN);
			}
		}
	//*****************************************************  </Bone no board est� na vertical>  ****************************************************
	} else {
	//****************************************************  <Bone no board est� na horizontal>  ****************************************************
		if ((useFirstNumberOnBoneOnBoardToCheck && (boneOnBoardToCheck->getBoneFirstNumberPositionOnBoard() == BONE_FIRST_NUMBER_LEFT)) ||
		   (!useFirstNumberOnBoneOnBoardToCheck && (boneOnBoardToCheck->getBoneFirstNumberPositionOnBoard() == BONE_FIRST_NUMBER_RIGHT)) ) {
			if (boneConectionPoint == BONE_CONECTION_POINT_RIGHT) //posi��o inv�lida (ficava em cima do bone do board
				return false;

			if (boneConectionPoint == BONE_CONECTION_POINT_LEFT) {
				boneToInsertNewXpos = boneOnBoardToCheck->getXCenterPosOnBoardMatrix() - 9;
				boneToInsertNewYPos = boneOnBoardToCheck->getYCenterPosOnBoardMatrix();
				boneToInsertNewVerticalDirection = false;
				boneToInsertNewBoneFirstNumberPositionOnBoard = (useFirstNumberOnBoneToInsert ? BONE_FIRST_NUMBER_RIGHT : BONE_FIRST_NUMBER_LEFT);
			} else if (boneConectionPoint == BONE_CONECTION_POINT_UP) {
				boneToInsertNewXpos = boneOnBoardToCheck->getXCenterPosOnBoardMatrix() - 2;
				boneToInsertNewYPos = boneOnBoardToCheck->getYCenterPosOnBoardMatrix() - 4;
				boneToInsertNewVerticalDirection = true;
				boneToInsertNewBoneFirstNumberPositionOnBoard = (useFirstNumberOnBoneToInsert ? BONE_FIRST_NUMBER_DOWN : BONE_FIRST_NUMBER_UP);
			} else if (boneConectionPoint == BONE_CONECTION_POINT_DOWN) {
				boneToInsertNewXpos = boneOnBoardToCheck->getXCenterPosOnBoardMatrix() - 2;
				boneToInsertNewYPos = boneOnBoardToCheck->getYCenterPosOnBoardMatrix() + 4;
				boneToInsertNewVerticalDirection = true;
				boneToInsertNewBoneFirstNumberPositionOnBoard = (useFirstNumberOnBoneToInsert ? BONE_FIRST_NUMBER_UP : BONE_FIRST_NUMBER_DOWN);
			}

		} else if ((useFirstNumberOnBoneOnBoardToCheck && (boneOnBoardToCheck->getBoneFirstNumberPositionOnBoard() == BONE_FIRST_NUMBER_RIGHT)) ||
				  (!useFirstNumberOnBoneOnBoardToCheck && (boneOnBoardToCheck->getBoneFirstNumberPositionOnBoard() == BONE_FIRST_NUMBER_LEFT)) ) {
			if (boneConectionPoint == BONE_CONECTION_POINT_LEFT) //posi��o inv�lida (ficava em cima do bone do board
				return false;

			if (boneConectionPoint == BONE_CONECTION_POINT_UP) {
				boneToInsertNewXpos = boneOnBoardToCheck->getXCenterPosOnBoardMatrix() + 2;
				boneToInsertNewYPos = boneOnBoardToCheck->getYCenterPosOnBoardMatrix() - 4;
				boneToInsertNewVerticalDirection = true;
				boneToInsertNewBoneFirstNumberPositionOnBoard = (useFirstNumberOnBoneToInsert ? BONE_FIRST_NUMBER_DOWN : BONE_FIRST_NUMBER_UP);
			} else if (boneConectionPoint == BONE_CONECTION_POINT_RIGHT) {
				boneToInsertNewXpos = boneOnBoardToCheck->getXCenterPosOnBoardMatrix() + 9;
				boneToInsertNewYPos = boneOnBoardToCheck->getYCenterPosOnBoardMatrix();
				boneToInsertNewVerticalDirection = false;
				boneToInsertNewBoneFirstNumberPositionOnBoard = (useFirstNumberOnBoneToInsert ? BONE_FIRST_NUMBER_LEFT : BONE_FIRST_NUMBER_RIGHT);
			} else if (boneConectionPoint == BONE_CONECTION_POINT_DOWN) {
				boneToInsertNewXpos = boneOnBoardToCheck->getXCenterPosOnBoardMatrix() + 2;
				boneToInsertNewYPos = boneOnBoardToCheck->getYCenterPosOnBoardMatrix() + 4;
				boneToInsertNewVerticalDirection = true;
				boneToInsertNewBoneFirstNumberPositionOnBoard = (useFirstNumberOnBoneToInsert ? BONE_FIRST_NUMBER_UP : BONE_FIRST_NUMBER_DOWN);
			}
		}
	//****************************************************  </Bone no board est� na horizontal>  ***************************************************
	}


	//set �s novas propriedades do bone a inserir num bone do board
	boneToInsert->setXCenterPosOnBoardMatrix(boneToInsertNewXpos);
	boneToInsert->setYCenterPosOnBoardMatrix(boneToInsertNewYPos);
	boneToInsert->setBoneConectedToFirstNumber(boneToInsertNewBoneConectedToFirstNumber);
	boneToInsert->setBoneConectedToSecondNumber(boneToInsertNewBoneConectedToSecondNumber);
	boneToInsert->setVerticalDirection(boneToInsertNewVerticalDirection);
	boneToInsert->setBoneFirstNumberPositionOnBoard(boneToInsertNewBoneFirstNumberPositionOnBoard);

	if (boneToInsert->isDoubleBone()) {
		((DoubleBone*)boneToInsert)->setBoneConectedOnMiddleDownOrLeft(boneConectedOnMiddleDownOrLeft);
		((DoubleBone*)boneToInsert)->setBoneConectedOnMiddleUpOrRight(boneConectedOnMiddleUpOrRight);
	}

	return true;
}


/**
 * M�todo auxiliar que faz o update de um Bone* que ir� ser inserido num doubleBone do Board
 * @param boneOnBoardToCheck doubleBOne do Board onde o Bone* ser� inserido
 * @param boneToInsert Bone* que se quer inserir
 * @param boneConectionPoint Direc��o em que se quer inserir o Bone*
 * @param useFirstNumberOnBoneToInsert flag que indica para verificar o primeiro n�mero (true) ou o segundo n�mero (false) do bone onde se vai inserir o Bone*
 * @return true caso o update tenha sido feito com sucesso e o Bone* pode ser inserido, false se as condi��es dadas forem contra as regras ou inv�lidas
 */
bool Board::updateBoneToInsertOnDoubleBone(Bone* boneOnBoardToCheck, Bone* boneToInsert, BoneConectionPoint boneConectionPoint, bool useFirstNumberOnBoneToInsert) {
	if (boneOnBoardToCheck == NULL || boneToInsert == NULL)
		return false;

	//fun��o apenas trata da inser��o de bones em DoubleBones no board (ou seja a pe�a do board tem que ser um DoubleBone)
	if (!boneOnBoardToCheck->isDoubleBone())
		return false;

	DoubleBone* doubleBoneOnBoardToCheck = (DoubleBone*)boneOnBoardToCheck;

	//propriedades do bone que ser�o alterdadas
	Bone*  boneToInsertNewBoneConectedToFirstNumber = NULL;
	Bone*  boneToInsertNewBoneConectedToSecondNumber = NULL;
	size_t boneToInsertNewXpos = 0;
	size_t boneToInsertNewYPos = 0;
	bool   boneToInsertNewVerticalDirection = false;
	BoneFirstNumberPositionOnBoard boneToInsertNewBoneFirstNumberPositionOnBoard = BONE_FIRST_NUMBER_LEFT;


	//actualiza��o dos pointers
	if (useFirstNumberOnBoneToInsert)
		boneToInsertNewBoneConectedToFirstNumber = boneOnBoardToCheck;
	else
		boneToInsertNewBoneConectedToSecondNumber = boneOnBoardToCheck;



	//actualiza��o das restatntes propriedades
	if (doubleBoneOnBoardToCheck->getVerticalDirection()) {
	//**************************************************  <DoubleBone no board est� na vertical>  **************************************************
		if ( doubleBoneOnBoardToCheck->getBoneConectedOnMiddleDownOrLeft() == NULL || doubleBoneOnBoardToCheck->getBoneConectedOnMiddleUpOrRight() == NULL) {
			//as primeiras pe�as a inserir num doubleBone t�m sempre que ser perpendiculares a ele
			if (boneConectionPoint == BONE_CONECTION_POINT_UP || boneConectionPoint == BONE_CONECTION_POINT_DOWN)
				return false;
		}

		if (boneConectionPoint == BONE_CONECTION_POINT_LEFT) {
			if (doubleBoneOnBoardToCheck->getBoneConectedOnMiddleDownOrLeft() != NULL)
				return false; //j� tem um bone inserido na posi��o onde se quer adicionar a pe�a
			else {
				boneToInsertNewXpos = doubleBoneOnBoardToCheck->getXCenterPosOnBoardMatrix() - 7;
				boneToInsertNewYPos = doubleBoneOnBoardToCheck->getYCenterPosOnBoardMatrix();
				boneToInsertNewVerticalDirection = false;
				boneToInsertNewBoneFirstNumberPositionOnBoard = (useFirstNumberOnBoneToInsert ? BONE_FIRST_NUMBER_RIGHT : BONE_FIRST_NUMBER_LEFT);
			}
		} else if (boneConectionPoint == BONE_CONECTION_POINT_RIGHT) {
			if (doubleBoneOnBoardToCheck->getBoneConectedOnMiddleUpOrRight() != NULL)
				return false; //j� tem um bone inserido na posi��o onde se quer adicionar a pe�a
			else {
				boneToInsertNewXpos = doubleBoneOnBoardToCheck->getXCenterPosOnBoardMatrix() + 7;
				boneToInsertNewYPos = doubleBoneOnBoardToCheck->getYCenterPosOnBoardMatrix();
				boneToInsertNewVerticalDirection = false;
				boneToInsertNewBoneFirstNumberPositionOnBoard = (useFirstNumberOnBoneToInsert ? BONE_FIRST_NUMBER_LEFT : BONE_FIRST_NUMBER_RIGHT);
			}
		} else if (boneConectionPoint == BONE_CONECTION_POINT_UP) {
			if ((doubleBoneOnBoardToCheck->getBoneFirstNumberPositionOnBoard() == BONE_FIRST_NUMBER_UP && boneOnBoardToCheck->getBoneConectedToFirstNumber() != NULL) ||
				(doubleBoneOnBoardToCheck->getBoneFirstNumberPositionOnBoard() == BONE_FIRST_NUMBER_DOWN && boneOnBoardToCheck->getBoneConectedToSecondNumber() != NULL)	)
				return false; //j� tem um bone inserido na posi��o onde se quer adicionar a pe�a
			else {
				boneToInsertNewXpos = doubleBoneOnBoardToCheck->getXCenterPosOnBoardMatrix();
				boneToInsertNewYPos = doubleBoneOnBoardToCheck->getYCenterPosOnBoardMatrix() - 5;
				boneToInsertNewVerticalDirection = true;
				boneToInsertNewBoneFirstNumberPositionOnBoard = (useFirstNumberOnBoneToInsert ? BONE_FIRST_NUMBER_DOWN : BONE_FIRST_NUMBER_UP);
			}
		} else if (boneConectionPoint == BONE_CONECTION_POINT_DOWN) {
			if ((doubleBoneOnBoardToCheck->getBoneFirstNumberPositionOnBoard() == BONE_FIRST_NUMBER_DOWN && boneOnBoardToCheck->getBoneConectedToFirstNumber() != NULL) ||
				(doubleBoneOnBoardToCheck->getBoneFirstNumberPositionOnBoard() == BONE_FIRST_NUMBER_UP && boneOnBoardToCheck->getBoneConectedToSecondNumber() != NULL)	)
				return false; //j� tem um bone inserido na posi��o onde se quer adicionar a pe�a
			else {
				boneToInsertNewXpos = doubleBoneOnBoardToCheck->getXCenterPosOnBoardMatrix();
				boneToInsertNewYPos = doubleBoneOnBoardToCheck->getYCenterPosOnBoardMatrix() + 5;
				boneToInsertNewVerticalDirection = true;
				boneToInsertNewBoneFirstNumberPositionOnBoard = (useFirstNumberOnBoneToInsert ? BONE_FIRST_NUMBER_UP : BONE_FIRST_NUMBER_DOWN);
			}
		}
	//**************************************************  </DubleBone no board est� na vertical>  **************************************************
	} else {
	//*************************************************  <DoubleBone no board est� na horizontal>  *************************************************
		if ( doubleBoneOnBoardToCheck->getBoneConectedOnMiddleDownOrLeft() == NULL || doubleBoneOnBoardToCheck->getBoneConectedOnMiddleUpOrRight() == NULL) {
			//as primeiras pe�as a inserir num doubleBone t�m sempre que ser perpendiculares a ele
			if (boneConectionPoint == BONE_CONECTION_POINT_LEFT || boneConectionPoint == BONE_CONECTION_POINT_RIGHT)
				return false;
		}


		if (boneConectionPoint == BONE_CONECTION_POINT_LEFT) {
			if (doubleBoneOnBoardToCheck->getBoneConectedOnMiddleDownOrLeft() != NULL)
				return false; //j� tem um bone inserido na posi��o onde se quer adicionar a pe�a
			else {
				boneToInsertNewXpos = doubleBoneOnBoardToCheck->getXCenterPosOnBoardMatrix() - 9;
				boneToInsertNewYPos = doubleBoneOnBoardToCheck->getYCenterPosOnBoardMatrix();
				boneToInsertNewVerticalDirection = false;
				boneToInsertNewBoneFirstNumberPositionOnBoard = (useFirstNumberOnBoneToInsert ? BONE_FIRST_NUMBER_RIGHT : BONE_FIRST_NUMBER_LEFT);
			}
		} else if (boneConectionPoint == BONE_CONECTION_POINT_RIGHT) {
			if (doubleBoneOnBoardToCheck->getBoneConectedOnMiddleUpOrRight() != NULL)
				return false; //j� tem um bone inserido na posi��o onde se quer adicionar a pe�a
			else {
				boneToInsertNewXpos = doubleBoneOnBoardToCheck->getXCenterPosOnBoardMatrix() + 9;
				boneToInsertNewYPos = doubleBoneOnBoardToCheck->getYCenterPosOnBoardMatrix();
				boneToInsertNewVerticalDirection = false;
				boneToInsertNewBoneFirstNumberPositionOnBoard = (useFirstNumberOnBoneToInsert ? BONE_FIRST_NUMBER_LEFT : BONE_FIRST_NUMBER_RIGHT);
			}
		} else if (boneConectionPoint == BONE_CONECTION_POINT_UP) {
			if ((doubleBoneOnBoardToCheck->getBoneFirstNumberPositionOnBoard() == BONE_FIRST_NUMBER_UP && boneOnBoardToCheck->getBoneConectedToFirstNumber() != NULL) ||
					(doubleBoneOnBoardToCheck->getBoneFirstNumberPositionOnBoard() == BONE_FIRST_NUMBER_DOWN && boneOnBoardToCheck->getBoneConectedToSecondNumber() != NULL)	)
				return false; //j� tem um bone inserido na posi��o onde se quer adicionar a pe�a
			else {
				boneToInsertNewXpos = doubleBoneOnBoardToCheck->getXCenterPosOnBoardMatrix();
				boneToInsertNewYPos = doubleBoneOnBoardToCheck->getYCenterPosOnBoardMatrix() - 4;
				boneToInsertNewVerticalDirection = true;
				boneToInsertNewBoneFirstNumberPositionOnBoard = (useFirstNumberOnBoneToInsert ? BONE_FIRST_NUMBER_DOWN : BONE_FIRST_NUMBER_UP);
			}
		} else if (boneConectionPoint == BONE_CONECTION_POINT_DOWN) {
			if ((doubleBoneOnBoardToCheck->getBoneFirstNumberPositionOnBoard() == BONE_FIRST_NUMBER_DOWN && boneOnBoardToCheck->getBoneConectedToFirstNumber() != NULL) ||
				(doubleBoneOnBoardToCheck->getBoneFirstNumberPositionOnBoard() == BONE_FIRST_NUMBER_UP && boneOnBoardToCheck->getBoneConectedToSecondNumber() != NULL)	)
				return false; //j� tem um bone inserido na posi��o onde se quer adicionar a pe�a
			else {
				boneToInsertNewXpos = doubleBoneOnBoardToCheck->getXCenterPosOnBoardMatrix();
				boneToInsertNewYPos = doubleBoneOnBoardToCheck->getYCenterPosOnBoardMatrix() + 4;
				boneToInsertNewVerticalDirection = true;
				boneToInsertNewBoneFirstNumberPositionOnBoard = (useFirstNumberOnBoneToInsert ? BONE_FIRST_NUMBER_UP : BONE_FIRST_NUMBER_DOWN);
			}
		}
	//*************************************************  </DoubleBone no board est� na horizontal>  ************************************************
	}


	//set �s novas propriedades do bone a inserir num DoubleBone do board
	boneToInsert->setXCenterPosOnBoardMatrix(boneToInsertNewXpos);
	boneToInsert->setYCenterPosOnBoardMatrix(boneToInsertNewYPos);
	boneToInsert->setBoneConectedToFirstNumber(boneToInsertNewBoneConectedToFirstNumber);
	boneToInsert->setBoneConectedToSecondNumber(boneToInsertNewBoneConectedToSecondNumber);
	boneToInsert->setVerticalDirection(boneToInsertNewVerticalDirection);
	boneToInsert->setBoneFirstNumberPositionOnBoard(boneToInsertNewBoneFirstNumberPositionOnBoard);

	return true;
}


/**
 * M�todo est�tico que verifica se possivel desenhar um dado Bone numa dada posi��o de uma charMatrix
 * (ao estar est�tico permite ser usado noutras classes para evitar c�digo repetido.
 * @param charMatrix charMatrix onde se vai verificar se � v�lido o desenho de um Bone*
 * @param boneXCenter posi��o central do Bone* e que est� no eixo dos x da charMatrix onde se quer desennhar o Bone*
 * @param boneYCenter  posi��o central do Bone* e que est� no eixo dos y da charMatrix onde se quer desennhar o Bone*
 * @param verticalDirection flag que indica se se quer desenhar o Bone* na vertical (true) ou horizontal (false)
 * @return true caso seja v�lido o desenho do Bone* na posi��o e charMatrix dada
 */
bool Board::positionValidToDrawBoneOnCharMatrix(vector <vector <char> >& charMatrix, size_t boneXCenter, size_t boneYCenter, bool verticalDirection) {
	if (charMatrix.empty())
		return false;

	if (charMatrix[0].empty())
		return false;

	if (verticalDirection) {
		if ( ((boneXCenter + (BONE_VERTICAL_WIDTH  / 2)) >= charMatrix[0].size()) || ((boneXCenter - (BONE_VERTICAL_WIDTH  / 2)) < 0) ||
				((boneYCenter + (BONE_VERTICAL_HEIGHT / 2)) >= charMatrix.size())    || ((boneXCenter - (BONE_VERTICAL_HEIGHT / 2)) < 0) ) {
			return false;
		}
	} else {
		if ( ((boneXCenter + (BONE_HORIZONTAL_WIDTH  / 2)) >= charMatrix[0].size()) || ((boneXCenter - (BONE_HORIZONTAL_WIDTH  / 2)) < 0) ||
				((boneYCenter + (BONE_HORIZONTAL_HEIGHT / 2)) >= charMatrix.size())    || ((boneXCenter - (BONE_HORIZONTAL_HEIGHT / 2)) < 0) ) {
			return false;
		}
	}

	return true;
}


/**
 * M�todo auxiliar est�tico que verifica se se pode desenhar um dado Bone numa dada charMatrix sem se sobrepor a outros Bones
 * @param charMatrix charMatrix onde se vai verificar se � v�lido o desenho de um Bone*
 * @param boneXCenter posi��o central do Bone* e que est� no eixo dos x da charMatrix onde se quer desenhar o Bone*
 * @param boneYCenter posi��o central do Bone* e que est� no eixo dos y da charMatrix onde se quer desenhar o Bone*
 * @param verticalDirection flag que indica se se quer desenhar o Bone* na vertical (true) ou horizontal (false)
 * @param validBackGroundChar char que indica qual o caracter de backGround do Board e como tal v�lido para desenhar por cima
 * @return true caso seja possivel desenhar um bone na charMatrix sem ter problemas de sobreposi��o com outros Bones
 */
bool Board::checkIfPositionIsClearAndValidToDrawBone(vector <vector <char> >& charMatrix, size_t boneXCenter, size_t boneYCenter, bool verticalDirection, char validBackGroundChar) {
	if (!positionValidToDrawBoneOnCharMatrix(charMatrix, boneXCenter, boneYCenter, verticalDirection))
		return false;

	if (verticalDirection) {
		for (size_t y = boneYCenter - (BONE_VERTICAL_HEIGHT / 2); y <= boneYCenter + (BONE_VERTICAL_HEIGHT /2); ++y) {
			for (size_t x = boneXCenter - (BONE_VERTICAL_WIDTH / 2); x <= boneXCenter + (BONE_VERTICAL_WIDTH /2); ++x) {
				if (charMatrix[y][x] != validBackGroundChar)
					return false;
			}
		}
	} else {
		for (size_t y = boneYCenter - (BONE_HORIZONTAL_HEIGHT / 2); y <= boneYCenter + (BONE_HORIZONTAL_HEIGHT /2); ++y) {
			for (size_t x = boneXCenter - (BONE_HORIZONTAL_WIDTH / 2); x <= boneXCenter + (BONE_HORIZONTAL_WIDTH /2); ++x) {
				if (charMatrix[y][x] != validBackGroundChar)
					return false;
			}
		}
	}

	return true;
}


/**
 * M�todo est�tico que desenha um Bone* numa charMatrix
 * @param bone Bone* a desenhar
 * @param charMatrix charMatrix onde se vai desenhar o Bone*
 * @param boneXCenter posi��o central do Bone* e que est� no eixo dos x da charMatrix onde se quer desenhar o Bone*
 * @param boneYCenter posi��o central do Bone* e que est� no eixo dos y da charMatrix onde se quer desenhar o Bone*
 * @param verticalDirection flag que indica se se quer desenhar o Bone* na vertical (true) ou horizontal (false)
 * @param validBackGroundChar char que indica qual o caracter de backGround do Board e como tal v�lido para desenhar por cima
 * @return true caso seja possivel desenhar um bone na charMatrix sem ter problemas de sobreposi��o com outros Bones
 */
bool Board::drawBoneOnMatrix(Bone* bone, vector <vector <char> >& charMatrix, size_t boneXCenter, size_t boneYCenter,
		bool verticalDirection, char validBackGroundChar) {

	if (!checkIfPositionIsClearAndValidToDrawBone(charMatrix, boneXCenter, boneYCenter, verticalDirection, validBackGroundChar))
		return false;


	if (verticalDirection) {
		//********************************************************  <Bone na vertical>  ******************************************************
		//1� linha
		charMatrix[boneYCenter - 2][boneXCenter - 2] = '+';
		charMatrix[boneYCenter - 2][boneXCenter - 1] = '-';
		charMatrix[boneYCenter - 2][boneXCenter]     = '-';
		charMatrix[boneYCenter - 2][boneXCenter + 1] = '-';
		charMatrix[boneYCenter - 2][boneXCenter + 2] = '+';

		//2� linha
		charMatrix[boneYCenter - 1][boneXCenter - 2] = '|';
		charMatrix[boneYCenter - 1][boneXCenter - 1] = BOARD_BACKGROUND_CHAR;

		if (bone->getBoneFirstNumberPositionOnBoard() == BONE_FIRST_NUMBER_UP)
			charMatrix[boneYCenter - 1][boneXCenter] = bone->getFirstNumber();
		else
			charMatrix[boneYCenter - 1][boneXCenter] = bone->getSecondNumber();

		charMatrix[boneYCenter - 1][boneXCenter + 1] = BOARD_BACKGROUND_CHAR;
		charMatrix[boneYCenter - 1][boneXCenter + 2] = '|';

		//3� linha
		charMatrix[boneYCenter][boneXCenter - 2] = '+';
		charMatrix[boneYCenter][boneXCenter - 1] = '-';
		charMatrix[boneYCenter][boneXCenter]     = '-';
		charMatrix[boneYCenter][boneXCenter + 1] = '-';
		charMatrix[boneYCenter][boneXCenter + 2] = '+';

		//4� linha
		charMatrix[boneYCenter + 1][boneXCenter - 2] = '|';
		charMatrix[boneYCenter + 1][boneXCenter - 1] = BOARD_BACKGROUND_CHAR;

		if (bone->getBoneFirstNumberPositionOnBoard() == BONE_FIRST_NUMBER_UP)
			charMatrix[boneYCenter + 1][boneXCenter] = bone->getSecondNumber();
		else
			charMatrix[boneYCenter + 1][boneXCenter] = bone->getFirstNumber();

		charMatrix[boneYCenter + 1][boneXCenter + 1] = BOARD_BACKGROUND_CHAR;
		charMatrix[boneYCenter + 1][boneXCenter + 2] = '|';

		//5� linha
		charMatrix[boneYCenter + 2][boneXCenter - 2] = '+';
		charMatrix[boneYCenter + 2][boneXCenter - 1] = '-';
		charMatrix[boneYCenter + 2][boneXCenter]     = '-';
		charMatrix[boneYCenter + 2][boneXCenter + 1] = '-';
		charMatrix[boneYCenter + 2][boneXCenter + 2] = '+';
		//********************************************************  </Bone na vertical>  *****************************************************

	} else {
		//*******************************************************  <Bone na horizontal>  *****************************************************
		//1� linha
		charMatrix[boneYCenter - 1][boneXCenter - 4] = '+';
		charMatrix[boneYCenter - 1][boneXCenter - 3] = '-';
		charMatrix[boneYCenter - 1][boneXCenter - 2] = '-';
		charMatrix[boneYCenter - 1][boneXCenter - 1] = '-';
		charMatrix[boneYCenter - 1][boneXCenter]     = '+';
		charMatrix[boneYCenter - 1][boneXCenter + 1] = '-';
		charMatrix[boneYCenter - 1][boneXCenter + 2] = '-';
		charMatrix[boneYCenter - 1][boneXCenter + 3] = '-';
		charMatrix[boneYCenter - 1][boneXCenter + 4] = '+';


		//2� linha
		charMatrix[boneYCenter][boneXCenter - 4] = '|';
		charMatrix[boneYCenter][boneXCenter - 3] = BOARD_BACKGROUND_CHAR;

		if (bone->getBoneFirstNumberPositionOnBoard() == BONE_FIRST_NUMBER_LEFT)
			charMatrix[boneYCenter][boneXCenter - 2] = bone->getFirstNumber();
		else
			charMatrix[boneYCenter][boneXCenter - 2] = bone->getSecondNumber();

		charMatrix[boneYCenter][boneXCenter - 1] = BOARD_BACKGROUND_CHAR;
		charMatrix[boneYCenter][boneXCenter]     = '|';
		charMatrix[boneYCenter][boneXCenter + 1] = BOARD_BACKGROUND_CHAR;

		if (bone->getBoneFirstNumberPositionOnBoard() == BONE_FIRST_NUMBER_LEFT)
			charMatrix[boneYCenter][boneXCenter + 2] = bone->getSecondNumber();
		else
			charMatrix[boneYCenter][boneXCenter + 2] = bone->getFirstNumber();

		charMatrix[boneYCenter][boneXCenter + 3] = BOARD_BACKGROUND_CHAR;
		charMatrix[boneYCenter][boneXCenter + 4] = '|';

		//3� linha
		charMatrix[boneYCenter + 1][boneXCenter - 4] = '+';
		charMatrix[boneYCenter + 1][boneXCenter - 3] = '-';
		charMatrix[boneYCenter + 1][boneXCenter - 2] = '-';
		charMatrix[boneYCenter + 1][boneXCenter - 1] = '-';
		charMatrix[boneYCenter + 1][boneXCenter]     = '+';
		charMatrix[boneYCenter + 1][boneXCenter + 1] = '-';
		charMatrix[boneYCenter + 1][boneXCenter + 2] = '-';
		charMatrix[boneYCenter + 1][boneXCenter + 3] = '-';
		charMatrix[boneYCenter + 1][boneXCenter + 4] = '+';
		//*******************************************************  </Bone na horizontal>  ****************************************************
	}

	return true;
}


/**
 * M�tod que desenha um dado Bone* na Board
 * @param bone Bone* a desenhar
 * @return true caso tenha desenhado com sucesso o Bone*, false caso alguma regra tenha sido infringida ou a para desenhar posi��o tenha sido inv�lida
 */
bool Board::drawBoneOnBoard(Bone* bone) {
	return drawBoneOnMatrix(bone, this->boardMatrix, bone->getXCenterPosOnBoardMatrix(), bone->getYCenterPosOnBoardMatrix(),
			bone->getVerticalDirection(), BOARD_BACKGROUND_CHAR);
}


/// M�todo que incrementa o contador de n�mero de Bones  que est�o no Board
void Board::incrementNumberOfBonesOnBoard() {
	++numberOfBonesOnBoard;
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </Board management>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <gets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
vector<vector<char> > Board::getBoardMatrix() const {
	return boardMatrix;
}

Bone *Board::getEastWingBone() const {
	return eastWingBone;
}

DoubleBone *Board::getFirstDoubleBone() const {
	return (DoubleBone*)firstDoubleBone;
}

Bone *Board::getNorthWingBone() const {
	return northWingBone;
}

Bone *Board::getSouthWingBone() const {
	return southWingBone;
}

Bone *Board::getWestWingBone() const {
	return westWingBone;
}

unsigned Board::getNumberOfBonesOnBoard() const {
	return numberOfBonesOnBoard;
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </gets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <sets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
void Board::setBoardMatrix(vector<vector<char> > boardMatrix) {
	this->boardMatrix = boardMatrix;
}

void Board::setEastWingBone(Bone *eastWingBone) {
	this->eastWingBone = eastWingBone;
}

void Board::setFirstDoubleBone(Bone *firstDoubleBone) {
	this->firstDoubleBone = firstDoubleBone;
}

void Board::setNorthWingBone(Bone *northWingBone) {
	this->northWingBone = northWingBone;
}

void Board::setSouthWingBone(Bone *southWingBone) {
	this->southWingBone = southWingBone;
}

void Board::setWestWingBone(Bone *westWingBone) {
	this->westWingBone = westWingBone;
}

void Board::setNumberOfBonesOnBoard(unsigned numberOfBonesOnBoard) {
	this->numberOfBonesOnBoard = numberOfBonesOnBoard;
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </sets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

