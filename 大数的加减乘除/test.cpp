// 20190412.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

// ����������
class BigInt
{
public:
	BigInt(string str) :strDigit(str) {}
	BigInt operator=(const BigInt &src)//��ֵ�����������
	{
		return  BigInt(this->strDigit = src.strDigit);
	}
private:
	string strDigit;   // ʹ���ַ����洢������

	friend ostream& operator<<(ostream &out, const BigInt &src);
	friend BigInt operator+(const BigInt &lhs, const BigInt &rhs);
	friend BigInt operator-(const BigInt &lhs, const BigInt &rhs);
	friend BigInt operator/(const BigInt &lhs, const BigInt &rhs);
	friend bool isBig(const BigInt & lhs, const BigInt & rhs);
};
bool isBig(const BigInt & lhs, const BigInt & rhs)//�ж����������Ĵ�С
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
	else//������ͬ����λ�Ƚ�
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
// ��ӡ����
ostream& operator<<(ostream &out, const BigInt &src)
{
	out << src.strDigit;
	return out;
}
// �����ӷ�
BigInt operator+(const BigInt &lhs, const BigInt &rhs)
{
	/*
	�����ַ���l��r���Ӻ���ǰ����
	ͬλ�õ�������ӣ� ��λ flag  ����һ��������� string result
	ͬʱ���
	ĳ���ַ��������   ��Ҫ���ǽ�λ
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
// ��������
BigInt operator-(const BigInt &lhs, const BigInt &rhs)
{
	/*
	�Ҵ���ַ����������С���󱻼���
	���������ַ�������������λ��bool flag���� string result ������
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
		while (result[i] == '0'&&flag1==0&& i < result.size())//ȥ��ǰ�ߵ�0
		{
			i++;
		}
		flag1 = 1;
		result1.push_back(result[i]);
		i++;
	}
	
	return result1;
}
//��������
BigInt operator/(const BigInt &lhs, const BigInt &rhs)
{
	
	/*
	1.������С�ڳ�����ֱ�ӷ���0
	2.������������֮����Ϊ��׼
	��������һ�μ��ϳ���֮��(p)���㣬��ΪN1��
	�ڶ��μ���p-1���㣬��ΪN2,ֱ�������㣬�������

	ÿ�μ��㽫��������ȥN1����ΪM1��ֱ����С�ڻ��������������㱻���Ĵ���
	�����Ĵ������p���㣬�����ΪR1���������ջ

	��M1��ȥN2�������Ĵ������p-1���㣬���ΪR2��R2��ջ


	ֱ��NiΪ���������㹻���Ĵ���

	���R1+R2+.....+Rn������

	*/
	if (lhs.strDigit == rhs.strDigit)//�������ͳ������ֱ�ӷ���1
	{
		return  string("1");
	}
	if (!isBig(lhs, rhs))//�������ڱ�����
	{
		return string("0");
	}
	
	//һ����������������ڳ���
	//�õ����Ȳ�,��Ϊ��׼
	int lendiffer = lhs.strDigit.length() - rhs.strDigit.length();

	vector<BigInt>resvec;//��Ž����ջ
	BigInt lhstmp = lhs;

	//��ʼѭ��
	for (;lendiffer >= 0; --lendiffer)
	{
		
		BigInt rhstmp = rhs;

		int k = lendiffer;
		
		while (k > 0)//�����������0
		{
			rhstmp.strDigit+= '0';
			k--;
		}

		BigInt result("");//����ÿ�μ���Ľ��
		BigInt  count("");//��¼����


		//�ж�����0֮��ĳ�����������ڱ�����������һ�ζ����ܼ�
		if (isBig(rhstmp.strDigit, lhstmp.strDigit))
		{
			rhstmp= rhs;//������һ�ε����㣿����
			continue;
		}


		//������������������0֮��ĳ����� �������ٿ��Լ�һ��
		do
		{
			result = lhstmp - rhstmp;

			if (!isBig(result,rhstmp))//����֮��Ľ��С�ڼ�����ֹͣ����
			{
				lhstmp= result;
				count = count + BigInt("1");
				break;
			}

			lhstmp= result;
			count = count + BigInt("1");

		} while (isBig(result.strDigit, rhstmp.strDigit));
			

		k = lendiffer;//�ָ�k��ֵ��Ϊ������0
		while (k > 0)
		{
			count.strDigit.push_back('0');
			k--;
		}

		//����һ�ֵĽ������ӽ�������
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






