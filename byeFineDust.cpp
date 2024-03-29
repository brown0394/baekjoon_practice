#include <iostream>
#include <vector>
#include <cstring>

struct node {
	int i;
	int j;
	int dust;
};

class air {
private:
	int row;
	int col;
	int totalDust;
	int rowLen;
	int colLen;
	int** arr;
	std::vector<int> airPurifier;
	std::vector<node> dusts;
	void spreadDust();
	void purifyDust();
	void setDust();
public:
	air(int p_row, int p_col);
	~air();
	int getTotalDustAtT(int time);
};

air::air(int p_row, int p_col):row(p_row), col(p_col) {
	totalDust = 0;
	dusts.reserve(row * col);
	arr = new int* [row+5];
	for (int i = 0; i < row; ++i) {
		arr[i] = new int[col];
		for (int j = 0; j < col; ++j) {
			scanf("%d", &arr[i][j]);
			if (arr[i][j] == -1) airPurifier.push_back(i);
			else {
				if (arr[i][j] >= 5) dusts.push_back({i, j, arr[i][j]});
				totalDust += arr[i][j];
			}
		}
	}
	rowLen = row - 1;
	colLen = col - 1;
}

air::~air() {
	delete[] arr;
}

void air::spreadDust() {
	int remain, spread;
	for (auto it = dusts.begin(); it != dusts.end(); ++it) {
		spread = it->dust / 5;
		remain = 0;
		if (it->i + 1 < row && arr[it->i + 1][it->j] != -1) {
			arr[it->i + 1][it->j] += spread;
			++remain;
		}
		if (it->i - 1 >= 0 && arr[it->i - 1][it->j] != -1) {
			arr[it->i - 1][it->j] += spread;
			++remain;
		}
		if (it->j + 1 < col) {
			arr[it->i][it->j + 1] += spread;
			++remain;
		}
		if (it->j - 1 >= 0 && arr[it->i][it->j - 1] != -1) {
			arr[it->i][it->j - 1] += spread;
			++remain;
		}
		arr[it->i][it->j] -= (spread * remain);
	}
	dusts.clear();
}

void air::purifyDust() {
	totalDust -= (arr[airPurifier[0] - 1][0] + arr[airPurifier[1] + 1][0]);
	int temp1 = arr[airPurifier[0]][colLen];
	int temp2 = arr[airPurifier[1]][colLen];
	std::memmove(&arr[airPurifier[0]][2], &arr[airPurifier[0]][1], (colLen - 1) * 4);
	std::memmove(&arr[airPurifier[1]][2], &arr[airPurifier[1]][1], (colLen - 1) * 4);
	int temp3 = arr[0][0];
	int temp4 = arr[rowLen][0];
	std::memcpy(&arr[0][0], &arr[0][1], colLen * 4);
	std::memcpy(&arr[rowLen][0], &arr[rowLen][1], colLen * 4);
	bool checked = true;
	int idx = 1;
	while (checked) {
		checked = false;
		if (airPurifier[0] > idx) {
			checked = true;
			arr[airPurifier[0] - idx][0] = arr[airPurifier[0] - idx - 1][0];
			arr[idx - 1][colLen] = arr[idx][colLen];
		}
		if (airPurifier[1] + idx < rowLen) {
			checked = true;
			arr[airPurifier[1] + idx][0] = arr[airPurifier[1] + idx + 1][0];
			arr[row - idx][colLen] = arr[row - (idx+1)][colLen];
		}
		++idx;
	}
	arr[airPurifier[0] - 1][colLen] = temp1;
	arr[airPurifier[1] + 1][colLen] = temp2;
	arr[1][0] = temp3;
	arr[rowLen - 1][0] = temp4;
	arr[airPurifier[0]][1] = 0;
	arr[airPurifier[1]][1] = 0;
}

void air::setDust() {
	for (int i = 0; i < row; ++i) {
		for (int j = 0; j < col; ++j) {
			if (arr[i][j] > 4) {
				dusts.push_back({ i, j, arr[i][j] });
			}
		}
	}
}

int air::getTotalDustAtT(int time) {
	for (int i = 0; i < time; ++i) {
		spreadDust();
		purifyDust();
		setDust();
	}
	return totalDust;
}

int main() {
	int row, col, time;
	scanf("%d %d %d", &row, &col, &time);
	air a(row, col);
	printf("%d\n", a.getTotalDustAtT(time));
}