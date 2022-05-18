#include "stdafx.h"

#include "Errors.h"

vector<string> Errors::m_ErrorMsgs;

/**/
/*
Errors::InitErrorReporting()
NAME
	Errors::InitErrorReporting - Initializes the error vector

SYNOPSIS
	void Errors::InitErrorReporting();

DESCRIPTION

	It erases the vector components to initialize the vector.
RETURNS
	VOID

AUTHOR
	Ankit Kafle

DATE
	09:32 PM 5/6/2022

*/
/**/

void Errors::InitErrorReporting() {
	m_ErrorMsgs.erase(m_ErrorMsgs.begin(), m_ErrorMsgs.end());
}
/*void Errors::InitErrorReporting() {*/

/**/
/*
void Errors::RecordError(string a_emsg)

NAME
	void Errors::RecordError(string a_emsg) - records error

SYNOPSIS
	void Errors::DisplayErrors(string a_emsg)
	a_emsg -->Error message that needed to be stored

DESCRIPTION
	This function records the error message in a vector

RETURNS
	VOID
*/
/**/

void Errors::RecordError(string a_emsg) {
	m_ErrorMsgs.push_back(a_emsg);
}
/*void Errors::RecordError(string a_emsg)*/

/**/
/*
void Errors::DisplayErrors()

NAME
	void Errors::DisplayErrors() - displays last error

SYNOPSIS
	void Errors::DisplayErrors()

DESCRIPTION
	This function prints out the latest errors after translating the code

RETURNS
	VOID
*/
/**/

void Errors::DisplayErrors() {
	cout << m_ErrorMsgs.back() << endl;
}
/*void Errors::DisplayErrors() {*/