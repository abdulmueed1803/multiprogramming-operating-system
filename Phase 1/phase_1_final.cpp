
#include <iostream>
#include <string>
#include <fstream> 

using namespace std;

class OS
{   
    
    char M[100][4];
    char IR[4];
    char R[4];
    char buf[40];
    string st; 
    short int IC,SI;          
    bool C;         
       
    public:
        ifstream Input;
	    ofstream Output;


	    void init(){
            // cout <<endl<<"Inside init function to re-initialise the memory....."<<endl;
            for (int i = 0; i < 100; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    M[i][j]='\0'; 
                }
            }
            IR[4]={'\0'};
            R[4]={'\0'};
            C=false;
        }

	    void STARTEXECUTION(){
            // cout <<"STARTEXECUTION() fun. executing...." << endl;
            IC = 0; 
            EXECUTEUSERPROGRAM();
        }

        
	    void EXECUTEUSERPROGRAM(){
            
            while(1){
            
            //IR ke andar M[IC] daal denge
            for(int i=0; i<4; i++){
                IR[i] = M[IC][i];
            }

            IC++ ;

            // pahle memory address ki value nikaalenge
            int address = IR[2]-'0';
            address *= 10;
            address += (IR[3]-'0');

            if(IR[0] == 'L' && IR[1] == 'R'){
                // cout <<"Execeuting LR and loading the contents to R....." << endl;
                // uske baad R ke andar jo M[address] hai wo daal denge
                for(int i=0 ; i<4; i++){
                    R[i] = M[address][i];
                }
                // cout << "Contents of R after loading from the main memory: " ;
                for(int i=0 ; i<4; i++){
                    cout << R[i] ;
                }cout << endl;

                continue;
            }
            

            else if(IR[0] == 'S' && IR[1] == 'R'){
                // cout <<"Executing SR and loading contents from R to main memory...." << endl;
                //uske baad jo R mein pada hai wo M[address] mein daal denge
                // cout << "Contents of memory after loading SR to the memory address "<< address <<": ";
                for(int i=0; i<4; i++){
                    M[address][i] = R[i];
                    // cout << M[address][i];
                }  // cout << endl;

                continue;
            }

            else if(IR[0] == 'C' && IR[1] == 'R'){
                // cout <<"Executing CR...." << endl;
                int i=0;
                while(i < 4){
                if(M[address][i] != R[i])
                    break;
                i++;
                }

                //agar loop poora complete kar liya toh matlab dono barabar hain
                if(i==4)
                    C = true;

                continue;
            }

            if(IR[0] == 'B' && IR[1] == 'T'){
                //cout <<"Executing BT...." << endl;
                  if(C)
                    IC = address;

                continue;
            }

            else if(IR[0] == 'G' && IR[1] == 'D'){
                SI = 1;
                MOS(SI);

                continue;
            }

            else if(IR[0] == 'P' && IR[1] == 'D'){
                SI = 2;
                MOS(SI);

                continue;
            }

            else if(IR[0] == 'H'){
                SI = 3;
                MOS(SI);
                break;
            }

            else
                break;
        
            
        }
            
    }


        
	    void MOS(int SI){
            switch(SI){
                case 1:
                    READ();
                    break;
                case 2:
                    WRITE();
                    break;
                case 3:
                    TERMINATE();
                    break;
            }

            

        }
        void LOAD();
	    void READ();
	    void WRITE();
	    void TERMINATE();
	    
}; 
// yahan par class khatam hoti hai
//jo functions bade hain unki definition baahar likhi hai 
// aur jo zyada bade nahi hain unki andar

void OS::LOAD()
{
    
    int m=0;
    string st;
    while(!Input.eof())
    {   
        for(int i=0; i<40 ;i++){
            buf[i]='\0';
        } 
        getline(Input,st);

        int limit;
        if(st.size() > 40)
            limit = 40;
        else
            limit = st.size();

        for(int i=0; i<limit ; i++){
            buf[i]=st[i];
        }       
        
        /* cout << "Contents of Buffer: " ;
        for(int i=0; i<40; i++){
            cout << buf[i];
        } 
        cout << endl; */

        if(buf[0] == '$' && buf[1] == 'A' && buf[2] == 'M' && buf[3] == 'J')
        {   m=0;
            continue ;
        }


        else if(buf[0] == '$' && buf[1] == 'D' && buf[2] == 'T' && buf[3] == 'A')
        {
            STARTEXECUTION();        
        }


        else if(buf[0] == '$' && buf[1] == 'E' && buf[2] == 'N' && buf[3] == 'D')
        {  
            continue; 
        }


        else 
        {   //if m is greater than or equal to 100 we must abort our function 
            if(m >= 100)
                break;

            cout << "The value of memory address is...  " << m << endl;
            
            int b = 0; // yeh buf ki position hai
            for(int i=m;  i<m+10 ;i++)
            {   
                for(int j=0;j<4;j++)
                {
                    
                    M[i][j]=buf[b];
                    b++;
                    cout << M[i][j] ;

                }
                cout << " " <<i ;
                cout << endl;
                //yeh humari breaking condition hai
                // if(M[i][0] == 'H' || b >= 40)
                    // break;
                
            } 
            m=m+10;
        }

    }
}        


void OS::READ(){
   //  cout <<"Read function executing....." << endl;
    IR[4] ='0';                //agar 4th byte 0 nahi hai toh bana denge
    int address = IR[2] -'0'; //int value nikaali 3rd index ki
    address *= 10;           // usko 10 se multiply kiya and aur isi address par memory mein load karna hai
    // cout << "bhaiya GD ka address hai " << address << endl;           

    //agar address 100 ya usse zyada hua toh baahar chale jaayenge
    if(address >= 100){
        cout << "The memory is full!!" << endl;
        return;
    }

    //buffer mein input file se read karne ke liye  
    string st;
    for(int i=0; i<40 ;i++){
            buf[i]='\0';
        } 
        getline(Input,st);

        int limit;
        if(st.size() > 40)
            limit = 40;
        else
            limit = st.size();

        //st ki value buffer mein daal denge
        for(int i=0; i<limit ; i++){
            buf[i]=st[i];
        }
    
        //agar buffer mein $END hua toh return kar jaayenge
        if(buf[0] =='$' && buf[1] =='E' && buf[2] =='N' && buf[3] =='D'){
            return;
        }


    /* cout << "Contents of READ() buffer: ";
    for(int i=0; i<40; i++){
        cout << buf[i];
    }cout <<endl; */


    //ab jo bhi buf ki values hain wo hum main memory mein address pe daal denge
    int b=0;
    for(int i=address; i< address+10; i++){
        for(int j=0; j<4; j++){
            M[i][j] = buf[b];
            b++;
            cout << M[i][j] ;
        }
        cout << " " << i;
        cout << endl;
        
    }


    SI=0;
    
}

void OS::WRITE(){
    // cout <<"Write function executing....." << endl;
    IR[4] ='0';
    int address = IR[2]-'0';
    address *= 10;

    // ab hum memory mein address par se values nikaal ke output file mein daal denge
    for(int i= address; i< address+10; i++){
        for(int j=0; j<4; j++){
            Output << M[i][j] ;
        }
    }
    Output<<endl;


    SI=0;
    
}

void OS::TERMINATE(){
   // cout <<"Terminate function executing....." << endl;
    Output << endl << endl;

    SI=0;
    init(); // init function call kar denge load se pahle so that the memory gets empty again
    LOAD();
}


int main(){
    OS os;

    os.Input.open("input_Phase1.txt");
    os.Output.open("output_Phase1.txt");

    if(!os.Input)
    {
        cout<<"Soryy!!!, Failed To Open File !! "<<endl;
        return 0;
    }
    else
    {
        cout<<"The file exists"<<endl;
    }

    os.init();
    os.LOAD();
    cout<<"\nExecution  is done!! Kindly check the output file for the results!!"<<endl <<endl <<endl;

    return 0;
}