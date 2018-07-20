//  main.cpp
//  Sigma
/*
 This file will convert the binary into performed actions.
 We wil read the first four bits on a line to tell the isntruction.
 */
 
 // Here we have the libraries
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <map>
#include <cmath>
#include <utility>
#include "Anylitical.h"

using namespace std;

// Functions
void access_instr(std::vector<Analytical> &trueInstru); // we will use this to check what instruction
int bin2dec(string bin_3);
void math_instr();

// *****     GLOBAL VARIABLES     *****
vector<string> rawInstru ; // will capture every line of binary
Analytical nowAnylyze; //keeps track of current line of anylization

//int label = 0;
int i = 0;  
int check_ne1 = 0, check_ne2 = 0, check_e1 = 0, check_e2 = 0;
int must_stop = 0;

vector<Analytical> trueInstru; // this will split every line of binary to necessary parts
map<string, pair<int, bool>> reg_map;
vector<int> allRay;
vector<pair<int, int>> arr_list; 
/* ^This will be keep track of all of our arrays and their sizes In order to find the size of the array, we need to find the label of the array. The value(second) attached
to the corresponding array label(first) is the size of the array. To get to later arrays, all values need
to be added until the desired array is found. */


int main()                  // *****     START OF PROGRAM     *****
{
    // Below - All registers are declared and initialized.
    // By using a map, we replace the index with strings.
    reg_map["000"].first = 0; // r1 index is being replaced by the string
    reg_map["001"].first = 0; // r2
    reg_map["010"].first = 0; // r3
    reg_map["011"].first = 0; // r4
    reg_map["100"].first = 0; // r5
    reg_map["101"].first = 0; // r6
    reg_map["110"].first = 0; // r7
    reg_map["111"].first = 0; // r8
    
    ifstream infile;   // we have declared the file that has all of the instructions converted to binary
    infile.open("binary.o"); // we have opened the file that has all of the instructions converted to binary
    
    if(infile.fail()) //Test to see if the file did open, if not the program will stop
    {
        cout<<"File could not be opened"<<endl;
        cout<<"Ending Program!"<<endl;
        exit(1);
    }
    
    while(!infile.eof()) // read in the entire binary file, and push into a vector
    {
        string capTheBin = "";
        infile>>capTheBin;
        rawInstru.push_back(capTheBin);
    }
    
    int size = rawInstru.size();
    string capTheBin = "";
    // Below - we break up every line of binary in the rawInstru vector into the necessray parts.
    // The parts are placed into an object which has one data member for each part of the instruction.
    for(int i = 0; i < size; i++)
    {
        capTheBin=rawInstru[i].substr(0,5);
        nowAnylyze.setInstru(capTheBin);    // Stores instruction (Bits 0 - 4)
        capTheBin=rawInstru[i].substr(5,8);
        nowAnylyze.setRegOr1(capTheBin);    // Stores Operand 1 (Bits 5 - 12)
        capTheBin= rawInstru[i].substr(13,8);
        nowAnylyze.setRegOr2(capTheBin);    // Stores Operand 2 (Bits 13 - 20)
        capTheBin= rawInstru[i].substr(21,3);
        nowAnylyze.setAction(capTheBin);    // Stores "Power bits" (Bits 21 - 23)
        capTheBin=rawInstru[i].substr(24,3);
        nowAnylyze.setRegizter(capTheBin);  // Stores Register (Bits 24 - 26)
        capTheBin = "";
        
        trueInstru.push_back(nowAnylyze);   // Pushes the object back into a vector of these objects
    }
    
    // MASTER LOOP
    // In this loop, we do the actions corresponding to the binary instructions. 
    for(i = 0; i < size; i++){    // using index to keep track of where we are in file
    
        // ARITHMETIC OPERATIONS
        if (trueInstru[i].getInstru() == "00000" || trueInstru[i].getInstru() == "00001" || trueInstru[i].getInstru() == "00010" || trueInstru[i].getInstru() == "00011") //add
            access_instr(trueInstru);       // ADD / SUB / MUL / DIV
        
        // TAKING INPUT FROM USER
        else if (trueInstru[i].getInstru()=="00100")        //input
        {
            // First, the user saves directly into a register. (Direct access)
            string str = trueInstru[i].getRegizter();
            if(reg_map[str].second == false){    // this means we are working with a value
            
                //cout << "Enter a value: ";
                //cout << "Case input 1\n";
                cin >> reg_map[str].first;
                //cout << str << " now has " << reg_map[str].first << endl;
            }
            // Second, the user is saving into an array. (Indirect memory)
            // If the register has an address stored in it, any value stored into the register, 
            // will then be saved into the corresponding space in an array/ in memory.
            else if(reg_map[str].second == true){
            
                int ind = reg_map[str].first;
                //cout << "Register value: " << reg_map[str].first<< endl;
                //cout << "Ind: " << ind << endl;
                //cout << "Enter a value: ";
                //cout << "Case input 2\n";
                cin >> allRay[ind];
                //cout << "AllRay[" << ind << "]: " << allRay[ind] << endl;
            }
        }
        
        // OUTPUTING TO SCREEN FOR USER
        else if(trueInstru[i].getInstru()=="00101")
        {
            // Similar to output, the user can output the value in a register, or
            // the value in an array.
            // First, outputs a register value.
            string str = trueInstru[i].getRegizter();
            if(reg_map[str].second == false)
                cout<<reg_map[str].first<<endl;
            // Second, outputs a value in memory.
            else if(reg_map[str].second == true)
                cout << allRay[reg_map[str].first] << endl;
        }
        
        
        // STORING INTO REGISTERS OR MEMORY
        else if(trueInstru[i].getInstru()=="00111"){
            int value; // to store value
            string take, go_to = trueInstru[i].getRegizter();
            
            if(trueInstru[i].getAction() == "000"){
                take = trueInstru[i].getRegOr2();
                take = take.substr(5, 3);
                //value = bin2dec(take);
                reg_map[go_to].first = reg_map[take].first;
                cout << go_to << " now has " << reg_map[go_to].first << endl;
                //cout << reg_map[take].first << " put into " << go_to <<endl;
                //cout << "Register " << go_to << " is now " << reg_map[go_to].first << endl;
            }
            
            else if(trueInstru[i].getAction() == "100" || trueInstru[i].getAction() == "001"){
                take = trueInstru[i].getRegOr2();
                value = bin2dec(take);
                //if(trueInstru[i].getAction() == "100")
                reg_map[go_to].first = value;
                //else if(trueInstru[i].getAction() == "001")
                  //  reg_map[go_to] = allRay[value];
                //cout << "lit or address\n";
            }
            
            if(trueInstru[i].getAction() == "001")
            {// if it was an address, change the register to indicate so
                reg_map[go_to].second = true;
                //cout << "stored as address\n";
            }
            //cout << "Value stored\n";
            //cout <<"Reg 8: " <<  reg_map["111"].first << endl;
        }
        
        // JUMP instruction
        else if(trueInstru[i].getInstru()=="01000") 
        {
            //cout << "Beginning jump\n";
            string label, comp;
            label = trueInstru[i].getAction() + trueInstru[i].getRegizter();
            //cout << "label: " << label << endl;
            i = 0;
            int numo = trueInstru.size();
            
            while(i < numo){
                
                comp = trueInstru[i].getAction() + trueInstru[i].getRegizter();
                //cout << "comp: " << comp << endl;
                if(label == comp && trueInstru[i].getInstru() == "10001")
                    break;
                i++;
            }
                //cout << reg_map["000"].first; // r1 index is being replaced by the string
                //cout << reg_map["001"].first; // r2
                //cout << reg_map["010"].first; // r3
                //cout << reg_map["011"].first; // r4
                //cout << reg_map["100"].first; // r5
                //cout << reg_map["101"].first; // r6
                //cout << reg_map["110"].first; // r7
                //cout << reg_map["111"].first; // r8

            //cout << "Jump finished\n";
        }
        
        else if(trueInstru[i].getInstru()=="01001") // JUMP if equals 
        {
            // cout << "Reg 8: " <<  reg_map["111"].first << endl;
            // cout << "jump if = FUNCTION\n";
            
            int val1, val2;
            string op1 = trueInstru[i].getRegOr1(), op2 = trueInstru[i].getRegOr2();

            if(trueInstru[i].getAction() == "100"){ // first is a reg
                val1 = bin2dec(op1);
                //cout << "Value 1 (lit): " << val1 << endl;
            }
            
            else if(trueInstru[i].getAction() == "000"){ // first is a literal
                op1 = op1.substr(5, 3);
                val1 = reg_map[op1].first;
                //cout << "Value 1 (reg): " << val1 << endl;
            }
            op2 = op2.substr(5, 3)
            val2 = reg_map[op2].first;
            // cout << "Value 2: " << val2 << endl;
            // cout <<"Reg 8: " <<  reg_map["111"].first << endl;

            /*
            if(reg_map[op2].second == false)
            {
                if(val1 == val2)
                {
                    i++;
                    cout << "jump cond w/o address true\n";
                    check_e1++;
                } cout
            }
            */
            
            //if(reg_map[op2].second == true)
            //{
                if(val1 == val2)
                {
                    i++;
                    //cout << "jump cond w/ address true\n";
                    check_e2++;
                }
            //}
            //cout <<"Reg 8: " <<  reg_map["111"].first << endl;

        }
        
        // JUMP <
        else if(trueInstru[i].getInstru()=="01010"){
            if(trueInstru[i].getRegOr1() > trueInstru[i].getRegOr2())
                i ++;
                //cout << "jump cond found\n";
        }
        
        // JUMP <= 
        else if(trueInstru[i].getInstru()=="01011"){
            if(trueInstru[i].getRegOr1() >= trueInstru[i].getRegOr2())
                i++;
                //cout << "jump cond found\n";
        }
        
        // JUMP >
        else if(trueInstru[i].getInstru()=="01100"){
            if(trueInstru[i].getRegOr1() < trueInstru[i].getRegOr2())
                i++;
                //cout << "jump cond found\n";
        }
        
        // JUMP >=
        else if(trueInstru[i].getInstru()=="01101"){
            if(trueInstru[i].getRegOr1() <= trueInstru[i].getRegOr2())
                i++;
                //cout << "jump cond found\n";
        }
        
        // JUMP !=
        else if(trueInstru[i].getInstru() == "10011"){
            //cout << "jump if != found\n";
            int val1, val2;
            string op1 = trueInstru[i].getRegOr1(), op2 = trueInstru[i].getRegOr2();
            //cout << op1 << endl;
            
            if(trueInstru[i].getAction() == "100")
            {
                val1 = bin2dec(op1);
                //cout << "Value 1(lit): " << val1 << endl;
            }
           
            else if(trueInstru[i].getAction() == "000")
            {
                op1 = op1.substr(5, 3);
                val1 = reg_map[op1].first;
                //cout << "Value 1(reg): " << val1 << endl;
            }
            
            op2 = op2.substr(5, 3);
            val2 = reg_map[op2].first;
            
            //cout << "Value 2: " << val2 << endl;
            
            
            if(reg_map[op2].second == false)
            {
                if(val1 != val2)
                {
                    i++;
                    //cout << "jump cond w/o address true\n";
                    check_ne1++;
                }
            }
            
            else if(reg_map[op2].second == true)
            {
                if(val1 != allRay[val2])
                {
                    i++;
                    //cout << "jump cond w/ address true\n";
                    check_ne2++;
                }
            }
            must_stop++;
            
            if(must_stop == 10)
            {
                //cout << "it is here!\n";
                break;
            }
        }
        
        
        else if(trueInstru[i].getInstru()=="01111")
        {
            //cout << "Beginning clear...\n";
            reg_map["000"].first = 0; // r1
            reg_map["001"].first = 0; // r2
            reg_map["010"].first = 0; // r3
            reg_map["011"].first = 0; // r4
            reg_map["100"].first = 0; // r5
            reg_map["101"].first = 0; // r6
            reg_map["110"].first = 0; // r7
            reg_map["111"].first = 0; // r8
            reg_map["000"].second = false; // r1 index is being replaced by the string
            reg_map["001"].second = false; // r2
            reg_map["010"].second = false; // r3
            reg_map["011"].second = false; // r4
            reg_map["100"].second = false; // r5
            reg_map["101"].second = false; // r6
            reg_map["110"].second = false; // r7
            reg_map["111"].second = false; // r8
            //cout << "Clear complete!\n";
        }
        else if(trueInstru[i].getInstru()=="10000")
        {
            //cout << "At array make\n";
            string size_bin = trueInstru[i].getRegOr1(), name = trueInstru[i].getRegOr2();
            int size, label = bin2dec(name);   

            if(trueInstru[i].getAction() == "000")
            {
                size_bin = size_bin.substr(4, 3);
                size = reg_map[size_bin].first;
            }
            else if(trueInstru[i].getAction() == "100")
                size = bin2dec(size_bin);
            
            for(int i = 0; i < size; i++)
                allRay.push_back(0);
            //cout << "Array made\n";
            
            arr_list.push_back((make_pair(label, size)));
            //cout << "Pair pushed back\n";
        }
       // else if(trueInstru[i].getInstru()=="10001")
            //cout << "label recognized\n";
        else if(trueInstru[i].getInstru()=="11111")
        {
            return 0;
        }
    }
    //cout << "Equal 1: " << check_e1 << endl;
    //cout << "Equal 2: " << check_e2 << endl;
    //cout << "Not Equal 1: " << check_ne1 << endl;
    //cout << "Not Equal 2: " << check_ne2 << endl;
    return 0;
}
//                  *****     END OF MAIN PROGRAM     *****


int bin2dec(string bin_3)   // this converts binary value to decimal
{
    int dec = 0;
    int expo=0;
    long cap;
    cap = bin_3.size()-1;
    expo = (int)cap;
    for (int i = 0; i<bin_3.size(); i++){
        if (bin_3[i]=='0'){
            expo--;
            continue;
        }else if(bin_3[i]=='1'){
            dec = dec + pow(2,expo);
            expo--;
        }
    }
    return dec;
}
// We have some instruction code stored in here
// Otherwise, some of it is stored in main in the MASTER LOOP
void access_instr(std::vector<Analytical> &trueInstru)
{
    string capNtrasnfer = "", cp = "",last = "";
    int num =0;
    int siz = trueInstru.size();
        //cout<<"---------------------------------"<<i<<endl;
        //cout<<endl;
        //cout<<trueInstru[i].getInstru()<<endl;
        if(trueInstru[i].getAction()=="000")
        {
            string deltaInstru = trueInstru[i].getInstru();
            if(trueInstru[i].getInstru() == "00000" || trueInstru[i].getInstru() == "00001" || trueInstru[i].getInstru() == "00010" || trueInstru[i].getInstru() == "00011")
            {
                capNtrasnfer=trueInstru[i].getRegOr1();
                capNtrasnfer= capNtrasnfer.substr(5,3);
                cp = trueInstru[i].getRegOr2();
                cp = cp.substr(5,3);
                
                last = trueInstru[i].getRegizter();
                string deltaInstru = trueInstru[i].getInstru();
                
                if(deltaInstru=="00000")
                {
                    reg_map[last].first = reg_map[capNtrasnfer].first + reg_map[cp].first;
                    //cout<< "Printed line "<<i<<endl;
                    //cout<<reg_map[last].first<<endl;
                }else if(deltaInstru=="00001")
                {
                    reg_map[last].first = reg_map[capNtrasnfer].first - reg_map[cp].first;
                    //cout<< "Printed line "<<i<<endl;
                    //cout<<reg_map[last].first<<endl;
                }else if(deltaInstru=="00010")
                {
                    reg_map[last].first = reg_map[capNtrasnfer].first * reg_map[cp].first;
                    //cout<< "Printed line "<<i<<endl;
                   // cout<<reg_map[last].first<<endl;
                    
                }else if(deltaInstru=="00010")
                {
                    reg_map[last].first = reg_map[capNtrasnfer].first / reg_map[cp].first;
                  //  cout<< "Printed line "<<i<<endl;
                   // cout<<reg_map[last].first<<endl;
                }
            }
            /*
            else if(deltaInstru == "00111")
            {
                string from = trueInstru[i].getRegOr2(), target = trueInstru[i].getRegizter();
                from = from.substr(4, 3);
                
                reg_map[target].first = reg_map[from].first;
            }
            */
        }
        else if(trueInstru[i].getAction()=="010")
        {
             if(trueInstru[i].getInstru() == "00000" || trueInstru[i].getInstru() == "00001" || trueInstru[i].getInstru() == "00010" || trueInstru[i].getInstru() == "00011")
            {
                capNtrasnfer = trueInstru[i].getRegOr1();
                capNtrasnfer= capNtrasnfer.substr(5,3);
                
                cp= trueInstru[i].getRegOr2();

                last = trueInstru[i].getRegizter();
                
                num = bin2dec(cp);
                
                string deltaInstru= trueInstru[i].getInstru();
                
                //cout << "CapN: " << capNtrasnfer << endl;
                //cout << "Num: " << num << endl;
                //cout << "DeltaIns: " << deltaInstru << endl;
                
                if(deltaInstru=="00000")
                {
                    //if(reg_map[last].second == false)
                    //{
                    reg_map[last].first = reg_map[capNtrasnfer].first + num;
                   // cout<< "Printed line "<<i<<endl;
                    //cout<<"Reg " << last << " is now " << reg_map[last].first<<endl;
                    //}
                    //else if(reg_map[last].second == true)
                    //{
                        //reg_map[last].first;
                    //}
                }else if(deltaInstru=="00001")
                {
                    reg_map[last].first = reg_map[capNtrasnfer].first - num;
                    //cout<< "Printed line "<<i<<endl;
                    //cout<<reg_map[last].first<<endl;
                }else if(deltaInstru=="00010")
                {
                    reg_map[last].first = reg_map[capNtrasnfer].first * num;
                    //cout<< "Printed line "<<i<<endl;
                    //cout<<reg_map[last].first<<endl;
                    
                }else if(deltaInstru=="00010")
                {
                    reg_map[last].first = reg_map[capNtrasnfer].first / num;
                    //cout<< "Printed line "<<i<<endl;
                    //cout<<reg_map[last].first<<endl;
                }
            }
        }
        else if(trueInstru[i].getAction()== "100")
        {
            string deltaInstru= trueInstru[i].getInstru();

            if(trueInstru[i].getInstru() == "00000" || trueInstru[i].getInstru() == "00001" || trueInstru[i].getInstru() == "00010" || trueInstru[i].getInstru() == "00011")
            {
                string capNtrasnfer = "", cp = "", last = "";
                int num;
                
                capNtrasnfer = trueInstru[i].getRegOr2();
                capNtrasnfer= capNtrasnfer.substr(5,3);
                
                cp= trueInstru[i].getRegOr1();
                
                last = trueInstru[i].getRegizter();
                
                num = bin2dec(cp);
                
                reg_map[capNtrasnfer];
                
                if(deltaInstru=="00000")
                {
                    reg_map[last].first = reg_map[capNtrasnfer].first + num;
                    
                    cout<<reg_map[last].first<<endl;
                   // cout<< "Printed line "<<i<<endl;
                    
                }else if(deltaInstru=="00010")
                {
                    reg_map[last].first = reg_map[capNtrasnfer].first - num;
                    
                    cout<<reg_map[last].first<<endl;
                   // cout<< "Printed line "<<i<<endl;
                    
                }else if(deltaInstru=="00010")
                {
                    reg_map[last].first = reg_map[capNtrasnfer].first * num;
                    cout<<reg_map[last].first<<endl;
                  //  cout<< "Printed line "<<i<<endl;
                }else if(deltaInstru=="00011")
                {
                    reg_map[last].first = reg_map[capNtrasnfer].first / num;
                    cout<<reg_map[last].first<<endl;
                   // cout<< "Printed line "<<i<<endl;
                }
            }
        }
        else if(trueInstru[i].getAction()== "110")
        {
           if(trueInstru[i].getInstru() == "00000" || trueInstru[i].getInstru() == "00001" || trueInstru[i].getInstru() == "00010" || trueInstru[i].getInstru() == "00011")
            {
                string capNtrasnfer = "", cp= "", last = "";
                
                int num1 =0;
                int num2=0;
                capNtrasnfer=trueInstru[i].getRegOr1();
                capNtrasnfer= capNtrasnfer.substr(5,3);
                cp = trueInstru[i].getRegOr2();
                cp = cp.substr(5,3);
                last = trueInstru[i].getRegizter();
                num1= bin2dec(capNtrasnfer);
                num2= bin2dec(cp);
                
                string daInstru= trueInstru[i].getInstru();
                
                if(daInstru=="00000")
                {
                    reg_map[last].first = num1 + num2;
                    cout<<reg_map[last].first;
                }else if( daInstru=="00010")
                {
                    reg_map[last].first = num1 - num2;
                    cout<<reg_map[last].first;
                }else if(daInstru=="00010")
                {
                    reg_map[last].first = num1* num2;
                    cout<<reg_map[last].first;
                }else if(daInstru=="00011")
                {
                    reg_map[last].first = num1 / num2;
                    cout<<reg_map[last].first;
                }
            }  
        }
        
        else if(trueInstru[i].getAction()== "101")
        {
            // were going to have a label for the array funcitons
            string label = "", sizeofArray = "";
            int num;
            label = trueInstru[i].getRegOr2();
            
            int num2= bin2dec(label);
            
            sizeofArray= trueInstru[i].getRegOr1();
            
            num = bin2dec(sizeofArray);
            
           // cout<<"size of array: "<<num<<endl;
            if (trueInstru[i].getInstru()=="10000")
            {
                for(int i = 0; i< num; i++)
                {
                    allRay.push_back(0);
                }
                arr_list.push_back((make_pair(num2, num)));
            }
            if( trueInstru[i].getInstru()=="00100")
            {
               // for(int i = start)
            }
        }

}