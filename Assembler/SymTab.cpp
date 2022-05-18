//
//		Implementation of the symbol table class.  This is the format I want for commenting functions.
//
#include "stdafx.h"
#include "SymTab.h"

/*
NAME

    AddSymbol - adds a new symbol to the symbol table.

SYNOPSIS

    void AddSymbol( const string &a_symbol, int a_loc );
    	a_symbol	-> The name of the symbol to be added to the symbol table.
    	a_loc		-> the location to be associated with the symbol.

DESCRIPTION

    This function will place the symbol "a_symbol" and its location "a_loc"
    in the symbol table.
*/
void SymbolTable::AddSymbol( const string &a_symbol, int a_loc )
{
    // If the symbol is already in the symbol table, record it as multiply defined.
    map<string, int>::iterator st = m_symbolTable.find( a_symbol );
    if( st != m_symbolTable.end() ) {

        st->second = multiplyDefinedSymbol;
        return;
    }
    // Record a the  location in the symbol table.
    m_symbolTable[a_symbol] = a_loc;
}
/*void SymbolTable::AddSymbol( const string &a_symbol, int a_loc )*/

/*
NAME

    DisplaySymbolTable() - Displays the symbol table

SYNOPSIS

    void DisplayErrors()

DESCRIPTION

    This function displays the symbol index, symbol itself, and the location of the symbol

RETURNS

    VOID

*/
void SymbolTable::DisplaySymbolTable() {

    int index = 0;
    cout << "Output from the test Program:" << endl;
    cout << "Symbol Table: " << endl;
    
    map<string, int>::iterator it;

    cout << "Symbol#\t\t Symbol\t\t Location" << endl;
    for (it = m_symbolTable.begin(); it != m_symbolTable.end(); it++) {
        cout << index++ << "\t\t" << it->first << "\t\t" << it->second << endl;
    }

    cout << "_______________________________________________" << endl;

    cout << "Press Enter to continue..." << endl;
    cin.ignore();
}
/*void SymbolTable::DisplaySymbolTable() */

/**/
/*
 NAME

    LookupSymbol- Looks for symbol in the symbol table

 SYNOPSIS

    bool LookupSymbol(const string &a_symbol, int &a_loc)
        a_symbol       -->       The Symbol to look for
        a_loc          -->       If the symbol is found places the symbol's location in a_loc

DESCRIPTION

    Looks up if a symbol exists in the symbol table.

RETURN

    Returns true if symbol exists, false otherwise

*/
/**/

bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc) {

    if (m_symbolTable.find(a_symbol) != m_symbolTable.end()) {
        a_loc = m_symbolTable[a_symbol];
        return true;
    }
    return false;
}
/*bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc) {*/