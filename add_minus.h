




void add(number *a,number *b,number *c)
{
    (*c).volume = max((*b).volume ,(*a).volume) + 1;
    (*c).num=new long long [(*c).volume];
    for(int i=0;i< (*c).volume;i++)
        ((*c).num)[i]=0;

    for(int i=0;i<(*a).volume;i++) // these four lines are replacement of last system of adding/subtraction
        ((*c).num)[i]+=((*a).num)[i];
    for(int i=0;i<(*b).volume;i++)
        ((*c).num)[i]+=((*b).num)[i];
    fix(c);

    nsort(c);
    //-----------------------------
    if((*a).point ||(*b).point)
        {
            (*c).point=true;
            (*c).volume_float = max((*a).volume_float,(*b).volume_float);
            (*c)._float=new short[(*c).volume_float];
            for(int i=0;i<(*c).volume_float;i++)//clearing c._float
                ((*c)._float)[i]=0;
            if((*a).point)
                for(int i=0;i<(*a).volume_float;i++)
                    ((*c)._float)[i]+=((*a)._float)[i];
            if((*b).point)
                for(int i=0;i<(*b).volume_float;i++)
                    ((*c)._float)[i]+=((*b)._float)[i];
            //nsort for _float---------------------------
            nsort_float(c);
            //fix--------------------------------------------
            fix_float(c);
            //decrease volume_float;
            while(((*c)._float)[(*c).volume_float-1]==0  && (*c).volume_float!=1)
                    (*c).volume_float--;
            reduce_number_float(c);
            nsort(c) ;
        }

    decrease_volume(c);
    reduce_number_volume(c);
    count_digits(c);
}

void minuss(number *a,number *b,number *c)//c=a-b  minuss function is just like add function
{
    (*c).volume = max((*b).volume ,(*a).volume) + 1;
    (*c).num=new long long [(*c).volume];
    for(int i=0;i< (*c).volume;i++)
        ((*c).num)[i]=0;
    for(int i=0;i<(*a).volume;i++) // these four lines are replacement of last system of adding/subtraction
        ((*c).num)[i]+=((*a).num)[i];
    for(int i=0;i<(*b).volume;i++)
        ((*c).num)[i]-=((*b).num)[i];
    fix(c);
    nsort(c);
      //-----------------------------
    if((*a).point ||(*b).point)
        {
            (*c).point=true;
            (*c).volume_float = max((*a).volume_float,(*b).volume_float);
            (*c)._float=new short[(*c).volume_float];
            for(int i=0;i<(*c).volume_float;i++)//clearing c._float
                ((*c)._float)[i]=0;
            if((*a).point)
                for(int i=0;i<(*a).volume_float;i++)
                    ((*c)._float)[i]+=((*a)._float)[i];
            if((*b).point)
                for(int i=0;i<(*b).volume_float;i++)
                    ((*c)._float)[i]-=((*b)._float)[i];
            nsort_float(c);
            fix_float(c);
            decrease_float(c);
            reduce_number_float(c);
            nsort(c) ;
        }
    decrease_volume(c);
    reduce_number_volume(c);
    count_digits(c);
}