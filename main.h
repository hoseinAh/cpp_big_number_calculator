#include<iostream>
#include<math.h>
#include<fstream>
#include<string>
using namespace std;


struct number{   //14695845682657759.448
    long long int *num;  //{82567759,46958456,1}
    short volume=0;//3
    short len=0;//number of digits  17
    bool point=0;//agar ashar dashtim = true  true
    short *_float;//{4,4,8}
    short volume_float=0;//number of digits after '.'   3
    char error=0;//zero := OK// using char to reduce memory
};
/* for dividing and remainder we have to convert the numbers to one-digit one-digit form */
struct number8bit{
    short *num; //2 byte for every digit
    short volume=0;//number of digits ,volume acts as "len"
};
void count_digits(number *a);
void decrease_volume(number *a);//index with zero value at the end of number cost us more operation  so by reducing volume we save time specially in power operation
void decrease_float(number *a);
void str2number(string a,number *A)// converting string to number structure
{
    int x=0;
    //sign---------------
    long long int sign = 1;
    if(a[0]=='+')
        x=1;
    else if(a[0]=='-')
        {
            sign = -1;
            x=1;
        }
    else{}
    //radix-----------------
    int radix_point=0;
    for(int i=0;i<a.length();i++)
    {
        if(a[i]=='.')
            {
                (*A).point=true;
                break;
            }
        radix_point++;
    }//i is the location of '.' in string a
    if((*A).point)
        {
            (*A).volume_float=a.length()-1-radix_point;
            (*A)._float=new short[(*A).volume_float];
            for(int i=0;i<(*A).volume_float;i++)
                ((*A)._float)[i]=sign*(a[i+1+radix_point]-48);
            decrease_float(A);
        }
    (*A).len=(radix_point-x);
    (*A).volume = ceil((radix_point-x)/8.0);
    (*A).num= new long long int[(*A).volume] ;
    for(int i=0;i<(*A).volume;i++)
        ((*A).num)[i]=0;
    long long int t=1;
    for(int j=0,i=radix_point-1;i>-1+x;i--)
    {
        if(t==100000000)
            t=1;
        ((*A).num)[j++/8]+=t*(a[i]-48)*sign;
        t*=10;
    }
    decrease_volume(A);
    count_digits(A);
}

void print(number *A,ofstream &fp,int range)
{
    if((*A).error)
        {
             switch ((*A).error)
                {
                    case 12:
                        fp<<" power is not integer";
                        break;
                    case 13:
                        fp<<" negative power cant be used";
                        break;
                    case 14:
                        fp<<" very very big number" ;
                        break;
                    case 15:
                        fp<<" divisor zero";
                        break;
                    case 16:
                        fp<<" remainder input negative";
                        break;
                    case 17:
                        fp<<" input not integer";
                        break;
                    case 18:
                        fp<<" square input negative";
                        break;
                    default :
                         fp<<" unknown error";
                }
        }
    else
        {
            int i=(*A).volume;
            i--;
            int sing=1;//

            if((*A).num[i]<0 )//چاپ کردن عدد منفی
                {
                    fp<<'-';
                    sing=-1;
                }
            else if((*A).point)
                if(((*A)._float)[(*A).volume_float-1]<0)
                    {
                        fp<<'-';
                        sing=-1;
                    }
            if(i<0)
                    fp<<'0';
            else
                {   /* sometime the num is {1,1} which is 1,00000001 the below part handle it*/
                    fp<<sing*((*A).num)[i--];
                    for(int t=0;i>=0;i--)
                        {
                            t=  ((*A).num)[i]*sing;
                            if(t<10)
                                fp<<"0000000"<<t;
                            else if(t<100)
                                fp<<"000000"<<t;
                            else if(t<1000)
                                fp<<"00000"<<t;
                            else if(t<10000)
                                fp<<"0000"<<t;
                            else if(t<100000)
                                fp<<"000"<<t;
                            else if(t<1000000)
                                fp<<"00"<<t;
                            else if(t<10000000)
                                fp<<"0"<<t;
                            else
                                fp<<t;
                        }
                }
            if((*A).point)  //if float part exits this part show it
                {   if(range<(*A).volume_float)
                    (*A).volume_float = range;
                    decrease_float(A);
                    fp<<'.';

                    for(int i=0;i<(*A).volume_float;i++)
                        fp<<sing*((*A)._float)[i];
                }
        }
    fp<<endl<<endl;
}

bool is_negative(number *a) // a<0
{
    if( ((*a).num)[(*a).volume-1]<0 )
        return true;
    else if((*a).point)
        if(((*a)._float)[(*a).volume_float-1]<0)
            return true;

    return false;
}

void opposite_maker(number *a)//a=*-1;
{
    for(int i=0;i<(*a).volume;i++)
        ((*a).num)[i]*=-1;
    if((*a).point)
        for(int i=0;i<(*a).volume_float;i++)
        ((*a)._float)[i]*=-1;
}

void fix(number *a);
void nsort(number *a)
{    /*in some situations   100000002 - 8  ->{-6,1}
                output is printed 10000000-6
        it is solved by nsort negative sort*/
    int n=((*a).volume)-1;
    while(((*a).num)[n]==0)n--;
    for(int i=n;i>0;i--)
        {
            if(((*a).num)[i]>0)
                {
                    ((*a).num)[i]-=1;
                    ((*a).num)[i-1]+=100000000;
                }
            else if(((*a).num)[i]<0)
                {
                    ((*a).num)[i]+=1;
                    ((*a).num)[i-1]-=100000000;
                }
        }
    fix(a);
}
void fix(number *a)
{   //in some situations {100000012,0} -> {12,1}
    for(int i=0 ;i<((*a).volume)-1;i++)
        {
            ((*a).num)[i+1]+=((*a).num)[i]/100000000;
            ((*a).num)[i]=((*a).num)[i]%100000000;
        }
}

void fix(number8bit *a);
void nsort(number8bit *a)
{
    int n=((*a).volume)-1;
    while(((*a).num)[n]==0)n--;
    for(int i=n;i>0;i--)
        {
            if(((*a).num)[i]>0)
                {
                    ((*a).num)[i]-=1;
                    ((*a).num)[i-1]+=10;
                }
            else if(((*a).num)[i]<0)
                {
                    ((*a).num)[i]+=1;
                    ((*a).num)[i-1]-=10;
                }
        }
    fix(a);
}
void fix(number8bit *a)
{
    for(int i=0 ;i<((*a).volume)-1;i++)
        {
            ((*a).num)[i+1]+=((*a).num)[i]/10;
            ((*a).num)[i]=((*a).num)[i]%10;
        }
}

void nsort_float(number *c)//nsort for _float
{
    int j=(*c).volume-1;
    while(((*c).num)[j]==0 && j!=0)
        j--;
    if(((*c).num)[j]>0)
        {
            ((*c).num)[0]-=1;
            ((*c)._float)[0]+=10;
        }
    if(((*c).num)[j]<0)
        {
            ((*c).num)[0]+=1;
            ((*c)._float)[0]-=10;
        }
    for(int i=0;i<(*c).volume_float-1;i++)
        {
            if(((*c)._float)[i]>0)
                {
                    ((*c)._float)[i]-=1;
                    ((*c)._float)[i+1]+=10;
                }
            else if(((*c)._float)[i]<0)
                {
                    ((*c)._float)[i]+=1;
                    ((*c)._float)[i+1]-=10;
                }
        }
}
void fix_float(number*c)
{
    for(int i=(*c).volume_float-1;i>0;i--)
        {
            ((*c)._float)[i-1]+=((*c)._float)[i]/10;
            ((*c)._float)[i]=((*c)._float)[i]%10;
        }
    ((*c).num)[0]+=((*c)._float)[0]/10;
    ((*c)._float)[0]=((*c)._float)[0]%10;
}

void decrease_volume(number *a)//index with zero value at the end of number cost us more operation  so by reducing volume we save time specially in power operation
{
      while( ((*a).num)[((*a).volume)-1]==0 && (*a).volume!=1)
            (*a).volume--;
}
void decrease_volume(number8bit *a)//index with zero value at the end of number cost us more operation  so by reducing volume we save time specially in power operation
{
      while( ((*a).num)[((*a).volume)-1]  ==  0  && ((*a).volume)!=1 )
            (*a).volume--;
}
void decrease_float(number * a)
{
    while(((*a)._float)[(*a).volume_float-1]==0 && (*a).volume_float!=1 )
        {(*a).volume_float--;}
}

void count_digits(number *a)//must be used after void decrease_volume
{

    if( (*a).volume ==1  )
        if(((*a).num)[0]==0)
            (*a).len=1;
    else
        {
           if(((*a).num)[(*a).volume -1] > 0)
               {
                    (*a).len= log10(((*a).num)[  ((*a).volume) - 1])+1;
                    (*a).len+=8*(((*a).volume) - 1);
               }
           else
               {
                   //opposite_maker(a);
                    (*a).len= log10( -1*((*a).num)[ ((*a).volume) - 1] )+1;// -1* does the opposite_maker's job
                    (*a).len+=8*(((*a).volume) - 1);
                    //opposite_maker(a);
               }
        }
}

void reduce_number_volume(number *a)// instead of changing volume value , we allocate new and less memory and delete the last one
{
    int needed_volume=(*a).volume;
    long long int *A=new long long int[needed_volume];
    for(int i=0;i<needed_volume;i++)
        A[i]=((*a).num)[i];
    delete [] (*a).num;
    (*a).num=A;
}
void reduce_number_float(number *a)// instead of changing volume_float value , we allocate new and less memory and delete the last one
{
    int needed_volume=(*a).volume_float;
    short *A=new short[needed_volume];
    for(int i=0;i<needed_volume;i++)
        A[i]=((*a)._float)[i];
    delete [] (*a)._float;
    (*a)._float=A;
}

void  swap_number(number *a, number *b)  // (volume) volume must be eqaule//   a=b  turn to "asign"
{
      (*a).len=(*b).len;
      (*a).volume=(*b).volume;
      for(int i=0;i<(*b).volume;i++)
              ((*a).num)[i]=((*b).num)[i];

      if((*b).point)
          {
              (*a).point =true;
              (*a).volume_float=(*b).volume_float;
               for(int i=0;i<(*b).volume_float;i++)
                    ((*a)._float)[i]=((*b)._float)[i];
          }
}

bool _less_eqale(number8bit *a,int q,number8bit *b)//(a*q)<=b
{       /*remainder and _less_eqale(B,q,acopy) functions are written again ,
        last time we declared number *A which may caused memory violation(error code 0x0374) and memory could not get free ,
         but now just declare a simple pointer array and a short int*/
    short *A=new short[(*a).volume+1];
    short Avolume=(*a).volume+1;// now real A.volume is equal to (*b).volume
    for(int i=0;i<Avolume;i++)
        A[i]=0;
    for(int i=0;i<Avolume-1;i++)
        A[i]=(q*((*a).num)[i]);
    for(int i=0;i<Avolume-1;i++)//A=a*q
        {
            A[i+1]+=A[i]/10;
            A[i]=A[i]%10;
        } // we can merge these two last loops together , but it may corrupt the product of multiply
    if(A[Avolume-1]==0)//a*q may has one digit more or may not, Avolume--  reduce
        Avolume--;

    if(Avolume>(*b).volume)
        {
            delete []A;
            return false ;
        }
    else if (Avolume<(*b).volume)
        {
            delete []A;
            return true ;
        }
    else
        for(int i=(*b).volume-1 ;i>-1;i--)
            {
                if(((*b).num)[i] >A[i])
                    {
                        delete []A;
                        return true ;
                    }

                else if(((*b).num)[i] <A[i])
                    {
                        delete []A;
                        return false ;
                    }
            }
     delete []A;
     return true;
}

bool _less_eqale(number *a,number *b)//a<=b
{
    if((*b).len > (*a).len)//
        return true;
    else if((*b).len < (*a).len)
        return false;
    if((*a).point || (*b).point)
        {

            if((*b).volume_float <(*a).volume_float)
                 return true;
            else if((*b).volume_float >(*a).volume_float)
                return false;
        }
    else// lens and volume_floats   equal
        {
            for(long long int i= (*b).volume -1;i>-1;i--  )
                {
                    if(((*b).num)[i] >((*a).num)[i])
                        return true;
                    else if(((*b).num)[i] <((*a).num)[i])
                        return false;
                }
            if((*a).point || (*b).point)
                {
                    for(long long int i= 0;i<(*b).volume_float;i++ )
                        {
                            if(((*b).num)[i] >((*a).num)[i])
                                return true;
                            else if(((*b).num)[i] <((*a).num)[i])
                                return false;
                        }
                }
        }
    return true;
}

bool _less(number *a,number *b)//a<b  just comparing with length of number then the value
{
    if((*b).len > (*a).len)// volume is equal
        return true;
    else if((*b).len < (*a).len)
        return false;
    else// len is equal
        {
            for(long long int i= (*b).volume -1;i>-1;i--  )
                {
                    if(((*b).num)[i] >((*a).num)[i])
                        return true;
                    else if(((*b).num)[i] <((*a).num)[i])
                        return false;
                }
        }
    return false;
}

bool is_zero(number8bit *a)
{
    if((*a).volume==1)
        if(((*a).num)[0]==0)
            return true;
    return false;
}
bool is_zero(number *a)
{
    if((*a).point)
        {
            for(int i=0;i<(*a).volume_float;i++)
                if(((*a)._float)[i]!=0)
                    return false;
        }
    if((*a).volume!=1)
        return false;
    if(((*a).num)[0]!=0)
        return false;
    return true;
}
bool is_one(number *a)
{
    if((*a).point)
        {
            for(int i=0;i<(*a).volume_float;i++)
                if(((*a)._float)[i]!=0)
                    return false;
        }
    if((*a).volume!=1)
        return false;
    if(((*a).num)[0]!=1)// in this line is_one differed from is_zero
        return false;
    return true;
}

void _64to8bit(number *a,number8bit *b); //turning 12.5202 to 125202
void _8to64bit(number8bit *a,number *b); //{0,1,3,4,2,5} ->{543210,0}
void remainder(number8bit *a,number8bit *b ,int q); // a = a-b*q

void reverseStr(string &str);
string fix_string(string a, int position);

// input processing functions.---------------------------------------------------------------
// the math error handled at the function;
string remove_spaces(string input)  //  "12+ 23    =" -> "12+32="
{
    int i=0;
    for(i=0;i<input.length();)
        if(input[i]==' '||input[i]=='\n')
            input.erase(i+input.begin());
        else
            i++;
    return input;
}
bool is_operator(char a)
{
    if(a=='+')
        return true;
    else if(a=='-')
        return true;
    else if(a=='/')
        return true;
    else if(a=='%')
        return true;
    else if(a=='^')
        return true;
    else if(a=='*')
        return true;

    return false;
}
bool sqrt_wanted(string a)// check the input to see the "sqrt" is used , so it can find the root
{
    if(a.find("sqrt") != std::string::npos)
        return true;
    return false;
}
string give_a(string &a) // the function store the first number into string B and erase from string input, so the first character from string input is the operator
{
    string b="";
    int i;
    b+=a[0];
    for( i=1;!is_operator(a[i]);i++)
        b+=a[i];
    a.erase(0,i);//erasing the first number from input string
    return b;
}
char give_op(string &a)
{
    char op=a[0];
    a.erase(a.begin());//erasing the operator from input string
    return op;
}
void _parse_input(string input,string &a,char &op,string &b)
{
    input=remove_spaces(input);
    a=give_a(input);
    op=give_op(input);
    // after removing the first number and the operator from string input , only the second number left in it
    // "-12+-43"->"+-43"->"-43"
    b=input;
}

bool chek_extra_char(string input)
{
     for(int i=0;i<input.length();i++)
        {
            if(!(input[i]<58 && input[i]>47))
                if(!is_operator(input[i]))
                    if(input[i]!='.')// '.' cant be in is_operator function
                        return false;
        }
    return true;
}
bool op_misuse(string input)//operator misuse
{
    if(input[0]=='*'||input[0]=='/'||input[0]=='%'||input[0]=='^'||input[0]=='.')
        return true;

    for(int i=0;i<input.length();i++)
        {
            if(input [i]=='.')
                {
                    if(!(input[i+1]>47 && input[i+1]<58))
                        return true;
                    if(!(input[i-1]>47 && input[i-1]<58))
                        return true;
                }
            else if(input[i]=='*'||input[i]=='/'||input[i]=='%'||input[i]=='^'||input[i]=='+'||input[i]=='-')
                {
                    if(!(input[i+1]=='-'||input[i+1]=='+'))
                        if(!(input[i+1]>47 && input[i+1]<58))
                            return true;
                }
        }
    return false;
}
bool two_num_and_one_op(string input)
{
    bool op= false;
    bool a=false;
    bool b=false;
    int i=0;
    if(input[0]=='+' || input[0]=='-')
    {i++;a=true;}
    for(;i<input.length();i++)
         if(!(input[i]>47 && input[i]<58))
            {
                op=true;  break;
            }
        else
            {
                a=true;
            }
    if(op)
         if(input[input.length()-1]>47 && input[input.length()-1]<58)
            b=true;
//  cout<< ' '<<a<<' '<<op <<' '<< b<<' '<< input[input.length()-1]<<endl;
    return a&&b&&op;
}
