#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>

using namespace std;


enum class TokenType
{
    _return ,
    int_lit,
    semi
};


struct Token
{
    TokenType type;
    optional<string> value{};
};

//*********************************************************************************** */

vector<Token> tokenize(const string& str)
{
    vector<Token> tokens;
    string buf;  

    for(int i=0; i < str.length() ; i++)// all the raw of data OR all data in the file
    {
        char c = str.at(i);//Access the character at index i of the string str, with bounds checking.

        if(isalpha(c))// if alpha , it should be start with alpha
        {
            buf.push_back(c);
            i++;

            while(isalnum(str.at(i)))// if alpha or num
            {
                buf.push_back(str.at(i));
                i++;
            }

            i--;// if he spot a space 

            if(buf=="return")
            {
                tokens.push_back({.type=TokenType::_return});
                buf.clear();
                continue;//try to remove it !!!!    
            }

            else 
            {
                cerr<<"there are an error in 54 lines"<<endl;
                exit(EXIT_FAILURE); 
            }
       }
        else if(isdigit(c))
        {
            buf.push_back(c);
            i++;

            while(isdigit(str.at(i)))
            {
                buf.push_back(str.at(i));
                i++;
            }

            i--;
            tokens.push_back({.type=TokenType::int_lit , .value=buf});
            buf.clear();

        }
        else if(c==';')
        {
            tokens.push_back({.type=TokenType::semi});
            //buf.clear();
        }

        else if(isspace(c)) 
        {
            continue; // ignore the space
        }

        else 
        {
            cerr<<"fall in 86 line"<<endl;
            exit(EXIT_FAILURE);
        }


    }
    return tokens;
}



string tokens_to_asm(const vector<Token>& tokens)
{

    stringstream output;

    output<<"global _start\n_strat\n";

    for(int i=0;i<tokens.size();i++)
    {
        const Token& token=tokens.at(i);

        if(token.type==TokenType::_return)
        {
            if(i+1 < tokens.size() && tokens.at(i+1).type ==TokenType::int_lit)
                if(i+2 < tokens.size() && tokens.at(i+2).type ==TokenType::semi)
                {
                    output<<"   mov rax, 60\n";
                    output<<"   mov rdi, "<<tokens.at(i+1).value.value()<<"\n";
                    output<<"   syscall";
                }
        }
    }
    return output.str();
}

int main(int argc , char* argv[])
{ //It’s used to receive command-line arguments passed when you run a program. 
    
    

    
    string content ;
    {
        //string content ; if you put it here will give error undefined that mean the {} are close everthing and do file close
        stringstream contents_stream;

        fstream input(argv[1] , ios::in);//input is a varible ,
        
        contents_stream << input.rdbuf();//This line copies the entire file contents into contents_stream, which is typically an ostringstream.
        
        content = contents_stream.str();


    }

    vector<Token> the_file=tokenize(content);


    //cout<<tokens_to_asm(the_file)<<endl;

    {
        fstream objFile("obj.asm",ios::out);
        objFile<<tokens_to_asm(the_file);
    }

    system("nasm -felf64 obj.asm");
    system("ld -o out obj.o");


    return 0;
}
