#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//function for displaying the contents in the file
void display(FILE *fptr){
    char str = fgetc(fptr);
    while(str != EOF){
        printf("%c",str);
        str = fgetc(fptr);
    }
    fclose(fptr);
}

//function for passone
void passone(char label[10],char opcode[10],char operand[10],char code[10],char mnemonic[3]){
    FILE *finp , *finte , *fsym , *fop , *flen;
    int locctr , start , length;
    
    finp = fopen("input.txt","r");
    finte = fopen("intermediate.txt","w");
    fsym = fopen("symtab.txt","w");
    fop = fopen("optab.txt","r");
    flen = fopen("length.txt","w");
    
    fscanf(finp,"%s\t%s\t%s",label , opcode , operand); //take the first line of the input file
    
    if(strcmp(opcode,"START")== 0){ //if first line of input file is start then take the start position from operand and add it to start
        sscanf(operand , "%x",&start);
        locctr = start;//assign locctr as start
        fprintf(finte , "\t%s\t%s\t%s\n",label , opcode , operand); //print the first line of input to intermediate
        fscanf(finp,"%s\t%s\t%s",label , opcode , operand); //get the next line of input
    }
    else
        locctr=0; //if there is no starting address , then put locctr as 0
    
    while(strcmp(opcode,"END")!=0){//loop till input opcode is END
        fprintf(finte , "%x\t%s\t%s\t%s\n",locctr , label , opcode , operand); // print all the input line with locctr to intermediate file
        if(strcmp(label , "**") != 0){
            fprintf(fsym,"%s\t%x\n",label , locctr); //if the label is not ** then that means the line has a symbol . print that to the symtab
        }
    
    //checking whether the opcode is present in optab
    //iterate thru all the lines of optab and check whether code = opcode , if yes increment locctr by 3
        fscanf(fop,"%s\t%s",code , mnemonic);
        while(strcmp(code , "END")!= 0){
            if(strcmp(opcode,code)==0){
               locctr+=3;
               break;
            }
            fscanf(fop,"%s\t%s",code,mnemonic);
        }
    
    //incrementing locctr based on WORD , RESW , BYTE and RESB
    //if WORD , increase locctr by 3
    //if RESW , increase locctr by 3*operand
    //if BYTE , increase locctr by 1
    //if RESB , increase locctr by operand
    
        if(strcmp(opcode,"WORD")==0){
            locctr+=3;
        }
        else if(strcmp(opcode,"RESW")==0){
            locctr+=3*(atoi(operand));
        }
        
        else if(strcmp(opcode,"BYTE")==0){
            locctr++;
        }
        
        else if(strcmp(opcode,"RESB")==0){
            locctr+=atoi(operand);
        }
        
        fscanf(finp,"%s\t%s\t%s",label , opcode , operand);//taking the next line of the input file
    }
    
    fprintf(finte , "\t%s\t%s\t%s\n",label , opcode , operand);
    //finding length of the code
    length = locctr - start;
    fprintf(flen,"%x",length); //printing the length to length.txt
    
    //closing all the files
    fclose(finp);
    fclose(fop);
    fclose(finte);
    fclose(fsym);
    fclose(flen);
    
}

int main(){
    char label[10], opcode[10], operand[10];
    char code[10], mnemonic[3];
    FILE *finp1 , *finte1 , *fsym1 , *fop1 , *flen1;
    
    passone(label, opcode, operand, code, mnemonic);
    
    finp1 = fopen("input.txt","r");
    fop1 = fopen("optab.txt","r");
    finte1 = fopen("intermediate.txt","r");
    fsym1 = fopen("symtab.txt","r");
    flen1 = fopen("length.txt","r");
    
    printf("\nthe contents of input.txt\n");
    display(finp1);
    printf("\nthe contents of optab.txt\n");
    display(fop1);
    printf("\nthe contents of symtab.txt\n");
    display(fsym1);
    printf("\nthe contents of intermediate.txt\n");
    display(finte1);
    printf("\nthe length of the file in length.txt\n");
    display(flen1);
    
    return 0;
}



/*OUTPUT

the contents of input.txt
**              START           2000
**              LDA             FIVE
**              STA             ALPHA
**              LDCH            CHARZ
**              STCH            C1
ALPHA           RESW            2
FIVE            WORD            5
CHARZ           BYTE            C'Z'
C1              RESB            1
**              END             **
the contents of optab.txt
LDA     03
STA         0f
LDCH    53
STCH    57
END         *
the contents of symtab.txt
ALPHA   200c
FIVE    2012
CHARZ   2015
C1      2016

the contents of intermediate.txt
        **      START   2000
2000    **      LDA     FIVE
2003    **      STA     ALPHA
2006    **      LDCH    CHARZ
2009    **      STCH    C1
200c    ALPHA   RESW    2
2012    FIVE    WORD    5
2015    CHARZ   BYTE    C'Z'
2016    C1      RESB    1
        **      END     **

the length of the file in length.txt
17
*/
