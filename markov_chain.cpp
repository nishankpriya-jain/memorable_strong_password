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

struct node  *root,*combo_root;

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

long long int str2num(string s){
   long long int x=0;
   for(int i=0;i<s.size();i++){
        x=(x*10)+s[i]-'0';
   }
   
   return x;
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


void printTree(string tmp,struct node * now,string file_name){
     int f=0;
     for(int i=0;i<300;i++){
         if(now->link[i]!=NULL){
            f++;
            printTree(tmp+(char)i,now->link[i],file_name);
         }
     }
     if(!f){
        ofstream out;
        out.open(file_name.c_str(),ofstream::app);
        //cout<<tmp<<" "<<now->ct<<endl;
        out<<tmp<<" "<<now->ct<<endl;
        out.close();
     }
     
     return ;
}

string createDummy(int order){
   string tmp="";
   for(int i=0;i<order;i++){
      tmp+="+";
   }
  return tmp;
}

void deleteTree(struct node * now){
    for(int i=0;i<300;i++){
         if(now->link[i]!=NULL){
            deleteTree(now->link[i]);
         }
    }
    free(now);    
}

double query(struct node* now,string s,int sz){
    
    struct node* tmp=now;
    double num,deno;
    cout<<"in query "<<s<<" sz="<<sz<<endl;
    for(int i=0;i<sz-1;i++){
       cout<<"char now="<<s[i]<<" ";
       tmp=tmp->link[(int)s[i]];
    } 
    deno=tmp->ct;
    tmp=tmp->link[(int)s[sz-1]];
    num=tmp->ct;
    cout<<endl;
    cout<<"char= "<<s[sz-1]<<" ascii="<<(int)s[sz-1]<<" num="<<num<<" deno="<<deno<<" prob="<<(num/deno)<<endl;
    return (num/deno);    
}

void testPass(struct node* now,int order){
	string in_pass,dummy,tmp,window;
	cout<<"password: ";
	cin>>in_pass;
	cout<<"------------------------\n";
	double total_prob=1;
	int window_size=order+1;
	vector<double> prob_char;
	double tmp_prob;
	dummy=createDummy(order);
	tmp=dummy+in_pass;
	for(int pos=0;pos<=(int)tmp.size()-window_size;pos++){
	    window=tmp.substr(pos,window_size);
            tmp_prob=query(now,window,window_size);         
	    prob_char.push_back(tmp_prob);
	    total_prob*=tmp_prob;
	}		
     
     	cout<<"probability of password= "<<total_prob<<endl;

}

void  insertNgram(struct node* now,string s,int sz,long long int wt){
   
    struct node* tmp=now;
    for(int i=0;i<sz;i++){
        if(tmp->link[(int)s[i]]==NULL){
            tmp->link[(int)s[i]]=makeNode();
        }
       tmp=tmp->link[(int)s[i]];
       tmp->ct = tmp->ct +wt;
    }
    
}

void  insertNgram(struct node* now,string s,int sz,long long int wt1,long long int wt2){
    //cout<<"inserting "<<s<<" "<<wt1<<" "<<wt2<<endl;
    struct node* tmp=now;
    for(int i=0;i<sz;i++){
        if(tmp->link[(int)s[i]]==NULL){
            tmp->link[(int)s[i]]=makeNode();
        }
       tmp=tmp->link[(int)s[i]];
       if(i==sz-1){
       		tmp->ct = tmp->ct +wt1;
       }
       else{
       		tmp->ct = tmp->ct +wt2;
       }
      // cout<<"s[i]="<<s[i]<<" ct="<<tmp->ct<<endl;
    }
    
}



void inputTrain(int order){
    //input the strings from the target files and insert the N-grams in the model.
     ifstream input;
     string file_name,pass,tmp,r1,r2,window,dummy;
     dummy=createDummy(order);
     int window_size=order+1;
     long long int total_ct=0,tmp_ct=0,wt;      
     for(int fno=0;fno<20;fno++){
        if(fno==10 || fno==12 || fno==18)continue;
       file_name="input/input"+int2str(fno)+".txt";
       input.open(file_name.c_str(),ifstream::in);
       tmp_ct=0;
          while( input>>r1 ){
                  
                 if(fno>17){
                     input>>r2;
                     pass=r2;
                     wt=str2num(r1);   
                 }
                 else{
                     pass=r1;
                     wt=1LL;	  
                 }
                 
                 cout<<pass<<"wt= "<<wt<< " filename="<<file_name<<endl;
                 pass=dummy+pass;
                 for(int pos=0;pos <=(int)pass.size()-window_size;pos++){
                     window=pass.substr(pos,window_size);
                     insertNgram(root,window,window_size,wt);
                     //cout<<"window="<<window<<endl;
                 }
               tmp_ct++;
               total_ct++; 
               cout<<tmp_ct<<endl; 
         }
       input.close();
       printTree("",root,"output/tree.txt");
       deleteTree(root);  
       root=makeNode();
       cout<<"tmp_ct= "<<tmp_ct<<endl;
     }
     cout<<"total_ct= "<<total_ct<<endl;

}


void trainFromTree(string file_name){
	ifstream in;
	in.open(file_name.c_str(),ifstream::in);
	string s;
	long long int wt;
	while(in>>s){
	   in>>wt;
	   insertNgram(combo_root,s,s.size(),wt);
	}
	return;
}



int main(){
	int order=3;
	root=makeNode();
	combo_root=makeNode();
	//cout<<"calling training\n";
	//inputTrain(order);
	//cout<<"calling prints\n";
	//printChains("",root);
	
	//cout<<"printing to output/tree.txt\n";
	//printTree("",root,"output/tree.txt");
	//cout<<"Training from tree\n";
	//trainFromTree("output/tree.txt");
	//printChains("",combo_root);
	cout<<"Training from combo_tree\n";
	trainFromTree("output/combo_tree.txt");
	
	//cout<<"printing to output/combo_tree.txt\n";
	//printTree("",combo_root,"output/combo_tree.txt");
	for(int i=0;i<100;i++)
	testPass(combo_root,order);	               
return 0;
}
