#include "StdAfx.h"
#include "CampoDados.h"

ListaCamposDados CampoDados::LISTA_CAMPOS;

ListaCamposDados& CampoDados::getListaCampos()
{
	return LISTA_CAMPOS;
}

CampoDados::CampoDados(Id id) : id(id)
{
}

CampoDados::~CampoDados(void)
{
}

Id CampoDados::getId()
{
	return this->id;
}

void CampoDados::setTexto(string texto)
{
	this->texto = texto;
}

string CampoDados::getTexto()
{
	return texto;
}

// escreve um campo de dados para ficheiro, o id e o texto, o nome do campo em si é
// escrito pela ListaCamposDados
void CampoDados::escreverFicheiro(ofstream& out)
{
	out.write((char*)&id, sizeof(id));
	out.write("\n", 1);
	out.write((char*)texto.c_str(), sizeof(char) * texto.size() + 1);
	out.write("\n", 1);
}

// toString()
ostream& operator<<(ostream& out, CampoDados* campoDados) {
	string nome = CampoDados::getListaCampos().getNome(*campoDados);
	out << nome;
	if(nome.length() > 0 && campoDados->texto.length() > 0)
		out << " : ";
	out << campoDados->texto;
    return out;
}
