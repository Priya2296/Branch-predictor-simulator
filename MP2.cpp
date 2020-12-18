#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <algorithm> 
#include <vector>
#include<limits>

#include <bitset>

using namespace std;
string::size_type sz;



class Smith
{
    public:
    int b,prev_state;
    string tracefile;
    string true_output;
    string predict;
    long mispredictions=0;
    long prediction_count=0;
    vector<string> op;
    vector<string> addresses;

    Smith(int b,string tracefile)
    {
        this->b=b;
        this->tracefile=tracefile;
    }

    void initial_state()
    {
        prev_state=pow(2,b)/2;

    }
    void actual_outcome()
    {
    
        string ops;
        string Hex_Add;
        string line;
        
        ifstream myfile(tracefile);
        if (myfile.is_open())
        {
            while (getline(myfile,line))
            {
                istringstream iss(line);
			    iss>>Hex_Add>>ops;
                addresses.push_back(Hex_Add);
                op.push_back(ops);
                
            }
            myfile.close();
                
                
        }
        
        
        

        for(int i=0;i<op.size();i++)
        {
            if(op[i].compare("n")==0)
            {
                //cout<<"True output is Not Taken"<<endl;
                true_output="NT";
                
                prediction();
                misprediction();
                update();
            }
            if(op[i].compare("t")==0)
            {
                //cout<<"True output is Taken"<<endl;
                true_output="T";
                
                prediction();
                misprediction();
                update();
            }
            
        }
        print();
        cout<<"OUTPUT"<<endl;
        cout<<"number of predictions:     "<<prediction_count<<endl;
        cout<<"number of mispredictions:  "<<mispredictions<<endl;
        printf("misprediction rate:        %0.2f%%\n",round(MisPredictionRate()));
        cout<<"FINAL COUNTER CONTENT:     "<<prev_state<<endl;


    }
    float round(float variable1)
    {
        float fval = (int)(variable1 * 100 + .5);
        return (float)fval / 100;
    }
    
    void prediction()
    {
        
        prediction_count=prediction_count+1;
        int c;
        c=(pow(2,b)/2);
        //cout<<"c is"<<c<<endl;
        //cout<<"previous state is"<<prev_state<<endl;
        
        if(prev_state<c)
        {
            predict="NT";
            //cout<<"Prediction is"<<predict<<endl;
        }
        else
        {
            predict="T";
            //cout<<"Prediction is"<<predict<<endl;
        }

        

        
    }
    void misprediction()
    {
        if(true_output!=predict)
        {
            mispredictions=mispredictions+1;
        }
        //cout<<"Misprediction count is"<<mispredictions<<endl;
    }
    void update()
    {
        if(true_output=="T")
        {
            if(prev_state==pow(2,b)-1)
            {
                prev_state=pow(2,b)-1;
            }
            else
            {
                prev_state=prev_state+1;
            }
            
        }
        if(true_output=="NT")
        {
            if(prev_state==0)
            {
                prev_state=0;
            }
            else
            {
                prev_state=prev_state-1;
            }
            
        }
    }
    void print()
    {
        cout<<"COMMAND\n";
        cout<<"./sim smith "<<b<<" "<<tracefile<<endl;
    }

    float MisPredictionRate()
    {
        return ((float)mispredictions/(float)prediction_count)*100.00;
    }




};

class Bimodal
{
    public:
    int m,dec;
    int index;
    string tracefile;
    vector<string> op;
    vector<string> addresses;
    vector<int> table;
    string true_output;
    string predict;
    long mispredictions=0;
    long prediction_count=0;
    
    


    Bimodal(int m,string tracefile)
    {
        this->m=m;
        this->tracefile = tracefile;
    }
    void readfile()
    {
        string ops;
        string Hex_Add;
        string line;
        
        ifstream myfile(tracefile);
        if (myfile.is_open())
        {
            while (getline(myfile,line))
            {
                istringstream iss(line);
			    iss>>Hex_Add>>ops;
                
                addresses.push_back(Hex_Add);
                
                op.push_back(ops);
                
                
            }
            myfile.close();
        }
        for(int i=0;i<op.size();i++)
        {
            if(op[i].compare("n")==0)
            {
                true_output="NT";
                dec=stoi(addresses[i], &sz, 16);
                //cout<<dec<<endl;
                getIndex(dec);
                prediction();
                misprediction();
                update(true_output);
                
            }
            if(op[i].compare("t")==0)
            {
                true_output="T";
                dec=stoi(addresses[i], &sz, 16);
                //cout<<dec<<endl;
                getIndex(dec);
                prediction();
                misprediction();
                update(true_output);
                
            }
            
            
        }
            
        print();
        cout<<"OUTPUT"<<endl;
        cout<<"number of predictions:     "<<prediction_count<<endl;
        cout<<"number of mispredictions:  "<<mispredictions<<endl;
        
        
		
        printf("misprediction rate:        %0.2f%%\n", round(MisPredictionRate()));
        printFinalContent();

    }

    float round(float variable1)
    {
        float fval = (int)(variable1 * 100 + .5);
        return (float)fval / 100;
    }
    


    void fillPredictionTable()
    {
        for(int i=0;i<pow(2,m);i++)
        {
            table.push_back(4);
        }
        //cout<<"table size"<<table.size()<<endl;

    }
    void getIndex(int dec)
    {
        //cout<<"Dec"<<dec<<endl;
        
        dec=dec/4;
        index=fmod(dec,pow(2,m));
    
        //cout<<"Bimodal Index is"<<index<<endl; 

    }
    
    string prediction()
    {
        //cout<<"Hi"<<endl;
        //cout<<index<<endl;
        prediction_count=prediction_count+1;
        //cout<<table[0]<<endl;
        //cout<<table[index]<<endl;
        if(table[index]<4)
        {
            predict="NT";
        }
        else
        {
            predict="T";
        }
        return predict;
        
    }
    void misprediction()
    {
        if(true_output!=predict)
        {
            mispredictions++;
        }
        //cout<<"Misprediction count is"<<mispredictions<<endl;
    }
    void update(string true_output)
    {
        //cout<<"Update"<<endl;
        //cout<<"Index"<<index<<endl;
        //cout<<"True output is"<<true_output<<endl;
        if(true_output=="T")
        {
            if(table[index]==7)
            {
                table[index]=7;
            }
            else
            {
                table[index]=table[index]+1;
            }
        }
        if(true_output=="NT")
        {
            //cout<<"Not taken"<<endl;
            if(table[index]==0)
            {
                table[index]=0;
            }
            else
            {
                table[index]=table[index]-1;
            }
            
        }
        //cout<<"Updated prediction table in bimodal"<<table[index];
            
        
    }
    void print()
    {
        cout<<"COMMAND\n";
        cout<<"./sim bimodal "<<m<<" "<<tracefile<<endl;
    }

    float MisPredictionRate()
    {
        return 100.00*((float)mispredictions/(float)prediction_count) ;
        
    }
    

    

    void printFinalContent()
    {
        cout<<"FINAL BIMODAL CONTENTS"<<endl;
        for(int i=0;i<table.size();i++)
        {
            cout<<i<<"     "<<table[i]<<"\n";
        }
    }

};

class Gshare
{
    public:
    int m1,n,dec;
    int index;
    string tracefile;
    vector<string> op;
    vector<string> addresses;
    vector<int> table;
    string true_output;
    string predict;
    long mispredictions=0;
    long prediction_count=0;
    string bhr;
    
    
    

    Gshare(int m1,int n,string tracefile)
    {
        this->m1=m1;
        this->n=n;
        this->tracefile=tracefile;
    }
    void readfile()
    {
        
    
        string ops;
        string Hex_Add;
        string line;
        ifstream myfile(tracefile);
        if (myfile.is_open())
        {
            while (getline(myfile,line))
            {
                
                
                
                istringstream iss(line);
			    iss>>Hex_Add>>ops;
                
                    
                addresses.push_back(Hex_Add);
                
                op.push_back(ops);
                
            }
            myfile.close();
                
                
        }
        //cout<<op.size()<<endl;
        for(int i=0;i<op.size();i++)
        {
            //cout<<"Hi"<<endl;
            if(op[i].compare("n")==0)
            {
                //cout<<"True output is Not Taken"<<endl;
                true_output="NT";
                dec=stoi(addresses[i], &sz, 16);
                getIndex(dec);
                
                prediction();
                
                misprediction();
                update(true_output);
                updateBHR(true_output);
            }
            if(op[i].compare("t")==0)
            {
                //cout<<"True output is Taken"<<endl;
                true_output="T";
                dec=stoi(addresses[i], &sz, 16);
                getIndex(dec);
                
                prediction();
                misprediction();
                update(true_output);
                updateBHR(true_output);
            }
            
        }
        print();
        cout<<"OUTPUT"<<endl;
        cout<<"number of predictions:     "<<prediction_count<<endl;
        cout<<"number of mispredictions:  "<<mispredictions<<endl;
        printf("misprediction rate:        %0.2f%%\n",round(MisPredictionRate()));
        printFinalContent();

    

    }
    float round(float variable1)
    {
        float fval = (int)(variable1 * 100 + .5);
        return (float)fval / 100;
    }
    
    void fillPredictionTable()
    {
        for(int i=0;i<pow(2,m1);i++)
        {
            table.push_back(4);
        }

    }
    void fillBHR()
    {
        for(int i=0;i<n;i++)
        {
            bhr.push_back('0');
        }
    }
    
    void getIndex(int dec)
    {
        
	    long number = dec / 4;
	    long endNbits = number % long(pow(2, n));
	    long startBits = number / long(pow(2, n));
	    long nDec = stol(bhr, &sz, 2);
	    long xoredBits = endNbits ^ nDec;
	    long new1 = startBits * long(pow(2, n)) + xoredBits;
	    index = new1 % long(pow(2, m1));  
        //cout<<"Gshare index"<<index<<endl;


    }
    string prediction()
    {
        prediction_count=prediction_count+1;
        //cout<<"Hi"<<endl;
        //cout<<table.size()<<endl;
        //cout<<"index is"<<index<<endl;
        //cout<<table[index]<<endl;
        //cout<<"Element"<<table[index]<<endl;
        //cout<<"Index of Gshare is"<<index<<endl;
        if(table[index]<4)
        {
            predict="NT";
            //cout<<"NT"<<endl;
        }
        else
        {
            predict="T";
            //cout<<"T"<<endl;
        }
        return predict;
        
    }
    void misprediction()
    {
        //cout<<"MP"<<endl;
        if(true_output!=predict)
        {
            mispredictions++;
        }
        //cout<<"Misprediction count is"<<mispredictions<<endl;
    }
    void update(string true_output)
    {
        if(true_output=="T")
        {
            if(table[index]==7)
            {
                table[index]=7;
            }
            else
            {
                table[index]=table[index]+1;
            }
        }
        if(true_output=="NT")
        {
            if(table[index]==0)
            {
                table[index]=0;
            }
            else
            {
                table[index]=table[index]-1;
            }
            
        }
    }
    void updateBHR(string true_output)
    {
        string new_bhr;

        if(true_output=="T")
        {
            new_bhr.push_back('1');
        }
        else
        {
            new_bhr.push_back('0');
        }
        for (auto i = bhr.begin(); i != bhr.end(); i++) {
			new_bhr.push_back(*i);
            
		}
        //cout<<new_bhr;
		new_bhr.pop_back();
        bhr=new_bhr;
        //cout<<"BHR IS"<<bhr<<endl;
        
       
        
    }
    void print()
    {
        cout<<"COMMAND\n";
        cout<<"./sim gshare "<<m1<<" "<<n<<" "<<tracefile<<endl;
    }

    float MisPredictionRate()
    {
        return ((float)mispredictions/(float)prediction_count)*100.00 ;
        
    }

    void printFinalContent()
    {
        cout<<"FINAL GSHARE CONTENTS"<<endl;
        for(int i=0;i<table.size();i++)
        {
            cout<<i<<"     "<<table[i]<<"\n";
        }
    }
        

    
};
class Hybrid
{
    public:
    int k,m1,n,m2;
    int index,dec;
    string tracefile;
    
    vector<string> op;
    vector<string> addresses;
    
    string true_output;
    string predict;
    long mispredictions=0;
    long prediction_count=0;
    Bimodal* bimodal;
    Gshare*  gshare;
    vector<int> chooser;

    Hybrid(int k,int m1,int n,int m2,string tracefile)
    {
        this->k=k;
        this->m1=m1;
        this->n=n;
        this->m2=m2;
        this->tracefile=tracefile;
        bimodal=new Bimodal(m2,tracefile);
        gshare=new Gshare(m1,n,tracefile);
    }

    void readfile()
    {
        string ops;
        string Hex_Add;
        string line;
        
        ifstream myfile(tracefile);
        if (myfile.is_open())
        {
            while (getline(myfile,line))
            {
                
                
                
                istringstream iss(line);
			    iss>>Hex_Add>>ops;
                
                    
                addresses.push_back(Hex_Add);
                
                op.push_back(ops);
                
            }
            myfile.close();
                
                
        }
        bimodal->fillPredictionTable();
        gshare->fillBHR();
        gshare->fillPredictionTable();

        //cout<<"file size"<<op.size()<<endl;
        for(int i=0;i<op.size();i++)
        {
            if(op[i].compare("n")==0)
            {
                //cout<<"True output is Not Taken"<<endl;
                true_output="NT";
                dec=stoi(addresses[i], &sz, 16);
                //cout<<"decimal address"<<dec<<endl;
                //cout<<"Table size"<<table.size()<<endl;
                getIndex(dec);
                GetPredictions();
                
                misprediction();
                update(true_output);
                updateChooser();
                updateBHR(true_output);
            }
            if(op[i].compare("t")==0)
            {
                true_output="T";
                dec=stoi(addresses[i], &sz, 16);
                getIndex(dec);
                GetPredictions();
                misprediction();
                update(true_output);
                
                updateChooser();
                updateBHR(true_output);

            }
            
            

        }
        print();
        cout<<"OUTPUT"<<endl;
        cout<<"number of predictions:     "<<prediction_count<<endl;
        cout<<"number of mispredictions:  "<<mispredictions<<endl;
        printf("misprediction rate:        %0.2f%%\n",round(MisPredictionRate()));
        printFinalContent();

    }

    float round(float variable1)
    {
        float fval = (int)(variable1 * 100 + .5);
        return (float)fval / 100;
    }
    
    void fillChooserTable()
    {
        for(int i=0;i<pow(2,k);i++)
        {
            chooser.push_back(1);
        }
        //cout<<chooser.size()<<endl;
    }
    
    void getIndex(int dec)
    {
        long number = dec / long(pow(2,2));
        index=number % long(pow(2,k));
        //cout<<"Chooser index is"<<index<<endl;

    }
    

    void GetPredictions()
    {
        prediction_count=prediction_count+1;
        int c;
        c=chooser[index];
        //cout<<"entry in chooser table"<<c<<endl;
        if(c>=2)
        {
            //cout<<"Going to take Gshare prediction"<<endl;
            gshare->getIndex(dec);
            predict=gshare->prediction();
            //cout<<"Gshare pred"<<predict<<endl;
        }
        else
        {
            //cout<<"Going to take bimodal prediction"<<endl;
            bimodal->getIndex(dec);

            predict=bimodal->prediction();
            //cout<<"Bimod pred"<<predict<<endl;
        }
        //cout<<"prediction is"<<predict<<endl;
        
    }
    void misprediction()
    {
        //cout<<"actual output"<<true_output<<endl;
        //cout<<"predicted output"<<predict<<endl;
        if(true_output!=predict)
        {
            mispredictions++;
        }
        //cout<<"Misprediction count"<<mispredictions<<endl;
    }
    void update(string true_output)
    {
        int c=chooser[index];
        if(c>=2)
        {
            gshare->update(true_output);
        }
        else
        {
            bimodal->update(true_output);
        }
        
    }
    void updateChooser()
    {
        
        int c=chooser[index];
        string bimodal_predict;
        string gshare_predict;
        
        if(c<2)
        {
            bimodal_predict=predict;
            gshare->getIndex(dec);
            gshare_predict=gshare->prediction();
        
        }
        else
        {
            gshare_predict=predict;
            bimodal->getIndex(dec);
            bimodal_predict=bimodal->prediction();
        }
        
        
        
        //cout<<"Bimodal predict"<<bimodal_predict<<endl;
        
        //cout<<"Gshare predict"<<gshare_predict<<endl;
        if(gshare_predict==true_output && bimodal_predict!=true_output)
        {
            if(chooser[index]==3)
            {
                chooser[index]=3;
            }
            else
            {
                chooser[index]=chooser[index]+1;
            }
        }
        if(bimodal_predict==true_output && gshare_predict!=true_output)
        {
            //cout<<"Hi"<<endl;
            if(chooser[index]==0)
            {
                chooser[index]=0;
            }
            else
            {
                chooser[index]=chooser[index]-1;
            }
            
        }
        //cout<<"Updated chooser"<<chooser[index]<<endl;
            
        
    }
    void updateBHR(string true_output)
    {
        //cout<<"BHR Update"<<endl;
        gshare->updateBHR(true_output);

    }

    void print()
    {
        cout<<"COMMAND\n";
        cout<<"./sim hybrid "<<k<<" "<<m1<<" "<<n<<" "<<m2<<" "<<tracefile<<endl;
    }

    float MisPredictionRate()
    {
        return ((float)mispredictions/(float)prediction_count)*100.00 ;
        
    }

    void printFinalContent()
    {
        cout<<"FINAL CHOOSER CONTENTS\n";
        for(int i=0;i<chooser.size();++i)
        {
            cout<<i<<"      "<<chooser[i]<<"\n";
        }
        gshare->printFinalContent();
        bimodal->printFinalContent();

    }
};



int main(int argc, char* argv[])
{
    string option=argv[1];
    if(option=="smith")
    {
        int b=stoi(argv[2]);
        Smith s(b,argv[3]);
    
        s.initial_state();
        s.actual_outcome();

    }
    if(option=="bimodal")
    {
        int m=atoi(argv[2]);
        Bimodal b(m,argv[3]);
        b.fillPredictionTable();
    
        b.readfile();
    }
    if(option=="gshare")
    {
        int m1=atoi(argv[2]);
        int n=atoi(argv[3]);
        Gshare g(m1,n,argv[4]);
        g.fillPredictionTable();
        g.fillBHR();
        g.readfile();
    
    }
    if(option=="hybrid")
    {
        int k=atoi(argv[2]);
        int m1=atoi(argv[3]);
        int n=atoi(argv[4]);
        int m2=atoi(argv[5]);
        Hybrid h(k,m1,n,m2,argv[6]);
        Gshare gshare(m1,n,argv[6]);
        Bimodal bimodal(m2,argv[6]);
        
        
    
        h.fillChooserTable();
        h.readfile();
    }
}


    
    
    

    

 
