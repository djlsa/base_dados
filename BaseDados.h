#pragma once

// esta classe representa os m�todos est�ticos usados para controlar as opera��es sobre a base de dados
class BaseDados
{
private:
	// indice de pesquisa, o id � o hashcode da palavra, frase ou fragmento a pesquisar
	static map<Id, ListaItems> INDICE;
	static string NOME_FICHEIRO;
public:
	BaseDados(void);
	~BaseDados(void);

	// adiciona item ao indice
	static void adicionarIndice(string str, ItemPesquisavel* item);

	static void verDetalhe(Id id);
	static ItemPesquisavel* adicionarItem(string tipo, string nome);
	static void removerItem(Id id);
	static bool adicionarDados(Id id, string nome, string texto);
	static bool removerDados(Id id, string nome);
	static bool adicionarRelacao(Id id1, Id id2, int ordem = numeric_limits<int>::max());
	static bool removerRelacao(Id id1, Id id2);
	static ListaItems pesquisar(string str);

	static void escreverFicheiro();
	static void lerFicheiro(string nomeFicheiro);
};
