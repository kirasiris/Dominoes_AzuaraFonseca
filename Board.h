#ifndef BOARD_H_
#define BOARD_H_

/**\file Board.h
 * \brief Classe que faz a gest�o do tabuleiro de jogo do Domino.
 *
 * @version Projecto 2 - Programa��o - Domino - 10-11 - MIEIC
 * @author Carlos Miguel Correia da Costa - ei09097
 */


#include <iostream>
#include <vector>
#include <string>

#include "defs.h"
#include "Bone.h"
#include "DoubleBone.h"


using std::vector;
using std::string;
using std::cout;
using std::ostream;


#ifdef USE_BOOST
	#include <boost/serialization/access.hpp>
	#include <boost/serialization/vector.hpp>
	#include <boost/serialization/string.hpp>
	#include <boost/serialization/version.hpp>
//	#include <boost/serialization/export.hpp>
#endif

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <Enums>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
enum BoardInsertPoint { NORTH_WING, SOUTH_WING, EAST_WING, WEST_WING };
enum BoneConectionPoint { BONE_CONECTION_POINT_UP, BONE_CONECTION_POINT_DOWN, BONE_CONECTION_POINT_LEFT, BONE_CONECTION_POINT_RIGHT };


/**
 * Operador que devolve o BoneConectionPoint logicamente oposto
 * @param boneConectionPoint BoneConectionPoint que se pretende "inverter"
 * @return BoneConectionPoint que � o inverso do que foi dado
 */
BoneConectionPoint operator!(const BoneConectionPoint& boneConectionPoint);

/**
 * M�todo que devolve a string associada � enumera��o BoardInsertPoint
 * @param boneInsertionPoint BoardInsertPoint que se pretende obter a descri��o
 * @return descri��o da BoardInsertPoint dada
 */
string getBoardInsertionPointDescription(BoardInsertPoint boneInsertionPoint);

/**
 * Operador que permite mostrar de forma mais legivel a enumera��o BoardInsertPoint
 * @param out stream de output
 * @param boneInsertionPoint BoardInsertPoint que se pretende mostrar
 * @return o stream de output, de forma a permitir encadeamentos de streams
 */
ostream& operator<<(ostream& out, const BoardInsertPoint& boneInsertionPoint);
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </Enums>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <auxiliary structs>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
class NewBoneConection {
public:
	Bone* boneToInsert;
	BoardInsertPoint wingOnBoardToInsertBone;

	NewBoneConection() : boneToInsert(NULL), wingOnBoardToInsertBone(WEST_WING) {}
	virtual ~NewBoneConection() {}

private:
#ifdef USE_BOOST
	friend class boost::serialization::access;

	template<class Archive> void serialize(Archive & archive, const unsigned int version) {
		archive & boneToInsert;
		archive & wingOnBoardToInsertBone;
	}
#endif
};
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </auxiliary structs>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



//###############################################################   Board   ######################################################################
/**
 * \brief Classe que faz a gest�o do tabuleiro de jogo do Domino.
 */
class Board {
//=============================================================   <private>   ====================================================================
private:
	Bone* eastWingBone;
	Bone* westWingBone;
	Bone* northWingBone;
	Bone* southWingBone;
	Bone* firstDoubleBone;

	unsigned numberOfBonesOnBoard;
	vector< vector<char> > boardMatrix;


#ifdef USE_BOOST
	friend class boost::serialization::access;

	template<class Archive> void serialize(Archive & archive, const unsigned int version) {
		archive & eastWingBone;
		archive & westWingBone;
		archive & northWingBone;
		archive & southWingBone;
		archive & firstDoubleBone;
		archive & boardMatrix;
		archive & numberOfBonesOnBoard;
	}
#endif
//=============================================================   </private>   ===================================================================


//==============================================================   <public>   ====================================================================
public:
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <constructors-destructors>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	/**
	 * Default contructor de Board
	 */
	Board();

	/**
	 * Destructor de Board
	 */
	~Board();
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </constructors-destructors>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <Board management>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	/**
	 * M�todo qye inicializa a charMatrix do Board
	 */
	void initBoard();

	/**
	 * M�todo que reinicializa o Board
	 */
	void resetBoard();


	/**
	 * M�todo que retorna o Bone* correspondente ao BoardInsertPoint no Board
	 * @param insertionPoint enumera��o com os 4 pontos possiveis de inserir bones
	 * @return o apontador para o Bone pretendido ou NULL se esse local de inser��o ainda n�o tiver Bones associados
	 */
	Bone* getCorrespondentBoneToBoardInsertPoint(BoardInsertPoint insertionPoint);


	/**
	 * M�todo que retorna a enumer��o BoneFirstNumberPositionOnBoard correspospondente � enumer��o BoneConectionPoint
	 * @param boneConectionPoint Enumer��o que se quer converter
	 * @return Enumer��o correspondente
	 */
	BoneFirstNumberPositionOnBoard getCorrespondentBoneFirstNumberPositionOnBoard(BoneConectionPoint boneConectionPoint);


	/**
	 * M�todo que mostra na consola a charMatrix do Board
	 */
	void showBoardCLI();


	/**
	 * M�todo que trata da inser��o de um Bone, num dado ponto de inser��o, numa determinada direc��o no Board
	 * @param newBoneConection NewConection com o Bone* a inserir e em que ponto de inser��o do Board
	 * @param boneConectionPoint Direc��o em que inserir o Bone*
	 * @param firstBone Flag que indica se o Bone a inserir � o primeiro bone do Board
	 * @return true caso a inser��o tenha sido feita com sucesso, ou false, caso n�o possa desenhar no sitio pretendido
	 * 		   tenha desrespeitado alguma regra do jogo ou fosse colocar o Bone numa posi��o que saisse fora da charMatrix do Board
	 */
	bool insertBone(NewBoneConection newBoneConection, BoneConectionPoint boneConectionPoint, bool firstBone);

	/**
	 * M�todo que faz o update dos pointers que o Board tem para a gest�o das inser��es. S� deve ser executado depois da inser��o.
	 * @param boneOnBoardToCheck Bone* onde se inseriu o novo Bone*
	 * @param newBoneConection Bone* que foi inserido e sitio de inser��o no Board
	 * @param boneConectionPoint Direc��o em que o Bone* foi inserido
	 * @param firstBone Flag que indica se o Bone* inserido foi o primeiro da Board
	 */
	void updateBoardWingPointers(Bone* boneOnBoardToCheck, NewBoneConection newBoneConection, BoneConectionPoint boneConectionPoint, bool firstBone);

	/**
	 * M�todo que verifica se � possivel "desenhar" on dado Bone* numa dada direc��o, noutro que j� esteja no Board
	 * @param boneOnBoardToCheck Bone* onde se quer inserir o novo Bone*
	 * @param boneToInsert Bone* que se quer inserir
	 * @param boneConectionPoint Direc��o em que se quer inserir o novo Bone*
	 * @return true caso seja possivel desenhar o Bone* na Board
	 */
	bool positionValidToDrawBoneOnBoard(NewBoneConection newBoneConection, BoneConectionPoint boneConectionPoint);

	/**
	 * M�todo auxiliar que verifica se um dado DoubleBone* � v�lido para inserir no Board
	 * @param boneOnBoardToCheck Bone* onde o doubleBone seria inserido
	 * @param boneToInsert doubleBone a inserir
	 * @param boneConectionPoint Dire��o do doubleBone a inserir
	 * @param useFirstNumberOnBoneOnBoardToCheck flag que indica para verificar o primeiro n�mero (true) ou o segundo n�mero (false) do bone onde se vai inserir o doubleBone
	 * @param useFirstNumberOnBoneToInsert flag que indica para verificar o primeiro n�mero (true) ou o segundo n�mero (false) do doubleBone
	 * @return true caso seja possivel inserir o doubleBone que se pretendia
	 */
	bool checkIfDoubleBonePositionIsValidToDrawOnBoard(Bone* boneOnBoardToCheck, Bone* boneToInsert, BoneConectionPoint boneConectionPoint, bool useFirstNumberOnBoneOnBoardToCheck, bool useFirstNumberOnBoneToInsert);

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
	bool checkIfBonePositionIsValidToDrawOnBoard(Bone* boneOnBoardToCheck, Bone* boneToInsert, BoneConectionPoint boneConectionPoint, bool useFirstNumberOnBoneOnBoardToCheck, bool useFirstNumberOnBoneToInsert, bool boneOnBoardToCheckIsDouble);

	/**
	 * M�todo auxiliar que faz o update de um Bone* que ir� ser inserido noutro Bone* do Board
	 * @param boneOnBoardToCheck bone do Board onde o Bone* ser� inserido
	 * @param boneToInsert Bone* que se quer inserir
	 * @param boneConectionPoint Direc��o em que se quer inserir o Bone*
	 * @param useFirstNumberOnBoneOnBoardToCheck flag que indica para verificar o primeiro n�mero (true) ou o segundo n�mero (false) do bone onde se vai inserir o Bone*
	 * @param useFirstNumberOnBoneToInsert flag que indica para verificar o primeiro n�mero (true) ou o segundo n�mero (false) do Bone* a inserir
	 * @return true caso o update tenha sido feito com sucesso e o Bone* pode ser inserido, false se as condi��es dadas forem contra as regras ou inv�lidas
	 */
	bool updateBoneToInsertOnBone(Bone* boneOnBoardToCheck, Bone* boneToInsert, BoneConectionPoint boneConectionPoint, bool useFirstNumberOnBoneOnBoardToCheck, bool useFirstNumberOnBoneToInsert);

	/**
	 * M�todo auxiliar que faz o update de um Bone* que ir� ser inserido num doubleBone do Board
	 * @param boneOnBoardToCheck doubleBOne do Board onde o Bone* ser� inserido
	 * @param boneToInsert Bone* que se quer inserir
	 * @param boneConectionPoint Direc��o em que se quer inserir o Bone*
	 * @param useFirstNumberOnBoneToInsert flag que indica para verificar o primeiro n�mero (true) ou o segundo n�mero (false) do bone onde se vai inserir o Bone*
	 * @return true caso o update tenha sido feito com sucesso e o Bone* pode ser inserido, false se as condi��es dadas forem contra as regras ou inv�lidas
	 */
	bool updateBoneToInsertOnDoubleBone(Bone* boneOnBoardToCheck, Bone* boneToInsert, BoneConectionPoint boneConectionPoint, bool useFirstNumberOnBoneToInsert);

	/**
	 * M�todo est�tico que verifica se possivel desenhar um dado Bone numa dada posi��o de uma charMatrix
	 * (ao estar est�tico permite ser usado noutras classes para evitar c�digo repetido.
	 * @param charMatrix charMatrix onde se vai verificar se � v�lido o desenho de um Bone*
	 * @param boneXCenter posi��o central do Bone* e que est� no eixo dos x da charMatrix onde se quer desennhar o Bone*
	 * @param boneYCenter  posi��o central do Bone* e que est� no eixo dos y da charMatrix onde se quer desennhar o Bone*
	 * @param verticalDirection flag que indica se se quer desenhar o Bone* na vertical (true) ou horizontal (false)
	 * @return true caso seja v�lido o desenho do Bone* na posi��o e charMatrix dada
	 */
	static bool positionValidToDrawBoneOnCharMatrix(vector <vector <char> >& charMatrix, size_t boneXCenter, size_t boneYCenter, bool verticalDirection);

	/**
	 * M�todo auxiliar est�tico que verifica se se pode desenhar um dado Bone numa dada charMatrix sem se sobrepor a outros Bones
	 * @param charMatrix charMatrix onde se vai verificar se � v�lido o desenho de um Bone*
	 * @param boneXCenter posi��o central do Bone* e que est� no eixo dos x da charMatrix onde se quer desenhar o Bone*
	 * @param boneYCenter posi��o central do Bone* e que est� no eixo dos y da charMatrix onde se quer desenhar o Bone*
	 * @param verticalDirection flag que indica se se quer desenhar o Bone* na vertical (true) ou horizontal (false)
	 * @param validBackGroundChar char que indica qual o caracter de backGround do Board e como tal v�lido para desenhar por cima
	 * @return true caso seja possivel desenhar um bone na charMatrix sem ter problemas de sobreposi��o com outros Bones
	 */
	static bool checkIfPositionIsClearAndValidToDrawBone(vector <vector <char> >& charMatrix, size_t boneXCenter, size_t boneYCenter, bool verticalDirection, char validBackGround);

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
	static bool drawBoneOnMatrix(Bone* bone, vector <vector <char> >& charMatrix, size_t boneXCenter, size_t boneYCenter, bool verticalDirection, char validBackGroundChar = BOARD_BACKGROUND_CHAR);

	/**
	 * M�tod que desenha um dado Bone* na Board
	 * @param bone Bone* a desenhar
	 * @return true caso tenha desenhado com sucesso o Bone*, false caso alguma regra tenha sido infringida ou a para desenhar posi��o tenha sido inv�lida
	 */
	bool drawBoneOnBoard(Bone* bone);

	/// M�todo que incrementa o contador de n�mero de Bones  que est�o no Board
	void incrementNumberOfBonesOnBoard();
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </Board management>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <gets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    vector<vector<char> > getBoardMatrix() const;
    Bone *getEastWingBone() const;
    DoubleBone *getFirstDoubleBone() const;
    Bone *getNorthWingBone() const;
    Bone *getSouthWingBone() const;
    Bone *getWestWingBone() const;
    unsigned getNumberOfBonesOnBoard() const;
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </gets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <sets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    void setBoardMatrix(vector<vector<char> > boardMatrix);
    void setEastWingBone(Bone *eastWingBone);
    void setFirstDoubleBone(Bone *firstDoubleBone);
    void setNorthWingBone(Bone *northWingBone);
    void setSouthWingBone(Bone *southWingBone);
    void setWestWingBone(Bone *westWingBone);
    void setNumberOfBonesOnBoard(unsigned numberOfBonesOnBoard);
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </sets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//===============================================================   </public>   =================================================================
};

#endif /* BOARD_H_ */
