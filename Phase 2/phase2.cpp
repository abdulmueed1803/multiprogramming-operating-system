#include <iostream>
#include <cstdlib>
#include <time.h>
#include <string>
#include <fstream> 

using namespace std;

class PCB{
    public:
        int JID;
        int TTL;
        int TLL;
};
class OS
{
    char M[300][4],IR[4],R[4],buf[40]; 
    bool visited[30]; //for allocate function() to check whether the no generated is already present
    short int IC; 
    int SI, TI, PI;
    int TTC, LLC;
    int RA;//for calculation of real address 
    int PTR, movingPtr;        
    bool C;   
    bool flag = false;
    PCB pcb;      
       
    public:
        fstream Input;
	    fstream Output;

	    
        void init();
        void endprogram();
	    void LOAD();
        void STARTEXECUTION();
        void EXECUTEUSERPROGRAM();
	    void MOS();
	    void READ();
	    void WRITE();
	    void TERMINATE(int EM);
        void SIMULATION();
        int ADDRESSMAP(int VA);
        int allocate();
        void printmemory();

        
	    
}; 
// yahan par class khatam hoti hai
//jo functions bade hain unki definition baahar likhi hai 

void OS::init(){
    for(int i=0;i<300;i++){
        for(int j=0; j<4;j++){
            M[i][j] = '\0';
        }
    }

    for(int i=0; i<30;i++)visited[i]=0;//initialising visited with all zeros

    IR[4]={'\0'};
    R[4]={'\0'};
    TTC =0, LLC=0;
    SI=0, TI=0, PI=0;
    C=false;
    pcb.JID=0;
    pcb.TTL=0;
    pcb.TLL=0;


}

void OS::endprogram()
{
    Output<<"IC: "<<IC<<endl;
    Output<<"IR: ";
    for(int i=0; i<4;i++)Output<<IR[i];
    Output << endl;
    Output<<"TTC: "<<TTC<<endl;
    Output<<"LLC: "<<LLC<<endl;
    Output <<endl<<endl;

    cout<<"\nSI = "<<SI<<"  TI ="<<TI<<"  PI="<<PI<<endl;
    cout<<"TTC = "<<TTC<<"  LLC="<<LLC<<endl;
    
}


void OS::LOAD()
{   
    cout << "Inside Load function...."<<endl;
    init();
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
        cout << "Contents of buffer: ";
        for(int i=0; i<40;i++)cout <<buf[i];
        cout << endl;

        if(buf[0] == '$' && buf[1] == 'A' && buf[2] == 'M' && buf[3] == 'J')
        {   
            flag = false;
            //calculating JID
            for(int i=4;i<=7;i++)pcb.JID = (pcb.JID*10)+(buf[i]-'0');
            cout << "JID: " << pcb.JID<<endl;
            
            //calculating TTL
            for(int i=8;i<=11;i++)pcb.TTL = (pcb.TTL*10)+(buf[i]-'0');
            cout << "TTL: " << pcb.TTL<<endl;
            
            //calculating TLL
            for(int i=12;i<=15;i++)pcb.TLL =(pcb.TLL *10) + (buf[i]-'0');
            cout << "TLL: " << pcb.TLL<<endl;

            //taking a number number to store page table and initialising PTR
            int number = allocate();
            cout <<"Block for page table: "<<number<<endl;
            PTR = number*10;
            cout << "PTR: "<<PTR<<endl;
            movingPtr=PTR;
            for(int i=PTR;i<PTR+10;i++){
                M[i][0] ='0';
                M[i][2] ='*';
                M[i][3] ='*';
            }

        }


       else if(buf[0] == '$' && buf[1] == 'D' && buf[2] == 'T' && buf[3] == 'A'){
            if(flag == true) continue ;
            STARTEXECUTION();
       }

       else if(buf[0] == '$' && buf[1] == 'E' && buf[2] == 'N' && buf[3] == 'D'){
            cout <<"INSIDE $END"<<endl;
            continue;
       }

        //program card 
        else{
            //allocaing a number for frame and updating page table
            if(flag) continue;

            int number = allocate();
            cout << "Number allocated: "<<number<<endl;
            // updating page table
            M[movingPtr][0] ='1';
            M[movingPtr][2] = char((number/10)+48);
            M[movingPtr][3] = char((number%10)+48);
            movingPtr++;

            //uploading the program card into the main memory
            int k=0;
            for(int i=number*10; i<(number*10)+10; i++){
                for(int j=0;j<4;j++) M[i][j] = buf[k++] ;
            }

            //printmemory();
        }
       
    //while loop ka bracket
    }
    exit(0);
}        

void OS::STARTEXECUTION(){
    cout <<"Inside STARTEXECUTION()..."<<endl;
    IC = 0; 
    EXECUTEUSERPROGRAM();
}


void OS::EXECUTEUSERPROGRAM(){
    cout << "Inside EXECUTEUSERPROGRAM()......"<<endl;
    int tempPtr=PTR;
    int temp[2];
    int VA ;
    

    while(true){
    cout << "IC: "<<IC <<endl;
    RA = ADDRESSMAP(IC); 
    cout << "Returned RA of IC: "<<RA<<endl;
    for(int i=0;i<4;i++)
        IR[i] = M[RA][i];
    
    cout <<"Contents of IR: ";
    for(int i=0; i<4; i++)cout << IR[i];
    cout << endl;

    IC++;
    temp[0] = IR[2];
    temp[1] = IR[3];
    //if not a number then operand erroe
    if(( !isdigit(temp[0]) || !isdigit(temp[1]) ) ){
        PI = 2;
    } 
    else{
        VA = (IR[2]-'0')*10 + (IR[3] -'0');
    }

    cout <<"VA: "<<VA<<endl;
    RA = ADDRESSMAP(VA);
    cout << "Retuned RA of IR: "<<RA<<endl;

    if(IR[0] == 'G' && IR[1] == 'D'){
        SI = 1;
    } 

    else if(IR[0] == 'P' && IR[1] == 'D') {
        SI = 2;
    }

    else if(IR[0] == 'H' && IR[1] == '\0') {
        SI = 3;
        PI = 0 ;
    }

    else if(IR[0] == 'L' && IR[1] == 'R'){
        if(RA != 0){
            for(int i=0; i<4; i++)R[i] = M[RA][i];
            
        }
            

    }

    else if(IR[0] == 'S' && IR[1] == 'R'){
        if(PI == 0){
            for(int i=0; i<4; i++) M[RA][i] = R[i];
            
        }
            
    }

    else if(IR[0] == 'C' && IR[1] == 'R'){
        if(RA != 0){
            int i;
            for(i=0; i<4;i++){
                if(M[RA][i] != R[i]) {
                    C=false;
                    break;
                }
        }

        if(i==4) C=true;
        }
        
    }

    else if(IR[0] == 'B' && IR[1] == 'T'){
        if(RA != 0){
            if(C==true){
            IC = VA ;
        }
        
        }   
    }

    else {
        PI = 1;
    }

    SIMULATION();

    if(PI !=0 || SI != 0 || TI != 0){
        MOS();
    }

    //return bracket of while loop
}
    
    
   

    //return bracket of executeuserprogram
}

int OS::ADDRESSMAP(int VA){
    cout << "Inside Address map for finding the real address...."<<endl;
    if(VA >=0 && VA<100){
    int VA1 = VA/10;
    int VA2= VA%10;
    cout << "VA1: "<<VA1<<" and VA2: "<<VA2<<endl;

    if(M[PTR+VA1][2]=='*' && M[PTR+VA1][3] =='*'){
        cout <<"making PI = 3 "<<endl;
        PI = 3;
        return 0;
    } 

    // cout << M[PTR+VA1][2]<<endl;
    int frameNo = (M[PTR+VA1][2]-'0')*10 + (M[PTR+VA1][3]-'0');
    cout <<"frame number is: "<<frameNo<<endl;
    return (frameNo*10) + (VA2) ;
    }
    PI=2;
    return 0;
    
}


void OS::MOS(){
    cout << "Inside MOS()...."<<endl;
    cout <<"contents of IR while inside MOS(): ";
    for(int i=0; i<4; i++){
        cout << IR[i];
    }cout <<endl;
    cout << "TI: "<<TI<<endl;
    cout <<"PI: "<<PI<<endl;
    cout <<"SI: "<<SI<<endl;

    if(!TI){
        cout << "Inside TI=0...."<<endl;

        if(PI == 1)TERMINATE(4);

        else if(PI == 2)TERMINATE(5);

        else if(PI == 3){
            //if GD or SR, then valid fault and OS will have to handle it
            cout << "checking the page fault..."<<endl;
            if((IR[0] == 'G' && IR[1] == 'D') || (IR[0] == 'S' && IR[1] == 'R')){
                cout << "page fault valid....." << endl;
                TTC -= 1;
                int number = allocate();
                M[movingPtr][0]='1';
                M[movingPtr][2]=char((number/10)+48);
                M[movingPtr][3]=char((number%10)+48);
                cout <<"Location of "<< IR[0]<<IR[1] << ": " <<M[movingPtr][2]<<M[movingPtr][3]<<endl;
                movingPtr++ ;

                PI=0;
                IC--;

                EXECUTEUSERPROGRAM();
            
                    
            }


            else 
                TERMINATE(6);
        }

        else if(SI == 1)READ();

        else if(SI == 2)WRITE();

        else if(SI == 3){
            cout << "Terminate(0) called......"<<endl;
            TERMINATE(0);
            }
    }

    else{
        if(PI == 1)TERMINATE(7);

        else if(PI == 2)TERMINATE(8);

        else if(PI == 3){
            cout << "Inside PI = 3"<<endl;
            TERMINATE(3);
        }

        if(SI == 1)TERMINATE(3);

        else if(SI == 2){
            //pahle jo buffer mein hai wo output file mein likho
            TERMINATE(3);
        }

        else if(SI == 3)TERMINATE(0);
        
    }
//return bracket of mos
}


void OS::READ(){
    cout << "Inside Read Function..." << endl;

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

    cout <<"buffer Inside read(): ";
    for(int i=0; i<40; i++) cout << buf[i];
    cout << endl;

    //ab jo bhi buf ki values hain wo hum main memory mein address pe daal denge
    if(buf[0] == '$' && buf[1] =='E' && buf[2] =='N' && buf[3] == 'D')TERMINATE(1);

    
    int temp = RA ;
    cout << "temp: "<<temp<<endl;
    cout << "reading from input file and writing into the memory......"<<endl;
    int n=0;
    for(int i=temp; i<temp+10;i++){
        for(int j=0; j<4;j++){
            if(buf[n] == '\0') break;
            M[i][j] = buf[n];
            n++;
        }
    }
    cout <<"contents of memory......"<<endl;
    for(int i=temp;i<temp+10;i++){
        cout << "M["<<i<<"]: ";
        for(int j=0; j<4; j++){
            cout << M[i][j];
        }cout << endl;
    }cout <<endl;

    cout << "just above EXECUTEUSERPROGRAM() of READ()....."<<endl;

    SI =0 ;
    // EXECUTEUSERPROGRAM();

    //return bracket of read function
}

void OS::WRITE(){
    cout << "Inside Write function....."<<endl;

    LLC += 1;
    if(LLC > pcb.TLL)TERMINATE(2);


    cout << "RA: "<<RA<<endl;
    cout <<"contents of memory while writing..."<<endl;
    for(int i=RA;i<RA+10;i++){
        cout << "M["<<i<<"]";
        for(int j=0; j<4;j++) {
            cout << M[i][j];
        }
        cout <<endl;
    }cout << endl;
    // we will write the contents into Output file from memory location RA through RA+9
    cout <<"writing to the output file.."<<endl;
    for(int i=RA; i<RA+10;i++){
        for(int j=0; j<4;j++){
            Output << M[i][j] ;
        }
    }
    Output<<endl;
    cout << "written in output file.."<<endl;
    cout << "just above EXECUTEUSERPROGRAM() of WRITE()....."<<endl;
    SI=0 ;
    // EXECUTEUSERPROGRAM();
}

void OS::TERMINATE(int EM){
    cout << "Inside Terminate Function...." << endl;
    Output << "JID: " << pcb.JID <<endl;

    if(EM !=0) TTC -= 1 ;
    flag = true ;
    switch (EM)
    {
    case 0:
        Output <<" NO ERROR!!"<<endl;
        break;
    case 1:
        Output <<" OUT OF DATA!!"<<endl;
        break;
    case 2:
        Output <<" LINE LIMIT EXCEEDED!!"<<endl;
        break;
    case 3:
        Output <<" TIME LIMIT EXCEEDED!!"<<endl;
        break;
    case 4:
        Output <<" OPERATION CODE ERROR!!"<<endl;
        break;
    case 5:
        Output <<"OPERAND ERROR!!"<<endl;
        break;
    case 6:
        Output <<" INVALID PAGE FAULT!!"<<endl;
        break;
    case 7:
        Output <<"TIME LIMIT EXCEEDED And OPERATION CODE ERROR!!"<<endl;
        break;
    case 8:
        Output <<"TIME LIMIT EXCEEDED And OPERAND ERROR!!"<<endl;
        break;
    default:
        break;
    }
    endprogram();
    SI=0,TI=0,PI=0;

    LOAD();
}


void OS::SIMULATION(){
    TTC += 1;
    if(TTC > pcb.TTL)TI=2;
}


int OS::allocate(){
    //srand(time(0));
    int i;
    while(true){
        i= rand()%30;
        if(visited[i] ==1)continue;
        else break;
    }
    visited[i] = 1;
    return i;
}

void OS:: printmemory(){
    for(int i=0; i<300; i++){
        cout << "M["<<i<<"]:  ";
        for(int j=0; j<4; j++) cout <<M[i][j];
        cout <<endl;
    }cout << endl<<endl;
}


int main(){
    OS os;

    os.Input.open("input_phase_2.txt", ios::in);
    os.Output.open("output_phase_2.txt", ios::out);

    if(!os.Input)
    {
        cout<<"Soryy!!!, Failed To Open File !! "<<endl;
    }
    else
    {
        cout<<"The file exists"<<endl;
    }


    os.LOAD();
    os.printmemory();
    cout<<"\n Execution  is done!! check the output file for the results!!"<<endl;

    return 0;
}