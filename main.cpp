/**\file main.cpp
 * \brief Ficheiro com a interface com o utilizador.
 *
 * @version Projecto 2 - Programa��o - Domino - 10-11 - MIEIC
 * @author Carlos Miguel Correia da Costa - ei09097
 */


#include <iostream>
#include "utils.h"
#include "Domino.h"
#include "defs.h"

using std::cout;

#ifdef USE_BOOST
	#include <boost/serialization/export.hpp>

	BOOST_CLASS_EXPORT_GUID(Bone, "Bone");
	BOOST_CLASS_EXPORT_GUID(DoubleBone, "DoubleBone");
	BOOST_CLASS_EXPORT_GUID(Player, "Player");
	BOOST_CLASS_EXPORT_GUID(NewBoneConection, "NewBoneConection");
	BOOST_CLASS_EXPORT_GUID(Board, "Board");
	BOOST_CLASS_EXPORT_GUID(Boneyard, "Boneyard");
	BOOST_CLASS_EXPORT_GUID(Domino, "Domino");
#endif


int main() {
	utils::clearConsoleScreen();

	int option = 1;
	bool roundLoaded = false;

	do {
		utils::clearConsoleScreen();
		cout << "#########################   PROGRAMMING - PROJECT 2   ##########################\n";
		cout << "  >>>                          DOMINO - ALL FIVES                           <<<  \n";
		cout << "#################################################################################\n\n\n";

		cout << " 1 - New game\n";
		cout << " 2 - Load game\n\n";
		cout << " 0 - Exit\n\n\n" << endl;


		option = utils::getIntCin("  >>> Option: ", "Enter one of the options mentioned above!\n", 0, 3);



		switch (option) {
			case 1: {
				Domino *domino = new Domino(50);
				domino->initDominoGameCLI(false);
				utils::clearConsoleScreen();

				delete domino;
				break;
			}

			case 2: {
				Domino *loadedDomino = new Domino(50);
				roundLoaded = loadedDomino->loadDominoCLI();
				utils::clearConsoleScreen();

				if (roundLoaded)
					loadedDomino->initDominoGameCLI(roundLoaded);

				utils::clearConsoleScreen();

				delete loadedDomino;
				break;
			}

			default:
				break;
			}

	} while (option != 0);



	return 0;
}
