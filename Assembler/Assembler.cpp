//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"
#include "SymTab.h"

/**/
/*
Assembler::Assembler()
NAME
    Assembler::Assembler - It is the constructor of the assembler class.
SYNOPSIS
    Assembler::Assembler(int argc, char *argv[]);
        argc	->	The total number of command-line arguments.
        argv	->	An array consisting of strings of program and file name.
DESCRIPTION
    It is the constructor for the assembler class.
    argc and argv are passed to the file access constructor.
RETURNS
    No return value, as this is a constructor.
*/
/**/
Assembler::Assembler( int argc, char *argv[] )
: m_facc( argc, argv )
{
    // Nothing else to do here at this point.
}

// Destructor
Assembler::~Assembler( )
{
}


/**/
/*
Assembler::PassI()
NAME
    Assembler::PassI - Helps in filling the symbol table.
SYNOPSIS
    Assembler::PassI();
DESCRIPTION
    It establishes the location of the labels.
RETURNS
    void
*/
/**/
void Assembler::PassI() 
{
    int loc = 0;        // Tracks the location of the instructions to be generated.

    // Successively process each line of source code.
    for( ; ; ) {

        // Read the next line from the source file.
        string line; 
        if( ! m_facc.GetNextLine( line ) ) {

            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }
        // Parse the line and get the instruction type.
        Instruction::InstructionType st =  m_inst.ParseInstruction( line );

        // If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement and report an error if it isn't.
        if( st == Instruction::InstructionType::ST_End ) return;

        // Labels can only be on machine language and assembler language
        // instructions.  So, skip comments.
        if( st == Instruction::InstructionType::ST_Comment )  
        {
        	continue;
	    }
        // Handle the case where there is an error.
        
        // If the instruction has a label, record it and its location in the
        // symbol table.
        if( m_inst.isLabel( ) ) {
            m_symtab.AddSymbol( m_inst.GetLabel( ), loc );
        }
        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction( loc );
    }

}
/*void Assembler::PassI() */


/**/
/*
Assembler::PassII()
NAME
    Assembler::PassII - Translates the code and records errors.
SYNOPSIS
    Assembler::PassII();
DESCRIPTION
    Translates the code in the each line and stores them in the emulator. Also records and displayes errors.
    Calls different function to translate different operations like machine or assembly
RETURNS
    void
*/
/**/
void Assembler::PassII() {
    
    m_facc.rewind(); //clean all the flags and go back to the beginning of the file
    int loc = 0;
    
    //Initialize error 
    Errors::InitErrorReporting();
    string content; 

    //title
    cout << "Translation of Program:" << endl;
    cout << "Location\tContents\t\tOriginal Statement" << endl;

    while (true) {
        //read the line from the source
        string line;
        if (!m_facc.GetNextLine(line)) {
            //if there are no more lines, we are probably missing the end statement
            Errors::RecordError("Error! No End Statement!");
            Errors::DisplayErrors();
            return;

        }
       
        //parsing the line and getting the instruction type
        Instruction::InstructionType st = m_inst.ParseInstruction(line);

        //check if end is the last statement
        if (st == Instruction::InstructionType::ST_End) {

            cout << "\t\t\t\t" << m_inst.GetInstruction() << endl;

            while (m_facc.GetNextLine(line)) {
                if (!line.empty()) {
                    Errors::RecordError("Error! Last Statement is not the End");
                    Errors::DisplayErrors();
                }
                else {
                    continue;
                }
                
            }
        
            break;
            
        }


        //if it is a comment, there is nothing to do
        if (st == Instruction::InstructionType::ST_Comment) {
            cout <<"\t\t\t\t\t\t"<< m_inst.GetInstruction() << endl;
        }
        
        //if error, store and display error
        if (st == Instruction::InstructionType::ST_Error) {
            Errors::RecordError("Error! Invalid Operation "+ m_inst.GetInstruction());
            Errors::DisplayErrors();
            //return;
        }

        //check the label length 
        if (m_inst.GetLabel().length() > 15) {
            Errors::RecordError("Error! Very large label in  " + m_inst.GetInstruction());
            Errors::DisplayErrors();
            //return;
        }

        //check if the label is valid or not
        if (isdigit(m_inst.GetLabel()[0])) {
            Errors::RecordError("Error! Label cannot be integer or start with integer in  " + m_inst.GetInstruction());
            Errors::DisplayErrors();
            //return;
        }

        //check the memory status
        if (m_inst.LocationNextInstruction(loc) > 999999) {
            Errors::RecordError("Error! Memory Overload!");
            Errors::DisplayErrors();
            return;
        }

        
        //if it is a assembly instruction, calling the AssemblyInstruction function
        if (st == Instruction::InstructionType::ST_AssemblerInstr) {
            
            AssemblyInstruction(content, loc);
            
        }
        //if it is a machine instruction, calling the MachineInstruction function
        else if (st == Instruction::InstructionType::ST_MachineLanguage) {
            MachineInstruction(content, loc);

        }

    }
}
/*void Assembler::PassII()*/

/**/
/*
Assembler::AssemblyInstruction()
NAME
    Assembler::AssemblyInstruction - Process the assembly language instruction and record errors.
SYNOPSIS
    void Assembler::AssemblyInstruction(string& a_content, int& a_loc)
        a_content ->  content of the program.
        a_loc     -> the location of the code.
DESCRIPTION
    It translates the code, inserts into emulator and finds and errors. It basically handles the assembly code instruction.
RETURNS
    void.
*/
/**/
void Assembler::AssemblyInstruction(string& a_content, int& a_loc)
{
    //checking the operands to see if there is any missing or in wrong format
    if (!m_inst.GetOperand2().empty()) {
        Errors::RecordError("Error! Operand 2 found in Assembly Instruction!");
        Errors::DisplayErrors();
    }
    if (m_inst.GetOperand1().empty()) {
        Errors::RecordError("Error! Missing Operand 1 in " + m_inst.GetOpCode());
        Errors::DisplayErrors();

    }
    else if (!m_inst.IsNumericOperand1()) {
        Errors::RecordError("Error! Operand must be Numeric in " + m_inst.GetOpCode());
        Errors::DisplayErrors();
         }

    if (m_inst.IsNumericOperand1()) {
        if (stoi(m_inst.GetOperand1()) > 10000) {
            Errors::RecordError("Error! Very large value of Operand 1 in " + m_inst.GetOpCode());
            Errors::DisplayErrors();
        }
    }

    //handling ORG Operation
    if (m_inst.GetOpCode() == "ORG") {
        if (!m_inst.GetLabel().empty()) {
            Errors::RecordError("Error! Label found in ORG!");
            Errors::DisplayErrors();
        }
        

        cout  << a_loc << "\t\t\t\t" << m_inst.GetInstruction() << endl;
    }
    //if not ORG, must be DC or DS
    else {

        if (m_inst.GetLabel().empty()) {
            Errors::RecordError("Error! Label not found in "+m_inst.GetOpCode());
            Errors::DisplayErrors();
            //return;
        }
        else {
            //looking if the symbol is already defined
            int temp;
            m_symtab.LookupSymbol(m_inst.GetLabel(), temp);
            if (temp == m_symtab.multiplyDefinedSymbol) {
                Errors::RecordError("Error! Symbol Defined in Multiple Locations");
                Errors::DisplayErrors();
            }
        }

        //DS doesnot need the content, so finding out the content only for those which is needed
        if (m_inst.GetOpCode() == "DS") {
            cout << a_loc << "\t\t\t\t" << m_inst.GetInstruction() << endl;
        }
        else {
            a_content = m_inst.GetOperand1();

            while (a_content.size() != 9) {
                a_content = "0" + a_content;
            }

            //inserting into the memory
            m_emul.insertMemory(a_loc, stoll(a_content));

            cout << a_loc << "\t   " << a_content << "\t\t" << m_inst.GetInstruction() << endl;
        }
    }
    //finding location of next instruction
    a_loc = m_inst.LocationNextInstruction(a_loc);

    
}
/*void Assembler::AssemblyInstruction(string& a_content, int& a_loc)*/


/**/
/*
Assembler::MachineInstruction()
NAME
    Assembler::MachineInstruction - Process the machine language instruction and record errors.
SYNOPSIS
    void Assembler::MachineInstruction(string& a_content, int& a_loc)
        a_content ->  content of the program.
        a_loc     -> the location of the code.
DESCRIPTION
    It translates the code, inserts into emulator and finds and errors. It basically handles the machine code instruction.
RETURNS
    void.
*/
/**/
void Assembler::MachineInstruction(string& a_content, int& a_loc)
{
    //finding Opcide and making it two digit
    string OpCode = to_string(m_inst.GetNumOpCode());
    int location = 0;
    string locate;

    if (OpCode.size() != 2) {
        OpCode = "0" + OpCode;
    }

    //handling operation HALT
    if (m_inst.GetOpCode() == "HALT") {
        if (!m_inst.GetOperand1().empty()) {
            Errors::RecordError("Error! Operand found in " + m_inst.GetOpCode());
            Errors::DisplayErrors();
        }
        if (!m_inst.GetLabel().empty()) {
            Errors::RecordError("Error! Label found in " + m_inst.GetOpCode());
            Errors::DisplayErrors();
        }
    }

    //check if the label that we are using is already used
    if (!m_inst.GetLabel().empty()) {
        int temp;
        m_symtab.LookupSymbol(m_inst.GetLabel(), temp);
        if (temp == m_symtab.multiplyDefinedSymbol) {
            Errors::RecordError("Error! Symbol Defined in Multiple Locations");
            Errors::DisplayErrors();
        }
    }

    //checking the presence and data type of the operand1 and calculating for those operation which ignores the register
    if (!m_inst.IsNumericOperand1()) {

        if (m_inst.GetNumOpCode() != 11 && m_inst.GetNumOpCode() != 12 && m_inst.GetNumOpCode() != 17) {
            Errors::RecordError("Error! No Register found in " + m_inst.GetInstruction());
            Errors::DisplayErrors();
        }

        if (!m_inst.GetOperand2().empty()) {
            Errors::RecordError("Error! Extra Operand found in " + m_inst.GetOpCode());
            Errors::DisplayErrors();
        }

        a_content = OpCode + "9";
        if (!m_inst.GetOperand1().empty()) {
            m_symtab.LookupSymbol(m_inst.GetOperand1(), location);

            if (location == 0) {
                Errors::RecordError("Error! Cannot find the location of the symbol " + m_inst.GetOperand1());
                Errors::DisplayErrors();
            }
        }
        locate = to_string(location);

        while (locate.size() != 6) {
            locate = "0" + locate;
        }
        a_content = a_content + locate;

    }
    else {

        //checking the register size
        if (stoi(m_inst.GetOperand1()) < 0 || stoi(m_inst.GetOperand1()) > 9) {
            Errors::RecordError("Error::Invalid Register value");
            Errors::DisplayErrors();
        }

        if (m_inst.GetOperand2().empty()) {
            Errors::RecordError("Error! Operand 2 missing in " + m_inst.GetNumOpCode());
            Errors::DisplayErrors();
        }

        //handling errors for operations that need two registers
        if (m_inst.GetNumOpCode() >= 7 && m_inst.GetNumOpCode() <= 10) {

            if (!m_inst.IsNumericOperand2()) {
                Errors::RecordError("Error! Operand 2 must be numeric in " + m_inst.GetOpCode());
                Errors::DisplayErrors();
            }
            else {
                if (stoi(m_inst.GetOperand2()) < 0 || stoi(m_inst.GetOperand2()) > 9) {
                    Errors::RecordError("Error::Invalid Register value");
                    Errors::DisplayErrors();
                }
            }

            a_content = OpCode + m_inst.GetOperand1() + m_inst.GetOperand2();

            while (a_content.size() != 9) {
                a_content = a_content + "0";
            }

        }
        else {
            a_content = OpCode + m_inst.GetOperand1();
            m_symtab.LookupSymbol(m_inst.GetOperand2(), location);
            
            //checking if the memory that is being used is defined or not.
            if (location == 0) {
                Errors::RecordError("Error! Cannot find the location of the symbol " + m_inst.GetOperand2());
                Errors::DisplayErrors();
            }

            //finding the content
            locate = to_string(location);

            while (locate.size() != 6) {
                locate = "0" + locate;
            }
            a_content = a_content + locate;
        }
    }
    /*void Assembler::MachineInstruction(string& a_content, int& a_loc)*/

    //inserting into memory and calculating location of next instruction
    m_emul.insertMemory(a_loc, stoll(a_content));
    cout << a_loc << "        " << a_content << "           " << m_inst.GetInstruction() << endl;
    a_loc = m_inst.LocationNextInstruction(a_loc);

}

/**/
/*
Assembler::RunProgramInEmulator()
NAME
    Assembler::RunProgramInEmulator - Runs the emulator.
SYNOPSIS
    void Assembler::RunProgramInEmulator();
DESCRIPTION
    Runs the emulator if there are no errors.
RETURNS
    void.
*/
/**/
void Assembler::RunProgramInEmulator() {
    cout << "----------------------------------------------" << endl;
    cout << "Press Enter to continue..." << endl;
    cin.ignore();
    cout << "Results from Emulating Program:" << endl;

    //run only when there are no errors
    if (Errors::NoError()) {
        m_emul.runProgram();
    }
    else {
        cout << "Emulator cannot run because of Errors!" << endl;
    }
    cout << "End of Emulation" << endl;
}
/*void Assembler::RunProgramInEmulator()*/