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
   for(int i=(int)s.size()-1;i>=0;i--){
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


string createDummy(int order){
   string tmp="";
   for(int i=0;i<order;i++){
      tmp+="+";
   }
  return tmp;
}

double query(string s,int sz){
    
    struct node* tmp=root;
    double num,deno;
    cout<<s<<" sz="<<sz<<endl;
    for(int i=0;i<sz-1;i++){
       tmp=tmp->link[(int)s[i]];
    } 
    deno=tmp->ct;
    tmp=tmp->link[(int)s[sz-1]];
    num=tmp->ct;
    
    cout<<"char= "<<s[sz-1]<<" ascii="<<(int)s[sz-1]<<" num="<<num<<" deno="<<deno<<" prob="<<(num/deno)<<endl;
    return (num/deno);    
}

void testPass(int order){
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
            tmp_prob=query(window,window_size);         
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
    cout<<"inserting "<<s<<" "<<wt1<<" "<<wt2<<endl;
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
       cout<<"s[i]="<<s[i]<<" ct="<<tmp->ct<<endl;
    }
    
}



void inputTrain(int order){
    //input the strings from the target files and insert the N-grams in the model.
     ifstream input;
     string file_name,pass,tmp,window,dummy;
     dummy=createDummy(order);
     int window_size=order+1;
     FILE *fp;
     
     for(int fno=0;fno<1;fno++){
       //file_name="input"+int2str(fno);
       file_name="input/dummy.txt";
       input.open(file_name.c_str(),ifstream::in);
       //fp=fopen(file_name.c_str(),"r");
        int tmp_ct=0;
         //while(tmp_ct<3){
          while( input>>pass ){
         // while(fscanf(fp,"%s",pass.c_str()) ){
                 //cin >> pass;
                 cout<<pass<<endl;
                 pass=dummy+pass;
                 for(int pos=0;pos <=(int)pass.size()-window_size;pos++){
                     window=pass.substr(pos,window_size);
                     insertNgram(root,window,window_size,1LL);
                     //cout<<"window="<<window<<endl;
                 }
               tmp_ct++; 
               cout<<tmp_ct<<endl; 
         }
       input.close();
       //fclose(fp);  
      cout<<"tmp_ct= "<<tmp_ct<<endl;
     }

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
        cout<<tmp<<" "<<now->ct<<endl;
        out<<tmp<<" "<<now->ct<<endl;
        out.close();
     }
     
     return ;

}

void trainFromTree(){
	ifstream in;
	in.open("output/tree.txt",ifstream::in);
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
	inputTrain(order);
	cout<<"calling prints\n";
	//printChains("",root);
	
	cout<<"printing to output/tree.txt\n";
	printTree("",root,"output/tree.txt");
	cout<<"Training from tree\n";
	trainFromTree();
	cout<<"printing to output/combo_tree.txt\n";
	printTree("",combo_root,"output/combo_tree.txt");
	//testPass(order);	               
        
return 0;
}
