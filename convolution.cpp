/*
解题思路：
因为使用双端队列对单个数组的处理时O(n)复杂度的，
我们使用同样的方法进行二维数组的处理
首先按照行进行使用滑动窗口的方法进行处理，得到第一遍处理的结果
之后按照列对第一遍处理的结果进行二次处理，地到最终结果
扫描一行是O(m),n行是O(mn),第二遍处理第一列是O(n),m-k行是O(n*(m-k))，那么总的复杂度是O(mn+n*(m-k))也就相当于O(mn)
*/

#include<iostream>
#include<vector>
#include <deque>
using namespace std;

//找到滑动窗口内最大值
vector<int> findMaxInSigleWindow(int n, int k,vector<int> row) {
	vector<int> ans;
	int N[10000] = { 0 };
	int i, j;
	bool smallThanK = true;
	for (int i = 0; i < row.size(); i++) {
		N[i] = row[i];
	}
	//先让两个指针指向第一个位置
	int *head = &N[0];
	int *end = &N[0];
	deque<int> max_deque;
	smallThanK = true;
	for (i = 0; i < n; i++) {
		//表示窗口还没满
		if (smallThanK == true) {
			//开头
			if (max_deque.empty()) {
				max_deque.push_back(N[i]);
			}
			//之后
			else {
				//大于队列中队头元素，清空队列，将新元素作为队头
				if (N[i] > max_deque.front()) {
					max_deque.clear();
					max_deque.push_back(N[i]);
				}
				//小于队头元素，从队列后边往前比，找到比它小的弹出，直到与之相等或大则将新元素压入
				else {
					while (N[i] > max_deque.back()) {
						max_deque.pop_back();
					}
					max_deque.push_back(N[i]);
				}
			}
			head++;//压入新元素了，指针后移
			if (head - end > k - 1) {
				smallThanK = false;
				//cout << max_deque.front() << " ";
				int tmp = max_deque.front();
				ans.push_back(tmp);
			}
		}
		//窗口满了该向后滑动了
		else {
			//如果即将要出去的值为当前的最大值
			if (*end == max_deque.front()) {
				//弹出队头元素
				max_deque.pop_front();
				//如果新元素比当前队头元素大，则清空队列，将新元素作为当前最大值
				if (N[i] > max_deque.front()) {
					max_deque.clear();
					max_deque.push_back(N[i]);
				}
				//如果新元素不比队头元素大，则从后向前比较，去除比它小的，之后进行压入
				else {
					while (N[i] > max_deque.back()) {
						max_deque.pop_back();
					}
					max_deque.push_back(N[i]);
				}
			}
			//如果即将要出去的不是最大值
			else {
				//如果新元素比当前队头元素大，则清空队列，将新元素作为当前最大值
				if (N[i] > max_deque.front()) {
					max_deque.clear();
					max_deque.push_back(N[i]);
				}
				//如果新元素不比队头元素大，则从后向前比较，去除比它小的，之后进行压入
				else {
					while (N[i] > max_deque.back()) {
						max_deque.pop_back();
					}
					max_deque.push_back(N[i]);
				}
			}
			head++;
			end++;
			//cout << max_deque.front() << " ";
			int tmp = max_deque.front();
			ans.push_back(tmp);
		}
	}
	return ans;
}

//void test_findMaxInSigleWindow() {
//	vector<int> input = { 1,2,3,4,5,6,7,8 };
//	vector<int> ans;
//	ans = findMaxInSigleWindow(8, 3, input);
//	for (int i = 0; i < ans.size(); i++) {
//		cout << ans[i] << " ";
//	}
//	cout << endl;
//	system("pause");
//}

//使用双端队列找最大值方法
vector<vector<int>> findMaxUseDeque(vector<vector<int>> matrix, int k) {
	vector<vector<int>> ans;
	vector<vector<int>> ansT;
	vector<vector<int>> ansTmp;
	int row = matrix.size();
	int col = matrix[0].size();
	//先是按照行进行处理一遍
	for (int i = 0; i < row; i++) {
		vector<int> rowTmp;
		rowTmp = findMaxInSigleWindow(col, k, matrix[i]);
		ansTmp.push_back(rowTmp);
	}
	/*cout << ansTmp.size() << " " << ansTmp[0].size() << endl;;*/
	//之后对其结果按列处理一遍
	for (int i = 0; i < ansTmp[0].size(); i++) {
		vector<int> colTmp;
		vector<int> colDeal;
		for (int j = 0; j < row; j++) {
			int tmp = ansTmp[j][i];
			colDeal.push_back(tmp);
		}
		colTmp = findMaxInSigleWindow(colDeal.size(), k, colDeal);
		ansT.push_back(colTmp);
	}
	//计算得到的矩阵转置一下为正确结果
	for (int i = 0; i < ansT[0].size(); i++) {
		vector<int> tmp;
		for (int j = 0; j < ansT.size(); j++) {
			int t = ansT[j][i];
			tmp.push_back(t);
		}
		ans.push_back(tmp);
		
	}
	return ans;
}

//这个是直接穷举找最大值的方法
vector<vector<int>> findMax(vector<vector<int>> matrix,int k) {
	vector<vector<int>> ans;
	int row = matrix.size();
	int col = matrix[0].size();
	for (int i = 0; i <= row - k; i++) {
		vector<int> ansRow;
		for (int j = 0; j <= col - k; j++) {
			int max = 0;
			for (int p = i; p < i + k; p++) {
				for (int q = j; q < j + k; q++) {
					if (max < matrix[p][q]) {
						max = matrix[p][q];
					}
				}
			}
			ansRow.push_back(max);
		}
		ans.push_back(ansRow);
	}
	return ans;
}

int main() {
	int m, n, k;
	vector<vector<int>> pic;
	while (cin >> m >> n >> k) {
		//输入
		for (int i = 0; i < m; i++) {
			vector<int> row;
			int temp;
			for (int j = 0; j < n; j++) {
				cin >> temp;
				row.push_back(temp);
			}
			pic.push_back(row);
		}
		cout << "输入完成" << endl;
		//处理
		vector<vector<int>> ans = {};
		//ans = findMax(pic, k);
		ans = findMaxUseDeque(pic, k);
		cout << "计算完成" << endl;
		for (int i = 0; i < ans.size(); i++) {
			for (int j = 0; j < ans[0].size(); j++) {
				cout << ans[i][j] << " ";
			}
			cout << endl;
		}
		cout << "输出完成" << endl;
	}
	return 0;
}