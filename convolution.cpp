/*
����˼·��
��Ϊʹ��˫�˶��жԵ�������Ĵ���ʱO(n)���Ӷȵģ�
����ʹ��ͬ���ķ������ж�ά����Ĵ���
���Ȱ����н���ʹ�û������ڵķ������д����õ���һ�鴦��Ľ��
֮�����жԵ�һ�鴦��Ľ�����ж��δ����ص����ս��
ɨ��һ����O(m),n����O(mn),�ڶ��鴦���һ����O(n),m-k����O(n*(m-k))����ô�ܵĸ��Ӷ���O(mn+n*(m-k))Ҳ���൱��O(mn)
*/

#include<iostream>
#include<vector>
#include <deque>
using namespace std;

//�ҵ��������������ֵ
vector<int> findMaxInSigleWindow(int n, int k,vector<int> row) {
	vector<int> ans;
	int N[10000] = { 0 };
	int i, j;
	bool smallThanK = true;
	for (int i = 0; i < row.size(); i++) {
		N[i] = row[i];
	}
	//��������ָ��ָ���һ��λ��
	int *head = &N[0];
	int *end = &N[0];
	deque<int> max_deque;
	smallThanK = true;
	for (i = 0; i < n; i++) {
		//��ʾ���ڻ�û��
		if (smallThanK == true) {
			//��ͷ
			if (max_deque.empty()) {
				max_deque.push_back(N[i]);
			}
			//֮��
			else {
				//���ڶ����ж�ͷԪ�أ���ն��У�����Ԫ����Ϊ��ͷ
				if (N[i] > max_deque.front()) {
					max_deque.clear();
					max_deque.push_back(N[i]);
				}
				//С�ڶ�ͷԪ�أ��Ӷ��к����ǰ�ȣ��ҵ�����С�ĵ�����ֱ����֮��Ȼ������Ԫ��ѹ��
				else {
					while (N[i] > max_deque.back()) {
						max_deque.pop_back();
					}
					max_deque.push_back(N[i]);
				}
			}
			head++;//ѹ����Ԫ���ˣ�ָ�����
			if (head - end > k - 1) {
				smallThanK = false;
				//cout << max_deque.front() << " ";
				int tmp = max_deque.front();
				ans.push_back(tmp);
			}
		}
		//�������˸���󻬶���
		else {
			//�������Ҫ��ȥ��ֵΪ��ǰ�����ֵ
			if (*end == max_deque.front()) {
				//������ͷԪ��
				max_deque.pop_front();
				//�����Ԫ�رȵ�ǰ��ͷԪ�ش�����ն��У�����Ԫ����Ϊ��ǰ���ֵ
				if (N[i] > max_deque.front()) {
					max_deque.clear();
					max_deque.push_back(N[i]);
				}
				//�����Ԫ�ز��ȶ�ͷԪ�ش���Ӻ���ǰ�Ƚϣ�ȥ������С�ģ�֮�����ѹ��
				else {
					while (N[i] > max_deque.back()) {
						max_deque.pop_back();
					}
					max_deque.push_back(N[i]);
				}
			}
			//�������Ҫ��ȥ�Ĳ������ֵ
			else {
				//�����Ԫ�رȵ�ǰ��ͷԪ�ش�����ն��У�����Ԫ����Ϊ��ǰ���ֵ
				if (N[i] > max_deque.front()) {
					max_deque.clear();
					max_deque.push_back(N[i]);
				}
				//�����Ԫ�ز��ȶ�ͷԪ�ش���Ӻ���ǰ�Ƚϣ�ȥ������С�ģ�֮�����ѹ��
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

//ʹ��˫�˶��������ֵ����
vector<vector<int>> findMaxUseDeque(vector<vector<int>> matrix, int k) {
	vector<vector<int>> ans;
	vector<vector<int>> ansT;
	vector<vector<int>> ansTmp;
	int row = matrix.size();
	int col = matrix[0].size();
	//���ǰ����н��д���һ��
	for (int i = 0; i < row; i++) {
		vector<int> rowTmp;
		rowTmp = findMaxInSigleWindow(col, k, matrix[i]);
		ansTmp.push_back(rowTmp);
	}
	/*cout << ansTmp.size() << " " << ansTmp[0].size() << endl;;*/
	//֮����������д���һ��
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
	//����õ��ľ���ת��һ��Ϊ��ȷ���
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

//�����ֱ����������ֵ�ķ���
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
		//����
		for (int i = 0; i < m; i++) {
			vector<int> row;
			int temp;
			for (int j = 0; j < n; j++) {
				cin >> temp;
				row.push_back(temp);
			}
			pic.push_back(row);
		}
		cout << "�������" << endl;
		//����
		vector<vector<int>> ans = {};
		//ans = findMax(pic, k);
		ans = findMaxUseDeque(pic, k);
		cout << "�������" << endl;
		for (int i = 0; i < ans.size(); i++) {
			for (int j = 0; j < ans[0].size(); j++) {
				cout << ans[i][j] << " ";
			}
			cout << endl;
		}
		cout << "������" << endl;
	}
	return 0;
}