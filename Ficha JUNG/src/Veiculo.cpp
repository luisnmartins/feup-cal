/*
 * Veiculo.cpp
 *
 *  Created on: 28/03/2017
 *      Author: up201503344
 */

#include "Veiculo.h"

Veiculo::Veiculo(No local) : disponivel(true){
	this->local=local;

}

Veiculo::~Veiculo() {
	// TODO Auto-generated destructor stub
}

No Veiculo::getLocal() const
{
	return local;
}

