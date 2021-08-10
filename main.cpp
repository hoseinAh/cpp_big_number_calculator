#include<iostream>
#include<math.h>
#include<fstream>
#include<string>
#include "main.h"
#include "add_minus.h"
#include "power_multiply.h"
#include "divide_remainder.h"
using namespace std;

void find_nearest_root(number *a,number *c) // 15234 : 1 52 34 ->2 0 0   652468 : 65 24 68 -> 9 0 0
{   /* first we need to  find how many zeros we want ,then find the (square +1) of last one or two digits by while(temp>100 ) or while(temp>10)*/
    int i=0; //number of zeros
    long long int temp=((*a).num)[(*a).volume-1];

    if((*a).len%2==0)
        {
            i=(*a).len/2;
            i--;// teadad zero ha
            while(temp>100)
                temp/=10;
        }
    else
        {
            i=(*a).len/2;
            while(temp>10)
                temp/=10;
        }

    temp=sqrt(temp)+1;

    string A;
    if(temp==10) //if temp == 10  we cant A+= 10+48 because it is another character not "10"
        A+="10";
    else
        A+=temp+48;
    for(int j=0;j<i;j++)
        A+='0';

    str2number(A,c);
}
void divide_by_two(number *a)
{   /* for dividing by two we start from the first digit and then the digits after that for example:
        52134: 5 2 1 3 4
               5 2 1 3 2
               5 2 1 1 2+5*1
               5 2 0 1+5*1 7
               5 1 0+5*0 6 7
               2 1+5*1 0 6 7
               2 6 0 6 7    , in here we do that 8digits by 8digits */
    ((*a).num)[0]/=2;
    for(int i=1;i<(*a).volume;i++)
        {
            ((*a).num)[i-1]+=50000000*(((*a).num)[i]%2);
            ((*a).num)[i]/=2;
        }
    decrease_volume(a);
}

void square(number *a,number *c)
{/* the square function is based on Babylonian method  see wikipedia.org/wiki/Methods_of_computing_square_roots
    at first we find the nearest root to input 'a' which is founded by find_nearest_root() then we do the square algorithm : X=sqrt(a)
        do{
        X=0.5( X + a/X);
        }while( X^2 > a);
         1: A=a/x , 2: B=x+A , 3: B/=2 , 4: x=B , 5 : x^2 <= a
        all  arithmetic operations are based on integers not floating numbers because the nearest root is integer
        due to the ability of functions to work with floating numbers, we can calculate the digits after decimal point, but we avoid it.*/
    if(is_negative(a))
        (*c).error=18;//18:= square input negative

    else
        {
            (*a).point=false;
            (*a).volume_float=0;
            number *near_root=new number;
            bool _continue =true;
            find_nearest_root(a,c);//cout<<"fisrt X";print (c);
            do{
                number *A=new number;
                divide(a,c,A,0);

                number *B=new number;
                add(A,c,B);//B=A+c

                divide_by_two(B);// B/=2

                swap_number(c,B); //c=B

                number *C = new number;
                multi(c,c,C);//C=c^2  c*c
                delete [](*A).num;
                delete A;
                delete [](*B).num;
                delete B;

                if(_less_eqale(C,a))
                    _continue= false;
                delete [](*C).num;
                delete C;
            }while(_continue);
        }
}
int main()
{
    string input_file_name;
    cout<<"enter the input file name without the extension: ";
    cin>>input_file_name;
    ifstream input;

    input.open(input_file_name+".in");
    if(!input.is_open())
        {
            cout<<"input file error ,is_not_open" ;
            return 0;
        }
    ofstream fp;
    fp.open(input_file_name+".out");
    int Statement_number=1;
    int range;
    cout<<"how many digits after the decimal point, enter zero for default 20(this is the extra feature): ";
    cin >>range;
    if(range ==0)
        range=20;

    while(true)
        {
            string _input;// every line from input file gets to this string
            getline(input,_input);
            _input=remove_spaces(_input); //removing ' ' from the string   : "12 + 43  =" -> "12+43="
            if(_input[_input.length()-1]=='=')// erasing the '=', this is added because the "EXIT" doesnt have it
                _input.erase(_input.length()-1);
            if(_input=="EXIT")
                break;
            cout<<"Statement "<<Statement_number;
            fp<<"------------ Statement "<<Statement_number++<<" ------------"<<endl;
            number *A=new number;
            number *B=new number;
            number *C=new number;
            //input processing starts from here, it only detect syntax errors like "12+=", math errors are detected inside the arithmetic functions
            if(sqrt_wanted(_input))
                {
                    fp<<_input<<endl;
                    fp<<"=\n";
                    _input.erase(0,5);  // turning "sqrt(512)" to "512"
                    _input.erase(_input.length()-1);
                    str2number(_input,A);
                    square(A,C);
                    print(C,fp,range);
                }
            else if(!chek_extra_char(_input))// "12a +32f=" // expected characters are numbers , 6 defined operator and '.'
                {
                    fp<<_input<<"=\n input error, extra char\n\n";
                }
            else if (!two_num_and_one_op(_input))
                {
                    fp<<_input<<"=\n input error, two number and one operator not found\n\n";
                }
            else if (op_misuse(_input))
                {
                    fp<<_input<<"=\n input error, operator misuse\n\n";
                }
            else
                {
                    string a,b;
                    char op;
                    _parse_input(_input,a,op,b);

                    str2number(a,A);
                    str2number(b,B);
                  //  count_digits(A);

                    if(op=='+')
                        add(A,B,C);
                    else if(op=='-')
                        minuss(A,B,C);
                    else if(op=='*')
                        multi(A,B,C);
                    else if(op=='/')
                        divide(A,B,C,range);
                    else if(op=='^')
                        power(A,B,C);
                    else if(op=='%')
                        find_remainder(A,B,C);

                    fp<<a<<endl;
                    fp<<op<<endl;
                    fp<<b<<endl;
                    fp<<'='<<endl;
                    print(C,fp,range);
                }
            //print(C,fp,range);
            cout<<" done\n";
            if((*A).volume)
                delete []((*A).num);
            if((*A).point)
                delete []((*A)._float);
            delete A;

            if((*B).volume)
                delete []((*B).num);
            if((*B).point)
                delete []((*B)._float);
            delete B;

            if((*C).volume)
                delete []((*C).num);
            if((*C).point)
                delete []((*C)._float);
            delete C;
        }
    fp.close();
    input.close();
    cout<<"finished";
}
