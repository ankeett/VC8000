#include "stdafx.h"

#include "Instruction.h"
#include "Errors.h"

/**/
/*
Instruction::InstructionType Instruction::ParseInstruction()
NAME
    Instruction::InstructionType Instruction::ParseInstruction - Returning the type of instruction.
SYNOPSIS
    Instruction::InstructionType Instruction::ParseInstruction(string a_line);
        a_line -> a string to be parsed.
DESCRIPTION
    The function extracts different components of instruction
Returns
    Returns the instruction type

*/
/**/
Instruction::InstructionType Instruction::ParseInstruction(string a_line)
{
    // Record the original statement.  This will be needed in the sceond pass.
    m_instruction = a_line;

    // Delete any comment from the line.
    DeleteComment( a_line );

    // Record label, opcode, and operands.  Up to you to deal with formatting errors.
    bool isFormatError = RecordFields( a_line );
    
    // Check if this is a comment.
    if (m_Label.empty() && m_OpCode.empty())
    {
        return InstructionType::ST_Comment;
    }
    // Return the instruction type.  This has to be handled in the code.
    return m_type;
}
/*Instruction::InstructionType Instruction::ParseInstruction(string a_line)*/

/**/
/*
bool Instruction::RecordFields( )
NAME

    bool Instruction::RecordFields( const string &a_line ) - Record the fields that make up the instructions.

SYNOPSIS

    bool Instruction::RecordFields( const string &a_line )
    string a_line    -->   The line whose fields are to be recorded. Passes as const reference to avoid change

DESCRIPTION

    This function records the type of the instruction that we are getting. 

RETURNS
    bool true if sucessfully records the fields, false if there is format error

*/
/**/


bool Instruction::RecordFields( const string &a_line )
{
    // Get the fields that make up the instruction.
    bool isFormatError = !ParseLineIntoFields( a_line, m_Label, m_OpCode, m_Operand1, m_Operand2);

    //throw error if extra operand is found
    if (isFormatError) {
        Errors::RecordError("Error! Extra Operand Found");
        Errors::DisplayErrors();
    }

    // if code was a comment, there is nothing to do.
    if (m_OpCode.empty() && m_Label.empty()) return isFormatError;

    // Record whether the operands are numeric and their value if they are.
    m_IsNumericOperand1 = isStrNumber(m_Operand1);
    if (m_IsNumericOperand1) m_Operand1NumericValue = stoi(m_Operand1);

    m_IsNumericOperand2 = isStrNumber(m_Operand2);
    if (m_IsNumericOperand2) m_Operand2NumericValue = stoi(m_Operand2);

    // For the sake of comparing, convert the op code to upper case.
    for (char& c : m_OpCode)
    {
        c = toupper(c);
    }
    // - Determining and recording the instruction type from the op code.
    // - Recording the numberic Op code for machine lanuage equivalents.

    vector<string> MEquivalent = { "ADD","SUB","MULT","DIV","LOAD","STORE","ADDR","SUBR","MULTR","DIVR","READ","WRITE","B","BM","BZ","BP","HALT" };
    vector<string> AEquivalent = { "DC","DS","ORG","END" };
    for (int i = 0; i < (int)MEquivalent.size(); i++) {
        if (m_OpCode == MEquivalent.at(i)) {
            m_type = InstructionType::ST_MachineLanguage;
            m_NumOpCode = i + 1;
            return true;
        }
    }

    for (int i = 0; i < (int) AEquivalent.size(); i++) {
        
        if (i < (int)AEquivalent.size() - 1) {
            if (m_OpCode == AEquivalent.at(i)) {
                m_type = InstructionType::ST_AssemblerInstr;
                return true;
            }
        }
        if (m_OpCode == AEquivalent.at((int)AEquivalent.size() - 1)) {
            m_type = InstructionType::ST_End;
            return true;
        }
    }

    //determining the comment
    if (m_Label.empty() && m_OpCode.empty())
    {
        m_type = InstructionType::ST_Comment;
        return true;
    }

    //If it is not anything else, must be an error
    m_type = InstructionType::ST_Error;
    return true;
}
/*bool Instruction::RecordFields( const string &a_line )*/

/**/
/*
Instruction::ParseLineIntoFields()
NAME

    bool Instruction::ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
    string& a_Operand1, string& a_Operand2) - Parses the instructions into fields.

SYNOPSIS

    bool Instruction::ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
    string& a_Operand1, string& a_Operand2)
    string a_line    -->   The line to parse.
    a_label          -->   Records the label in the instruction
    a_OpCode         -->   Records the opCode in the instruction
    a_Operand1       -->   Records the Operand1 in the instruction
    a_Operand2       -->   Records the Operand2 in the instruction

DESCRIPTION

    This function parses the instruction into fields. (label, opcode, operand1, operand2)

RETURNS
    bool true if sucessfully parses the instruction into fields, false if there were additional characters 

*/
/**/

bool Instruction::ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
    string& a_Operand1, string& a_Operand2)
{
    // Get rid of any commas from the line.
    replace(a_line.begin(), a_line.end(), ',', ' ');

    // Get the elements of the line.  That is the label, op code, operand1, and operand2.
    string endStr;
    a_label = a_OpCode = a_Operand1 = a_Operand2 = "";
    istringstream ins(a_line);
    if (a_line[0] == ' ' || a_line[0] == '\t')
    {
        a_label = "";
        ins >> a_OpCode >> a_Operand1 >> a_Operand2 >> endStr;
    }
    else
    {
        ins >> a_label >> a_OpCode >> a_Operand1 >> a_Operand2 >> endStr;
    }
    // If there is extra data, return false.
    return endStr.empty() ? true : false;
}
/*bool Instruction::ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
    string& a_Operand1, string& a_Operand2)*/


/**/
/*
Instruction::isStrNumber(const string& a_str)
NAME

    bool Instruction::isStrNumber(const string& a_str)

SYNOPSIS

    bool Instruction::isStrNumber(const string& a_str)
    string a_str    -->   String that is checked if it contains number

DESCRIPTION

    This function checks if a string has numbers or not.

RETURNS
    bool true if the string is all numbers, otherwise returns false

*/
/**/

bool Instruction::isStrNumber(const string& a_str)
{
    if (a_str.empty()) return false;

    // If there is a - or a plus, make sure there are more characters.
    int ichar = 0;
    if (a_str[0] == '-' || a_str[0] == '+')
    {
        ichar++;
        if (a_str.length() < 2) return false;
    }
    // Make sure that the remaining characters are all digits
    for (; ichar < a_str.length(); ichar++)
    {
        if (!isdigit(a_str[ichar])) return false;
    }
    return true;
}
/*bool Instruction::isStrNumber(const string& a_str)*/
