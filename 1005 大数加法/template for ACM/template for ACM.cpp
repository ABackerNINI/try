//Template for ACM

#define ll long long
#define INF 0x7FFFFFFF
#define LINF 0x7FFFFFFFFFFFFFFF

#include "stdio.h"
#include "stdlib.h"

#include <map>
#include <queue>
#include <stack>
#include <cmath>
#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include <algorithm>

using namespace std;

#define MAXN ()

class BigNum{
#define MAXOFEACH 10000

public:
	//maxLen为数在10进制下的位数
	//BigNum(){}
	BigNum(int maxLen){ malloc(maxLen); orderd = true; }
	BigNum(const BigNum &num){ *this = num; }
	BigNum(BigNum &&num) :data(NULL){ *this = _STD move(num); }

	void assign(ll num){//O(1)
		clear();
		if (num < 0){
			isPos = false; num = -num;
		}
		else isPos = true;
		data[0] = num; orderd = false;
	}
	void assign(const string &num){//O(N)
		clear();
		int bound1, bound2, idx = num.size(); ll each;

		if (num.front() == '-'){ isPos = false; bound1 = (num.size() - 1) / 4 + 1; bound2 = 1; }
		else { isPos = true; bound1 = num.size() / 4 + 1;  bound2 = 0; }

		fitting(bound1 + 5);

		for (int i = 0; i < bound1; ++i){
			each = 0;
			for (int j = 0; j < 4 && idx > bound2; ++j){
				each += (num[--idx] - '0')*((ll)pow(10, j));
			}
			data[i] = each;
		}
		orderd = true;
	}

	BigNum &operator=(ll num){
		assign(num); return *this;
	}
	BigNum &operator=(const BigNum &num){//O(N)
		if (this != &num){
			malloc((num.maxLen - 2) << 2); isPos = num.isPos; orderd = num.orderd;
			memcpy(data, num.data, sizeof(ll)*maxLen);
		}
		return *this;
	}
	BigNum &operator=(BigNum &&num){//O(1)
		if (this != &num){
			std::swap(data, num.data);
			maxLen = num.maxLen; isPos = num.isPos; orderd = num.orderd;
		}
		return *this;
	}

	BigNum &operator++(){ *this += 1; return *this; }
	BigNum &operator--(){ *this -= 1; return *this; }
	BigNum &operator++(int){ BigNum tmp = *this; *this += 1; return tmp; }
	BigNum &operator--(int){ BigNum tmp = *this; *this -= 1; return tmp; }

	BigNum &operator+=(ll num){//O(1)
		if ((isPos&&num >= 0) || (!isPos&&num < 0))data[0] += abs(num);
		else data[0] -= abs(num);
		orderd = false; return *this;
	}
	BigNum &operator-=(ll num){//O(1)
		if ((isPos&&num >= 0) || (!isPos&&num < 0))data[0] -= abs(num);
		else data[0] += abs(num);
		orderd = false; return *this;
	}
	BigNum &operator*=(ll num){//O(N)
		arrange(); fitting(((getDigit() + getDigit(num)) >> 2) + 5);
		if (num < 0){ isPos = !isPos; num = -num; }
		for (int i = 0; i < maxLen; ++i)data[i] *= num;
		orderd = false; return *this;
	}
	BigNum &operator/=(ll num){//O(N)
		_ASSERT(num != 0); arrange();
		if (num < 0){ isPos = !isPos; num = -num; }
		for (int i = maxLen - 1; i >= 0; --i){
			if (i>0)data[i - 1] += (data[i] % num) * 10000;
			data[i] /= num;
		}
		orderd = true; return *this;
	}
	BigNum &operator%=(ll &num){}

	BigNum &operator+=(const BigNum &num){//O(N)
		fitting(num.maxLen);
		if (isPos^num.isPos)for (int i = 0; i < num.maxLen; ++i)data[i] -= num.data[i];
		else for (int i = 0; i < num.maxLen; ++i)data[i] += num.data[i];
		orderd = false; return *this;
	}
	BigNum &operator-=(const BigNum &num){//O(N)
		fitting(num.maxLen);
		if (isPos^num.isPos)for (int i = 0; i < num.maxLen; ++i)data[i] += num.data[i];
		else for (int i = 0; i < num.maxLen; ++i)data[i] -= num.data[i];
		orderd = false; return *this;
	}
	BigNum &operator*=(BigNum &num){//O(k*N*M)
		arrange(); num.arrange();
		int bound = num.getDigit();
		if (bound == 1 && num.data[0] == 0){
			clear(); return *this;
		}

		fitting(((getDigit() + bound) >> 2) + 5);
		if (!num.isPos)isPos = !isPos;

		for (int i = 0; i < bound; ++i){
			if (num.data[i]>0)*this *= num.data[i];
			if (i < bound - 1)*this *= 10000;
		}

		orderd = false; return *this;
	}
	BigNum &operator/=(BigNum &num){
		arrange(); num.arrange(); int bound1 = getDigit(), bound2 = num.getDigit();
		if (bound2 == 1 && num.data[0] == 0)_ASSERT(1); BigNum ans((maxLen - 2) << 2);
		if (isPos ^ num.isPos)ans.isPos = true;
		else ans.isPos = false;

		ans = -1; bool p = isPos;
		while (arrange(), isPos == p){
			*this -= num;
			++ans;
		}
		*this = _STD move(ans);
		return *this;
	}
	BigNum &operator%=(BigNum &num){}

	BigNum getLastReminder(){}

	template<typename t>BigNum operator+(const t &num)const{
		BigNum tmp = *this;
		tmp += num;
		return tmp;
	}
	template<typename t>BigNum operator-(const t &num)const{
		BigNum tmp = *this;
		tmp -= num;
		return tmp;
	}
	template<typename t>BigNum operator*(const t &num)const{
		BigNum tmp = *this;
		tmp *= num;
		return tmp;
	}
	template<typename t>BigNum operator/(const t &num)const{
		BigNum tmp = *this;
		tmp /= num;
		return tmp;
	}
	template<typename t>BigNum operator%(const t &num)const{
		BigNum tmp = *this;
		tmp %= num;
		return tmp;
	}

	void clear(){
		for (int i = 0; i < maxLen; ++i)data[i] = 0;
	}
	void arrange(){//O(N)
		if (orderd)return;
		ll carry = 0;
		for (int k = maxLen - 1; k >= 0; --k){
			if (data[k] != 0){
				if (data[k] < 0){//变号
					isPos = !isPos;
					for (int i = 0; i <= k; ++i)data[i] = -data[i];
				}

				for (int i = 0; i < maxLen; ++i){
					data[i] += carry;
					carry = data[i] / 10000;
					data[i] %= 10000;
					if (data[i] < 0){
						data[i] += 10000;
						--carry;
					}
				}
				orderd = true; return;
			}
		}
		isPos = true; orderd = true;
	}
	void print(bool nextLine = true){//O(N)
		arrange();
		if (!isPos)putchar('-');
		for (int i = maxLen - 1; i >= 0; --i){
			if (data[i] != 0){
				printf("%d", data[i]);
				for (int j = i - 1; j >= 0; --j){
					printf("%04I64d", data[j]);
				}if (nextLine)putchar('\n'); return;
			}
		}
		printf("0");
		if (nextLine)putchar('\n');
	}
	void debug(int n = 10){//O(N)
		for (int i = 0; i < n; ++i)printf("%I64d\n", data[i]);
		printf("\n");
	}

	string ToString(){}

	~BigNum(){ if (data)delete[]data; }
private:
	ll *data; int maxLen; bool isPos, orderd;

	int getDigit()const{//O(N)
		int i;
		for (i = maxLen - 1; i > 0; --i){
			if (data[i] != 0)break;
		}
		return i + 1;
	}
	template<typename t>int getDigit(t num){//O(N)
		int i = 0;
		while (num > 0){
			num /= 10; ++i;
		}
		return i;
	}
	void fitting(int maxLen){//O(N)
		if (maxLen > this->maxLen){
			ll *_data = new ll[maxLen]; for (int i = 0; i < maxLen; ++i)_data[i] = 0;
			memcpy(_data, data, sizeof(ll)*this->maxLen);
			delete[]data; data = _data;
			this->maxLen = maxLen;
		}
	}
	void malloc(int maxLen){//O(N)
		this->maxLen = (maxLen >> 2) + 2;
		data = new ll[this->maxLen];
		//for (int i = 0; i < this->maxLen; ++i)data[i] = 0;
	}
};

int main(){
	BigNum bn1(10000 + 5), bn2(10000 + 5);
	string a, b; cin >> a >> b;
	bn1.assign(a); bn2.assign(b);
	bn1 *= bn2;

	bn1.print();

	return 0;
}