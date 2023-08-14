#include <iostream>
using namespace std;
 
int main()
{
    double pi = 1; // 存储圆周率的近似值
    int n = 1000; // 存储乘积的项数
    for (int i = 1; i <= n; i++)
    {
        pi *= (2.0 * i) / (2.0 * i - 1) * (2.0 * i) / (2.0 * i + 1); // 计算第i项并累乘到pi中
    }
    pi *= 2; // 乘以2得到圆周率
    cout << pi << endl; // 输出结果
    return 0;
}

