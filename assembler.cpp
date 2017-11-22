#include <iostream>
#include ,fstream>
#include <cstdlib>
using namespace std;

string reg_to_bin(string reg);

int main()
{
    ifstream fin("sum.carlos");
    if(fin.fail())
    {
            cout << "File open failed\n";
            exit(0);
    }
    
    stirng opcode;
    
    fin >> opcode;

    if(opcode == "PUT")
    {
        out << "0011";
        fin >> num;
        out << dec_to_binary(num);

    }


}

string reg_to_bin(string reg)
{
    if(reg == "R1")
        return "000";
    else if(reg == "R2")
        return "001";
    else if(reg == "R3")
        return "010";
    else if(reg == "R4")
        return "011";
    else if(reg == "R5")
        return "100";
    else if(reg == "R6")
        return "101";
    else if(reg == "R7")
        return "110";
    else if(reg == "R8")
        return "111";
    else
        return "Invalid register\n";

}