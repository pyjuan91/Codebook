/*產生fail function*/ 
void kmp_fail(char *s,int len,int *fail){
	int id=-1;
	fail[0]=-1;
	for(int i=1;i<len;++i){
		while(~id&&s[id+1]!=s[i])id=fail[id];
		if(s[id+1]==s[i])++id;
		fail[i]=id;
	}
}
/*以字串B匹配字串A，傳回匹配成功的數量(用B的fail)*/
int kmp_match(char *A,int lenA,char *B,int lenB,int *fail){
	int id=-1,ans=0;
	for(int i=0;i<lenA;++i){
		while(~id&&B[id+1]!=A[i])id=fail[id];
		if(B[id+1]==A[i])++id;
		if(id==lenB-1){/*匹配成功*/
			++ans, id=fail[id];
		}
	}
	return ans;
}

vector<int> prefix_function(string s) {
  int n = (int)s.length();
  vector<int> pi(n);
  for (int i = 1; i < n; i++) {
    int j = pi[i - 1];
    while (j > 0 && s[i] != s[j]) j = pi[j - 1];
    if (s[i] == s[j]) j++;
    pi[i] = j;
  }
  return pi;
}