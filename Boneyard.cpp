/**\file Boneyard.cpp
 * \brief Ficheiro com a implementa�o da classe Boneyard.
 *
 * @version Projecto 2 - Programa�o - Domino - 10-11 - MIEIC
 * @author Carlos Miguel Correia da Costa - ei09097
 */


#include "Boneyard.h"


//#ifdef USE_BOOST
//	BOOST_CLASS_EXPORT_GUID(Boneyard, "Boneyard")
//#endif


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <constructors-destructors>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
/**
 * Default constructor de Boneyard
 */
Boneyard::Boneyard() : bonePile(vector <Bone*>()), bonesInUse(vector <Bone*>()) {}


/**
 * Destructor de Boneyard que limpa de mem�ria os Bones que foram alocados dinamicamente
 */
Boneyard::~Boneyard() {
	this->destroyBoneyard();
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </constructors-destructors>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <BoneYard management>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// random generator function:
ptrdiff_t RandomFunction (ptrdiff_t i) { return rand()%i;}

// pointer object to it:
ptrdiff_t (*PointerToRandomFunction)(ptrdiff_t) = RandomFunction;

/**
 * M�todo que inicializa o Boneyard, criandos os Bones do Domino
 */
void Boneyard::initBoneyard() {
	//cria�o dos bones do jogo
	for (char firstNumber = '0'; firstNumber < MAX_BONE_NUMBER; ++firstNumber) {
		for (char secondNumber = firstNumber; secondNumber < MAX_BONE_NUMBER; ++secondNumber) {
			if (firstNumber == secondNumber) {
				bonePile.push_back(new DoubleBone(firstNumber, secondNumber));
			} else {
				bonePile.push_back(new Bone(firstNumber, secondNumber));
			}
		}
	}


	srand((unsigned)time(NULL));
	random_shuffle(bonePile.begin(), bonePile.end(), PointerToRandomFunction);
}


/**
 * M�todo que reinicializa o Boneyard (apaga os Bones antigos e cria novos, colocando o Boneyard no estado inicial)
 */
void Boneyard::resetBoneyard() {
	destroyBoneyard();
	initBoneyard();
}


/**
 * M�todo que dealoca de mem�ria os Bones do Boneyard (e os que est�o a ser usados pelos players ou j� est�o no Board)
 */
void Boneyard::destroyBoneyard() {
	//dealoca�o da mem�ria dos bones da bonepile
	for (size_t i = 0; i < bonePile.size(); ++i) {
		delete(bonePile[i]);
	}

	//dealoca�o da mem�ria dos restantes bones que j� estavam em jogo
	for (size_t i = 0; i < bonesInUse.size(); ++i) {
		delete(bonesInUse[i]);
	}

	bonePile.clear();
	bonesInUse.clear();
}


/**
 * M�todo que verifica se ainda � Bones para "comprar"
 * @return true caso ainda se possa comprar Bones
 */
bool Boneyard::empty() {
	return bonePile.empty();
}


/**
 * M�todo que trata da compra de um Bone ao Boneyard
 * @return Bone* que estava nos bones disponiveis para compra e que ainda n�o estava em jogo
 */
Bone* Boneyard::getLastBone() {
	if (bonePile.empty())
		return NULL;

	//obten�o do �ltimo bone da bonepile e retirada do mesmo
	Bone* lastBone = bonePile.back();
	//actualiza�o da posi�o que o bone ter� no bonesInUse (usado nos loads/saves)
	lastBone->setPositionOnBonesInUse(bonesInUse.size());

	bonePile.pop_back();

	//coloca�o do bone retirado no vector bonesInUse (vector usado para evitar memory leaks)
	bonesInUse.push_back(lastBone);

	return lastBone;
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </BoneYard management>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <gets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
vector<Bone*>& Boneyard::getBonePile() {
	return bonePile;
}

vector<Bone*>& Boneyard::getBonesInUse() {
	return bonesInUse;
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </gets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <sets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
void Boneyard::setBonePile(const vector<Bone*> &bonePile) {
	this->bonePile = bonePile;
}

void Boneyard::setBonesInUse(vector<Bone*>& bonesInUse) {
	this->bonesInUse = bonesInUse;
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </sets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<