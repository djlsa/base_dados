#pragma once

typedef unsigned long Id;

class Util
{
	
public:
	// coloca uma string toda em minusculas
	static void toLower(string& s)
	{
		transform(s.begin(), s.end(), s.begin(), ::tolower);
	}

	// gera o hashcode correspondente de uma string (identico ao algoritmo usado pelo java)
	// http://stackoverflow.com/questions/822363/proof-why-does-java-lang-string-hashcodes-implementation-match-its-documenta
	static Id hashCode(string s)
	{
		toLower(s);
		Id hash = 0;
		int length = s.length();
		for (int i = 0; i < length; i++)
		{
			hash = 31*hash + s[i];
		}
		return hash;
	}

	// gera um id unico, baseado no tempo atual, no nº de "ticks" decorridos desde o inicio do programa e um numero aleatorio
	static Id gerarId()
	{
		stringstream s;
		s << time(0) << ":" << clock() << ":" << rand();
		return hashCode(s.str());
	}
};