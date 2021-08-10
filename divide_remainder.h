
void reverseStr(string &str)
{
    int n = str.length();
    // swap character starting from two
    // corners
    for (int i = 0; i < n / 2; i++)
        swap(str[i], str[n - i - 1]);
}

bool _equal(number *a,number *b)
{
    if((*a).len != (*b).len)
        return false;
    for(int i=0;i<(*a).volume;i++)
        if( ((*a).num)[i]!=((*b).num)[i])
            return false;
    if((*a).point && ! (*b).point)  // 1.0==1
        {
            if ( (*a).volume_float !=1)
                return false;
            if ((*a)._float[0]!=0)
                return false;
        }
     if((*b).point && ! (*a).point) //1==1.0
        {
            if ( (*b).volume_float !=1)
                return false;
            if((*b)._float[0]!=0)
                return false;
        }
    if((*a).point && (*b).point)// 1.0 == 1.0
        {
            if((*a).volume_float != (*b).volume_float)
                return false;
            for (int i=0;i<(*a).volume_float;i++)
                if(  ((*a)._float)[i] != ((*b)._float)[i])
                    return false;
        }
    return true;
}
string fix_string(string a, int position)
{ // fisrt we remove '.' from string ,then add zeros as muxh as needed to begin or end of string  and then insert '.' at its right place
    bool negative =false;
    if(position<0)
    {
        reverseStr(a);
        position*=-1;
        negative =true;
    }
    int radix_point_position=0;
    for(int i=0;i<a.length();i++)
        {
            if(a[i]=='.')
                {
                    radix_point_position=i;
                    break;
                }
        }
    if(radix_point_position!=a.length())
        a.erase(a.begin()+radix_point_position);

    for(int i=position+1-(a.length()-radix_point_position);i>0;i--)
        a+='0';
    a.insert(a.begin()+radix_point_position+ position,'.');
    if(negative)
        reverseStr(a);
    return a;
}

void _64to8bit(number *a,number8bit *b) //turning 12.5202 to 125202
{
    (*b).volume=(*a).len+(*a).volume_float;
    (*b).num =new short[(*b).volume];
    for(int i=0;i<(*b).volume;i++)
        ((*b).num)[i]=0;
    int j=0;
    while(j<(*a).volume_float)
        ((*b).num)[j++]=((*a)._float)[(*a).volume_float-1-j];

    long long int temp=0;
    for(int i=0;i<(*a).volume;i++)
        {
            temp=((*a).num)[i];
            for(int t=0;t<8 && j< (*b).volume;t++)
            {
                ((*b).num)[j++]=temp%10;
                temp/=10;
            }
        }
    decrease_volume(b);
}
void _8to64bit(number8bit *a,number *b) //{0,1,3,4,2,5} ->{543210,0}
{
    long long  int temp=ceil((*a).volume/8.0);  // by declaring long long int we can use it later
    (*b).num =new long long int[temp];
    (*b).volume=temp;
    for(int k=0;k<temp;k++)
        ((*b).num)[k]=0;
    for(int i=0;i<(*a).volume;i++)
        {
            if(i%8==0)
                temp=1;
            ((*b).num)[i/8]+=((*a).num)[i]*temp;
            temp*=10;
        }
    count_digits(b);
}

void remainder(number8bit *a,number8bit *b ,int q) // a = a-b*q
{
        short *B=new short[(*a).volume];
        short Bvolume=(*a).volume;
        for(int i=0;i<Bvolume;i++)
            B[i]=0;

        for(int i=0;i<(*b).volume;i++)
                B[i]=q * ((*b).num)[i];
        for(int i=0;i<Bvolume-1;i++)
            {
               B[i+1]+=B[i]/10;
               B[i]=B[i]%10;
            }// we can merge these two last loops together , but it may corrupt the product of multiply
        for(int i=0;i<(*a).volume;i++)
            ((*a).num)[i]=((*a).num)[i]-B[i];

        nsort(a);
        decrease_volume(a);//decrease 'a' which is acopy  it is done at the start of do{}while
        delete []B;
}
void divide(number *a,number *b,number *c,int range)
{    /* divide and remainder is based on school method(in Iran)
            123452|52
                   ---
      acopy=123   Q=0023...
            104 q=2  r=acopy=19
            ---
       acopy=194
             156
             ---
              38
             ...
              ..
               .
    first we declare acopy.
    the volume must be one index more respect to B.
    for exaple :
                    125/5   -> B.volume=1
                    acopy.num=new short [B.volume+1]

    . in acopy we put the digits  of input 'a' one by one  .
    then we find the  max q , q*B <=acopy;
    then calculate the acopy-q*B  which is remainder for every step.
    then we add next digit of 'a' from right to acopy/remainder and do the whole thing until we used the last digit of 'a':  do{} while(i!=0);
    */
    if(range<20) // if range is less than 12 , it gets stuck in a loop
        range = 20;// default range is 20 digits
    bool anegativ=0, bnegativ=0;

    if( is_negative(a) )
        {
            anegativ=true;
            opposite_maker(a);
        }
    if(is_negative(b))
        {
            bnegativ=true;
            opposite_maker(b);
        }
    // these condition make process faster
    if(is_zero(b))
        (*c).error=15;//15:= divisor zero

    else if(_less(a,b) && !(*a).point && !(*b).point)
        str2number("0",c);

    else if(_equal(a,b))
        if((*a).point || (*b).point )
            str2number("1.0",c);
        else
             str2number("1",c);

    else if(is_one(b))// a/1
        {
            (*c).num=new long long int[(*a).volume];
            if((*a).point)
                (*c)._float=new short int[(*a).volume_float];
            swap_number(c,a);
        }
    else//1554545415211554241 / 54212354312321 error 0x347 soleved :)
        {   //a=b*Q+r
            /* the divide and remainder method are school based , we convert number structure to number*bit (12.52 ->1252)
            the doing the dividing
            if any of inputs a or b has decimal point then we add '.' to Q string and continue the dividing but instead on adding the last digit we add zero to acopy
             we continue until the acopy is zero or we reached demanded decimal number range which is given by user
            */
            number8bit *A=new number8bit;
            number8bit *B=new number8bit;
            _64to8bit(a,A);
            _64to8bit(b,B);

            number8bit *acopy=new number8bit;
            string Q;//Quotient

            (*acopy).num= new short [(*B).volume +1];
            for(int j=0;j<(*B).volume+1;j++)//clearing acopy
                ((*acopy).num)[j]=0;

            (*acopy).volume=0;
            int i=(*A).volume;// 'i' tells us which digit from 'a' is used
            int q;//Quotient digit
            do
                {
                    for(int j=(*acopy).volume-1;j>-1;j--)
                        ((*acopy).num)[j+1]=((*acopy).num)[j];

                    (*acopy).volume++;
                    ((*acopy).num)[0]=((*A).num)[--i]; //adding digit to acopy
                    decrease_volume(acopy);// ***  this line  wasnt in last presentation and fixed the Quotient errors ***
                    //---------------------------- finding q
                    q=1;
                    while(_less_eqale(B,q,acopy))// B*q <= acopy
                        q++;
                    q--;
                    Q+=q+48; //we add q to string Q
                    //---------------------------------------
                    if(q)
                        remainder(acopy,B,q );//  acopy = acopy - B*q // remainder function doesnt need extra number , acopy is enough

                }while(i!=0);
            if((*a).point || (*b).point )
                {
                    i=range + (*b).volume_float-(*a).volume_float;
                    Q+='.';
                    do
                        {
                            i--;
                            for(int j=(*acopy).volume-1;j>-1;j--)
                                ((*acopy).num)[j+1]=((*acopy).num)[j];

                            (*acopy).volume++;
                            ((*acopy).num)[0]=0; //adding digit to acopy
                            decrease_volume(acopy);// ***  this line  wasnt in last presentation and fixed the Quotient errors ***
                            //---------------------------- finding q
                            q=1;
                            while(_less_eqale(B,q,acopy))  // B*q <= acopy
                                q++;
                            q--;
                            Q+=q+48; //we add q to string Q
                            //---------------------------------------
                            if(q)
                                remainder(acopy,B,q );//  acopy = acopy - B*q
                            }while(i!=0 && !is_zero(acopy) );
                        // fixing the string Q
                       // at this point Q can be 10.2 *10^-2 , we have to turn it to 0.102
                        Q=fix_string(Q,(*b).volume_float-(*a).volume_float);
                    }
            str2number(Q,c); //turning the  final Quotient from string to number stucture

            if((*B).volume)
                delete []((*B).num);
            delete B;
            if((*A).volume)
                delete []((*A).num);
            delete A;
            if((*acopy).volume)
                delete []((*acopy).num);
            delete acopy;
        }
   // this part handle the positive/negative sign
   if(anegativ ^ bnegativ)
       opposite_maker(c);
}


void find_remainder(number *a,number *b,number *c)
{       //find_remainder and divide functions are same but the output is acopy
    bool anegativ=0, bnegativ=0;
    if(((*a).num)[(*a).volume-1] <0)
        {
            anegativ=true;
            opposite_maker(a);
        }
    if(((*b).num)[(*b).volume-1] <0)
        {
            bnegativ=true;
            opposite_maker(b);
        }

    if((*a).point || (*b).point)
        (*c).error=17;//17: input not integer;

    if(anegativ || bnegativ)
        (*c).error=16;//16:= find_remainder input negative

    else if((*b).len==1 && ((*b).num)[0]==0)
        (*c).error=15;

    else if(_less(a,b)) //a < b ,  a%b
        {
           (*c).num=new long long int[(*a).volume];
           swap_number(c,a);
        }
    else if(_less_eqale(a,b)) // a%a
        str2number("0",c);

    else if((*b).len==1 && ((*b).num)[0]==1) //a%1
        str2number("0",c);

    else//1554545415211554241 54212354312321 error 0x347
    {   //a=b*Q+r
        number8bit *A=new number8bit;
        number8bit *B=new number8bit;
        _64to8bit(a,A);
        _64to8bit(b,B);

        number8bit *acopy=new number8bit;
        string Q;//Quotient

        (*acopy).num= new short [(*B).volume +1];
        for(int j=0;j<(*B).volume+1;j++)//clearing acopy
            ((*acopy).num)[j]=0;
        (*acopy).volume=0;
        int i=(*A).volume;
        int q;//Quotient digit
        do
            {
                for(int  j=(*acopy).volume-1;j>-1;j--)
                    ((*acopy).num)[j+1]=((*acopy).num)[j];//shifting acopy to right
                (*acopy).volume++;
                ((*acopy).num)[0]=((*A).num)[--i];
                decrease_volume(acopy);// ******  this line is added wasn't in phase one and fixed the Quotient erorrs
                //----------------------------
                q=1;
                while(_less_eqale(B,q,acopy))
                    q++;
                q--;
                //---------------------------------------
                if(q)
                    remainder(acopy,B,q );
                Q+=q+48;
            }while(i!=0);
        //acopy after do{}while is the remainder
        _8to64bit(acopy,c); //c=acopy

        if((*B).volume)
            delete []((*B).num);
        delete B;
        if((*A).volume)
            delete []((*A).num);
        delete A;
        if((*acopy).volume)
            delete []((*acopy).num);
        delete acopy;
    }
   /*not needed in here
   if(anegativ ^ bnegativ)
       opposite_maker(c); */
}
