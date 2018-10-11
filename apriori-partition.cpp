#include <bits/stdc++.h>

#define max 100
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
	for(int i=0; i<rand() % 200; i++)
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

map<vector<int>, int> Cn(map<vector<int> , int> Ln_1, int n, map <int , vector<int> > inp)
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
	} else {
		map<vector<int>, int> op;
		vector <vector<int> > cn;
		for(auto o = Ln_1.begin(); o != Ln_1.end(); o++)
			for(auto p = Ln_1.begin(); p != Ln_1.end(); p++)
				if((*o).first != (*p).first)
				{
					vector<int> i = (*o).first, j = (*p).first;
					vector <int> k;
					for(int m = 0; m < i.size(); m++)
						if(search(k, i[m]) == -1)
							k.push_back(i[m]);
					
					int yay = 1;
					for(int po=0; po<n-1 && yay == 1; po++)
						if(i[po] != j[po])
							yay = 0;
					if(yay == 0)
						break;
					for(int m = 0; m < j.size(); m++)
						if(search(k, j[m]) == -1)
							k.push_back(j[m]);
					
					int exists = 0;
					for(int m = 0; m < cn.size() && exists == 0; m++)
						exists = checkEq(cn[m], k);
					if(exists == 0)
						cn.push_back(k);
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

map<vector<int>, int> Ln(map<vector<int>, int> Cn, int min_sup=2)
{
	map<vector<int>, int> ln;
	for(auto j = Cn.begin(); j != Cn.end(); j++)
		if((*j).second >= min_sup)
			ln[(*j).first] = (*j).second;
	return ln;
}

void print(map<vector<int>, int> a, int n, char* type)
{
	char f_name[] = "./logs/", num[10];
	itoa(n, num, 10);
	strcat(f_name, type);
	strcat(f_name, num);
	strcat(f_name, ".txt"); //f_name = "./logs/" + type + n + ".txt"
	fstream f(f_name, ios::out);
	cout<<type<<" : "<<n<<endl;
	for(auto j = a.begin(); j != a.end(); j++)
	{
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

map<vector<int>, int> apply_apriori(map<int, vector<int> > c_0, int min_sup)
{
	map<vector<int>, int> c_curr, l_curr;
	int count = 0, d;
	while(l_curr.size() != 0 || count == 0)
	{
		c_curr = Cn(l_curr, count, c_0);
		map<vector<int>, int> bo =  Ln(c_curr, min_sup);
		if(bo.size() == 0)
			break;
		l_curr = bo;
		count++;
		cout<<l_curr.size()<<endl;
	}
	cout<<endl;
	return l_curr;
}

void partition_apriori(map<int, vector<int> > transac_r, int n = 10, float min_sup = 0.12)
{
	int t_len = transac_r.size();
	map<vector<int>, int> l_tot;	
	
	for(int i=0; i<t_len; i += n)
	{
		map<int, vector<int> > partition;
		
		int c = 0; 
		auto j = transac_r.begin();
		while(c < i && j != transac_r.end())
		{ j++; 	c++; }
		
		while(c < (i+n) && j != transac_r.end())
		{
			partition[(*j).first] = (*j).second;
			j++; c++;
		}
		//for(auto j = transac_r.begin() + i; j != transac_r.end() && j != transac_r.begin() + (i + n); j++)
		map<vector<int>, int> l_part = apply_apriori(partition, int(min_sup * n));
		if(l_part.size() == 0)
			break;
		print(l_part, i / 5 + 1, "l");
		for(auto j = l_part.begin(); j != l_part.end(); j++)
			if(l_tot.find((*j).first) == l_tot.end())
				l_tot[(*j).first] = (*j).second;
	}
	
	l_tot = Ln(l_tot);
	print(l_tot, 10000, "l");
}

int main()
{
	transacwrite();
	map <int, vector<int> > t_r = transacread();
	partition_apriori(t_r);	
	return 0;
}
