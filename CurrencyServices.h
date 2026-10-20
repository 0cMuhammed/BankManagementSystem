#pragma once
#include <iostream>
#include<vector>
#include<fstream>
#include "FileHandler.h"
#include "CurrencyRepository.h"

class CurrencyServices {

private:

	CurrencyRepository& m_RepositoryReference;

public:

	CurrencyServices(CurrencyRepository& Repo) : m_RepositoryReference(Repo) {};

	const CurrencyRepository& AccessRepository()  const noexcept {
		return m_RepositoryReference; // read only 
	}
	CurrencyRepository& AccessRepository()  noexcept {
		return m_RepositoryReference; // mutable
	}

};

