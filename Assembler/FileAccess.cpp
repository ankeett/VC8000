//
//  Implementation of file access class.
//
#include "stdafx.h"
#include "FileAccess.h"

/**/
/*
FileAccess::FileAccess()
NAME
    FileAccess::FileAccess - Helps in opening a file from which the code is to be read.
SYNOPSIS
    FileAccess::FileAccess(int argc, char *argv[]);
    argc	->	The total number of command-line arguments.
    argv	->	An array consisting of names of program and file to be opened.
DESCRIPTION
    This function is a constructor for FileAccess class which is used to
    open the file to read the code. It exits with error if the number of command-line
    argument is not appropriate if the file to be read from doesnt exist.
Returns
    No return value as this is a constructor.

*/
/**/
FileAccess::FileAccess( int argc, char *argv[] )
{
    // Check that there is exactly one run time parameter.
    if( argc != 2 ) {
        cerr << "Usage: Assem <FileName>" << endl;
        exit( 1 );
    }
    // Open the file.  One might question if this is the best place to open the file.
    // One might also question whether we need a file access class.
    m_sfile.open( argv[1], ios::in );

    // If the open failed, report the error and terminate.
    if( ! m_sfile ) {
        cerr << "Source file could not be opened, assembler terminated."
            << endl;
        exit( 1 ); 
    }
}
/*FileAccess::~FileAccess( int argc, char *argv[] )*/

/**/
/*
FileAccess::~FileAccess()
NAME
    FileAccess::~FileAccess - Helps in closing the file.
SYNOPSIS
    FileAccess::~FileAccess();
DESCRIPTION
    This function is a destructor for FileAccess class which closes the file
Returns
    No return value as this is a destructor.

*/
/**/
FileAccess::~FileAccess( )
{
    // Not that necessary in that the file will be closed when the program terminates, but good form.
    m_sfile.close( );
}
/*FileAccess::~FileAccess()*/


/**/
/*
FileAccess::GetNextLine()
NAME
    FileAccess::GetNextLine - Get the next line from the file.
SYNOPSIS
    bool FileAccess::GetNextLine(string &a_line);
    a_line -> Consists of the set of characters in the next line.
DESCRIPTION
    If the next lines exists in the file, this function reads the line
    and stores it in a_line, else returns.
Returns
    Returns true if next line exists and returns false otherwise.
*/
/**/
// Get the next line from the file.
bool FileAccess::GetNextLine( string &a_line )
{
    // If there is no more data, return false.
    if( m_sfile.eof() ) {
    
        return false;
    }
    getline( m_sfile, a_line );
    
    // Return indicating success.
    return true;
}
/*bool FileAccess::GetNextLine( string &a_line )*/

/**/
/*
FileAccess::rewind()
NAME
    FileAccess::rewind - Goes back to the beginning of file.
SYNOPSIS
    void FileAccess::rewind();
DESCRIPTION
    Cleans all the file flags and goes back to the beginning
    of the file.
Returns
    void
*/
/**/
void FileAccess::rewind( )
{
    // Clean all file flags and go back to the beginning of the file.
    m_sfile.clear();
    m_sfile.seekg( 0, ios::beg );
}
/*void FileAccess::rewind()*/