// /*****************************************************************************
// TITLE: Claims																																
// AUTHOR: Amish Mittal (1801CS07)
// Declaration of Authorship
// This txt file, claims.txt, is part of the assignment of CS321 at the 
// department of Computer Science and Engg, IIT Patna . 
// *****************************************************************************/

#include <bits/stdc++.h>

using namespace std;

int mainMemory[1<<24];
int total = 0;
int stackLimit = 1<<23;
int PC = 0, SP = 0, regA = 0, regB = 0;
vector<int> objectFile;
vector<string> mnemonics{"ldc","adc","ldl","stl","ldnl","stnl","add","sub","shl","shr","adj","a2sp","sp2a","call","return","brz","brlz","br","HALT"};

void executeOpcode(int opcode, int operand){
    switch(opcode){
        case 0: regB = regA;
	            regA = operand;
                break;
        case 1: regA += operand;
                break;
        case 2: regB = regA;
	            regA = mainMemory[SP + operand];
                break;
        case 3: mainMemory[SP + operand] = regA;
	            regA = regB;
                break;
        case 4: regA = mainMemory[regA + operand];
                break;
        case 5: mainMemory[regA + operand] = regB;
                break;
        case 6: regA = regB + regA;
                break;
        case 7: regA = regB-regA;
                break;
        case 8: regA = regB<<regA;
                break;
        case 9: regA = regB>>regA;
                break;
        case 10: SP = SP + operand;
                break;
        case 11: SP = regA;
	            regA = regB;
                break;
        case 12: regB = regA;
                regA = SP;
                break;
        case 13: regB = regA;
                regA = PC;
                PC = operand - 1;
                break;
        case 14: PC = regA;
                regA = regB;
                break;
        case 15: 
            if(regA == 0){
		        PC = PC + operand;
            }
            break;
        case 16: 
            if(regA < 0){
                PC = PC + operand;
            }
            break;
        case 17: PC = PC + operand;
            break;
        default: cout<<"Invalid opcode. Incorrect machine code. Aborting";
                exit(0);
    }    
}

int argumentrun(){
    if(PC >= (int)objectFile.size()){
        cout<<"Segmentation fault. Aborting. ";
        exit(0);
    }
    int opcode = 0xFF & objectFile[PC];  // last 8 bits
	int operand = objectFile[PC]>>8; //first 24 bits
    cout<<mnemonics[opcode];
	printf("\t%08X\n", operand);
	if (opcode == 18) {
		total++;
		return 0;   // back to main function
	}
    executeOpcode(opcode, operand);
	total++;
	PC++;
    if(SP > stackLimit){
        cout<<"Stack overflow. Aborting.";
        exit(0);
    }
	return 1;
}

pair<long, bool> read_operand (string &operand)
{
    if (!(int)operand.size()) {
        return {0, 0};
    }
    int len = (int)operand.size();
    char *str = (char *)malloc (len *  sizeof(char));
    for (int i = 0; i < (int)operand.size(); i++) {
        str[i] = operand[i];
    }
    for (int i = (int)operand.size(); i < strlen(str); i++) {
        str[i] = '\0';
    }
    char *end;
    long int num;
    num = strtol(str, &end, 10);
    if (!*end) {
        return {num, 1};
    }
    num = strtol(str, &end, 16);
    if (!*end) {
        return {num, 1};
    }
    return {-1, 0};
}

void dump(){
    int baseAddress;
	cout<<"Base address: ";
	string operand;
	cin>>operand;
	pair<long, bool> val = read_operand(operand);
	baseAddress = val.first;
	cout<<"No. of values: ";
    string offset;
	cin>>offset;
	pair<long, bool> val2 = read_operand(offset);
	for (int i = baseAddress; i < baseAddress + val2.first; i = i + 4) {
		printf("%08X %08X %08X %08X %08X\n", i, mainMemory[i], mainMemory[i+1], mainMemory[i+2], mainMemory[i+3]);	
	}
}

int advance(){
    cout<<"Emulator input: ";
    string temp;
    cin>>temp;
    if(temp == "-t" || temp == "-T"){
        int tempo = argumentrun();
        printf("A = %08X, B = %08X, PC = %08X, SP = %08X\n", regA, regB, PC, SP);
        if(!tempo){
            return 0;
        }
    }
    else if(temp == "-all" || temp == "-ALL"){
        while(argumentrun()){
            printf("A = %08X, B = %08X, PC = %08X, SP = %08X\n", regA, regB, PC, SP);
        }
        return 0;
    }
    else if(temp == "-dump" || temp == "-DUMP"){
        dump();
    }
    else{
        cout<<"Invalid emulator input"<<endl;
        return 1;
    }
    return 1;
}

int main(int argc, char*argv[]){
    int _temp;
    ifstream currfile(argv[1], ios::in | ios::binary);
    while(currfile.read((char*) &_temp, sizeof(int))){ 
    	objectFile.push_back(_temp);
    }
    int i = 0;
    for(auto x: objectFile){
        mainMemory[i] = x;
        i++;
    }
    cout<<"-t for trace\n-dump for memory dump\n-all for all commands\nEnter with hyphen\n";
    for(;1;){
        if(advance()){
            continue;
        }
        break;
    }
    cout<<"Total instructions executed: "<<total<<endl;
    return 0;
}