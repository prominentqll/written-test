// 20190412.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

// 大整数类型
class BigInt
{
public:
	BigInt(string str) :strDigit(str) {}
	BigInt operator=(const BigInt &src)//赋值运算符的重载
	{
		return  BigInt(this->strDigit = src.strDigit);
	}
private:
	string strDigit;   // 使用字符串存储大整数

	friend ostream& operator<<(ostream &out, const BigInt &src);
	friend BigInt operator+(const BigInt &lhs, const BigInt &rhs);
	friend BigInt operator-(const BigInt &lhs, const BigInt &rhs);
	friend BigInt operator/(const BigInt &lhs, const BigInt &rhs);
	friend bool isBig(const BigInt & lhs, const BigInt & rhs);
};
bool isBig(const BigInt & lhs, const BigInt & rhs)//判断两个大数的大小
{
	int llen = lhs.strDigit.length();
	int rlen = rhs.strDigit.length();

	if (llen > rlen)
	{
		return true;
	}
	else if (llen < rlen)
	{
		return false;
	}
	else//长度相同，按位比较
	{
		if (lhs.strDigit >= rhs.strDigit)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
// 打印函数
ostream& operator<<(ostream &out, const BigInt &src)
{
	out << src.strDigit;
	return out;
}
// 大数加法
BigInt operator+(const BigInt &lhs, const BigInt &rhs)
{
	/*
	遍历字符串l，r，从后往前遍历
	同位置的数字相加， 进位 flag  存入一个结果当中 string result
	同时完成
	某个字符串先完成   都要考虑进位
	*/
	string result;
	bool flag = false;
	int size1 = lhs.strDigit.length() - 1;
	int size2 = rhs.strDigit.length() - 1;
	int i = size1, j = size2;

	for (; i >= 0 && j >= 0; --i, --j)
	{
		int ret = lhs.strDigit[i] - '0' + rhs.strDigit[j] - '0';
		if (flag)
		{
			ret += 1;
			flag = false;
		}

		if (ret >= 10)
		{
			ret %= 10;
			flag = true;
		}
		result.push_back(ret + '0');
	}

	// i j
	if (i >= 0)
	{
		while (i >= 0)
		{
			int ret = lhs.strDigit[i] - '0';
			if (flag)
			{
				ret += 1;
				flag = false;
			}

			if (ret >= 10)
			{
				ret %= 10;
				flag = true;
			}
			result.push_back(ret + '0');
			i--;
		}
	}
	else if (j >= 0)
	{
		while (j >= 0)
		{
			int ret = rhs.strDigit[j] - '0';
			if (flag)
			{
				ret += 1;
				flag = false;
			}

			if (ret >= 10)
			{
				ret %= 10;
				flag = true;
			}
			result.push_back(ret + '0');
			j--;
		}
	}

	if (flag)
	{
		result.push_back('1');
	}

	reverse(result.begin(), result.end());
	return result; // return BigInt(result);
}
// 大数减法
BigInt operator-(const BigInt &lhs, const BigInt &rhs)
{
	/*
	找大的字符串左减数，小的左被减数
	遍历两个字符串，减法，借位（bool flag）， string result 存下来
	*/
	string result;
	bool flag = false;
	bool minor = false;

	string maxStr = lhs.strDigit;
	string minStr = rhs.strDigit;
	if (maxStr.length() < minStr.length())
	{
		maxStr = rhs.strDigit;
		minStr = lhs.strDigit;
		minor = true;
	}
	else if (maxStr.length() == minStr.length())
	{
		if (maxStr < minStr)
		{
			maxStr = rhs.strDigit;
			minStr = lhs.strDigit;
			minor = true;
		}
		else if (maxStr == minStr)
		{
			return string("0");
		}
	}
	else
	{
		;
	}

	int size1 = maxStr.length() - 1;
	int size2 = minStr.length() - 1;
	int i = size1, j = size2;

	for (; i >= 0 && j >= 0; --i, --j)
	{
		int ret = maxStr[i] - minStr[j];
		if (flag)
		{
			ret -= 1;
			flag = false;
		}

		if (ret < 0)
		{
			ret += 10;
			flag = true;
		}
		result.push_back(ret + '0');
	}

	while (i >= 0)
	{
		int ret = maxStr[i] - '0';
		if (flag)
		{
			ret -= 1;
			flag = false;
		}

		if (ret < 0)
		{
			ret += 10;
			flag = true;
		}
		result.push_back(ret + '0');
		i--;
	}

	if (minor)
	{
		result.push_back('-');
	}

	reverse(result.begin(), result.end());
	string result1;

	int flag1 = 0;
	for (int i = 0; i < result.size();)
	{
		if (result[0] == '-')
		{
			result1.push_back('-');
			i++;
		}
		while (result[i] == '0'&&flag1==0&& i < result.size())//去掉前边的0
		{
			i++;
		}
		flag1 = 1;
		result1.push_back(result[i]);
		i++;
	}
	
	return result1;
}
//大数除法
BigInt operator/(const BigInt &lhs, const BigInt &rhs)
{
	
	/*
	1.被除数小于除数，直接返回0
	2.计算两个长度之差作为基准
	给除数第一次加上长度之差(p)个零，记为N1；
	第二次加上p-1个零，记为N2,直到不加零，计算结束

	每次计算将被除数减去N1，差为M1，直到差小于或等于零结束，计算被减的次数
	给减的次数后加p个零，结果记为R1，将结果入栈

	用M1减去N2，给减的次数后加p-1个零，结果为R2，R2入栈


	直到Ni为除数，计算够减的次数

	最后将R1+R2+.....+Rn就是商

	*/
	if (lhs.strDigit == rhs.strDigit)//被除数和除数相等直接返回1
	{
		return  string("1");
	}
	if (!isBig(lhs, rhs))//除数大于被除数
	{
		return string("0");
	}
	
	//一般情况：被除数大于除数
	//得到长度差,作为基准
	int lendiffer = lhs.strDigit.length() - rhs.strDigit.length();

	vector<BigInt>resvec;//存放结果的栈
	BigInt lhstmp = lhs;

	//开始循环
	for (;lendiffer >= 0; --lendiffer)
	{
		
		BigInt rhstmp = rhs;

		int k = lendiffer;
		
		while (k > 0)//给被除数添加0
		{
			rhstmp.strDigit+= '0';
			k--;
		}

		BigInt result("");//保存每次减完的结果
		BigInt  count("");//记录次数


		//判断添完0之后的除数，如果大于被除数，代表一次都不能减
		if (isBig(rhstmp.strDigit, lhstmp.strDigit))
		{
			rhstmp= rhs;//进行下一次的运算？？？
			continue;
		}


		//如果被除数大于添加完0之后的除数， 代表至少可以减一次
		do
		{
			result = lhstmp - rhstmp;

			if (!isBig(result,rhstmp))//减完之后的结果小于减数，停止运算
			{
				lhstmp= result;
				count = count + BigInt("1");
				break;
			}

			lhstmp= result;
			count = count + BigInt("1");

		} while (isBig(result.strDigit, rhstmp.strDigit));
			

		k = lendiffer;//恢复k的值，为次数加0
		while (k > 0)
		{
			count.strDigit.push_back('0');
			k--;
		}

		//将第一轮的将结果添加进容器中
		resvec.push_back(count);
	}
	
	BigInt res("");
	for (int i = 0; i < resvec.size(); ++i)
	{
		res = res + resvec[i];
	}
	return res;

}

int main()
{
	BigInt int1("9785645649886874535428765");
	BigInt int2("28937697857832167849697653231243");
	BigInt int3("9785645649886874535428765");
	//28937707643477817736572188660008
	//28937707643477817736572188660008	
	BigInt  int4("100000000000");
	BigInt  int5("5");

	cout << int4 / int5 << endl;

	//cout << int1 + int2 << endl;
	//28937688072186517962823117802478
	//28937688072186517962823117802478
	//cout << int4 - int5 << endl;
	getchar();
	return 0;
}






