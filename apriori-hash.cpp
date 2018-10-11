#include <bits/stdc++.h>

#define max 50
#define min_sup 2
using namespace std;

int search(int a[], int n, int k)
{
	for(int i=0; i<n; i++)
		if(a[i] == k)
			return 1;
	return 0;	
}

int search(vector<int> a, int k)
{
	for(int i=0; i<a.size(); i++)
		if(a[i] == k)
			return i;
	return -1;	
}

int checkEq(vector<int> a, vector<int> b)
{
	if(a.size() != b.size())
		return 0;
	sort(a.begin(), a.end());
	sort(b.begin(), b.end());
	for(int i=0; i<a.size(); i++)
		if(a[i] != b[i])
			return 0;
	return 1;
}

void transacwrite()
{
	srand(time(NULL));
	fstream f("transactions.txt", ios::out);
	int start = rand() % 100;
	for(int i=0; i<rand() % 150; i++)
	{
		int items[max] = {0}, no = rand() % max + 1, count = 0;
		while(count < no)
		{
			int item = rand() % 100;
			if(search(items, no, item) == 0 && item != 0) 
				items[count++] = item;
		}
		sort(items, items+no);
		f<<'T'<<start<<' ';
		for(int i=0; i<no; i++)
			f<<", "<<items[i]<<' ';
		f<<'|'<<endl;
		start++;
	}
	f.close();
} 

map<int, vector<int> > transacread()
{
	fstream f("transactions.txt", ios::in);
	char c1, c2;
	map<int, vector<int> > m;
	
	while(!f.eof())
	{
		
		int tid, num;
		f>>c1>>tid>>c2;
		int item;
		while(c1 != '|')
		{
			f>>item>>c1;
			m[item].push_back(tid);
		}
	}
	
	for(auto i = m.begin(); i != m.end(); i++)
	{
		cout<<(*i).first<<" : ";
		vector <int> a = (*i).second;
		for(int j=0; j < a.size(); j++)
			cout<<a[j]<<", ";
		cout<<endl;
	}
	f.close();
	return m;
}

map<vector<int>, int> Cn(map<int, vector<vector<int> > > Hn_1, int n, map <int , vector<int> > inp)
{
	if(n == 0)
	{
		map <vector<int>, int> a;
		for(auto j = inp.begin(); j != inp.end(); j++)
		{
			vector <int> b;
			b.push_back((*j).first);
			a[b] = (*j).second.size();
		}
		return a;
	} else if(n == 1)
	{
		map<vector<int>, int> op;
		vector<vector <int> > cn;
		for(auto o = Hn_1.begin(); o != Hn_1.end(); o++)
			for(auto p = Hn_1.begin(); p != Hn_1.end(); p++)
				for(auto k = (*o).second.begin(); k != (*o).second.end(); k++)
					for(auto l = (*p).second.begin(); l != (*p).second.end(); l++)
						if(!checkEq((*l), (*k)))
						{
							vector<int> i = (*k), j = (*l);
							for(int m = 0; m < i.size(); m++)
								if(search(j, i[m]) == -1)
									j.push_back(i[m]);
							cn.push_back(j);
						}
		
			for(auto i = cn.begin(); i != cn.end(); i++)
			{
				vector <int> k = inp[(*i)[0]]; 
				for(int j=1; j<(*i).size(); j++)
				{
					vector <int> boo = inp[(*i)[j]], ind;
					for(int m = 0; m < boo.size(); m++)
					{
						int f = search(k, boo[m]);
						if(f != -1)
							ind.push_back(f);	
					}
					vector <int> f_k;
					for(int m = 0; m < ind.size(); m++)
						f_k.push_back(k[ind[m]]);
					k = f_k;
				}
				op[(*i)] = k.size();
			}
			return op;
	} 
	
}

map<int, vector<vector<int> > > hashy(map<vector<int>, int> Cn)
{
	map<int, vector<vector<int> > > hn;
	for(auto j = Cn.begin(); j != Cn.end(); j++)
		if((*j).second >= min_sup)
		{
			if(hn.find((*j).second) == hn.end())
			{
				 vector<vector<int> > nw;
				 nw.push_back((*j).first);
				 hn[(*j).second] = nw;
				 continue;
			}
			
			vector<vector<int> > content = hn[(*j).second];
			int found = 0, i = ((*j).second*10 + (*j).first[0]) % 7;
			for(auto i = content.begin(); i != content.end() && found == 0; i++)
				found = checkEq((*i), (*j).first);
			if(found == 0)
				hn[(*j).second].push_back((*j).first);
		}
	return hn;
} 

void print(map<vector<int>, int> a, int n, char* type)
{
	char f_name[] = "./logs/", num[10];
	itoa(n, num, 10);
	strcat(f_name, type);
	strcat(f_name, num);
	strcat(f_name, ".txt"); //f_name = "./logs/" + type + n + ".txt"
	cout<<type<<" : "<<n<<endl;
	fstream f(f_name, ios::out);
	for(auto j = a.begin(); j != a.end(); j++)
	{
		if((*j).second < min_sup)
			continue;
		for(int i=0; i < (*j).first.size(); i++)
		{
			f<<(*j).first[i]<<", ";
			if(strcmp(type, "l") == 0)
				cout<<(*j).first[i]<<", ";
		}
		if(strcmp(type, "l") == 0)
			cout<<'\t'<<(*j).second<<endl;
		f<<'\t'<<(*j).second<<endl;
	}
	f.close();
}

int main()
{
	transacwrite();
	map <int, vector<int> > c_0 = transacread();
	map<vector<int>, int> c_curr;
	map<int, vector<vector<int> > > h_curr;
	int count = 0, d;
	
	c_curr = Cn(h_curr, count, c_0);
	h_curr = hashy(c_curr);
	print(c_curr, count +1, "c");
	count ++; 
	c_curr = Cn(h_curr, count, c_0);
	print(c_curr, count +1, "c");

	return 0;
}
