#include<stdio.h>
#include<bits/stdc++.h>
#include<string>
using namespace std;
//inputFun()
//makeNode()
//insertNgram()
//outputTree()
//processPass()

struct node{
  int ct;
   struct node* link[300];
};

struct node* root;

struct node* makeNode(){//create new node and initialise the elements.
   struct node* tmp=(struct node*)malloc(sizeof(struct node));
   tmp->ct=0;
   for(int i=0;i<300;i++){
       tmp->link[i]=NULL;
   }
   return tmp;
}

string int2str(int x){
     string tmp="";
     if(!x){
        tmp+='0';
     }
     while(x){
         tmp+= (char)('0'+(x%10));
         x/=10;
     }
     reverse(tmp.begin(),tmp.end());
   return tmp;
}

void printChains(string tmp,struct node* now){
     int f=0;
     for(int i=0;i<300;i++){
         if(now->link[i]!=NULL){
            f++;
            printChains(tmp+(char)i,now->link[i]);
         }
     }
     if(!f){
        cout<<tmp<<" "<<now->ct<<endl;
     }
     
     return ;
}

double query(string s,int sz){
    
    struct node* tmp=root;
    double num,deno;
    for(int i=0;i<sz-1;i++){
       tmp=tmp->link[(int)s[i]];
    } 
    deno=tmp->ct;
    tmp=tmp->link[(int)s[sz-1]];
    num=tmp->ct;
   
    return (num/deno);    
}

void testPass(int order){
	string in_pass,dummy,tmp,window;
	cout<<"password: ";
	cin>>in_pass;
	cout<<"\n";
	double total_prob=1;
	int window_size=order+1;
	vector<double> prob_char;
	double tmp_prob;
	dummy=createDummy(order);
	tmp=dummy+in_pass+"+";
	for(int i=0;i<=(int)tmp.size()-window_size;i++){
	    window=pass.substr(pos,window_size);
            tmp_prob=query(window,window_size);         
	    prob_char.push_bakc(tmp_prob);
	    total_prob*=tmp_prob;
	}		
     
     	cout<<"probability of password= "<<total_prob<<endl;

}

void  insertNgram(string s,int sz){
   
    struct node* tmp=root;
    for(int i=0;i<sz;i++){
        if(tmp->link[(int)s[i]]==NULL){
            tmp->link[(int)s[i]]=makeNode();
        }
       tmp=tmp->link[(int)s[i]];
       tmp->ct++;
    }

}

string createDummy(int order){
   string tmp="";
   for(int i=0;i<order;i++){
      tmp+="+";
   }
  return tmp;
}


void inputTrain(int order){
    //input the strings from the target files and insert the N-grams in the model.
     ifstream input;
     string file_name,pass,tmp,window,dummy;
     dummy=createDummy(order);
     int window_size=order+1;
     
     for(int fno=0;fno<1;fno++){
       file_name="input"+int2str(fno);
       input.open(file_name.c_str(),ifstream::in);
        int tmp_ct=0;
         while(tmp_ct<3){
                 cin >> pass;
                 pass=dummy+pass+"+";
                 for(int pos=0;pos <=(int)pass.size()-window_size;pos++){
                     window=pass.substr(pos,window_size);
                     insertNgram(window,window_size);
                     cout<<"window="<<window<<endl;
                 }
               tmp_ct++;  
         }
       input.close();  
     
     }

}

int main(){
	int order=3;
	root=makeNode();
	inputTrain(order);
	cout<<"callinf print\n";
	printChains("",root);
	testPass(order);	               

return 0;
}
