/*
 * main.cpp
 *
 *  Created on: 28/03/2017
 *      Author: up201503344
 */

#include "Emergencia.h"
#include "Menu.h"

int main()
{

	Emergencia emergencia(chooseAlgorithm());
	emergencia.readFiles();
	emergencia.displayGraph();

	menu_principal(emergencia);
	return 0;
}




