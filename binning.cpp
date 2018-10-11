#include <bits/stdc++.h>

#define max 20
using namespace std;

void print_bins(vector<vector<int> > part)
{
	for(auto i = part.begin(); i != part.end(); i++)
	{
		for(int j=0; j < (*i).size(); j++)
			cout<<(*i)[j]<<" ";
		cout<<endl;
	}
	cout<<endl;
}

vector<vector<int> > partition(vector<int> a, int bin_count)
{
	sort(a.begin(), a.end());
	vector<vector<int> > part;
	for(int i=0; i < a.size() / bin_count; i++)
	{
		vector<int> b;
		for(int i = 0; i < bin_count; i++)
			b.push_back(0);
		part.push_back(b);
	}
	
	for(int i=0; i < a.size(); i++)
		part[int(i / bin_count)][int(i % bin_count)] = a[i];
	return part;
}

int mean(vector<int> a)
{
	int s = 0;
	for(int i=0; i < a.size(); i++)
		s += a[i];
	return s / a.size();
}

vector<vector<int> > binning_by_mean(vector<int> a)
{
	vector<vector<int> > part = partition(a, 5);
	for(auto i = part.begin(); i != part.end(); i++)
	{
		int m = mean((*i));
		for(int j=0; j < (*i).size(); j++)
			(*i)[j] = m;
	}
	return part;
}

vector<vector<int> > binning_by_bound(vector<int> a)
{
	vector<vector<int> > part = partition(a, 5);
	for(auto i = part.begin(); i != part.end(); i++)
	{
		int l = (*i)[0], h = (*i)[(*i).size() - 1];
		for(int j=0; j < (*i).size(); j++)
		{
			if(abs((*i)[j] - l) > abs((*i)[j] - h))
				(*i)[j] = h;
			else
				(*i)[j] = l;
		}
	}
	return part;
}

int main()
{
	srand(time(NULL));
	vector<int> a, b;
	for(int i = 0; i < max; i++)
	{
		a.push_back(rand() % 100);
		cout<<a[i]<<" ";
	}

	cout<<endl;
	b = a;
	print_bins(binning_by_bound(a));
	print_bins(binning_by_mean(b));
}
