#include <bits/stdc++.h>
using namespace std;
class Chaskey{
public:
	Chaskey(string Message,string Key,int tl){
		this->Message=Message;
		this->Key=bitset<128>(Key);
		this->tagL=tl;
		 n=Message.length()/128;
	}
private:
	string Message;
	int flag=1;
	bitset <128 > Key;
	bitset <128> Key1,Key2;
	int tagL,n;
	
	
	
	//bitset <128> M[n]={};
	vector < bitset<128> > M;
	void partition_Message();
	void SubKeys();
	bitset <128> Times_Two(bitset<128> &K);
	//bitset <128> Pi_permutation();
	unsigned int  Shift(unsigned int B,int shif);
	bitset<128> Pi_permutation(bitset <128> Bs);
public:
	string  Run();

};

void Chaskey :: partition_Message()
{
	int low=0;
	int high=128;
	for (int i=0;i<n;i++)
	{
		string s="";
		for (int j=low;j<high && Message[j]!='\0';j++)
		{
			s+=Message[j];
		}
		if (s.length()==128)
		{
			M.push_back(bitset <128 >(s));
		}
		else
		{
			flag=2;
			s+="1";
			int sn=s.length();
			for (int k=sn;k<128;k++)
				s+="0";

			M[i]=bitset <128 >(s);

		}
		low=high;
		high+=128;
	}
}
void Chaskey :: SubKeys()
{
	Key1= Times_Two(Key);
	Key2= Times_Two(Key1);
}
bitset <128> Chaskey :: Times_Two(bitset<128> &K)
{
	bitset < 128 > Modulo(0);
	if (K.test(127)==false)
	{
		K=(K<<1)^Modulo;
		return K;
	}
	else{
		Modulo=Modulo^(bitset<128>("10000111"));
		K=(K<<1)^Modulo;
		return K;
	}


}
unsigned int Chaskey :: Shift(unsigned int B,int shif)
{
	string temps=bitset<32>(B).to_string();//right shift
	char temps2[32]={0};
	for (int i=n;i>=0;i--)
	{
		if (i-shif>=0)
		temps2[i]=temps[i-shif];
		else
			temps2[i]=temps[i-shif+n];
	}
	return (unsigned int)bitset<32>(temps2).to_ulong();

}

bitset<128> Chaskey:: Pi_permutation(bitset <128> Bs)
{
	//unsigned int Mod=1<<32;
	string Bit=Bs.to_string();
	string x0,x1,x2,x3;
	for (int i=0;i<32;i++)
	{
		x0+=Bit[i];
	}
	for (int i=32;i<64;i++)
	{
		x1+=Bit[i];
	}
	for (int i=64;i<96;i++)
	{
		x2+=Bit[i];
	}
	for (unsigned int i=96;i<Bit.size();i++)
	{
		x3+=Bit[i];
	}
	
	unsigned  int v0,v1,v2,v3,tempv0,tempv1,tempv2,tempv3;
	v0=bitset<32>(x0).to_ulong();
	v1=bitset<32>(x1).to_ulong();
	v2=bitset<32>(x2).to_ulong();
	v3=bitset<32>(x3).to_ulong();
	/*
	NO need to use Modulo as unsigned int get wrap around 2^32 
	link--->https://stackoverflow.com/questions/9193880/overflowing-of-unsigned-int
	*/

	tempv0=Shift((v1+v2),16)+(Shift(v3,8)^(v3+v2));
	tempv1=Shift((Shift(v1,5)^(v1+v0)),7)^((v3+v2)+(Shift(v1,5)^(v1+v0)));
	tempv2=Shift(((v3+v2)+(Shift(v1,5)^(v1+v0))),16);
	tempv3=(Shift((v1+v0),16)+Shift(v3,8))^(v3+v2)^Shift(Shift(v3,8)^(v3+v2),13);
	string temps="";

	temps+=bitset<32>(tempv0).to_string();
	temps+=bitset<32>(tempv1).to_string();
	temps+=bitset<32>(tempv2).to_string();
	temps+=bitset<32>(tempv3).to_string();
	return bitset<128>(temps);

}
string Chaskey :: Run()
{
	bitset<128> h;
	h=Key;
	bitset<128> Li;

	SubKeys();

	partition_Message();

	for (int i=1;i<n-1;i++)
	{
		h=Pi_permutation(h^M[i]);

	}

	/*
	checking size of ml is equal to n
	*/
	if (flag==1)
	{
		Li=Key1;
	}
	else{
		Li=Key2;
	}

	h=Pi_permutation(h^Li^M[n-1]);
	string ts="";

	for (int j=128-tagL;j<128;j++){
		if (h.test(j)==true)
		ts+='1';
		else ts+='0';
	}
	//cout<<"d12\n";

	return (ts);

}
int main(int argc, char const *argv[])
{
	string K="";
	srand(time(NULL));
	for (int i=0;i<128;i++)
		K+='0'+rand()%2;
	string Mess="";
	for (int i=0;i<1024;i++)
		Mess+='0'+rand()%2;
	Chaskey chs(Mess,K,64);
	cout<<Mess<<" \n\n\n"<<K<<endl;
	string tag=chs.Run();
	

	cout<<endl<<tag<<endl;
	return 0;
}
