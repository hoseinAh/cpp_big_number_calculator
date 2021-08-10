
void multi(number *A,number *B,number *C)
{       /*we split the multiplition to four parts
            C=(A+a)(B+b)= A*B + A*b + B*a + a*b
            12.32*4.02 = 12*4 + 12*0.02 + 4*0.23 + 0.32*0.2  */
        //A*B------------------
        (*C).volume=(*A).volume+(*B).volume;
        (*C).num=new long long int[(*C).volume];

        for(int i=0;i<(*C).volume;i++)//clearing the C.num
            ((*C).num)[i]=0;
         /* in the first presentation the loops below were used but in the new changes we avoid extra processes.
            the fix(C) costs more time but the inside loop processes are now lesser
            this improvement helps us in power module .
            *** this only improves when B.volume > 0.72*A.volume, we can rearrange the order but we just continue. in power B.volume is always more than or equal to A.volume
        for(int i=0; i<(*A).volume ;i++)
        {
            for(int j=0; j<(*B).volume ;j++)
                {
                    ((*C).num)[j+i+1]+=(((*A).num)[i]*((*B).num)[j]+((*C).num)[i+j])/100000000;
                    ((*C).num)[j+i]=(((*A).num)[i]*((*B).num)[j]+((*C).num)[i+j])%100000000;
                }
        }*/
        //A*B------------------
        for(int i=0; i<(*A).volume ;i++)
        {
            for(int j=0; j<(*B).volume ;j++)
                ((*C).num)[j+i]+=(((*A).num)[i]*((*B).num)[j]);
            fix(C);
        }
        //---------------------
        if((*A).point || (*B).point)
            {
                (*C).point=true;
                (*C).volume_float= (*A).volume_float + (*B).volume_float;
                (*C)._float=new short [(*C).volume_float];
                for(int i=0;i<(*C).volume_float;i++)
                    ((*C)._float)[i]=0;
                //A*b-------------  A*b and B*a are same;
                if((*B).point)
                {   /*first we create Ab which hold the A*b but it is not a float
                    12.32 * 4.02 ->Ab=12*02=24
                    the we put the radix point by splitting the Ab */
                    int Abvolume=(*A).volume+ ceil((*B).volume_float /8.0);
                    long long int *Ab=new long long int [Abvolume];
                    for(int i=0;i<Abvolume;i++)
                        Ab[i]=0;

                    long long int t=1;
                    for(int i=(*B).volume_float -1;i>-1;i--)//Ab=12*02=24
                        {
                            if(t>10000000)
                            t=1;
                            for (int j=0;j<(*A).volume;j++)
                                {
                                    Ab[j+((*B).volume_float-i-1)/8+1]+=(((*A).num)[j]*((*B)._float)[i]*t+  Ab[j+((*B).volume_float-i-1)/8])/100000000;
                                    Ab[j+((*B).volume_float-i-1)/8]=(((*A).num)[j]*((*B)._float)[i]*t+  Ab[j+((*B).volume_float-i-1)/8])%100000000;
                                }
                            t*=10;
                        }
                    //splitting 24 -> 2.4
                    //adding 0.4 to C._float
                    int k=0;
                    int position =(*B).volume_float-1;
                    while(position>-1)
                        {
                            ((*C)._float)[position--]+=Ab[k/8]%10;
                            Ab[k/8]/=10;
                            k++;
                        }
                    //the rest of Ab must be sorted again and the added to C.num
                    long long int temp= pow(10,8-(*B).volume_float%8 );
                    long long int temp2=pow(10,(*B).volume_float%8 );
                    for(int i=k/8;i<Abvolume-1;i++)
                         {
                             Ab[i]+=(Ab[i+1]%temp2)*temp;
                             Ab[i+1]/=temp2;
                         }
                    for(int i=k/8,j=0;i<Abvolume;i++,j++)
                        ((*C).num)[j]+=Ab[i];
                    fix(C);
                    delete []Ab;
                }
                //B*a-------------A*b and B*a are same;
                 if((*A).point)
                {
                    int Bavolume=(*B).volume+ ceil((*A).volume_float /8.0);
                    long long int *Ba=new long long int [Bavolume];
                    for(int i=0;i<Bavolume;i++)
                        Ba[i]=0;

                    long long int t=1;
                    for(int i=(*A).volume_float -1;i>-1;i--)
                        {
                            if(t>10000000)
                                t=1;
                            for (int j=0;j<(*B).volume;j++)
                                {
                                    Ba[j+((*A).volume_float-i-1)/8+1]+=(((*B).num)[j]*((*A)._float)[i]*t+  Ba[j+((*A).volume_float-i-1)/8])/100000000;
                                    Ba[j+((*A).volume_float-i-1)/8]=(((*B).num)[j]*((*A)._float)[i]*t+  Ba[j+((*A).volume_float-i-1)/8])%100000000;
                                }
                            t*=10;
                        }

                    int k=0;//temp variable
                    int position =(*A).volume_float-1;
                    while(position>-1)
                        {
                            ((*C)._float)[position--]+=Ba[k/8]%10;
                            Ba[k/8]/=10;
                            k++;
                        }

                    long long int temp= pow(10,8-(*A).volume_float%8 );
                    long long int temp2=pow(10,(*A).volume_float%8 );
                    for(int i=k/8;i<Bavolume-1;i++)
                         {
                             Ba[i]+=(Ba[i+1]%temp2)*temp;
                             Ba[i+1]/=temp2;
                         }

                    for(int i=k/8,j=0;i<Bavolume;i++,j++)
                        ((*C).num)[j]+=Ba[i];
                    fix(C);
                    fix_float(C);
                    fix(C);
                    delete []Ba;
                }
                //a*b-------------
                /* we just do A._float * B._float */
                if((*A).point&& (*B).point)
                    {
                        for(int i=(*A).volume_float -1;i>-1;i--)
                            {
                                for (int j=(*B).volume_float -1;j>-1;j--)
                                    ((*C)._float)[i+j+1]+=((*A)._float)[i]*((*B)._float)[j];
                                fix_float(C);
                            }
                        fix(C);
                    }
                decrease_float(C);
                reduce_number_float(C);
            }
        decrease_volume(C);
        reduce_number_volume(C);
        count_digits(C);
}
void multi_for_power(number *A,number *B,number *C) //****** C.num and C._float  are declared outside of function, it is just like multi function at top of here
{
        (*C).volume=(*A).volume+(*B).volume;
        //(*C).num=new long long int[(*C).volume];-------------------------------------------------------
        for(int i=0;i<(*C).volume;i++)//clearing the C.num
            ((*C).num)[i]=0;

        for(int i=0; i<(*A).volume ;i++)
            {
                for(int j=0; j<(*B).volume ;j++)
                    ((*C).num)[j+i]+=(((*A).num)[i]*((*B).num)[j]);
                fix(C);
            }
        if((*A).point || (*B).point)
            {
                (*C).point=true;
                (*C).volume_float= (*A).volume_float + (*B).volume_float;
                //(*C)._float=new short [(*C).volume_float];------------------------------------------------
                for(int i=0;i<(*C).volume_float;i++)
                    ((*C)._float)[i]=0;
                //A*b-------------  A*b and B*a are same;
                if((*B).point)
                    {
                        int Abvolume=(*A).volume+ ceil((*B).volume_float /8.0);
                        long long int *Ab=new long long int [Abvolume];
                        for(int i=0;i<Abvolume;i++)
                            Ab[i]=0;
                        long long int t=1;
                        for(int i=(*B).volume_float -1;i>-1;i--)
                            {
                                if(t>10000000)
                                    t=1;
                                for (int j=0;j<(*A).volume;j++)
                                    {
                                        Ab[j+((*B).volume_float-i-1)/8+1]+=(((*A).num)[j]*((*B)._float)[i]*t+  Ab[j+((*B).volume_float-i-1)/8])/100000000;
                                        Ab[j+((*B).volume_float-i-1)/8]=(((*A).num)[j]*((*B)._float)[i]*t+  Ab[j+((*B).volume_float-i-1)/8])%100000000;
                                    }
                                t*=10;
                            }
                        int k=0;
                        int position =(*B).volume_float-1;
                        while(position>-1)
                            {
                                ((*C)._float)[position--]+=Ab[k/8]%10;
                                Ab[k/8]/=10;
                                k++;
                            }
                        long long int temp= pow(10,8-(*B).volume_float%8 );
                        long long int temp2=pow(10,(*B).volume_float%8 );
                        for(int i=k/8;i<Abvolume-1;i++)
                             {
                                 Ab[i]+=(Ab[i+1]%temp2)*temp;
                                 Ab[i+1]/=temp2;
                             }
                        for(int i=k/8,j=0;i<Abvolume;i++,j++)
                            ((*C).num)[j]+=Ab[i];
                        fix(C);
                        delete []Ab;
                    }
                 if((*A).point)
                    {
                        int Bavolume=(*B).volume+ ceil((*A).volume_float /8.0);
                        long long int *Ba=new long long int [Bavolume];
                        for(int i=0;i<Bavolume;i++)
                            Ba[i]=0;
                        long long int t=1;
                        for(int i=(*A).volume_float -1;i>-1;i--)
                            {
                              if(t>10000000)
                                t=1;
                              for (int j=0;j<(*B).volume;j++)
                                  {
                                      Ba[j+((*A).volume_float-i-1)/8+1]+=(((*B).num)[j]*((*A)._float)[i]*t+  Ba[j+((*A).volume_float-i-1)/8])/100000000;
                                      Ba[j+((*A).volume_float-i-1)/8]=(((*B).num)[j]*((*A)._float)[i]*t+  Ba[j+((*A).volume_float-i-1)/8])%100000000;
                                  }
                                t*=10;
                            }
                        int k=0;
                        int position =(*A).volume_float-1;
                        while(position>-1)
                            {
                                ((*C)._float)[position--]+=Ba[k/8]%10;
                                Ba[k/8]/=10;
                                k++;
                            }
                        long long int temp= pow(10,8-(*A).volume_float%8 );
                        long long int temp2=pow(10,(*A).volume_float%8 );
                        for(int i=k/8;i<Bavolume-1;i++)
                             {
                                 Ba[i]+=(Ba[i+1]%temp2)*temp;
                                 Ba[i+1]/=temp2;
                             }
                        for(int i=k/8,j=0;i<Bavolume;i++,j++)
                            ((*C).num)[j]+=Ba[i];
                        fix(C);
                        fix_float(C);
                        fix(C);
                        delete []Ba;
                    }
                if( (*A).point && (*B).point)
                    {
                        for(int i=(*A).volume_float -1;i>-1;i--)
                            {
                                for (int j=(*B).volume_float -1;j>-1;j--)
                                    ((*C)._float)[i+j+1]+=((*A)._float)[i]*((*B)._float)[j];
                                fix_float(C);
                            }
                        fix(C);
                    }
                decrease_float(C);
               // reduce_number_float(C);  delete and new [] cost us time
            }
        decrease_volume(C);
        //reduce_number_volume(C); delete and new [] cost us time
        //count_digits(C);
}

void power(number *a,number *b,number *c)
{/* in this function we are using multi_for_power function which is declared out side, but by writing it inside and make it suitable for each part,
    it can make the process faster , but to make the code readable we avoid it*/
            if (is_negative(b))
                {
                    (*c).error=13;//13 := negative power cant be used
                }
            else if( is_zero(b))
                {
                    if((*a).point)
                        str2number("1.0",c);
                    else
                        str2number("1",c);
                }
            else if (is_one(b))
                {
                    (*c).num=new long long int [(*a).volume];
                    if((*a).point)
                        (*c)._float=new short [(*a).volume_float];
                    swap_number(c,a);
                }
            else if((*b).point )
                {
                    (*c).error=12;//12 := power is not integer
                }
            else if(is_one(a))
                {
                    if((*a).point)
                        str2number("1.0",c);
                    else
                        str2number("1",c);
                }
            else if( is_zero(a))
                {
                    if((*a).point)
                        str2number("0.0",c);
                    else
                        str2number("0",c);
                }
            else if((*b).volume >1 || ((*b).num)[0]>850000)
                {
                    (*c).error =14; //14 := very big number
                }
            else
                {
                    int n=((*b).num)[0];
                    int Max=ceil(10+max(((*a).len * n )/8.0,1.0));
                    (*c).num=new long long int[Max];
                    (*c).volume=1;//real volume is not one but during the swapping process its get to real value
                    (*c).num[0]=1;
                    number *d=new number;
                    (*d).num=new long long int[Max];

                    if((*a).point)
                        {
                            (*c).point =true;
                            //(*d).point =true;
                            (*c)._float=new short [(*a).volume_float*n];
                            for(int i=0;i<(*a).volume_float*n;i++)
                                ((*c)._float)[i]=0;
                            (*d)._float=new short [(*a).volume_float*n];
                            for(int i=0;i<(*a).volume_float*n;i++)
                                ((*d)._float)[i]=0;
                        }
                    if(n<=10000)
                    for(long long int i=0;i<n;i++)
                        {
                            //number *d=new number;
                            /*d work as c in multi function is a temp structure and it is declared outside of loop ,
                             if it was declared inside , n times it must be declared and also n times 'new' and 'delete' must be done;
                                and for that we write void  multi_for_power*/
                            multi_for_power(a,c,d);//d=a*c;
                            swap_number(c,d);//c=d;
                        }
                    /*we can split the power process if n>10000
                    C=A^B  , B= q*10000 + r
                    C=A^(q*10000+r)=(A^10000)^q * A^r;
                    this method worked for 2^850000 under 12 seconds and without splitting 199 seconds but for (2^900000)   in the c=cc^q part ,memory violation appeared 0x005.
                    for 2^1 000 000 , I gave 2^100 000 as A  and 10 as B ,it worked ,but  for directly 2^1 000 000 ,memory violation appeared 0x005.
                    */
                    else
                        {
                            int r=n%10000;
                            int q=n/10000;

                            number *cc=new number;
                            number *rr=new number;
                            Max=ceil(max(((*a).len * 10000 )/8.0,1.0));
                            (*cc).num=new long long int[Max];
                            (*cc).volume=1;//real volume is not one but during the swapping process its get to real value
                            (*cc).num[0]=1;
                            Max=ceil(max(((*a).len * r )/8.0,1.0));
                            (*rr).num=new long long int[Max];
                            (*rr).volume=1;//real volume is not one but during the swapping process its get to real value
                            (*rr).num[0]=1;

                             if((*a).point)
                                {
                                    (*cc).point =true;
                                    (*cc)._float=new short [(*a).volume_float*10000];
                                    for(int i=0;i<(*a).volume_float*10000;i++)
                                        ((*cc)._float)[i]=0;

                                    (*rr).point =true;
                                    (*rr)._float=new short [(*a).volume_float*r];
                                    for(int i=0;i<(*a).volume_float*r;i++)
                                        ((*rr)._float)[i]=0;
                                }
                          // rr=A^r
                            for(long long int i=0;i<r;i++)
                                {

                                    multi_for_power(a,rr,d);
                                    swap_number(rr,d);
                                }
                            //cc=A^100000;
                            for(long long int i=0;i<10000;i++)
                                {

                                    multi_for_power(a,cc,d);
                                    swap_number(cc,d);
                                }
                           // c=cc^q
                            for(long long int i=0;i<q;i++)
                                {
                                    multi_for_power(cc,c,d);
                                    swap_number(c,d);
                                }
                            //c=d*rr
                            multi_for_power(d,rr,c);

                            delete []((*rr).num);
                            if((*rr).point)
                                delete []((*rr)._float);
                            delete rr;
                            delete []((*cc).num);
                            if((*cc).point)
                                delete []((*cc)._float);
                            delete cc;
                        }
                    delete []((*d).num);
                    if((*d).point)
                        delete []((*d)._float);
                    delete d;
                    reduce_number_volume(c);
                        if((*c).point)
                    reduce_number_float(c);
                }
}
