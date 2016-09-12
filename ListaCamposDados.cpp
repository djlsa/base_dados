#include "StdAfx.h"
#include "ListaCamposDados.h"

ListaCamposDados::ListaCamposDados(void)
{
}

ListaCamposDados::~ListaCamposDados(void)
{
}

map<Id, CampoDados*>& ListaCamposDados::getCampos()
{
	return campos;
}

void ListaCamposDados::adicionar(CampoDados* campo)
{
	this->campos[campo->getId()] = campo;
}

CampoDados* ListaCamposDados::get(Id id)
{
	if(this->campos.find(id) != this->campos.end() )
	{
		return campos[id];
	}
	campos[id] = new CampoDados(id);
	return campos[id];
}

CampoDados* ListaCamposDados::get(string nome)
{
	CampoDados::getListaCampos().adicionarNome(nome);
	Id id = Util::hashCode(nome);
	return this->get(id);
}

void ListaCamposDados::remover(string nome)
{
	CampoDados* campo = get(nome);
	campos.erase(Util::hashCode(nome));
	delete campo;
}

void ListaCamposDados::adicionarNome(string nome)
{
	Id id = Util::hashCode(nome);

	nomes[id] = nome;
	if(campos[id] == 0)
		campos[id] = new CampoDados(id);
}

string ListaCamposDados::getNome(CampoDados& campo)
{
	Id id = campo.getId();
	if(this->nomes.find(id) != this->nomes.end() )
	{
		return this->nomes[id];
	}
	return "";
}

void ListaCamposDados::escreverNomesFicheiro(ofstream& out)
{
	int size = nomes.size();
	out.write((char*)&size, sizeof(size));
	out.write("\n", 1);
	map<Id, string>::iterator iter = nomes.begin();
	while(iter != nomes.end() )
	{
		out.write((char*)iter->second.c_str(), sizeof(char) * iter->second.length() + 1);
		out.write("\n", 1);
		iter++;
	}
}

void ListaCamposDados::lerNomesFicheiro(ifstream& in)
{
	int size = 0;
	in.read((char*)&size, sizeof(size));
	in.ignore();
	while(size-- > 0)
	{
		string nome;
		getline(in, nome, '\0');
		in.ignore();
		adicionarNome(nome);
	}
}

void ListaCamposDados::escreverCamposFicheiro(ofstream& out)
{
	int size = campos.size();
	out.write((char*)&size, sizeof(size));
	out.write("\n", 1);
	map<Id, CampoDados*>::iterator iter = campos.begin();
	while(iter != campos.end() )
	{
		campos[iter->first]->escreverFicheiro(out);
		iter++;
	}
}

void ListaCamposDados::lerCamposFicheiro(ifstream& in)
{
	int size = 0;
	in.read((char*)&size, sizeof(size));
	in.ignore();
	while(size-- > 0)
	{
		Id id;
		in.read((char*)&id, sizeof(id));
		in.ignore();
		string texto;
		getline(in, texto, '\0');
		in.ignore();
		campos[id] = new CampoDados(id);
		campos[id]->setTexto(texto);
	}
}

ostream& operator<<(ostream& out, ListaCamposDados& listaCampoDados)
{
	map<Id, CampoDados*>::iterator iter = listaCampoDados.campos.begin();
	while(iter != listaCampoDados.campos.end() )
	{
		out << "\t" << listaCampoDados.campos[iter->first] << endl;
		iter++;
	}
	return out;
}
