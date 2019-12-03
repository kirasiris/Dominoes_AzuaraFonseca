#ifndef DOMINO_H_
#define DOMINO_H_

/**\file Domino.h
 * \brief Classe que faz a gest�o e visualiza��o numa interface CLI de um jogo de Domino.
 *
 * @version Projecto 2 - Programa��o - Domino - 10-11 - MIEIC
 * @author Carlos Miguel Correia da Costa - ei09097
 */


#include <vector>
#include <string>
#include <fstream>

#include "defs.h"
#include "Board.h"
#include "Player.h"
#include "Boneyard.h"
#include "utils.h"


using std::vector;
using std::string;
using std::ifstream;
using std::ofstream;


#ifdef USE_BOOST
	#include <boost/serialization/access.hpp>
	#include <boost/archive/text_iarchive.hpp>
	#include <boost/archive/text_oarchive.hpp>
	#include <boost/serialization/vector.hpp>
	#include <boost/serialization/string.hpp>
	#include <boost/serialization/version.hpp>
//	#include <boost/serialization/export.hpp>
#endif



//###############################################################   Domino   ####################################################################
/**
 * \brief Classe que faz a gest�o e visualiza��o numa interface CLI de um jogo de Domino.
 */
class Domino {
//=============================================================   <private>   ===================================================================
private:
	unsigned scoreToCompleteGame;
	Boneyard gameBoneYard;
	Board gameBoard;
	vector<Player> gamePlayers;
	size_t currentPlayerIndex;
	size_t winnerPlayerIndex;
	unsigned numberOfConsecutivePlayersUnableToAct;


#ifdef USE_BOOST
	friend class boost::serialization::access;

	template<class Archive> void serialize(Archive & archive, const unsigned int version) {
		archive & scoreToCompleteGame;
		archive & gameBoneYard;
		archive & gameBoard;
		archive & gamePlayers;
		archive & currentPlayerIndex;
		archive & winnerPlayerIndex;
		archive & numberOfConsecutivePlayersUnableToAct;
	}
#endif
//=============================================================   </private>   ===================================================================


//==============================================================   <public>   ====================================================================
public:
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <constructors-destructors>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	/**
	 * Defautl contructor de Domino (explicit para evitar convers�es implicitas)
	 * @param _scoreToCompleteGame score minimo que os players t�m que fazer para ganhar o jogo
	 */
	explicit Domino(unsigned _scoreToCompleteGame = 100);

	/**
	 * Destructor de Domino
	 */
	virtual ~Domino();
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </constructors-destructors>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <Domino management>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	/**
	 * M�todo que inicializa o Domino (Board, Boneyard e players)
	 */
	void initDomino();

	/**
	 *  M�todo auxiliar que inicializa os players do Domino
	 */
	void initPlayers();


	/**
	 * M�todo auxiliar que faz o reset aos Bones dos players do Board
	 */
	void resetPlayersBones();

	/**
	 * M�todo auxiliar que faz a inicializa�ao de um player do Domino
	 * @param player Player a inicializar
	 */
	void initPlayer(Player& player);

	/**
	 * M�todo que prepara a pr�xima round
	 */
	void initNextRound();


	/**
	 * M�todo que faz a compra de um Bone* ao Boneyard do Domino
	 * @return Bone* recebido ou NULL caso n�o haja mais bones para comprar
	 */
	Bone* getNewBoneFromBoneyard();

	/**
	 * M�todo que devolve o player que est� a jogar actualmente
	 * @return Player que est� a jogar
	 */
	Player& getCurrentPlayer();

	/**
	 * M�todo que trata da passagem da vez de um player
	 */
	void playerPassedTurn();


	/**
	 * M�todo que calcula qual deve ser o primeiro Player a come�ar uma round
	 * Ou seja primeiro come�am os que t�m o maior doubleBone
	 * Se nenhum player tiver doubleBones come�a o que tiver a pe�a com maior pontos
	 */
	void calcFirstPlayerToAct();

	/**
	 * M�todo que actualiza o indice do player que est� a jogar para o pr�ximo player
	 */
	void updateCurrentPlayerIndexToNextPlayer();

	/**
	 * M�todo que actualiza o indice do player que est� a jogar para o player anterior
	 */
	void updateCurrentPlayerIndexToPreviousPlayer();

	/**
	 * M�todo que verifica se o winnerPlayerIndex est� correcto
	 * Ou seja se � mesmo o indice do player que ganhou a round
	 */
	void confirmRoundWinnerPlayerIndex();


	/**
	 * M�todo que verifica se um determinado player pode jogar um Bone*
	 * @param player Player a verificar
	 * @return true caso esse player tenha pelo menos um Bone* v�lido para jogar
	 */
	bool checkIfPlayerCanAct(Player& player);

	/**
	 * M�todo que verifica se dois Bones se podem conectar
	 * @param boneOnBoard Bone que est� no Board
	 * @param boneToInsert Bone que se quer inserir
	 * @return true caso seja possivel conectar os dois bones
	 */
	bool checkIfNodesCanConect(Bone* boneOnBoard, Bone* boneToInsert);

	/**
	 * M�todo que verifica se uma round est� bloqueada, ou seja ningu�m terminou a round mas ningu�m pode jogar, por n�o ter nenhum Bone* v�lido
	 * @return true caso a round actual esteja bloqueada
	 */
	bool checkIfRoundIsBlocked();

	/**
	 * M�todo que verifica se a round actual acabou, ou seja se algum player j� n�o tem Bones para jogar
	 * @return true caso a round tenha terminado
	 */
	bool checkIfRoundIsFinish();

	/**
	 * M�todo que verifica se o jogo j� terminou, ou seja se alguem j� atingiu o n�mero minimo de pontos para ganhar um jogo
	 * @return true caso o jogo tenha terminado
	 */
	bool checkIfGameIsFinish();

	/**
	 * M�todo que determina qual foi o player que ganhou o jogo (actualiza a vari�vel winnerPlayerIndex)
	 */
	void calcWinner();

	/**
	 * M�todo que verifica qual � o player que tem menos pontos na sua hand de Bones
	 * @return indice do player no vector de players gamePlayers do jogador que tem o menor n�mero de pontos na sua hand de bones
	 */
	size_t calcPlayerWithLowerPointsOnRoundBlocked();

	/**
	 * M�todo que calcula os pontos de bonus a dar a quem ganhou a round
	 * @param roundBlocked flag que indica se a round acabou normalmente ou se acabou por ficar bloqueada
	 * @return Pontos de bonus a atribuir ao player que ganhou a round (ainda falta fazer o arredondamento para o mais pr�ximo multiplo de 5)
	 */
	int calcRemainScoreOnRoundCompletion(bool roundBlocked);

	/**
	 * M�todo auxiliar que calculo o n�mero de pontos que um player tem na sua Bone hand
	 * @param player player que onde se vai calcular o numero de pontos
	 * @return numero de pontos que o player tem na sua bone hand
	 */
	unsigned int calcSumPlayerBonesPoints(Player& player);


	/**
	 * M�todo que calula o n�mero de pontos que um player obteria se inseri-se o bone no insertion point dado (�til para a AI)
	 * @param newBoneConection Classe auxiliar que compacta o Bone* e o ponto de inser��o no Board
	 * @param firstBone flag que indica se � o primeiro Bone* a inserir no Board
	 * @return pontos que se obteria se o Bone fosse inserido naquela posi��o
	 */
	unsigned int calcScoreOnBoneInsertion(NewBoneConection newBoneConection, bool firstBone);

	/**
	 * M�todo auxiliar que calcula o n�mero de pontos que um Bone que est� no Board tem (para a contagem de pontos da jogada)
	 * @param boneOnBoard Bone* que est� no Board
	 * @return pontos que esse Bone* pode contribuir para uma jogada
	 */
	unsigned int calcBoneScoreOnBoard(Bone* boneOnBoard, BoardInsertPoint boardInsertionPoint);


	/**
	 * M�todo que verifica se a posi��o do bone a inserir na segunda pe�a jogada est� correcta (para garantir a correcta inicialia��o dos pointers)
	 * @param boneOnBoardToCheck bone que est� no board
	 * @param boneToInsert bone que se quer inserir
	 * @param boardInsertionPoint Ponto de conec��o onde se quer inserir o bone
	 * @return true caso a inser��o seja valida
	 */
	bool checkIfSecondBoneInsertionIsValid(Bone *boneOnBoardToCheck, Bone *boneToInsert, BoardInsertPoint boardInsertionPoint, BoneConectionPoint boneConectionPoint);

	/**
	 * M�todo usado na fun��o em baixo pra evitar repeti��o de c�digo
	 */
	void updateBestBoneConectionOnBoardWingAI(
			Bone* boneOnBoardToCheck,
			Bone* boneToInsert,
			BoardInsertPoint boardInsertionPoint,
			bool firstDoubleHasPrimaryConectionPointAvailable,
			bool firstBone,
			vector <NewBoneConection>& validBones,
			NewBoneConection& bestBoneConection,
			NewBoneConection& currentBoneConection,
			unsigned& bestScoreFound,
			unsigned& currentBoneScore);

	/**
	 * M�todo que calcula qual o melhor Bone* a inserir no Board dado um vector de Bones (�til para a AI)
	 * @param playerBonesAI vector de Bones de um player
	 * @param firstBone flag que indica que o Bone a inserir ser� o primeiro no Board
	 * @return Retorna a melhor conec��o que se pode ter tendo em conta a heuristica sugerida no enunciado
	 */
	NewBoneConection calcBestBoneToPlaceOnBoardAI(vector <Bone*>& playerBonesAI, bool firstBone);

	//responsabilidade de organiza��o das pe�as passada para o utilizador
//	BoneConectionPoint calcBestBoneConectionPointOnBoardAI(NewBoneConection newBoneConection);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </Domino management>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <CLI>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	/**
	 * M�todo que faz a gest�o de um jogo de Domino usando a interface CLI
	 * @param roundLoaded flag que indica se o jogo que se vai fazer teve a round actual carregada de um ficheiro
	 */
	void initDominoGameCLI(bool gameLoaded);

	/**
	 * M�todo que trata da adi��o dos players ao Domino usando a interface CLI
	 */
	void addPlayersCLI();

	/// M�todo que mostra o tabuleiro e os bones do player actual
	void showBoardAndPlayerStatus();

	/**
	 * M�todo que trata da jogada do player actual
	 * @param firstBone flag que indica se o player vai inserir o primeiro bone na Board
	 */
	void currentPlayerTurnCLI(bool firstBone);

	/**
	 * M�todo auxiliar que seleciona o indice do Bone* que o player quer usar usando a interface CLI
	 * @return retorna o indice que o player selecionou (supoem-se que foram mostrados os bones e respectivos indices antes de pedir o indice)
	 */
	size_t selectBoneToUseFromPlayer();

	/**
	 * M�todo auxiliar que seleciona em que sitio do Board o player quer inserir o Bone que selecionou anteriormente
	 * @return Retorna o sitio do Board onde o player quer inserir o Bone* que tinha selecionado
	 */
	BoardInsertPoint selectBoardInsertionPointToUseFromPlayer();

	/**
	 * M�todo auxiliar que seleciona em que direc��o o player quer inserir o Bone* que tinha selecionado antes
	 * @return Retorna a dirc��o que o player selecionou da tabela mostrada
	 */
	BoneConectionPoint selectBoneConectionPointToUseFromPlayer();


	/**
	 * M�todo auxiliar que mostra os scores quanda a round termina ou � bloqueada e actualiza a pontua��o do vencedor em conformidade
	 * @param roundBlocked flag que indica se a round acabou normalmente (false), ou se acabou bloqueada (true)
	 */
	void calcAndShowScoresOnRoundEnd(bool roundBlocked);

	/**
	 * M�todo que mostra os scores quanda a round � bloqueada e actualiza a pontua��o do vencedor em conformidade
	 */
	void showScoresOnRoundBlock();

	/**
	 * M�todo que mostra os scores quanda a round termina e actualiza a pontua��o do vencedor em conformidade
	 */
	void showScoresOnRoundEnd();

	/**
	 * M�todo que mostra os scores quanda a jogo termina
	 */
	void showScoresOnGameEnd();

	/**
	 * M�todo que trata da interface CLI para o carregamento de um jogo de Domino
	 * @return true caso tenha sido feito o carregamento de um jogo anterir com sucesso
	 */
	bool loadDominoCLI();

	/**
	 * M�todo que trata da interface CLI para o backup de um jogo de Domino
	 * @return true caso tenha sido feito o backup do jogo actual com sucesso
	 */
	bool saveDominoCLI();
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </CLI>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <Load - save>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	/**
	 * M�todo que trata do carregamento de um jogo a partir de um ficheiro
	 * Caso o ficheiro n�o exista lan�a uma excep��o (FileDoesntExistException)
	 * @param filename Nome do ficheiro onde est� o jogo a carregar
	 */
	void loadDomino(string filename) throw(FileDoesntExistException);

	/**
	 * M�todo que trata do backup do jogo actual para um ficheiro
	 * Caso o ficheiro para onde se ia guardar os dados j� exista lan�a uma excep��o (FileAlreadyExistException)
	 * @param filename nome do ficheiro onde se vai tentar guardar o jogo actual
	 * @param overWrite flag que indica que se o ficheiro existir, fa�a overwrite e n�o lance excep��o
	 */
	void saveDomino(string filename, bool overWrite) throw(FileAlreadyExistException);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <Load - save>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <gets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	size_t getCurrentPlayerIndex() const;
    Board getGameBoard() const;
    Boneyard getGameBoneYard() const;
    vector<Player>& getGamePlayers();
    unsigned getNumberOfConsecutivePlayersUnableToAct() const;
	unsigned getScoreToCompleteGame() const;
	size_t getWinnerPlayerIndex() const;
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </gets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <sets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    void setCurrentPlayerIndex(size_t currentPlayerIndex);
    void setGameBoard(Board gameBoard);
    void setGameBoneYard(Boneyard gameBoneYard);
    void setGamePlayers(const vector<Player>& gamePlayers);
	void setNumberOfConsecutivePlayersUnableToAct(unsigned numberOfConsecutivePlayersUnableToAct);
	void setScoreToCompleteGame(unsigned scoreToCompleteGame);
	void setWinnerPlayerIndex(size_t winnerPlayerIndex);
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </sets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//==============================================================   </public>   ====================================================================
};


#endif /* DOMINO_H_ */
