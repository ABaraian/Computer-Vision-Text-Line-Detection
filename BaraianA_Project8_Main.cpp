#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <math.h>
using namespace std;
class boxNode{
    public:
        int boxType;
        int minR;
        int minC;
        int maxR;
        int maxC;
        boxNode* next;
        
        boxNode(int a,int b, int c, int d,int e, boxNode* n){
            boxType=a;
            minR=b;
            minC=c;
            maxR=d;
            maxC=e;
            next=n;
        }
};
class docImage{
    public:
        int thrVal;
        int numRows;
        int numCols;
        int minVal;
        int maxVal;
        int numStructRows;
        int numStructCols;
        int structMin;
        int structMax;
        int rowOrigin;
        int colOrigin;
        int** imgAry;
        int* structElem;
        int* HPP;
        int* VPP;
        int* binHPP;
        int* binVPP;
        int* morphHPP;
        int* morphVPP;
        boxNode* listHead;
        int runsHPP;
        int runsVPP;
        int readingDirection;

    //constructor
    void loadImage(ifstream& inFile,ifstream& structElemFile){
        structElem = new int[numStructRows];
       
        for(int row=0;row<numStructRows;row++){ //setting up structAry
           
                structElemFile>>structElem[row];
            
        }
        imgAry = new int*[numRows+2];
        for(int i=0;i<=numRows+1;i++){
            imgAry[i] = new int[numCols+2];
        }
       
        for(int row=0;row<=numRows+1;row++){
            for(int col=0;col<=numCols+1;col++){
                imgAry[row][col]=0;
            }
        }
        
        for(int row=1;row<=numRows;row++){  //setting up img ary
            for(int col=1;col<=numCols;col++){
                inFile>>imgAry[row][col];
            }
        }
        HPP= new int[numRows+2];
        VPP = new int[numCols+2];
        binHPP = new int[numRows+2];
        binVPP = new int[numCols+2];
        morphHPP = new int[numRows+2];
        morphVPP = new int[numCols+2];
        for(int i=0;i<=numRows+1;i++){
            HPP[i]=0;
            binHPP[i]=0;
            morphHPP[i]=0;
        }
         for(int i=0;i<=numCols+1;i++){
            VPP[i]=0;
            binVPP[i]=0;
            morphVPP[i]=0;
        }
    }
    void computePP(){
        for(int row=1;row<=numRows;row++){
            for(int col=1;col<=numCols;col++){
                if(imgAry[row][col]>0){
                    HPP[row-1]++;
                    VPP[col-1]++;
                }
            }
        }
    }
    void binaryThreshold(int reg[],int bin[],int size){
        for(int i=0;i<size;i++){
            if(reg[i]>=thrVal){
                bin[i]=1;
            }
        }
    }
    void printPP(int* a,ofstream& file,int size){
        for(int i=0;i<size;i++){
            file<<a[i]<<" ";
        }
        file<<endl;
    }
    boxNode* computeZoneBox(){
        int minR=1;
        int minC=1;
        int maxR=numRows;
        int maxC=numCols;
        while(binHPP[minR]==0 && minR<=numRows){
            if(binHPP[minR]==0){
                minR++;
            }
        }
        while(binHPP[maxR]==0 && maxR>=1){
            if(binHPP[maxR]==0){
                maxR--;
            }
        }
        while(binVPP[minC]==0 && minC<=numCols){
            if(binVPP[minC]==0){
                minC++;
            }
        }
         while(binVPP[minC]==0 && maxC>=1){
            if(binVPP[minC]==0){
                maxC--;
            }
        }

        boxNode* B = new boxNode(1,minR,minC,maxR,maxC,nullptr);
        return B;

    }
    void morphClosing(int* arr1,int* arr2,int size){
        int jOffset,cindex;
        bool match;
        int* temp = new int[size];
        for(int i=0;i<size;i++){
                        temp[i]=0;

        }
        for(int i=1;i<size;i++){
            if(arr1[i]==1){
                jOffset = i-colOrigin;
                while(jOffset<= i+colOrigin){
                    temp[jOffset]=1;
                    jOffset++;
                }
            }
        }
      
          for(int i=1;i<size;i++){
            

            if(temp[i]==1){
                match = true;
                            jOffset=i-colOrigin;
                while(match==true &&jOffset<=i+colOrigin){
                    if(temp[jOffset]==0){
                        match=false;
                    } 
                    jOffset++;
            }
            if(match){
                arr2[i]=1;
            }
            else{
                arr2[i]=0;
            }
            }
            
        }
       
    }
    void lsitInsert(boxNode* a){
        a->next=listHead->next;
        listHead->next=a;
    }
    int computePPruns(int* PP,int lastIndex ){
        int numRuns=0;
        int index=1;
        while(index<=lastIndex){
            while(PP[index]==0 && index<=lastIndex){
                if(PP[index]==0){
                    index++;
                }
            }
            while(PP[index]>0 && index<=lastIndex){
                if(PP[index]>0){
                    index++;
                }
            }
            numRuns++;
        }
        return numRuns;

    }
    void computeVerticalTextBox(boxNode* zBox){
        int minR = zBox->minR;
        int minC = zBox->minC;
        int maxR=zBox->maxR;
        int maxC=minC;
        int index=1;
        
        while(morphVPP[maxC]==0 && maxC<=numCols){
            if(morphVPP[maxC]==0){
                maxC++;
            }
        }
        
        minC=maxC;
        while(maxC<=numCols){
        while(morphVPP[maxC]>0 && maxC<=numCols){
            if(morphVPP[maxC]>0){
                maxC++;
            }
        }
        boxNode* B = new boxNode(2,minR,minC,maxR,maxC,nullptr);
        lsitInsert(B);
        minC=maxC;
        while(morphVPP[minC]==0 && minC<=numCols){
            if(morphVPP[minC]==0){
                minC++;
            }
        }
        maxC=minC;
        }
    }
    void computeHorizontalTextBox(boxNode* zBox){
        int minR = zBox->minR;
        int minC = zBox->minC;
        int maxR=minR;
        int maxC=zBox->maxC;
        int index=1;
        while(morphHPP[maxR]==0 && maxR<=numRows){
            if(morphHPP[maxR]==0){
                maxR++;
            }
        }
        minR=maxR;
        while(maxR<=numRows){
        while(morphHPP[maxR]>0 && maxR<=numRows){
            if(morphHPP[maxR]>0){
                maxR++;
            }
        }
        boxNode* B = new boxNode(2,minR,minC,maxR,maxC,nullptr);
        lsitInsert(B);
        minR=maxR;
        while(morphHPP[minR]==0 && minR<=numRows){
            if(morphHPP[minR]==0){
                minR++;
            }
        }
        maxR=minR;
        }
    }
    void computeDirection(ofstream& file){
        int factor =2; 
        int direction=0;
        if(runsHPP<=2&&runsVPP<=2){
            file<<"The zone may be a non-text zone"<<endl;
        }
        else if(runsHPP>=factor*runsVPP){
            file<<"The document reading direction is horizontal!"<<endl;
            readingDirection=1;
        }
        else if(runsVPP>=factor*runsHPP){
            file<<"The document reading direction is vertical"<<endl;
            readingDirection=2;
        }
        else{
            file<<"The zone may be a non-text zone"<<endl;
        }
        readingDirection;
    }
    void imgReformat(int** a,ofstream &file){
            int max=0;
           for(int row=1;row<numRows+2;row++){
                for(int col=1;col<numCols+2;col++){
                   if(max<a[row][col]){
                    max=a[row][col];
                   }
                  
                }
            
            }
            
            
           // file<<numRows<<" "<<numCols<<" "<<min<<" "<<max<<endl;
            string str = to_string(max);
            int width = str.length(), r=1,c=1,ww;
            
           
            while(r<=numRows){
                c=1;
                while(c<=numCols){
                    if(a[r][c]>0){
                    file<<a[r][c];
                    }
                    else{
                        file<<".";
                    }
                    str= to_string(a[r][c]);
                    ww=str.length();
                    
                    while(ww<=width){
                        file<<" ";
                        ww++;
                    }
                   
                    c++;
                }
                file<<endl;
                r++;
            }
            file<<endl;
           
        
        }
    void overlayBox(){
        boxNode* curr= listHead->next;
        int startR,endR,startC,endC,type;
        while(curr->boxType!=1){
            startC=curr->minC;
            endC=curr->maxC;
            startR=curr->minR;
            endR=curr->maxR;
            type=curr->boxType;
            for(int col=startC;col<=endC;col++){
                imgAry[startR][col]=type;
                imgAry[endR][col]=type;
            }
            for(int row=startR;row<=endR;row++){
                imgAry[row][endC]=type;
                imgAry[row][startC]=type;
            }
            curr=curr->next;
        }    }
    void printBox(ofstream& file){
        boxNode* curr= listHead->next;
        while(curr->next!=nullptr){
            file<<curr->boxType<<endl;
            file<<curr->minR<<" "<<curr->minC<<" "<<curr->maxR<<" "<<curr->maxC<<endl;
            curr=curr->next;
        }
    }
};
int main(int argc,const char* argv[]){
    ifstream inFile,structElemFile;
    ofstream outFile1,outFile2;
    int thrVal;

    inFile.open(argv[1]);
    thrVal=atoi(argv[2]);
    structElemFile.open(argv[3]);
    outFile1.open(argv[4]);
    outFile2.open(argv[5]);

    docImage documentImage;
    documentImage.thrVal=thrVal;
    inFile>>documentImage.numRows>>documentImage.numCols>>documentImage.minVal>>documentImage.maxVal;
    structElemFile>>documentImage.numStructRows>>documentImage.numStructCols>>documentImage.structMin>>documentImage.structMax;
    structElemFile>>documentImage.rowOrigin>>documentImage.colOrigin;

    documentImage.loadImage(inFile,structElemFile); //STEP 1
    outFile1<<"Below is the input image"<<endl;
    documentImage.imgReformat(documentImage.imgAry,outFile1);

    documentImage.computePP();  //STEP 2
    outFile2<<"Below is HPP"<<endl;
    documentImage.printPP(documentImage.HPP,outFile2,documentImage.numRows+2);
    outFile2<<"Below is VPP"<<endl;
    documentImage.printPP(documentImage.VPP,outFile2,documentImage.numCols+2);

    documentImage.binaryThreshold(documentImage.HPP,documentImage.binHPP,documentImage.numRows+2);  //STEP 3
    documentImage.binaryThreshold(documentImage.VPP,documentImage.binVPP,documentImage.numCols+2);
    outFile2<<"Below is binHPP"<<endl;
    documentImage.printPP(documentImage.binHPP,outFile2,documentImage.numRows+2);
    outFile2<<"Below is binVPP"<<endl;
    documentImage.printPP(documentImage.binVPP,outFile2,documentImage.numCols+2);

    documentImage.listHead = new boxNode(0,0,0,0,0,nullptr);
    boxNode* zBox = documentImage.computeZoneBox();
    documentImage.lsitInsert(zBox);
    outFile2<<"Below is the linked list after insert input zone box"<<endl;
    documentImage.printBox(outFile2);

    documentImage.morphClosing(documentImage.binHPP,documentImage.morphHPP,documentImage.numRows+2);
    documentImage.morphClosing(documentImage.binVPP,documentImage.morphVPP,documentImage.numCols+2);
    outFile2<<"Below is morphHPP, after performing morphCLosing on HPP"<<endl;
    documentImage.printPP(documentImage.morphHPP,outFile2,documentImage.numRows+2);
    outFile2<<"Below is morphVPP after performing morphClosing on VPP"<<endl;
    documentImage.printPP(documentImage.morphVPP,outFile2,documentImage.numCols+2);

    documentImage.runsHPP=documentImage.computePPruns(documentImage.morphHPP,documentImage.numRows);
    documentImage.runsVPP=documentImage.computePPruns(documentImage.morphVPP,documentImage.numCols);
    outFile2<<"The number of runs in morphHPP-runsHPP is "<<documentImage.runsHPP<<endl;
    outFile2<<"The number of runs in morphVPP-runsVPP is "<<documentImage.runsVPP<<endl;

    documentImage.computeDirection(outFile1);
    outFile2<<"readingDirection is "<<documentImage.readingDirection<<endl;
    if(documentImage.readingDirection==1){
        documentImage.computeHorizontalTextBox(documentImage.listHead->next);
    }
    else if(documentImage.readingDirection==2){
        documentImage.computeVerticalTextBox(documentImage.listHead->next);

    }
    documentImage.overlayBox();
        documentImage.imgReformat(documentImage.imgAry,outFile1);

    documentImage.printBox(outFile2);
}