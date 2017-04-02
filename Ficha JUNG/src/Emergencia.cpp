#include "Emergencia.h"
#include "Rua.h"

Emergencia::Emergencia() {
	ID_ARESTA_GERAL = 1;
}

Emergencia::~Emergencia() {

}

Vertex<No>* Emergencia::findNo(int id) {

	vector<Vertex<No>*> vertices = myGraph.getVertexSet();
	for (unsigned int i = 0; i < vertices.size(); i++) {
		if (vertices.at(i)->getInfo().getID() == id)
			return vertices.at(i);

	}

	return NULL;

}

void Emergencia::readFiles() {

	ifstream inFile;
	std::string line;

	long long int idNo = 0;
	float X = 0;
	float Y = 0;
	char token;

	//Ler o ficheiro nosNormals.txt
	inFile.open("nos.txt");

	if (!inFile) {
		cerr << "Unable to open file datafile.txt";
		exit(1);   // call system to stop
	}

	while (getline(inFile, line)) {

		std::stringstream linestream(line);

		linestream >> idNo >> token >> X >> token >> Y;
		No n(idNo, X, Y);
		myGraph.addVertex(n);

	}

	inFile.close();

	//Ler o ficheiro hospitalNos.txt
	inFile.open("hospitais.txt");

	if (!inFile) {
		cerr << "Unable to open file datafile.txt";
		exit(1);   // call system to stop
	}

	while (getline(inFile, line)) {

		std::stringstream linestream(line);

		linestream >> idNo >> token >> X >> token >> Y;

		No n(idNo, X, Y);

		myGraph.addVertex(n);
		hospitais.push_back(myGraph.getVertex(n));

	}

	inFile.close();

	//Ler o ficheiro inemNos.txt
	inFile.open("INEM.txt");

	if (!inFile) {
		cerr << "Unable to open file datafile.txt";
		exit(1);   // call system to stop
	}

	while (getline(inFile, line)) {

		std::stringstream linestream(line);

		linestream >> idNo >> token >> X >> token >> Y;

		No n(idNo, X, Y);

		myGraph.addVertex(n);
		Veiculo v(myGraph.getVertex(n));
		INEM.push_back(v);

	}

	inFile.close();

	//Ler o ficheiro bombeirosNos.txt
	inFile.open("bombeiros.txt");

	if (!inFile) {
		cerr << "Unable to open file datafile.txt";
		exit(1);   // call system to stop
	}

	while (getline(inFile, line)) {

		std::stringstream linestream(line);

		linestream >> idNo >> token >> X >> token >> Y;

		No n(idNo, X, Y);

		myGraph.addVertex(n);
		Veiculo v(myGraph.getVertex(n));
		bombeiros.push_back(v);

	}

	inFile.close();

	//Ler o ficheiro policiaNos.txt
	inFile.open("policia.txt");

	if (!inFile) {
		cerr << "Unable to open file datafile.txt";
		exit(1);   // call system to stop
	}

	while (getline(inFile, line)) {

		std::stringstream linestream(line);

		linestream >> idNo >> token >> X >> token >> Y;

		No n(idNo, X, Y);

		myGraph.addVertex(n);
		Veiculo v(myGraph.getVertex(n));
		policia.push_back(v);

	}

	inFile.close();

	int idRua = 0;
	int idAresta = 0;
	int idNo1 = 0, idNo2 = 0;

	//Ler o ficheiro arestas.txt
	inFile.open("arestas.txt");

	if (!inFile) {
		cerr << "Unable to open file datafile.txt";
		exit(1);   // call system to stop
	}

	while (getline(inFile, line)) {

		std::stringstream linestream(line);

		linestream >> idAresta >> token >> idNo1 >> token >> idNo2;

		Vertex<No>* v1 = findNo(idNo1);
		Vertex<No>* v2 = findNo(idNo2);
		No no1 = v1->getInfo();
		No no2 = v2->getInfo();

		double weight = sqrt(
				pow((no2.getX() - no1.getX()), 2)
						+ pow((no2.getY() - no1.getY()), 2));

		/*cout << "Id Aresta: " << idAresta << "   no1: " << no1.getID()
		 << "   no2: " << no2.getID() << endl;*/
		myGraph.addEdge(idAresta, no1, no2, weight);

		ID_ARESTA_GERAL++;

	}

	inFile.close();

	string nomeRua, twoWays;
	int NoID;

	//Ler o ficheiro ruas.txt
	inFile.open("ruas.txt");

	if (!inFile) {
		cerr << "Unable to open file datafile.txt";
		exit(1);   // call system to stop
	}

	while (getline(inFile, line)) {

		std::stringstream linestream(line);

		linestream >> idRua >> token;
		getline(linestream, nomeRua, ';');
		getline(linestream, twoWays, ';');

		Rua r(idRua, nomeRua, ((twoWays == "True") ? true : false));
		do {
			linestream >> NoID >> token;
			r.setNoID(NoID);
		} while (token == ',');
		ruas.push_back(r);

		if (twoWays == "True") {

			for (int i = r.getNosID().size() - 1; i > 0; i--) {

				Vertex<No>* v1 = findNo(r.getNosID().at(i));
				Vertex<No>* v2 = findNo(r.getNosID().at(i - 1));
				No no1 = v1->getInfo();
				No no2 = v2->getInfo();

				double weight = sqrt(
						pow((no2.getX() - no1.getX()), 2)
								+ pow((no2.getY() - no1.getY()), 2));

				myGraph.addEdge(ID_ARESTA_GERAL, no1, no2, weight);

				ID_ARESTA_GERAL++;
			}
		}

	}

	inFile.close();

}

void Emergencia::getCall(GraphViewer *gv,int noID,int polFlag,int bombFlag,int inemFlag) {

	No localizacao;
	for(int i=0; i<myGraph.getVertexSet().size(); i++)
	{
		if(myGraph.getVertexSet()[i]->getInfo().getID() == noID)
			localizacao = myGraph.getVertexSet()[i]->getInfo();
	}

	myGraph.dijkstraShortestPath(localizacao);
	Vertex<No>* policiaAssistencia = findPolicia();
	Vertex<No>* bombAssistencia = findBomb();
	Vertex<No>* INEMAssistencia = findINEM();

	if(polFlag != 0)
	{
		vector<No> pathPolicia = myGraph.getPath(policiaAssistencia->getInfo(), localizacao);
			for(unsigned int i=0; i< pathPolicia.size(); i++)
			{
				cout<<"ID Policia NO: "<<pathPolicia[i].getID() << endl;
			}
	}

	if(bombFlag != 0)
	{
		vector<No> pathINEM = myGraph.getPath(INEMAssistencia->getInfo(),localizacao);
			for(unsigned int i = 0; i < pathINEM.size();i++)
			{
				cout << "ID INEM NO: " << pathINEM[i].getID()<< endl;
			}
	}

	if(inemFlag != 0)
	{
		vector<No> pathBomb = myGraph.getPath(bombAssistencia->getInfo(),localizacao);
			for(unsigned int i = 0; i < pathBomb.size();i++)
			{
				cout << "ID Bombeiros NO: " << pathBomb[i].getID() << endl;;
			}
	}


}

void Emergencia::displayGraph(GraphViewer *gv) {

	//TODO delete
	cout << "display" << endl;

	gv->createWindow(600, 600);

	gv->defineEdgeColor("blue");
	gv->defineVertexColor("yellow");
	gv->defineEdgeCurved(false);

	vector<Vertex<No>*> vertexSet = myGraph.getVertexSet();

	typename vector<Vertex<No>*>::const_iterator it = vertexSet.begin();
	typename vector<Vertex<No>*>::const_iterator ite = vertexSet.end();

	for (; it != ite; it++) {

		No addno = (*it)->getInfo();

		gv->addNode(addno.getID(), (addno.getX() * 2) + 20,
				-(addno.getY() * 2) + 420);
	}

	it = vertexSet.begin();
	ite = vertexSet.end();
	typename vector<Edge<No> >::iterator itEdges;
	typename vector<Edge<No> >::iterator iteEdges;

	for (; it != ite; it++) {

		vector<Edge<No> > edgesvec = (*it)->getAdj();
		itEdges = edgesvec.begin();
		iteEdges = edgesvec.end();
		for (; itEdges != iteEdges; itEdges++) {
			gv->addEdge((itEdges)->getID(), (*it)->getInfo().getID(),
					(itEdges)->getDest()->getInfo().getID(),
					EdgeType::DIRECTED);
		}

	}
	colorNodes(gv);
	gv->rearrange();

}

void Emergencia::colorNodes(GraphViewer *gv) const {
	vector<Veiculo>::const_iterator it = this->INEM.begin();
	for (it; it != INEM.end(); it++) {
		gv->setVertexColor((*it).getlocalNode().getID(), "green");
	}
	it = this->policia.begin();
	for (it; it != policia.end(); it++) {
		gv->setVertexColor((*it).getlocalNode().getID(), "blue");
	}
	it = this->bombeiros.begin();
	for (it; it != bombeiros.end(); it++) {
		gv->setVertexColor((*it).getlocalNode().getID(), "red");
		//gv->setVertexIcon((*it).getLocal().getID(),"icon.gif");
	}

}

Vertex<No>* Emergencia::findINEM() {

	int posicaofinal = 0;
	int distAtual;
	int distMinima = INT_MAX;

	for (int i = 0; i < INEM.size(); i++) {
			if (INEM[i].getDisponibilidade()) {
				distAtual = INEM[i].getLocal()->getDist();
				if (distAtual < distMinima) {
					distMinima = distAtual;
					posicaofinal = i;
				}
			}
		}
		return INEM[posicaofinal].getLocal();

}

void Emergencia::drawBombPath(GraphViewer *gv, Vertex<No>* path)
{

}
Vertex<No>* Emergencia::findBomb(){
	int posicaofinal = 0;
	int distAtual;
	int distMinima = INT_MAX;

		for (int i = 0; i < bombeiros.size(); i++) {
			if (bombeiros[i].getDisponibilidade()) {

				distAtual = bombeiros[i].getLocal()->getDist();
				if (distAtual < distMinima) {
					distMinima = distAtual;
					posicaofinal = i;
				}
			}
		}
	return bombeiros[posicaofinal].getLocal();
}

Vertex<No>* Emergencia::findPolicia() {
	int posicaofinal = 0;
		int distAtual;
		int distMinima = INT_MAX;
		for (int i = 0; i < policia.size(); i++) {
			if (policia[i].getDisponibilidade()) {
				distAtual = policia[i].getLocal()->getDist();
				if (distAtual < distMinima) {
					distMinima = distAtual;
					posicaofinal = i;
				}
			}
		}
		return policia[posicaofinal].getLocal();

}
