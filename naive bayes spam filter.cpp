#include<bits/stdc++.h>
#include<fstream>
using namespace std;

map<string,double> cnt_in_spam;
map<string,double> cnt_in_ham;
double spam_cnt,ham_cnt,prob_spam,prob_ham;
int words_in_spam,words_in_ham;
double findsgivenw(string word,double cnt)
{
	double ans,res;
	int i;
	if(cnt_in_spam[word]<=3.0 || cnt_in_ham[word]<=3.0)
	return 0.4;
	ans=((cnt_in_spam[word]/spam_cnt)/((cnt_in_spam[word]/spam_cnt)+(cnt_in_ham[word]/ham_cnt)));
	res=ans;
	return res;
}
int main()
{
	bool isspam,may_be_spam;
	int err_in_spam=0,err_in_ham=0,spam_in_test=0,ham_in_test=0,words_in_spam=0,words_in_ham=0,i;
	double res,numer,denom,cnt,word_cnt,threshold=0.9,prob_given_spam,accuracy;
	spam_cnt=0.0;ham_cnt=0.0;
	string str,word;
	ifstream fin("train.txt");
	while(fin>>str)
	{
		if(str[0]=='/');
		else if(str=="ham")
		{
		    ham_cnt=ham_cnt+1;
		    isspam=0;
		}
		else if(str=="spam")
		{
		    spam_cnt=spam_cnt+1;
		    isspam=1;
		}
		else
		{
			fin>>cnt;
			if(isspam==1)
			{
				cnt_in_spam[str]=cnt_in_spam[str]+cnt;
				words_in_spam=words_in_spam+cnt;
			}
			else
			{
				cnt_in_ham[str]=cnt_in_ham[str]+cnt;
				words_in_ham=words_in_ham+cnt;
			}
		}
	}
	fin.close();
	ifstream gin("test.txt");
	isspam=0;
	prob_spam=0.5;
	prob_ham=1.0-prob_spam;
	double max_prob_spam=-1,max_prob_ham=-1,min_prob_spam=2,min_prob_ham=2;
	while(gin>>word)
	{
		numer=1.0;
	    denom=1.0;
		if(word[0]=='/') ;
		else
		{
			if(word=="spam")
			isspam=1;
			else isspam=0;
			while(gin>>word)
			{
				if(word[0]=='/')
				break;
				gin>>word_cnt;
				res=findsgivenw(word,word_cnt);
				numer=numer*(res/prob_spam);
				for(i=0;i<word_cnt-1;i++)
				numer=numer*res/prob_spam;
				denom=denom*((1.0-res)/prob_ham);
				for(i=0;i<word_cnt-1;i++)
				denom=denom*(1.0-res)/prob_ham;
			}
			numer*=prob_spam;
			denom*=prob_ham;
			prob_given_spam=(numer/(numer+denom));
			if(prob_given_spam>threshold)
			may_be_spam=1;
			else may_be_spam=0;
			if(isspam==1)
			{
				spam_in_test++;
				max_prob_spam=max(max_prob_spam,prob_given_spam);
				min_prob_spam=min(min_prob_spam,prob_given_spam);
			}
			else
			{
				ham_in_test++;
				max_prob_ham=max(max_prob_ham,prob_given_spam);
				min_prob_ham=min(min_prob_ham,prob_given_spam);
			}
			if(isspam!=may_be_spam)
			{
				if(isspam==1)
				err_in_spam++;
				else err_in_ham++;
			}
		}
	}
	gin.close();
	cout<<"\n";
	cout<<"ham count : "<<ham_cnt<<"\n";
	cout<<"Spam count : "<<spam_cnt<<"\n";
	cout<<"\n"<<"spams in test : "<<spam_in_test<<"\nham_in_test : "<<ham_in_test<<"\n";
	cout<<"\nerrors in spam predictions : "<<err_in_spam<<"\n";
	cout<<"\nerrors in ham predictions(false positives) : "<<err_in_ham<<"\n";
	accuracy =1.0 - (err_in_spam + err_in_ham)/((double)spam_in_test + ham_in_test);
	accuracy*=100.0;
	cout<<"\naccuracy = "<<accuracy<<"\n";
	return 0;
}
