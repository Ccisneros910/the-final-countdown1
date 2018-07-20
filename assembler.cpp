#include <iostream>
//#include <ifstream>     // allows us to read files
//#include <ofstream>     // allows us to write files
// using the below because it can do both
#include <fstream>      // allows us to both read and write files
#include <cstdlib>
#include <cmath>        // used in str_to_int fnc
#include <string>       // we NEED THIS
#include <vector>  
#include <map>
#include <utility>
// vectors in the simulator
using namespace std;

// FUNCTIONS TO CONVERT
void inst_convert(string instr, ofstream &out); // check instruction of a line

void arithmetic(string instr, ofstream &out); // IF mathematical instruction
void access(string ins, ofstream &out);  // IF accessing data

void array_make(string instr, ofstream &out);    // IF making arrays
void array_math(string instr, ofstream &out);

void jump_label(string isntr, ofstream &out);   // IF a jump label found
void jump_instr(string instr, ofstream &out);   // IF jump comparison found
void jump_cond(string instr, ofstream &out);
 
int str_to_int(string instr, int i, int s, int p);
string int_to_bin(int num);

string bit_size(char num, int size); // can use this to increase bit size of registers
  // makes a binary line of size 5, for last register
// ^ but what about the power bits

void error_detected(string isntruction, string description, string problem);

// *****     GLOBAL VARIABLES     *****
static int arr_cntr = 0;    // Use this to label arrays
map<string, int> arr_list;  // To keep track of array labels and size
map<string, pair<int, bool>> arr_info;

static int jmp_cntr = 1;
map<string, int> jump_labels;

bool have_errors = false;
      //CAN WE HAVE MULTIMAP???
//multimap<std::string, std::string> test;
//test["ADD"] = "00000";
map<string, string> all_instr;

int main()                              //*****     START OF PROGRAM     *****     
{
    arr_info["test"].first = 10;
    arr_info["test"].second = true;
    // first we have to read our instruction file
    string capIns = "";  //stores our instruction
    //std::vector<string> daRegister ;
  
    ifstream infile("instructions.txt"); // read in from the instructions file
    ofstream out("binary.o");       // write out to a new txt file
    
    all_instr["ADD"] = "00000";
    all_instr["SUB"] = "00001";
    all_instr["MUL"] = "00010";
    all_instr["DIV"] = "00011";
    all_instr["INP"] = "00100";
    all_instr["OUT"] = "00101";
    all_instr["STR"] = "00111";
    all_instr["JMP"] = "01000";
    all_instr["JME"] = "01001";
    all_instr["JMG"] = "01010";
    all_instr["JGE"] = "01011";
    all_instr["JML"] = "01100";
    all_instr["JLE"] = "01101";
    all_instr["JNE"] = "10011";
    all_instr["CLR"] = "011110000000000000000000000\n";
    all_instr["ARR"] = "10000";
    all_instr["HLT"] = "111110000000000000000000000\n";
    
    if(infile.fail())        // check if file open failed
    {
        cout << "File could not open\n";
        exit(0);
    }
    while(!infile.eof()) // IF file opened then we store our instrction list;
    {
        //cin.ignore();           // read in line of txt file
        getline(infile, capIns);   // read in whole line
        // cout << capIns << endl;
        // insList.push_back(capIns);  // add new line to vector
        // ^ we are not saving the instructions yet
        inst_convert(capIns, out);
    }
    infile.close();     // CLOSE THE FILE
    return 0;
}   //                          *****     END OF PROGRAM     *****


//                              *****      FUNCTION DEFINITIONS     *****
void inst_convert(string instr, ofstream &out)
{
    string check = instr.substr(0,3); // check the instruction
    out << all_instr[check];

    if (check == "ADD" || check == "SUB" || check == "DIV" || check == "MUL")
        arithmetic(instr, out);

    else if(check == "INP" || check == "OUT" || check == "STR")
        access(instr, out);
 
   else if(check == "JMP")
    {
       out << "0000000000000000";
       jump_instr(instr, out);
    }
   
    else if(check == "JME" || check == "JMG" || check == "JGE" || check == "JML" || check == "JLE" || check == "JNE")
        jump_cond(instr, out);
    
    else if(check == "ARR")
        array_make(instr, out);
    
    else if(check[0] >= 'a' && check[0] <= 'z')
    {
       out << "10001";
       jump_label(instr, out);
    }
}


void arithmetic(string instr, ofstream &out)       // Leave to Carlos
{
    // lets check which instruction we are doing at the end
    // first determine which reg
    int i = 3; // we can start after the instruction
    int length = instr.size();
    int count = 0;  // used to keep track of whether we have checked the first two operands
    bool oper_1 = false, oper_2 = false;
    
    while(i < length)
    {
        if(instr[i] == ' ') // this means that we are at the start of a new word
        {
            if(count == 2)  // we got the first two operands
                break;
            // work with the spaces 
            if(instr[i+1] == 'R') // if we have found a register, convert to the correct register
            {
                string check_reg = instr.substr(i+1,2); // check which register
                char number = instr[i+2];
                out << bit_size(number, 8);
                
                count++;
            }
            
            else if(instr[i+1] > '0' && instr[i+1] < '9') // check if number
            {
                string new_val;  // record the new number first as a string, then convert to int, then to binary
                int j = 1;
                
                while(instr[i+j] != ',') // continue until at the end of a number
                {
                    new_val += instr[i + j]; // adding the number onto a string
                    j++;
                }
                //cout << new_val << endl; // test to see if grabbing number
                // after we have found the string, we convert that into an integer value
                
                int val = str_to_int(new_val, 0, new_val.size()-1, new_val.size()-1);
                string bin = int_to_bin(val);       // convert integer to binary
                
                if(bin.size() < 8)  // if the converted binary string is smaller than 8 bits, add more as needed
                {
                    while(bin.size() < 8)
                        bin = '0' + bin;
                }
                //cout << bin << endl;
                if(bin.size() == 8)             // if the binary string is enough bit, write it out
                    out << bin;
                    
                else if(bin.size() > 8)         // if the binary string is bigger than 8 bits, then ERROR
                {
                    
                }
                
                if(count == 0)
                    oper_1 = true;
                else if(count == 1)
                    oper_2 = true;
                count++;
            }
        }
        i++;
    }
    // now we need to write the power bits
    //powerbit #1
    if(oper_1 == false)
        out << "0";
    else if(oper_1 == true)
        out << "1";
        
    // powerbit #2
    if(oper_2 == false)
        out << "0";
    else if(oper_2 == true)
        out << "1";
    
    // LASTLY, output the register to store into
    char last = instr[i+2];
    out << bit_size(last, 4) << endl;
}

void access(string ins, ofstream &out)       // Leave to Alejandro
{  
    string captureInstru = ins.substr(0,3); //check instruction
    
     if((captureInstru=="INP")||(captureInstru=="OUT"))
     {
        out << "0000000000000000";
        char reg = ins[5];
        out << bit_size(reg, 6);
     }
     
     // The store instruction will use two operands.
     // One will be a register or number that is going to be stored
     // The second is the register we are storing into
     // *   -   we will not write out exactly 16 bits in this case because
     // we will have two operands. So this should only be eight bits
     else if(captureInstru == "STR")
     {
        out<< "00000000"; //  *
        int i = 4; // we can start after the instruction
    
        if(ins[i] == 'R')   // if we have found a register, convert to the correct register
        {
            char first = ins[5];
            out << bit_size(first, 8);
    
            char second = ins[9]; // record the second register
            out << "000" << bit_size(second, 3);
         }
         
         // 2nd CASE: we know that it is a number
         else if(ins[i] >= '0' && ins[i] <= '9')    
         {
            int i = 4; // we can start after the instruction
            // int length = ins.size();
            string new_val;  // record the new number first as a string, then convert to int, then to binary
            int j = 0;
            
            while(ins[i+j] != ',') // continue until at the end of a number
            {
                new_val += ins[i + j]; // adding the number onto a string
                j++;
            }
            //cout << new_val << endl; // test to see if grabbing number
            // after we have found the string, we convert that into an integer value
            int val = str_to_int(new_val, 0, new_val.size()-1, new_val.size()-1);
            string bin = int_to_bin(val);       // convert integer to binary
            
            if(bin.size() < 8)  // if the converted binary string is smaller than 8 bits, add more as needed
            {
                while(bin.size() < 8)
                    bin = '0' + bin;
            }
            if(bin.size() == 8)             // if the binary string is enough bit, write it out
                out << bin;
                
            i = i + j + 3;
            char second = ins[i];
            out << "100" << bit_size(second, 3);
         }
         // 3rd CASE: we have an address(array name)
         else if(ins[i] >= 'a' && ins[i] <= 'z')
         {
            int j = 0;
            string arr_name;
            while(ins[i + j] != ',')
            {
                arr_name += ins[i + j]; // get the array name
                j++;
            }//now I need to get the register
            int first = arr_list[arr_name];
            string binary = int_to_bin(first);
            
            while(binary.size() < 8)
                binary = '0' + binary;

            int index = i + j + 3; // go to number of register
            char second = ins[index];
            // need to indicate that an address is stored in reg
            // will use power bit, & last 3 bits are offset
            out << binary << "001" << bit_size(second, 3);
        }
     }
    out<<endl;
}

void array_make(string instr, ofstream &out)
{
    // first I need to get the size of the array
    // then I will get the name of the array and convert that into a label (binary)
    bool reg_lit = false; // false - register, true - literal
    int place = 4;

    if(instr[place] == 'R') // we have a register for the size
    {
        char single = instr[5];
        out << bit_size(single, 8);
        place = place + 4;
    }

    else if(instr[place] > '0'  && instr[place] < '9')  // else we have a literal for the size
    {
        string size;  // record the new number first as a string, then convert to int, then to binary
        int j = 0;
        
        while(instr[place + j] != ',') // continue until at the end of a number
        {
            size += instr[place + j]; // adding the number onto a string
            j++;
        }

        int val = str_to_int(size, 0, size.size()-1, size.size()-1);
        string size_bin = int_to_bin(val);       // convert integer to binary
        
        if(size_bin.size() < 8)  // if the converted binary string is smaller than 8 bits, add more as needed
        {
            while(size_bin.size() < 8)
                size_bin = '0' + size_bin;
        }
        if(size_bin.size() == 8)             // if the binary string is enough bit, write it out
            out << size_bin;
            
        else if(size_bin.size() > 8)         // if the binary string is bigger than 8 bits, then ERROR
        {
            // HERE WE WILL HAVE AN ERROR FUNCITON   
        }
        place = place + j + 2;
        reg_lit = true;
    }
    // need to remember the lable used
    string arr_label_2;
    while(place < instr.size())
    {
        arr_label_2 += instr[place];
        place++;
    }
    cout << "made: " << arr_label_2 << endl;
    arr_list[arr_label_2] = arr_cntr; // remember place of label
    cout << "Saved: " << arr_list[arr_label_2] << endl;
    string arr_label = int_to_bin(arr_cntr);    // convert label into binary
    
    while(arr_label.size() < 8)     // increase operand width to 8
        arr_label = '0' + arr_label;
    
    out << arr_label;
    arr_cntr++;     // we incrememnt number of labels for any other arrays
    
    if(reg_lit == false)
        out << "0";
    else if(reg_lit == true)
        out << "1";
    
    out << "00000" << endl;
}

//                  *****     JUMP FUNCTIONS     *****
void jump_label(string instr, ofstream &out)
{
    out << "0000000000000000"; // need the offset
            
    if(jump_labels[instr] == 0)
    {
        // change the current counter to binary and make size of 5 bits
        jump_labels[instr] = jmp_cntr; // assign number to label
        jmp_cntr++; // increment the jump counter, we have one more jump label
    }
    
    string label = int_to_bin(jump_labels[instr]);    // convert label to binary
    while(label.size() < 6)     // make the proper width
        label = '0' + label;
            
    out << label << endl;       // and write out

}

void jump_instr(string instr, ofstream &out)
{
    //1) get the label
    int label_size = instr.size() - 5;
    string label;
    for(int i = 4; i < instr.size(); i++)
        label += instr[i];
    
    if(jump_labels[label] == 0)
    {
        jump_labels[label] = jmp_cntr;
        jmp_cntr++;
    }
    string bin_label = int_to_bin(jump_labels[label]);
    while(bin_label.size() < 6)
            bin_label = '0' + bin_label;
        
    out << bin_label << endl;
    
}

void jump_cond(string instr, ofstream &out)
{
    int i = 3, count = 0;
    bool oper_1 = false;
    
    while(i < instr.size())
    {
        if(instr[i] == ' ') // this means that we are at the start of a new word
        {
            if(count == 2)  // we got the first two operands
                break;
            // work with the spaces 
            if(instr[i+1] == 'R') // if we have found a register, convert to the correct register
            {
                char number = instr[i+2];
                out << bit_size(number, 8);
                
                count++;
            }
            
            else if(instr[i+1] > '0' && instr[i+1] < '9') // check if number
            {
                string new_val;  // record the new number first as a string, then convert to int, then to binary
                int j = 1;
                
                while(instr[i+j] != ',') // continue until at the end of a number
                {
                    new_val += instr[i + j]; // adding the number onto a string
                    j++;
                }
                //cout << new_val << endl; // test to see if grabbing number
                // after we have found the string, we convert that into an integer value
                
                int val = str_to_int(new_val, 0, new_val.size()-1, new_val.size()-1);
                string bin = int_to_bin(val);       // convert integer to binary
                
                if(bin.size() < 8)  // if the converted binary string is smaller than 8 bits, add more as needed
                {
                    while(bin.size() < 8)
                        bin = '0' + bin;
                }
                //cout << bin << endl;
                if(bin.size() == 8)             // if the binary string is enough bit, write it out
                    out << bin;
                    
                else if(bin.size() > 8)         // if the binary string is bigger than 8 bits, then ERROR
                {
                    
                }
                if(count == 0)
                    oper_1 = true;

                count++;
            }
        }
        i++;
    }
    if(oper_1 == false)
        out << "000000" << endl;
    else
        out << "100000" << endl;
}

// This will make the register size equal to 
string bit_size(char num, int size)
{
    string next;
    next = num + next;
    int number = str_to_int(next, 0, next.size()-1, next.size()-1) - 1;
    next = int_to_bin(number);
    while(next.size() < size)
        next = '0' + next;
    return next;
}

// *****     THE FOLLOWING TWO FUCTIONS ARE FOR STRING TO INT AND INT TO BINARY CONVERSION     *****
int str_to_int(string reg, int i, int s, int p)
{
    if( i == s)
        return (reg[i] - '0');
    else
    {
        int num = reg[i] - '0';
        return (num*pow(10, p) + str_to_int(reg, i + 1, s, p - 1));
    }
}

string int_to_bin(int num)
{
    string bin = "";
    if(num == 0)
        return "0";

    while(num != 0)
    {
        bin = to_string(num%2) + bin;
        num = num / 2;
    }
    return bin;
}
// *****     THE ABOVE TWO FUCTIONS ARE FOR STRING TO INT AND INT TO BINARY CONVERSION     *****

// *****     THE BELOW FUNCTION IS FOR THE CASE OF ERRORS     *****
void error_detected(string instruction, string description, string problem)
{
    cout << "Error with instruction: " << instruction << endl;
    cout << description <<  problem << endl;
    
    if(have_errors == false)
        have_errors = true;
}
