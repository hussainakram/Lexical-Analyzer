//
//  main.cpp
//  lexical analyzer
//
//  Created by Hussain Akram on 4/3/18.
//  Copyright © 2018 Hussain Akram. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <map>
#include <string>
using namespace std;

fstream inputFile("inputfile.txt");

struct Token
{
    int id;
    string value;
};

char getNextChar()
{
    char input;
    inputFile.get(input);
    return input;
}

Token isKeyword(string s)
{
    Token foundToken;
    typedef map<string,int> StringIntMap;
    int j = 0;
    StringIntMap keywords;
    string keyword_list[] = {"begin", "end", "if", "then", "else", "while", "do", "not", "and", "div", "or", "mod", "program", "var", "array", "of", "integer", "real", "function", "procedure"};
    for (int i = 109; i < 129; i++) {
        keywords[keyword_list[j]] = i;
        j++;
    }
    
    auto it = keywords.find(s);
    if (it != keywords.end()) {
        foundToken.value = it->first;
        foundToken.id = it->second;
    }
    else
    {
        foundToken.id = 101;
        foundToken.value = s;
    }
    return foundToken;
}

Token generateToken()
{
    Token t;
    char c;
    int state = 0;
    //for accepting comments
    while(1){
        switch(state)
        {
            case 0:
                c = getNextChar();
                if (c == '{') {
                    state = 1;
                }
                else if (c == '+')
                {
                    t.id = 106;
                    t.value = '+';
                    return t;
                }
                else if (c == '-')
                {
                    t.id = 106;
                    t.value = '-';
                    return t;
                }
                else if (c == ':')
                {
                    state = 5;
                }
                else if (c == '*')
                {
                    t.id = 107;
                    t.value = '*';
                    return t;
                }
                else if (c == '/')
                {
                    t.id = 107;
                    t.value = '/';
                    return t;
                }
                else if (c == '>')
                {
                    state = 6;
                }
                else if (c == '<')
                {
                    state = 7;
                }
                else if (c == '=')
                {
                    t.id = 105;
                    t.value = "ET";
                    return t;
                }
                else if (isdigit(c))
                {
                    state = 8;
                    t.value += c;
                }
                else if (isalpha(c))
                {
                    t.value += c;
                    state = 13;
                }
                else if(iswspace(c)){
                    t.id =-1;
                    return t;
                }
                else{
                    t.id =404;
                    return t;
                }
                break;
                
            case 1:
                c = getNextChar();
                if (c != '*' && c != '}') {
                    state =2;
                }
                else if (c == '}')
                {
                    state = 0;
                }
                else if (c == '*')
                {
                    state = 3;
                }
                break;
            case 2:
                c = getNextChar();
                while (c != '}') {
                    if (c == inputFile.eof()) {
                        exit(0);
                    }
                    c = getNextChar();
                }
                state = 0;
                break;
            case 3:
                c = getNextChar();
                while (c != '*') {
                    if (c == inputFile.eof()) {
                        exit(0);
                    }
                    c = getNextChar();
                }
                state = 4;
                break;
            case 4:
                c = getNextChar();
                if (c != '}') {
                    state = 3;
                }
                else
                {
                    state = 0;
                }
                break;
            case 5:
                c = getNextChar();
                if(c == '=')
                {
                    t.id = 108;
                    t.value = "ASSIGNOP";
                    return t;
                }
                break;
            case 6:
                c = inputFile.peek();
                
                if (c == '=')
                {
                    getNextChar();
                    t.value = 105;
                    t.value = "GE";
                    return t;
                }
                else
                {
                    t.id = 105;
                    t.value = "GT";
                    return t;
                }
                break;
            case 7:
                c = inputFile.peek();
                if(isalnum(c))
                {
                    t.id = 105;
                    t.value = "LT";
                    return t;
                }
                else if (c == '=')
                {
                    getNextChar();
                    t.id = 105;
                    t.value = "LE";
                    return t;
                }
                else if (c == '>')
                {
                    getNextChar();
                    t.id = 105;
                    t.value = "NE";
                    return t;
                }
                break;
            case 8:
                c = inputFile.peek();
                while (isdigit(c)) {
                    t.value += inputFile.get();
                    c = inputFile.peek();
                }
                
                if (c == '.')
                {
                    state = 9;
                    t.value += getNextChar();
                }
                else if (c=='E' || c=='e')
                {
                    state = 11;
                    t.value +=getNextChar();
                }
                else{
                    t.id=102;
                    return t;
                }
                break;
                
            case 9:
                c=getNextChar();
                if(isdigit(c)){
                    state = 10;
                    t.value +=c;
                }
                
                break;
                
            case 10:
                c= inputFile.peek();
                while(isdigit(c)){
                    t.value += getNextChar();
                    c=inputFile.peek();
                }
                if (c=='E' || c=='e'){
                    t.value+= getNextChar();
                    state= 11;
                }
                else{
                    t.id=103;
                    return t;
                }
                break;
            case 11:
                c = getNextChar();
                if(c=='+' || c=='-'){
                    t.value+=c;
                    state=12;
                }
                else if(isdigit(c)){
                    t.value+=c;
                    state=14;
                }
                break;
            case 12:
                c=getNextChar();
                if(isdigit(c)){
                    t.value+=c;
                    state=14;
                }
                break;
            case 13:
                c = getNextChar();
                while (isalnum(c))
                {
                    t.value += c;
                    c = getNextChar();
                }
                t = isKeyword(t.value);
                return t;
                break;
            case 14:
                c=inputFile.peek();
                while(isdigit(c)){
                    t.value+=c;
                    c= getNextChar();
                    
                }
                t.id=104;
                return t;
                break;
        }
    }
    // return t;
}
int main() {
    while(1)
    {
        while(iswspace(inputFile.peek()))
            getNextChar();
        if(!inputFile.eof()){
            
            Token token= generateToken();
            if (token.id == 101)
                cout << "Identifier\t\t" << token.value << "\n";
            else if (token.id >= 109 && token.id < 129)
                cout << "Keyword\t\t" << token.value << "\n";
            else if(token.id == 102)
                cout << "Int\t\t" << token.value << "\n";
            else if(token.id==103)
                cout << "Real\t\t" << token.value << "\n";
            else if(token.id==104)
                cout << "Exponential\t\t" << token.value << "\n";
            else if(token.id == 105)
                cout << "RELOP\t\t" << token.value << "\n";
            else if(token.id==106)
                cout << "ADD_OP \t\t" << token.value << "\n";
            else if(token.id==107)
                cout << "MUL_OP\t\t" << token.value << "\n";
            else if(token.id==108)
                cout << "Assignment operator\t\t" << token.value << "\n";
            else if(token.id==-1)
                cout << "";
            else
                cout << "invalid token" << endl;
        }
        else
            break;
    }
    
    return 0;
}
