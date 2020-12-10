// /*****************************************************************************
// TITLE: Claims																																
// AUTHOR: Amish Mittal (1801CS07)
// Declaration of Authorship
// This txt file, claims.txt, is part of the assignment of CS321 at the 
// department of Computer Science and Engg, IIT Patna . 
// *****************************************************************************/
#include <bits/stdc++.h>

using namespace std;

map<int, int> originalCodes;
vector<string> errors;
vector<string> warnings;
vector<string> objectFile;
vector<string> listingFile;
unordered_map<string, pair<int, int>> opcodes;
unordered_map<string, int> extras;
set<string> existingLabels;

void initializeOpcodes(){
    opcodes = {
        {"ldc", {0, 1}},
        {"adc", {1, 1}},
        {"ldl", {2, 1}},
        {"stl", {3, 1}},
        {"ldnl", {4, 1}},
        {"stnl", {5, 1}},
        {"add", {6, 0}},
        {"sub", {7, 0}},
        {"shl", {8, 0}},
        {"shr", {9, 0}},
        {"adj", {10, 1}},
        {"a2sp", {11, 0}},
        {"sp2a", {12, 0}},
        {"call", {13, 2}},
        {"return", {14, 0}},
        {"brz", {15, 2}},
        {"brlz", {16, 2}},
        {"br", {17, 2}},
        {"HALT", {18, 0}},
        {"data", {19, 1}},
        {"SET", {-1, 1}}
    };
    extras = {
        {"data", 1},
        {"SET", 1}
    };
}

vector<string> formatCompleteCode (vector<string> &_completeInput){
    vector<string> _completeCode;
    for(int i = 0; i < (int)_completeInput.size(); ++i){
        string _currcode;
        bool _startingWhite = 1;
        for(int j = 0; j < (int)_completeInput[i].size(); ++j){
            char temp = _completeInput[i][j];
            if(temp == ';'){
                break;
            }
            if(temp != ' ' && temp != '\t'){
                _startingWhite = 0;
            }
            if (_startingWhite && temp == ' ' or temp == '\t') {
                continue;
            }
            _currcode.push_back(temp);
        }
        bool _charpresent = 0;
        for(int j = 0; j < (int)_currcode.size(); ++j){
            if((_currcode[j] >= 'a' && _currcode[j] <= 'z') || (_currcode[j] >= 'A' && _currcode[j] <= 'Z')){
                _charpresent = 1;
                break;
            }
        }
        if(_charpresent){
            originalCodes[(int)_completeCode.size()] = i + 1;
            _completeCode.push_back(_currcode);
        }
    }
    return _completeCode;
}

bool isValidLabel(string &_label){
    if(((int)_label.size() != 0) && ((_label[0] >= 'a' && _label[0] <= 'z') || (_label[0] >= 'A' && _label[0] <= 'Z') || _label[0] == '_')){
        return 1;
    }
    return 0;
}

int getOpcodeFromMnenomic(string &mnenomic){
    if(opcodes.find(mnenomic) != opcodes.end()){
        return opcodes[mnenomic].first;
    }
    return -1;
}
pair<long, bool> useBaseOfArgument(string &_argument)
{
    if (!(int)_argument.size()) {
        return {0, true};
    }
    int _length = _argument.size();
    // dynamic allocation
    char *_preConvert = (char *) malloc (sizeof(char)*_length);
    int i = 0;
    for (i = 0; i < (int)_argument.size(); ++i) {
        *(_preConvert+i) = _argument[i];
    }
    for (i = (int)_argument.size(); i <= strlen(_preConvert)-1; ++i) {
        *(_preConvert+i) = '\0';
    }
    char *end;
    long _temp;
    _temp = strtol(_preConvert, &end, 10);
    if (!*end) {
        return {_temp, true};
    }
    _temp = strtol(_preConvert, &end, 8);
    if (!*end) {
        return {_temp, true};
    }
    _temp = strtol(_preConvert, &end, 16);
    if (!*end) {
        return {_temp, true};
    }
    else
        return {-1, false};
}
map<string, int> setLabel(vector<string> &_completeCode){
    int _count = 0;
    map<string, int> _setLabels;
    int _tempoo = 0;
    for(int i = 0; i < (int) _completeCode.size(); ++i){
        int _semicolonCount = 0;
        for(int j = 0; j < (int) _completeCode[i].size(); ++j){
            if(_completeCode[i][j] == ':'){
                _semicolonCount++;
            }
        }
        if(!_semicolonCount){
            _count++;
            continue;
        }
        if(_semicolonCount > 1){
            string _temp = "Wrong label format at line: ";
            _temp.append(to_string(originalCodes[i]));
            errors.push_back(_temp);
            continue;
        }
        string _label;
        int k = 0;
        while(_completeCode[i][k] != ':'){
            _label.push_back(_completeCode[i][k]);
            k++;
        }
        if (getOpcodeFromMnenomic(_label) != -1) {
            string _temp = "Label used \"" + _label + "\" is a programming keyword: ";
            _temp.append(to_string(originalCodes[i]));
            errors.push_back(_temp);
            continue;
        }
        if(isValidLabel(_label) == 0){
            string _temp = "Label is empty or starts with a character other than an alphabet and underscore at line: ";
            _temp.append(to_string(originalCodes[i]));
            errors.push_back(_temp);
            continue;
        }
        if(_setLabels.find(_label) != _setLabels.end()){
            string _temp = "Label already used at line: ";
            _temp.append(to_string(originalCodes[i]));
            errors.push_back(_temp);
            continue;
        }
        _setLabels[_label] = _count + _tempoo;
        stringstream _temp (_completeCode[i]);
        string _temp1, _temp2, _temp3;

        _temp>>_temp1;
        _temp>>_temp2;
        _temp>>_temp3;
        if((int)_temp2.size()){
            _count++;
        }
        if(_temp2 == "SET"){
            _setLabels[_label] = useBaseOfArgument(_temp3).first;
            _tempoo++;
        }
    }
    return _setLabels;
}


string properHex(int _count, int length){
    stringstream ss;
    ss<<hex<<_count;
    string ans (ss.str());
    reverse(ans.begin(), ans.end());
    while (length > (int)ans.size()){
        ans.push_back('0');
    }
    while (length < (int)ans.size()){
        ans.pop_back();
    }
    reverse(ans.begin(), ans.end());
    return ans;
}

int main(int argc, char* argv[]){
    freopen(argv[1], "r", stdin);
    vector<string> _completeInput;
    string _temp;
    while(getline(cin, _temp)){
        _completeInput.push_back(_temp);
    }
    // for(auto it = _completeCode.begin(); it != _completeCode.end(); ++it){
    //     cout<<*it<<endl;
    // }
    vector<string> _completeCode = formatCompleteCode(_completeInput);
    // for(auto it = _completeCode.begin(); it != _completeCode.end(); ++it){
    //     cout<<*it<<endl;
    // }
    initializeOpcodes();
    map<string, int> _setLabels = setLabel(_completeCode);

    int _count = 0;
    for(int i = 0; i < (int)_completeCode.size(); ++i){
        string _currline = _completeCode[i];
        stringstream _temp (_currline);
        string _currMnemonic;
        _temp>>_currMnemonic;
        if(*_currMnemonic.rbegin() != ':'){
            int _opcode = getOpcodeFromMnenomic(_currMnemonic);
            if(_opcode == -1){
                string _temp = "Invalid mnemonic at line: ";
                _temp.append(to_string(originalCodes[i]));
                errors.push_back(_temp);
                continue;
            }
            stringstream arguments (_currline);
            string _tempo;
            int _noOfArguments = 0;
            for(;arguments>>_tempo;){
                _noOfArguments++;
            }
            if(_noOfArguments >= 3){
                string _temp = "More number of operands at line: ";
                _temp.append(to_string(originalCodes[i]));
                errors.push_back(_temp);
                continue;
            }
            if(opcodes[_currMnemonic].second == 2){
                string _argument;
                _temp>>_argument;
                existingLabels.insert(_argument);
                if(_setLabels.find(_argument) == _setLabels.end()){
                    string _temp = _argument;
                    _temp.append(" label does not exist, line: ");
                    _temp.append(to_string(originalCodes[i]));
                    errors.push_back(_temp);
                    continue;
                }
                string _machineCode = properHex(_count, 4);
                _machineCode.push_back(' ');
                _machineCode.append(properHex(_setLabels[_argument] - _count - 1, 6));
                _machineCode.append(properHex(_opcode, 2));
                if(_currMnemonic == "call"){
                    _machineCode = properHex(_count, 4) + " " + properHex(_setLabels[_argument], 6) + properHex(_opcode, 2);
                }
                objectFile.push_back(_machineCode);
                listingFile.push_back(_machineCode + '\t' + _currline);
                _count++;
                continue;
            }
            if(_currMnemonic == "data"){
                string _argument;
                _temp>>_argument;
                if(!(int)_argument.size()){
                    string _temp = "No operand present at line: ";
                    _temp.append(to_string(originalCodes[i]));
                    if(_currMnemonic == "data"){
                        errors.push_back(_temp);
                    }
                    continue;
                }
                pair<long int, bool> _currOperand = useBaseOfArgument(_argument);
                if (_currOperand.second == 0){
                    string _temp = "Invalid Operand at line: ";
                    _temp.append(to_string(originalCodes[i]));
                    errors.push_back(_temp);
                    continue;
                }
                if (_currOperand.first < INT_MIN || _currOperand.first > INT_MAX) {
                    string _temp = "Possible integer overflow at line: ";
                    _temp.append(to_string(originalCodes[i]));
                    warnings.push_back(_temp);
                }
                string _machineCode = properHex(_count, 4);
                _machineCode.push_back(' ');
                _machineCode.append(properHex(_currOperand.first, 8));
                listingFile.push_back(_machineCode + " " + _completeCode[i]);
                objectFile.push_back(_machineCode);
                _count++;
                continue;
            }
            
            int _tempoo = 0;
            string _argument;
            _temp>>_argument;
            existingLabels.insert(_argument);
            if(to_string(_tempoo)=="0"){
                ;
            }
            if (opcodes[_currMnemonic].second && !(int)_argument.size()){
                string _temp = "Operand absent at position line: ";
                _temp.append(to_string(originalCodes[i]));
                errors.push_back(_temp);
                continue;
            }
            else if(opcodes[_currMnemonic].second == 0 && (int)_argument.size()){
                string _temp = "Invalid or extra operand at line: ";
                _temp.append(to_string(originalCodes[i]));
                errors.push_back(_temp);
                continue;
            }

            if((int)_argument.size() && ((_argument[0] >= 'a' && _argument[0] <= 'z') || (_argument[0] == '_'))){
                if(_setLabels.end() == _setLabels.find(_argument)){
                    string _temp = _argument;
                    _temp.append(" is not present, line: ");
                    _temp.append(to_string(originalCodes[i]));
                    errors.push_back(_temp);
                    continue;
                }
                int _tempoo = 0;
                string code = properHex(_count, 4 + _tempoo) + " " + properHex(_setLabels[_argument], 6+ _tempoo) + properHex(_opcode, 2+ _tempoo);
                listingFile.push_back(code + " " + _completeCode[i]);
                objectFile.push_back(code);
                _count++;
                continue;
            }
            else if((int)_argument.size() && ((_argument[0] >= 'A' and _argument[0] <= 'Z') || _argument[0] == '_')){
                if(_setLabels.end() == _setLabels.find(_argument)){
                    string _temp = _argument;
                    _temp.append(" is not present, line: ");
                    _temp.append(to_string(originalCodes[i]));
                    errors.push_back(_temp);
                    continue;
                }
                int _tempoo = 0;
                string code = properHex(_count, 4 + _tempoo) + " " + properHex(_setLabels[_argument], 6 + _tempoo) + properHex(_opcode, 2 + _tempoo);
                listingFile.push_back(code + " " + _completeCode[i]);
                objectFile.push_back(code);
                _count++;
                continue;
            }

            pair<long, bool> _operandMod = useBaseOfArgument(_argument);
            if (!_operandMod.second) {
                string _temp = "Wrong Operand format at line: ";
                _temp.append(to_string(originalCodes[i]));
                errors.push_back(_temp);
                continue;
            }
            string code = properHex(_count++, 4+_tempoo) + " " + properHex(_operandMod.first, 6+_tempoo) + properHex(_opcode, 2+_tempoo);
            listingFile.push_back(code + " " + _completeCode[i]);
            objectFile.push_back(code);
        }
        else{
            string _nextMnemonic;
            _temp>>_nextMnemonic;
            string _prevMnemonic = _currMnemonic;
            _currMnemonic = _nextMnemonic;
            stringstream chk(_completeCode[i]);
            int k = 0;
            string _tempo;
            int _noOfArguments = 0;
            while (chk>>_tempo){
                ++_noOfArguments;
                _noOfArguments += k;
            }
            if (_noOfArguments >= 4) {
                string _temp = "Extra operands at line: ";
                _temp.append(to_string(originalCodes[i]));
                errors.push_back(_temp);
                continue;
            }
            if (!(int)_currMnemonic.size()) {
                string _temp = properHex(_count, 4+k);
                _temp.push_back(' ');
                _temp.append(_completeCode[i]);
                listingFile.push_back(_temp);
                continue;
            }
            if (_currMnemonic == "SET") {
                string _argument; 
                _temp>>_argument;
                pair<long, bool> _operandMod = useBaseOfArgument(_argument);
                if(!_operandMod.second || !(int)_argument.size()){
                    string _temp = "Operand not present at line: ";
                    _temp.append(to_string(originalCodes[i]));
                    errors.push_back(_temp);
                    continue;
                }
                int _tempoo = 0;
                string code = properHex(_count, 4+_tempoo) + " " + properHex(_operandMod.first, 8+_tempoo);
                objectFile.push_back(code);
                listingFile.push_back(code + " " + _completeCode[i]);
                
                _currMnemonic.pop_back();
                _prevMnemonic.pop_back();
                _count++;
                continue;
            }
            if (_currMnemonic == "data") {
                string operand;
                _temp>>operand;
                if (!(int)operand.size()) {
                    string _temp = "Operand not present at line: ";
                    _temp.append(to_string(originalCodes[i]));
                    errors.push_back(_temp);
                    continue;
                }
                pair<long, bool> _operandMod = useBaseOfArgument(operand);
                if(!_operandMod.second) {
                    string _temp = "Wrong operand format at line: ";
                    _temp.append(to_string(originalCodes[i]));
                    errors.push_back(_temp);
                    continue;
                } 
                if(_operandMod.first < INT_MIN || _operandMod.first > INT_MAX){
                    string _temp = "Possible numeric overflow at line: ";
                    _temp.append(to_string(originalCodes[i]));
                    warnings.push_back(_temp);
                }
                int _tempoo = 0;
                string code = properHex(_count, 4+_tempoo) + " " + properHex(_operandMod.first, 8+_tempoo);
                listingFile.push_back(code + " " + _completeCode[i]);
                objectFile.push_back(code);
                _count++;
                continue;
            }
            int _opcode = getOpcodeFromMnenomic(_currMnemonic);
            if (_opcode == -1) {
                string _temp = "Invalid Mnemonic at line: ";
                _temp.append(to_string(originalCodes[i]));
                if(_opcode == -1)
                    errors.push_back(_temp);
                continue;
            }
            if (opcodes[_currMnemonic].second == 2) {
                string _argument;
                _temp>>_argument;
                existingLabels.insert(_argument);

                if (_setLabels.find(_argument) == _setLabels.end()) {
                    string _temp = _argument;
                    _temp.append("Label is not present at line: ");
                    _temp.append(to_string(originalCodes[i]));
                    errors.push_back(_temp);
                    continue;
                }
                int _tempoo = 0;
                string code = properHex(_count, 4+_tempoo) + " " + properHex(_setLabels[_argument] - _count - 1, 6+_tempoo) + properHex(_opcode, 2+_tempoo);
                if (_currMnemonic == "call") {
                    code = properHex(_count, 4+_tempoo) + " " + properHex(_setLabels[_argument], 6+_tempoo) + properHex(_opcode, 2+_tempoo);
                }
                objectFile.push_back(code);
                listingFile.push_back(code + " " + _completeCode[i]);
                _count++;
                continue;
            }
            string _argument;
            _temp>>_argument;
            existingLabels.insert(_argument);

            if(opcodes[_currMnemonic].second && !(int)_argument.size()){
                string _temp = "Operand not present at line: ";
                _temp.append(to_string(originalCodes[i]));
                errors.push_back(_temp);
                continue;
            }
            else if(opcodes[_currMnemonic].second == 0 && (int)_argument.size()) {
                string _temp = "Invalid operand at line: ";
                _temp.append(to_string(originalCodes[i]));
                errors.push_back(_temp);
                continue;
            }
            if ((int)_argument.size() && ((_argument[0] >= 'a' && _argument[0] <= 'z') || _argument[0] == ' ')){
                if (_setLabels.end() == _setLabels.find(_argument)){
                    string _temp = _argument;
                    _temp.append(" is not present at line: ");
                    _temp.append(to_string(originalCodes[i]));
                    errors.push_back(_temp);
                    continue;
                }
                int _tempoo = 0;
                string code = properHex(_count, 4+_tempoo) + " " + properHex(_setLabels[_argument], 6+_tempoo) + properHex(_opcode, 2+_tempoo);
                objectFile.push_back(code);
                listingFile.push_back(code + " " + _completeCode[i]);
                
                _count++;
                continue;
            }
            else if((int)_argument.size() && ((_argument[0] >= 'A' && _argument[0] <= 'Z') || _argument[0] == '_')){
                if(_setLabels.end() == _setLabels.find(_argument)) {
                    string _temp = _argument;
                    _temp.append(" is not present at line: ");
                    _temp.append(to_string(originalCodes[i]));
                    errors.push_back(_temp);
                    continue;
                }
                int _tempoo = 0;
                string code = properHex(_count, 4+_tempoo) + " " + properHex(_setLabels[_argument], 6+_tempoo) + properHex(_opcode, 2+_tempoo);
                objectFile.push_back(code);
                listingFile.push_back(code + " " + _completeCode[i]);
                
                _count++;
                continue;
            }
            pair<long, bool> _operandMod = useBaseOfArgument(_argument);
            if (_operandMod.second == 0) {
                string _temp = "Wrond Operand format at line: ";
                _temp.append(to_string(originalCodes[i]));
                errors.push_back(_temp);
                continue;
            }
            int _tempoo = 0;
            string code = properHex(_count, 4+_tempoo) + " " + properHex(_operandMod.first, 6+_tempoo) + properHex(_opcode, 2+_tempoo);
            objectFile.push_back(code);
            listingFile.push_back(code + " " + _completeCode[i]);
            _count++;
        }
    }
    string _inputFileName;
    for (int i = 0; i < (int)strlen(argv[1]); ++i){
        if(argv[1][i] != '.'){
            _inputFileName.push_back(argv[1][i]);
        }
        else{
            break;
        }
    }
    vector<string> _errorsBackup = errors;
    string _logFileName = _inputFileName;
    _logFileName.append(".log");
    ofstream _tempFile;
    _tempFile.open(_logFileName);
    for (auto itr : _setLabels) {
        if (existingLabels.count(itr.first) == 0) {
            errors.push_back("Warning: label \"" + itr.first + "\" not used");
        }
    }
    for (auto itr : warnings) {
        errors.push_back("Warning: " + itr);
    }
    for (auto itr : errors) {
        _tempFile<<itr<<endl;
    }
    _tempFile.close();
    if ((int)_errorsBackup.size()){
        cout<<"Compilation errors encountered. Log file with errors and warnings: ";
        cout<<_logFileName<<endl;
        cout<<"Errors:\n";
        for (auto itr : errors) {
            cout<<itr<<endl;
        }
        return 0;
    }
    else{
        cout<<"Compilation Successful. Log file with warnings: ";
        cout<<_logFileName<<endl;
    }
    string _listFileName = _inputFileName;
    _listFileName.append(".l");
    _tempFile.open(_listFileName);
    for (auto itr : listingFile) {
        _tempFile<<itr<<endl;
    }
    _tempFile.close();
    cout<<"The listing file is: "<<_listFileName<<endl;
    string _objectFileName = _inputFileName;
    _objectFileName.append(".o");
    _tempFile.open(_objectFileName, ios::binary | ios::out);
    for (auto it : objectFile) {
        string s1, s2;
        stringstream _tempo(it);
        _tempo>>s1>>s2;
        int num = 0;
        reverse(s2.rbegin(), s2.rend());
        for (int i = 0; i <= 7; ++i) {
            int val = 0;
            if (s2[i] >= 'a' && s2[i] <= 'f') {
                val = (s2[i]-'a' + 10);
            }
            else{
                val = s2[i]-'0';
            }
            num += val*(int)pow(16, i);
        }
        static_cast<int> (num);
        _tempFile.write((const char*)&num, sizeof(int));
    }
    _tempFile.close();
    cout<<"The object file is: "<<_objectFileName<<endl;
    cout<<"\nWarnings: \n";
    for (auto itr : errors) {
        _tempFile<<itr<<endl;
        cout<<itr<<endl;
    }
    return 0;
}